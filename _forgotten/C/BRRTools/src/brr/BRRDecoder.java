//BRRDecoder
//This code is part of the open source project BRRTools
//(c)2012 Bregalad
//See documentations for usage details

package brr;
import java.io.*;

public class BRRDecoder {

	static short p1, p2;
	static int a;
	static int looppos, loopcount, samplerate;

	public static void main(String[] args) throws FileNotFoundException, IOException {
	
		int size=0;
		looppos=0; loopcount=1; samplerate = 32000;
		p1=0; p2=0;
		short[][] samples;
		byte[] BRR = new byte[9];

		if(args.length<2) printInstructions();
		for(int i=2; i<args.length; i++)
			parseArg(args[i]);

		DataOutputStream outWav=new DataOutputStream(new FileOutputStream(args[1]));	//Create output file
		RandomAccessFile inBrr= new RandomAccessFile(args[0], "r");						//Input file

		size =(int)inBrr.length();							//BRR string must be a multiple of 9 bytes
		if(size%9!=0) {
			System.out.println("Error : BRR file isn't a multiple of 9 bytes.");
			System.exit(-1);
		}
		int blockamount = size / 9;
		System.out.println("Number of BRR blocks to decode : " + blockamount + ".");

		if(looppos<0 || looppos>=blockamount) {								//Make sure the loop position is in range
			System.out.println("Error : Loop position is out of range");
			System.exit(-1);
		}

		inBrr.seek(looppos*9);							//Read filter at loop point
		int loopfilter = (inBrr.readByte()&0x0c)>>2;

		short[] oldp1 = new short[loopcount];
		short[] oldp2 = new short[loopcount];		//Tables to remember value of p1, p2 when looping
													//Create sample buffer
		samples = new short[loopcount*(blockamount-looppos)+looppos][];

		inBrr.seek(0);							//Start to read at the beginning of the file
		int k=0;								//k indexes the sample buffer

		for(int i=0; i<looppos; i++,k++) {		//Read the start of the sample before loop point
			inBrr.read(BRR);
			samples[k] = DecodeBRR(BRR);		//Append 16 BRR samples to existing array
		}
		for(int j=0; j<loopcount; j++) {
			oldp1[j]=p1;						//Save the p1 and p2 values on each loop point encounter
			oldp2[j]=p2;
			inBrr.seek(looppos*9);
			for(int i=looppos; i<blockamount; i++,k++) {
				inBrr.read(BRR);
				samples[k] = DecodeBRR(BRR);	//Append 16 BRR samples to existing array
			}
		}

		if (loopcount>1) {
			switch (loopfilter) {
			case 0:
				System.out.println("Looping is stable.");
				break;
			case 1:
				if(oldp1[0]==oldp1[1]) System.out.println("Looping is stable.");
				else {
					System.out.println("Looping is unstable !");
					int i, j=0;
					for (i=0; i<loopcount-1; i++) {
						for (j=i+1; j<loopcount; j++) {
							if (oldp1[i]==oldp1[j]) break;
						}
						if (j<loopcount) break;
					}
					if (i<loopcount-1)
						System.out.println("Samples repeat after loop "+j+" to loop "+i+".");
					else
						System.out.println("Try a larger number of loops to figure the lenght until the sample repeats.");
				}
				break;
			default:
				if(oldp1[0]==oldp1[1]&&oldp2[0]==oldp2[1]) System.out.println("Looping is stable.");
				else {
					System.out.println("Looping is unstable !");
					int i, j=0;
					for (i=0; i<loopcount-1; i++) {
						for (j=i+1; j<loopcount; j++) {
							if (oldp1[i]==oldp1[j]&&oldp2[i]==oldp2[j]) break;
						}
						if (j<loopcount) break;
					}
					if (i<loopcount-1)
						System.out.println("Samples repeat after loop "+j+" to loop "+i+".");
					else
						System.out.println("Try a larger number of loops to figure the lenght until the sample repeats.");
				}
				break;
			}
		}
		outWav.writeBytes("RIFF");
		outWav.writeInt(Integer.reverseBytes((size<<1)+36));
		outWav.writeBytes("WAVEfmt ");
		outWav.writeInt(Integer.reverseBytes(16));				//Size of header
		outWav.writeShort(Short.reverseBytes((short)1));		//PCM format, no compression
		outWav.writeShort(Short.reverseBytes((short)1));		//Mono
		outWav.writeInt(Integer.reverseBytes(samplerate));		//Sample rate
		outWav.writeInt(Integer.reverseBytes(samplerate<<1));	//Byte rate
		outWav.writeShort(Short.reverseBytes((short)2));		//BlockAligh
		outWav.writeShort(Short.reverseBytes((short)16));		//16-bit samples
		outWav.writeBytes("data");
		outWav.writeInt(Integer.reverseBytes(k<<5));
		for(int i=0; i<k; i++) {							//write samplebuffer to file
			for(int j=0; j<16; j++)
				outWav.writeShort(Short.reverseBytes(samples[i][j]));
		}
		inBrr.close();
		outWav.close();
		System.out.println("Done !");
	}

