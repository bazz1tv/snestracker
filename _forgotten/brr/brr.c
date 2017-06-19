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
			for (x=0; x<16; x++)
			{
				printf ("sample_data[x] = 0x%x = %d\n", sample_data[x],sample_data[x]);
				
				if (sample_data[x] >= -8 && sample_data[x] <= 7)
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
				else if (sample_data[x] > -32768 && sample_data[x] <= 28672)
				{
					range = 12;
					if (range > maxrange)
						maxrange = range;
				}
				
				printf("range = %d\n", range);
				
				
				//cin>>derp;
			}
			printf ("Peak range value is: %d\n", maxrange);
			
			signed short steps_to_take;
			signed short min,max;
		
			if (maxrange == 0)
			{
				min = -8; max = 7;
				steps_to_take = 1;
			}
			else if (maxrange == 1)
			{
				min = -16; max = 14;
				steps_to_take =2;
			}
			else if (maxrange == 2)
			{
				min = -32; max = 28;
				steps_to_take = 4;
			}
			else if (maxrange == 3)
			{
				min = -64; max = 56;
				steps_to_take = 8;
			}
			else if (maxrange == 4)
			{
				min = -128; max = 112;
				steps_to_take = 16;
			}
			else if (maxrange == 5)
			{
				min = -256; max = 224;
				steps_to_take = 32;
			}
			else if (maxrange == 6)
			{
				min = -512; max = 448;
				steps_to_take = 64;
			}
			else if (maxrange == 7)
			{
				min = -1024; max = 896;
				steps_to_take = 128;
			}
			else if (maxrange == 8)
			{
				min = -2048; max = 1792;
				steps_to_take = 256;
			}
			else if (maxrange == 9)
			{
				min = -4096; max = 3584;
				steps_to_take = 512;
			}
			else if (maxrange == 10)
			{
				min = -8192; max = 7168;
				steps_to_take = 1024;
			}
			else if (maxrange == 11)
			{
				min = -16384; max = 14336;
				steps_to_take = 2048;
			}
			else if (maxrange == 12)
			{
				min = -32768; 
				max = 28672;
				steps_to_take = 4096;
			}
			
			
			for (x=0; x<16; x++)
			{
				
				
			
				
				
					signed short current_step = 0;
					signed short step_above = current_step+steps_to_take;
					signed short step_below = current_step-steps_to_take;
					signed short mark=0;
					
					if (sample_data[x] == 0)
					{
						nibs[x] = 0;
						break;
					}
					else if(sample_data[x] > 0)
					{
						while (1)
						{
							if (mark == 7)
								break;
							
							if(abs(current_step - step_above) <= abs(current_step - step_below))
							{
								nibs[x] = mark+1;
							}
						}
					}
				
					//
					printf ("sample_data[%d] = %d\n", x,sample_data[x]);
					while (1)
					{
						printf ("current_step = %d, mark = %d\n", current_step,mark);
						
						/*if (sample_data[x] < current_step && mark == -8)
						{
							nibs[x] = mark;
							break;
						}*/
						if (sample_data[x] == current_step /*&& mark != 0*/ )
						{
							printf ("Selected step = %d, mark = %d, for sample %d\n",current_step, mark, sample_data[x]);
							nibs[x] = mark;
							break;
						}
						else if (sample_data[x] < current_step)
						{
							signed short d1,d2;
							d1 = abs(sample_data[x] - previous_step);
							d2 = abs(sample_data[x] - current_step);
							
							if (d1 > d2)
							{
								printf ("Selected step = %d, mark = %d, for sample %d\n",current_step-steps_to_take, mark-1, sample_data[x]);
								nibs[x] = mark-1;
							}
							else
							{
								printf ("Selected step = %d, mark = %d, for sample %d\n",current_step-steps_to_take, mark-1, sample_data[x]);
								nibs[x] = mark;
							}
							break;
						}
						
						else if (sample_data[x] > current_step && mark == 7)
						{
							printf ("Selected step = %d, mark = %d, for sample %d\n",current_step, mark, sample_data[x]);
							nibs[x] = mark;
							break;
						}
						
						mark++;
						previous_step = current_step;
						current_step += steps_to_take;
				
						
						if (current_step > max)
						{
							printf("shouldn;t happen\n");
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
			//cin >> derp;
			
			//fputc(sample_data,out);
			//fputc(sample_data>>8,out);
			//cin >> derp;
		}
	}
	fclose(audiofile);
	fclose(out);
	
	return 0;
	
}
