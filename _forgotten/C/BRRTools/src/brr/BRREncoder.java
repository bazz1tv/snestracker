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

public class BRREncoder {
	static short p1, p2;
	static boolean[] FIRen;
	static int[] FIRstats;
	static byte[] BRRBuffer;
	static double resample;
	static char resample_type;
	static boolean loopflag;
	static int chans;
	static boolean automatic_samplerate;
	static int target_samplerate;
	static boolean wrap_en;
	static boolean fix_loop_en;
	static int loop_start;

	public static void main(String[] args) throws FileNotFoundException, IOException {
		loopflag = false; automatic_samplerate = false; wrap_en = true; fix_loop_en = false;
		resample = 1.0; resample_type = 'n';
		FIRen = new boolean[] {true, true, true, true};
		FIRstats = new int[] {0, 0, 0, 0};
		DataOutputStream outBrr;
		RandomAccessFile inWav;

		if(args.length<2) printInstructions();
		for(int i=2; i<args.length; i++)			//Parse all arguments
			parseArg(args[i]);
		outBrr=new DataOutputStream(new FileOutputStream(args[1]));	//Create output file
		inWav= new RandomAccessFile(args[0], "r");	//Input file

		byte loop;
		if(loopflag) loop = 0x02;					//Loop flag for all BRR blocks
		else loop = 0x00;

		BRRBuffer = new byte[9];					//9 bytes long buffer
		if (inWav.readInt()!=0x52494646) {			//"RIFF" letters
			System.out.println("Input file in unsuported format : \"RIFF\" block missing");
			System.exit(-1);
		}
		inWav.readInt();
		if (inWav.readInt()!=0x57415645 				//"WAVE letters
			|	inWav.readInt()!=0x666d7420) {			//"fmt " letters for sub-chunk1
			System.out.println("Input file in unsuported format : \"WAVEfmt\" block missing !");
			System.exit(-1);
		}
		int sc1size = Integer.reverseBytes(inWav.readInt());
		if(sc1size < 0x10									//Size of sub-chunk1 (header) must be at least 16
			|	Short.reverseBytes(inWav.readShort())!=1) {	//Must be in PCM format
			System.out.println("Input file in unsuported format : file must be uncompressed PCM !");
			System.exit(-1);
		}
		chans = Short.reverseBytes(inWav.readShort());		//Check how many channels
		if(chans!=1) System.out.println("Input is multi-channel : Will automatically be converted to mono.");

		int source_samplerate = Integer.reverseBytes(inWav.readInt());		//Read source sample rate
		inWav.readInt();											//Ignore byte rate
		short BlockAlign=Short.reverseBytes(inWav.readShort());		//Read block align and bits per sample numbers
		short BitPerSample=Short.reverseBytes(inWav.readShort());	
		if (BlockAlign != (BitPerSample*chans)>>3) {
			System.out.println("Block align problem");
			System.exit(-1);
		}
		inWav.skipBytes(sc1size-0x10);								//Skip possible longer header

		while(inWav.readInt()!=0x64617461)							//Skip other possible non-"data" blocks
			inWav.skipBytes(Integer.reverseBytes(inWav.readInt()));

		int Sub2Size = Integer.reverseBytes(inWav.readInt());		//Read sub-chunk 2 size
		short[] samples = new short[Sub2Size/BlockAlign];
		switch (BitPerSample) {
			case 8 : {
				short sample; byte ch;
				for(int i=0; i<Sub2Size/BlockAlign; i++) {
					sample=0;
					for(ch=0; ch<chans; ch++)
						sample +=(short)((inWav.readUnsignedByte()-0x80)/chans);		//Convert 8-bit samples to 16-bit
					samples[i]=(short)(sample<<8);
				}
			}break;
			case 16 : {
				byte ch;
				for(int i=0; i<Sub2Size/BlockAlign; i++) {
					samples[i]=0;
					for(ch=0; ch<chans; ch++)
						samples[i]+=(short)(Short.reverseBytes(inWav.readShort())/chans);
				}
			}break;
			default :
				System.out.println("Error : unsupported amount of bits per sample (8 or 16 are supported)");
				System.exit(-1);
		}

		if(automatic_samplerate)						//Set resample factor if auto samplerate mode
			resample = (double)source_samplerate/target_samplerate;	

		int new_loopsize=0;
		if(fix_loop_en) {
			double loopsize = (Sub2Size/BlockAlign - loop_start)/resample;
			if(loopsize<=16) {
				System.out.println("Error : The loop is too small for BRREncoder to resample it in an integer amount of BRR blocks.");
				System.exit(-1);
			}
			new_loopsize = (int)(loopsize)&~0xF;		//New loopsize is the multiple of 16 that comes before loopsize
			resample *= loopsize/new_loopsize;			//Adjust resampling
		}
		samples = resample(samples, resample_type);

		if ((samples.length & 0xF) != 0) {
			System.out.println("The Amount of PCM samples isn't a multiple of 16 !");
			System.out.println("The sample will be padded with " + (0x10-(samples.length&0x0F)) + " zeroes at the begining.");
			short[] a = new short[0x10-(samples.length & 0x0F)];
			samples = append (a, samples);
		}
		System.out.println("Size of file to encode : " + samples.length + " samples = " + (samples.length>>4) + " BRR blocks.");

		boolean initial_block = false;
		for (int i=0; i<16; i++)					//Initialization needed if any of the first 16 samples isn't zero
			initial_block |= samples[i]!=0;

		if(initial_block) {						//Write initial BRR block
				outBrr.write(new byte[] {loop, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
				System.out.println("Initial BRR block added.");
		}
		p1 = 0; p2 = 0;

		short[] samples2 = new short[16];
		for (int n=0; n<samples.length>>4; n++) {
			for (int i=0; i<16; i++)
				samples2[i]=samples[i+(n<<4)];				//Read 16 samples
			BRRBuffer = new byte[9];
			ADPCMBlockMash(samples2, n);					//Encode BRR block
			BRRBuffer[0] |= loop;							//Set the loop flag if needed
			if(n==(samples.length>>4)-1) BRRBuffer[0]|= 1;	//Set the end bit if we're on the last block
			outBrr.write(BRRBuffer);
		}
		System.out.println("Done !");
		if(fix_loop_en)
			if(initial_block)
				System.out.println("Position of the loop within the BRR sample : " +
						(samples.length-new_loopsize+16) + " samples = " + ((samples.length-new_loopsize+16)>>4) + " BRR blocks.");
			else
				System.out.println("Position of the loop within the BRR sample : " +
						(samples.length-new_loopsize) + " samples = " + ((samples.length-new_loopsize)>>4) + " BRR blocks.");

		for(int i=0; i<4; i++)
			if (FIRstats[i]>0) System.out.println("Filter "+i+" used on " + FIRstats[i] + " blocks.");
	}

	static void ADPCMBlockMash(short[] PCMData, int i) {
		int smin=0;
		int kmin=0;
		double dmin=Double.POSITIVE_INFINITY;
		for(int s=0; s<13; s++)
			for(int k=0; k<4; k++)
				if(FIRen[k]) {
					double d = ADPCMMash(s, k, PCMData, false);
					if (d < dmin)
					{
						kmin = k;		//Memorize the filter, shift values with smaller error
						dmin = d;
						smin = s;
					}
				}
		BRRBuffer[0] =(byte)((smin<<4)|(kmin<<2));
		ADPCMMash(smin, kmin, PCMData, true);
		FIRstats[kmin]++;
	}
	
	static double ADPCMMash(int shiftamount, int filter, short[] PCMData, boolean write) {
		int d;
		double d2=0.0;
		int vlin=0;
		int da, dp, c;
		short l1 = p1;
		short l2 = p2;
		int step = 1<<shiftamount;
		for(int i=0; i<16; i++) {
			switch(filter) {			//Compute linear prediction for filters
			case 1:
				vlin = l1 >> 1;
				vlin += (-l1) >> 5;
			case 0:
				break;
			
			case 2:
				vlin = l1;
				vlin += (-(l1 +(l1>>1)))>>5;
				vlin -= l2 >> 1;
				vlin += l2 >> 5;
				break;
			
			default:
				vlin = l1;
				vlin += (-(l1+(l1<<2) + (l1<<3)))>>7;
				vlin -= l2>>1;
				vlin += (l2+(l2>>1))>>4;
				break;
			}
			d = (PCMData[i]>>1) - vlin;		//Difference between linear prediction and current sample
			da = Math.abs(d);

			if (wrap_en && (da > 16384 && da < 32768)) {
				d = d - 32768 * ( d >> 24 );	// Take advantage of wrapping
			}

			dp = d + (step << 2) + (step >> 2);
			c = 0;
			if (dp > 0)
			{
				if (step > 1)
					c = dp /(step>>1);
				else
					c = dp<<1;
				if (c > 15)
					c = 15;
			}
			c -= 8;
			dp = (c<<(shiftamount-1));		/* quantized estimate of samp - vlin */
											/* edge case, if caller even wants to use it */
			if (shiftamount > 12 )
				dp = ( dp >> 14 ) & ~0x7FF;
			c &= 0x0f;						/* mask to 4 bits */
			l2 = l1;						/* shift history */
			l1 = (short)(clamp_16(vlin + dp)*2);
			d = PCMData[i]-l1;
			d2 += ((double)d)*d;			/* update square-error */

			if (write)						/* if we want output, put it in proper place */
				BRRBuffer[(i>>1)+1] |= c<<(4-((i&0x01)<<2));
		}
		if(write){
			p2 = l2;
			p1 = l1;
		}
		return d2;
	}

	static short clamp_16(int n)
	{
		if ((short)n != n)
			n =(short)(0x7FFF -(n>>24));
		return (short)n;
	}

	static void parseArg(String Arg) {
		if (Arg.charAt(0)=='-') {
			if(Arg.charAt(1)=='f') {
				if(Arg.length()<3) printInstructions();
				FIRen = new boolean[]{false, false, false, false};
				char c;
				for(int i=2; i<Arg.length(); i++) {		//Enable only specified filters
					c = Arg.charAt(i);
					switch(c) {
					case '0' : FIRen[0] = true;
							break;
					case '1' : FIRen[1] = true;
							break;
					case '2' : FIRen[2] = true;
							break;
					case '3' : FIRen[3] = true;
							break;
					default :
							printInstructions();
					}
				}
			}
			else if(Arg.charAt(1)=='w')						//Disable wrap trick command
				wrap_en = false;

			else if(Arg.charAt(1)=='r') {
				if (Arg.length()<4)	printInstructions();	//Resample command
				resample_type = Arg.charAt(2);
				resample = Double.parseDouble(Arg.substring(3));
				if (resample > 4.0 || resample <= 0.0) printInstructions();
			}
			else if(Arg.charAt(1)=='s') {					//Automatic resample command
				automatic_samplerate = true;
				resample_type = Arg.charAt(2);
				target_samplerate = Integer.parseInt(Arg.substring(3));
			}
			else if(Arg.charAt(1)=='l') {
				loopflag = true;							//Enable loop flag
				if(Arg.length()>2) {						//Enable loop-fix feature if a number is present in the arg
					loop_start = Integer.parseInt(Arg.substring(2));
					fix_loop_en = true;
				}
			}
			else printInstructions();
		}
		else printInstructions();
	}

	static void printInstructions() {
		System.out.println("BRR encoder 2.0 (c) 2012 Bregalad special tanks to Kode54");
		System.out.println("Arguments : infile.wav outfile.brr -l -w -f0123 -rc1.0 -sc32000");
		System.out.println("-l enable looping flag in the encoded BRR sample (default disabled)");
		System.out.println("If a number follows the -l flag, this is the loop point in samples." +
				"The data will be resampled so that the looped part of the sample is an integer # of BRR blocks.");
		System.out.println("-f enable filters for BRR blocks (default all enabled)");
		System.out.println("-w disable wrapping (encoded sample will be compatible with old/inacurate emus/SPC players)");
		System.out.println("-r resample input stream, followed by resample ratio (0.0 to 4.0)");
		System.out.println("-s automatically resample to get the specified samplerate (default 32kHz). (takes priority on -r).");
		System.out.println("Resampling interpolation type : -rn/-sn no interpolation, -rl/-sl linear, -rs/-ss sine, -rc/-sc cubic");
		System.exit(1);
	}

	static short[] resample(short[] samples, char type) {
		short[] out = new short[(int)(samples.length/resample)];
		switch(type) {
		case 'n':								//No interpolation
			for(int i=0; i<out.length; i++) {
				out[i] = samples[(int)(i*resample)];
			}
			break;
		case 'l':								//Linear interpolation
			for(int i=0; i<out.length; i++) {
				int a = (int)(i*resample);		//Whole part of index
				double b = i*resample-a;		//Fractional part of index
				if((a+1)==samples.length) out[i] = samples[a];	//This used only for the last sample
				else out[i] = (short)((1-b)*samples[a]+b*samples[a+1]);
			}
			break;
		case 's':								//Sine interpolation
			for(int i=0; i<out.length; i++) {
				int a = (int)(i*resample);
				double b = i*resample-a;
				double c = (1-Math.cos(b*Math.PI))/2;
				if((a+1)==samples.length) out[i] = samples[a];	//This used only for the last sample
				else out[i] = (short)((1-c)*samples[a]+c*samples[a+1]);
			}
			break;
		case 'c':										//Cubic interpolation
			samples = append(samples[0], samples);		//Duplicate first and last sample for interpolation
			samples = append(samples, samples[samples.length-1]);
			samples = append(samples, samples[samples.length-1]);
			for(int i=0; i<out.length; i++) {
				int a = (int)(i*resample);
				
				double b = i*resample-a;
				double b2 = b*b;
				double b3 = b2*b;
				double a0 = samples[a+3]-samples[a+2]-samples[a]+samples[a+1];
				double a1 = samples[a]-samples[a+1]-a0;
				double a2 = samples[a+2]-samples[a];
				out[i] =(short)(b3*a0 + b2*a1 + b*a2 + samples[a+1]);
			}
			break;
		default :
			printInstructions();
		}
		return out;
	}

	static short[] append(short[] t1, short[] t2) {		//Append 2 short arrays
		short[] a = new short[t1.length + t2.length];
		for(int i=0; i<t1.length; i++) {
			a[i]=t1[i];
		}
		for(int i=0; i<t2.length; i++) {
			a[t1.length+i]=t2[i];
		}
		return a;
	}

	static short[] append(short t1, short[] t2) {		//Append a short at the start of an array
		short[] a = new short[1+t2.length];
		a[0] = t1;
		for(int i=0; i<t2.length; i++) {
			a[1+i]=t2[i];
		}
		return a;
	}
	
	static short[] append(short[] t1, short t2) {		//Append a short to the end of an array
		short[] a = new short[t1.length + 1];
		for(int i=0; i<t1.length; i++) {
			a[i]=t1[i];
		}
		a[t1.length] = t2;
		return a;
	}
	
	static void PrintTbl(byte[] t){
		for(int i=0; i<t.length;i++) {
			System.out.print("$"+Integer.toHexString(t[i]&0xFF)+" ");
		}
		System.out.print("\n");
	}

	static void PrintTbl(short[] t){
		for(int i=0; i<t.length;i++) {
			System.out.print("$"+Integer.toHexString(t[i]&0xFFFF)+" ");
		}
		System.out.print("\n");
	}
}
