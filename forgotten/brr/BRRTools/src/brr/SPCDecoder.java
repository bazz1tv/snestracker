//BRRDecoder
//This code is part of the open source project BRRTools
//(c)2012 Bregalad
//See documentations for usage details

package brr;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;

public class SPCDecoder {
	static int looppos, loopcount, firstsample, lastsample, samplerate, looptimes;
	static double min_length;

	public static void main(String[] args) throws FileNotFoundException, IOException {
		int size=0;
		looppos=0; loopcount=1; firstsample=0; lastsample=0; samplerate = 16000; min_length = 0;
		int SampleAdr, LoopAdr;
		byte[] BRR = new byte[9];
		short[][] samples;

		if(args.length<2) printInstructions();
		for(int i=2; i<args.length; i++)
			parseArg(args[i]);

		RandomAccessFile inSPC = new RandomAccessFile(args[0], "r");		//Input file
		inSPC.seek(0x1015d);												//Sample location value in DSP
		int SampleLoc = ((((int)inSPC.readByte()&0xFF)<<8)+0x100);			//Location of sample table in file
		for(int sample=firstsample; sample<=lastsample; sample++) {
														//Create output file
			DataOutputStream outWav = new DataOutputStream(new FileOutputStream(args[1]+"_"+ sample +".wav"));
			inSPC.seek(SampleLoc +(sample<<2));							//Read sample number
			SampleAdr=Short.reverseBytes(inSPC.readShort())&0xFFFF;		//Read address of sample (make sure address isn't negative)
			LoopAdr=Short.reverseBytes(inSPC.readShort())&0xFFFF;
			looppos=(LoopAdr-SampleAdr)/9;								//Number of BRR blocks before loop

			int blockamount=0;
			byte flags;
			do{
				inSPC.seek(((SampleAdr+9*blockamount)&0xFFFF)+0x100);		//Compute the number of block before end of sample
				blockamount++;
				flags = inSPC.readByte();
			}while((flags&0x01)==0);
			boolean loop_en = (flags&0x02)!=0;							//Check if the sample has looping enabled

			int k=0;									//k indexes the sample buffer
			if(loop_en) {								//Samples with loop enabled
				System.out.print("Sample #" + sample + " Lenght : " + blockamount + " Loop : " +looppos+". ");
				inSPC.seek(LoopAdr+0x100);										//Read filter at loop point
				int loopfilter = (inSPC.readByte()&0x0c)>>2;
				BRRDecoder.p1=0;
				BRRDecoder.p2=0;
				
														//Implement the "minimum length" function
				looptimes = Math.max(loopcount, ((int)Math.ceil(min_length*samplerate/16)-looppos)/(blockamount-looppos));

				short[] oldp1 = new short[looptimes];
				short[] oldp2 = new short[looptimes];		//Tables to remember value of p1, p2 when looping

				samples = new short[looptimes*(blockamount-looppos)+looppos][];	//Create samples buffer
				for(int i=0; i<looppos; i++,k++) {		//Read the start of the sample before loop point
					inSPC.seek(((SampleAdr+9*i)&0xFFFF)+0x100);
					inSPC.read(BRR);
					samples[k] = BRRDecoder.DecodeBRR(BRR);	//Append 16 BRR samples to existing array
				}
				for(int j=0; j<looptimes; j++) {
					oldp1[j]=BRRDecoder.p1;					//Save the p1 and p2 values on each loop point encounter
					oldp2[j]=BRRDecoder.p2;
					for(int i=looppos; i<blockamount; i++,k++) {
						inSPC.seek(((SampleAdr+9*i)&0xFFFF)+0x100);
						inSPC.read(BRR);
						samples[k] = BRRDecoder.DecodeBRR(BRR);	//Append 16 BRR samples to existing array
					}
				}

				if (looptimes>1) {
					switch (loopfilter) {
					case 0:
						System.out.println("Looping is stable.");
						break;
					case 1:
						if(oldp1[0]==oldp1[1]) System.out.println("Looping is stable.");
						else {
							int i, j=0;
							for (i=0; i<looptimes-1; i++) {
								for (j=i+1; j<looptimes; j++) {
									if (oldp1[i]==oldp1[j]) break;
								}
								if (j<looptimes) break;
							}
							if (i<looptimes-1)
								System.out.println("Samples repeat after loop "+j+" to loop "+i+".");
							else
								System.out.println("Try a larger number of loops to figure the lenght until the sample repeats.");
						}
						break;
					default:
						if(oldp1[0]==oldp1[1]&&oldp2[0]==oldp2[1]) System.out.println("Looping is stable.");
						else {
							int i, j=0;
							for (i=0; i<looptimes-1; i++) {
								for (j=i+1; j<looptimes; j++) {
									if (oldp1[i]==oldp1[j]&&oldp2[i]==oldp2[j]) break;
								}
								if (j<looptimes) break;
							}
							if (i<looptimes-1)
								System.out.println("Samples repeat after loop "+j+" to loop "+i+".");
							else
								System.out.println("Try a larger number of loops to figure the lenght until the sample repeats.");
						}
						break;
					}
				}
			}
			else{											//Samples with loop disabled
				System.out.println("Sample #" + sample + " Lenght : " + blockamount + " Loop : disabled");

				samples = new short[blockamount][];
				for(k=0; k<blockamount; k++) {
					inSPC.seek(((SampleAdr+9*k)&0xFFFF)+0x100);
					inSPC.read(BRR);
					samples[k] = BRRDecoder.DecodeBRR(BRR);	//Append 16 BRR samples to existing array
				}
			}
			outWav.writeBytes("RIFF");
			outWav.writeInt(Integer.reverseBytes((size<<1)+36));
			outWav.writeBytes("WAVEfmt ");
			outWav.writeInt(Integer.reverseBytes(16));			//Size of header
			outWav.writeShort(Short.reverseBytes((short)1));	//PCM format, no compression
			outWav.writeShort(Short.reverseBytes((short)1));	//Mono
			outWav.writeInt(Integer.reverseBytes(samplerate));		//Sample rate
			outWav.writeInt(Integer.reverseBytes(samplerate<<1));		//Byte rate
			outWav.writeShort(Short.reverseBytes((short)2));	//BlockAligh
			outWav.writeShort(Short.reverseBytes((short)16));	//16-bit samples
			outWav.writeBytes("data");
			outWav.writeInt(Integer.reverseBytes(k<<5));
			for(int i=0; i<k; i++)
				for(int j=0; j<16; j++)							//Write samplebuffer to file
					outWav.writeShort(Short.reverseBytes(samples[i][j]));
			outWav.close();
		}
		inSPC.close();
	}

