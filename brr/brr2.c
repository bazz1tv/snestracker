#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	// no error checks we sUCK 
	FILE *audiofile, *out;
	signed short sample_data[16];
	signed short range=0;
	signed short maxrange=0;
	signed short nibs[16];
	signed short prev_sample=0;
	
	audiofile = fopen(argv[1], "rb");
	out = fopen("derp.raw","wb");
	if (audiofile)
	{
		while ((sample_data[0] = fgetc(audiofile)) != EOF)
		{
			unsigned char x=0;
			sample_data[0] |= fgetc(audiofile)<<8;
			// Now we have sample data in sample_data
			printf("Sample %d = 0x%x = %d\n",x,sample_data[0],sample_data[0]);
			char derp;
			//std::cin >> derp;
			
			for (x=1; x<16; x++)
			{
				// don't need to check for EOF cause the sample SHOULD be 16 aligned.
				sample_data[x] = fgetc(audiofile);
				sample_data[x] |= fgetc(audiofile)<<8;
			}
			
			// now we have 16 samples
			// now SEARCH FOR PEAK RANGE
			maxrange = range = 0;
			signed int lowest=0, highest=0;
			
			int d = abs(sample_data[0] - prev_sample);
			
			if (prev_sample > sample_data[0])
				d = -d;
			if (d < lowest)
				lowest = d;
			if (d > highest)
				highest = d;
			
			for (x=1; x<15; x++)
			{
				int d = abs(sample_data[x] - sample_data[x-1]);
			
				if (sample_data[x-1] > sample_data[x])
					d = -d;
				if (d < lowest)
					lowest = d;
				if (d > highest)
					highest = d;
				
				/*if (sample_data[x] >= -8 && sample_data[x] <= 7)
				{
					range = 0;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -16 && sample_data[x] <= 14)
				{
					range = 1;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -32 && sample_data[x] <= 28)
				{
					range = 2;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -64 && sample_data[x] <= 56)
				{
					range = 3;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -128 && sample_data[x] <= 112)
				{
					range = 4;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -256 && sample_data[x] <= 224)
				{
					range = 5;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -512 && sample_data[x] <= 448)
				{
					range = 6;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -1024 && sample_data[x] <= 896)
				{
					range = 7;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -2048 && sample_data[x] <= 1792)
				{
					range = 8;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -4096 && sample_data[x] <= 3584)
				{
					range = 9;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -8192 && sample_data[x] <= 7168)
				{
					range = 10;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -16384 && sample_data[x] <= 14336)
				{
					range = 11;
					if (range > maxrange)
						maxrange = range;
				}
				else if (sample_data[x] >= -32768 && sample_data[x] <= 28672)
				{
					range = 12;
					if (range > maxrange)
						maxrange = range;
				}
				
				printf("range = %d\n", range);*/
				
					
				
				//cin>>derp;
			}
			printf ("Peak range value is: %d\n", maxrange);
			
			signed short steps_to_take;
			signed short min,max;
		
			if (lowest >= -8 && highest <= 7)
			{
				min = -8; max = 7;
				steps_to_take = 1;
			}
			else if (lowest >= -16 && highest <= 14)
			{
				min = -16; max = 14;
				steps_to_take =2;
			}
			else if (lowest >= -32 && highest <= 28)
			{
				min = -32; max = 28;
				steps_to_take = 4;
			}
			else if (lowest >= -64 && highest <= 56)
			{
				min = -64; max = 56;
				steps_to_take = 8;
			}
			else if (lowest >= -128 && highest <= 112)
			{
				min = -128; max = 112;
				steps_to_take = 16;
			}
			else if (lowest >= -256 && highest <= 224)
			{
				min = -256; max = 224;
				steps_to_take = 32;
			}
			else if (lowest >= -512 && highest <= 448)
			{
				min = -512; max = 448;
				steps_to_take = 64;
			}
			else if (lowest >= -1024 && highest <= 896)
			{
				min = -1024; max = 896;
				steps_to_take = 128;
			}
			else if (lowest >= -2048 && highest <= 1792)
			{
				min = -2048; max = 1792;
				steps_to_take = 256;
			}
			else if (lowest >= -4096 && highest <= 3584)
			{
				min = -4096; max = 3584;
				steps_to_take = 512;
			}
			else if (lowest >= -8192 && highest <= 7168)
			{
				min = -8192; max = 7168;
				steps_to_take = 1024;
			}
			else if (lowest >= -16384 && highest <= 14336)
			{
				min = -16384; max = 14336;
				steps_to_take = 2048;
			}
			else if (lowest >= -32768 && highest <= 28672)
			{
				min = -32768; 
				max = 28672;
				steps_to_take = 4096;
			}
			
			
			// Compute steps from 0 for first guy
			x=0;
			signed short diff1 = abs(sample_data[0] - prev_sample);
			if (prev_sample > sample_data[x])
				diff1 = -diff1;
			
			if (diff1 == 0)
			{
				nibs[0] = 0;
				//break;
			}
			else if(diff1 > 0)
			{
				for (mark=0; mark < 7; mark++)
				{
					// find two marks closest X
					if (diff1 >= (steps_to_take*mark) && diff1 <= (steps_to_take*(mark+1)))
					{
						signed short d1 = abs((steps_to_take*mark)-diff1);
						signed short d2 = abs((steps_to_take*mark+1)-diff1);
					
						if (d1 > d2)
						{
							nibs[x] = mark+1;
						}
						else
						{
							nibs[x] = mark;
						}
						break;
					}
				}
			}
			else if (diff1 < 0)
			{
				for (mark=0; mark > -8; mark--)
				{
					// find two marks closest X
					if (diff1 <= (steps_to_take*mark) && diff1 >= (steps_to_take*(mark-1)))
					{
						int d1 = abs((steps_to_take*mark)-diff1);
						int d2 = abs((steps_to_take*mark-1)-diff1);
					
						if (d1 > d2)
						{
							nibs[x] = mark-1;
						}
						else
						{
							nibs[x] = mark;
						}
						break;
					}
				}
			}	
			
							
			for (x=1; x<16; x++)
			{
				
				signed short diff = abs(sample_data[x] - sample_data[x-1]);
				if (sample_data[x-1] > sample_data[x])
					diff = -diff;
				
				if (diff == 0)
				{
					nibs[x] = 0;
					//break;
				}
				else if(diff > 0)
				{
					for (mark=0; mark < 7; mark++)
					{
						// find two marks closest X
						if (diff >= (steps_to_take*mark) && diff <= (steps_to_take*(mark+1)))
						{
							signed short d1 = abs((steps_to_take*mark)-diff);
							signed short d2 = abs((steps_to_take*mark+1)-diff);
						
							if (d1 > d2)
							{
								nibs[x] = mark+1;
							}
							else
							{
								nibs[x] = mark;
							}
							break;
						}
					}
				}
				else if (diff < 0)
				{
					for (mark=0; mark > -8; mark--)
					{
						// find two marks closest X
						if (diff <= (steps_to_take*mark) && diff >= (steps_to_take*(mark-1)))
						{
							int d1 = abs((steps_to_take*mark)-diff);
							int d2 = abs((steps_to_take*mark-1)-diff);
						
							if (d1 > d2)
							{
								nibs[x] = mark-1;
							}
							else
							{
								nibs[x] = mark;
							}
							break;
						}
					}
				}	
				
			}
			
			
			
			// now we have max range
			
			
			
			
			signed short header;
			signed short last_chunk=0;
			signed short filterbits=0;
			char c;
			if ((c=fgetc(audiofile)) == EOF)
			{
				last_chunk = 1;
			}
			ungetc(c,audiofile);
		
			//make header byte
			header = ((maxrange&0x0f) <<4) /*| (filterbits<<2)*/ | last_chunk;
			printf("Header: 0x%x\n", header&0xff);
			fputc(header, out);
			//string nibbles together
			for (x=0; x<15; x+=2) //15 is right
			{
				signed short byte = ( ((nibs[x]<<4)&0xf0) | (nibs[x+1]&0xf) )&0xff;
					/*( ((nibs[x+1]<<4)&0xf0) | ((nibs[x])&0x0f)); */ // 
				printf("Output byte: 0x%x\n", byte);
				fputc(byte,out);
					
			}
			
			prev_sample = sample_data[15];
			
		}
	}
	fclose(audiofile);
	fclose(out);
	
	return 0;
	
}