	static short[] DecodeBRR (byte[] Data) {						//Decode a string of BRR bytes
		int Filter = (Data[0] & 0x0c)>>2;
		int ShiftAmount = (Data[0]>>4) & 0x0F;						//Read filter & shift amount
		short[] out = new short[(Data.length-1)<<1];

		for(int i=0; i<Data.length-1; i++) {							//Loop for each byte
			DecodeSample((byte)(Data[i+1]&0xF0), ShiftAmount, Filter);		//Decode high nybble
			out[i<<1]=p1;
			DecodeSample((byte)(Data[i+1]<<4), ShiftAmount, Filter);		//Decode low nybble
			out[(i<<1)+1]=p1;
		}
		return out;
	}

	static void DecodeSample (byte s, int ShiftAmount, int Filter) {
		a = (byte)(s>>4);						//Fix numbers that should be negative
		a = a<<(ShiftAmount-1);					//Valid shift count
		if(ShiftAmount>0x0C)
				a = (short)((a>>14) & 0x7FF);	//Values "invalid" shift counts
		switch(Filter) {						//Different formulas for 4 filters
			case 1:
				a += p1 >> 1;
				a += (-p1) >> 5;
			case 0:
				break;
			
			case 2:
				a += p1;
				a += (-(p1 + (p1 >> 1))) >> 5;
				a -= p2 >> 1;
				a += p2 >> 5;
				break;
			
			default:
				a += p1;
				a += (-(p1 + (p1 << 2) + (p1 << 3))) >> 7;
				a -= p2 >> 1;
				a += (p2 + (p2 >> 1)) >> 4;;
				break;
		}

		p2 = p1;
		p1 = (short)(BRREncoder.clamp_16(a)*2);
	}

	static void printInstructions() {
		System.out.println("BRR decoder 2.0 (c) 2012 Bregalad special thanks to Kode54");
		System.out.println("Arguments : infile.brr outfile.wav -n1 -l0 -s32000");
		System.out.println("-n number of times to loop through the sample, default 1");
		System.out.println("-l loop start point (in BRR block units), default 0");
		System.out.println("-s output samplerate, default 32000");
		System.exit(1);
	}

	static void parseArg(String Arg) {
		if (Arg.charAt(0)=='-') {
			if(Arg.charAt(1)=='l')
				looppos = Integer.parseInt(Arg.substring(2));
			else if(Arg.charAt(1)=='n')
				loopcount = Integer.parseInt(Arg.substring(2));
			else if(Arg.charAt(1)=='s')
				samplerate = Integer.parseInt(Arg.substring(2));
			else printInstructions();
		}
		else printInstructions();
	}
}