	static void printInstructions() {
		System.out.println("SPC decoder 2.0 (c) 2012 Bregalad special thanks to Kode54");
		System.out.println("Arguments : infile.spc outfile -n1 -f0 -l9 -s16000 -m1.0");
		System.out.println("-n number of times to loop through the sample(s), default 1");
		System.out.println("-f first sample # to decode (default : 0)");
		System.out.println("-l last sample # to decode (default : same as first)");
		System.out.println("-s output samplerate, default 32000");
		System.out.println("-m minimum output length : Loop the sample so that it is at least the specified length in seconds");
		System.exit(1);
	}

	static void parseArg(String Arg) {
		if (Arg.charAt(0)=='-') {
			if(Arg.charAt(1)=='l')
				lastsample = Integer.parseInt(Arg.substring(2));
			else if(Arg.charAt(1)=='f') {
				firstsample = Integer.parseInt(Arg.substring(2));
				lastsample = firstsample;
			}
			else if(Arg.charAt(1)=='n')
				loopcount = Integer.parseInt(Arg.substring(2));
			else if(Arg.charAt(1)=='s')
				samplerate = Integer.parseInt(Arg.substring(2));
			else if(Arg.charAt(1)=='m')
				min_length = Double.parseDouble(Arg.substring(2));
			else printInstructions();
		}
		else printInstructions();
	}
}
