#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
void error(){}

// move into Struct
typedef struct
{
	int format;
	int numchannels;
	int SampleRate;
	int DataRate;
	int DataBlockSize;
	int BitsPerSample;
	int FormatChunkSize;
	int DataChunkSize;
	int MainChunkSize;
} WAV;

FILE *audiofile, *out;
WAV wav;

int ReadFormatChunk(FILE *file)
{
	// chunk size
	
	wav.FormatChunkSize = fgetc(file);
	wav.FormatChunkSize |= fgetc(file)<<8;
	wav.FormatChunkSize |= fgetc(file)<<16;
	wav.FormatChunkSize |= fgetc(file)<<24;
	
	// format code
	// 2 bytes
	wav.format = fgetc(file);
	wav.format |= fgetc(file)<<8;
	if (wav.format != 1)
		printf ("Not PCM!\n");
	
	printf ("Format Code: 0x%x\n", wav.format);
	// number of interleaved channels
	
	wav.numchannels = fgetc(file);
	wav.numchannels |= fgetc(file)<<8;
	
	printf("#Channels: %d\n", wav.numchannels);
	// Sampling Rate
	wav.SampleRate = fgetc(file);
	wav.SampleRate |= fgetc(file)<<8;
	wav.SampleRate |= fgetc(file)<<16;
	wav.SampleRate |= fgetc(file)<<24;
	
	printf("Sample Rate: %d\n",wav.SampleRate);
	// Data Rate
	wav.DataRate = fgetc(file);
	wav.DataRate |= fgetc(file)<<8;
	wav.DataRate |= fgetc(file)<<16;
	wav.DataRate |= fgetc(file)<<24;
	printf("Data Rate: %d\n",wav.DataRate);
	// Data Block Size (bytes)
	wav.DataBlockSize = fgetc(file);
	wav.DataBlockSize |= fgetc(file)<<8;
	
	// Bits per Sample
	wav.BitsPerSample = fgetc(file);
	wav.BitsPerSample |= fgetc(file)<<8;
	printf("BitsPerSample: %d\n", wav.BitsPerSample);
	
	if (wav.FormatChunkSize == 16)
	{
		return 0;
	}
	
	// Seek to end of this chunk
	wav.FormatChunkSize -= 16;
	fseek(file,wav.FormatChunkSize,SEEK_CUR);
	
	return 1;
}

int ReadDataChunk(FILE *file)
{
	signed short sample_data[16];
	signed short range_table[13][16];
	int deviation_table[13][16];
	int sumdeviation[13];
	unsigned char start_index=0;
	
	// must process rest of Data Chunk Header
	//int ChunkSize;
	wav.DataChunkSize = fgetc(file);
	wav.DataChunkSize |= fgetc(file)<<8;
	wav.DataChunkSize |= fgetc(file)<<16;
	wav.DataChunkSize |= fgetc(file)<<24;
	
	printf ("Data Chunk Size: 0x%x\n", wav.DataChunkSize);
	
	if (wav.format != 0x01)
	{
		printf("Wav is not PCM\n");
		printf("I cannot decode this! Quitting!\n");
		exit(1);
	}
	if (wav.BitsPerSample != 16)
	{
		printf("Wav is not 16-bit samples\n");
		printf("I cannot decode this! Quitting!\n");
		exit(1);
	}
	
	int remainder = (wav.DataChunkSize/2)%16;
	if (remainder > 0)
	{
		printf("Chunk not divisible by 16. Remainder = %d\n", remainder);
		
		remainder = 16-remainder;
		printf ("Going to Patch the beginning with %d samples of 0x0000\n", remainder);
	}
	char x;
	for (x=0; x<remainder;x++)
	{
		sample_data[x] = 0;
	}

	start_index=remainder;
	
	printf ("Encoding with Filter=0 for all...\n");
	while (wav.DataChunkSize > 0)
	{
		unsigned char x=0;
		//sample_data[0] |= fgetc(audiofile)<<8;
		// Now we have sample data in sample_data
		//printf("Sample %d = 0x%x = %d\n",x,sample_data[0],sample_data[0]);
		
		for (x=start_index; x<16; x++)
		{
			// don't need to check for EOF cause the sample SHOULD be 16 aligned.
			sample_data[x] = fgetc(audiofile);
			sample_data[x] |= fgetc(audiofile)<<8;
			
			wav.DataChunkSize-=2;
			//printf("Sample %d = 0x%x = %d\n",x,sample_data[x],sample_data[x]);
		}
		start_index=0;
		
		//
		int r;
		for (r=0;r<=12;r++)
		{
			for (x=0; x<16; x++)
			{
				signed short steps,min,max;
				steps = 1<<r;
				
				
				
				signed short mark=0;
				signed short smallest_mark=0;
				
				
				if (sample_data[x] == 0)
				{
					range_table[r][x] = 0; // nibs is (unoptimized) byte-wide entries for resultant nibbles
					deviation_table[r][x] = 0;
				}
				else if(sample_data[x] > 0)
				{
					unsigned short smallest_distance=0xffff;
					for (mark=0; mark <= 7; mark++)
					{
						int d1 = abs((steps*mark)-sample_data[x]);
						
						if (d1 < smallest_distance)
						{
							smallest_distance = d1;
							smallest_mark=mark;
						}
					}
					range_table[r][x] = smallest_mark;
					deviation_table[r][x] = smallest_distance;
					
				}
				else if (sample_data[x] < 0)
				{
					unsigned short smallest_distance=0xffff;
					for (mark=0; mark >= -8; mark--)
					{
						// find two marks closest X
						
						int d1 = abs((steps*mark)-sample_data[x]);
						//signed short d2 = abs((steps_to_take*mark+1)-sample_data[x]);
						
						if (d1 < smallest_distance)
						{
							smallest_distance = d1;
							smallest_mark=mark;
						}
					}
					range_table[r][x] = smallest_mark;
					deviation_table[r][x] = smallest_distance;
				}
				
			}
		}
		
		//int i,y;
		// calculate individual deviants
		for (r=0;r<13;r++)
		{
			sumdeviation[r] = 0;
		}
		for (r=0; r<13; r++)
		{
			for (x=0;x<16;x++)
			{
				sumdeviation[r] += deviation_table[r][x];
			}
		}
		int mindeviation=sumdeviation[0];
		int range=0;
		for (r=0;r<13;r++)
		{
			if (sumdeviation[r] < mindeviation)
			{
				mindeviation = sumdeviation[r];
				range=r;
			};
		}
		
		// now write the header, assemble packet, and write to brr file
		signed short header;
		signed short last_chunk=0;
		signed short filterbits=0;
		
		if (wav.DataChunkSize <= 0)
		{
			last_chunk = 1;
		}
		//make header byte
		header = ((range&0x0f)<<4) /*| (filterbits<<2)*/ | last_chunk;
		//printf("Header: 0x%x\n", header&0xff);
		fputc(header, out);
		//string nibbles together
		for (x=0; x<15; x+=2) //15 is right
		{
			// is the order correct?
			signed short byte = ( ((range_table[range][x]<<4)&0xf0) | (range_table[range][x+1]&0xf) )&0xff;
			/*( ((nibs[x+1]<<4)&0xf0) | ((nibs[x])&0x0f)); */ //
			//printf("Output byte: 0x%x\n", byte);
			fputc(byte,out);
			
		}
		
		
		
		//scanf("%c",&c);
		
	}
	
	printf("Encoding Complete!\n");
	return 0;
}

#define FMT_CHNK 0
#define DATA_CHNK 1
int ReadNextChunk(FILE *file)
{
	// Read ID (4 Bytes)
	char c[4];
	c[0] = fgetc(file);
	c[1] = fgetc(file);
	c[2] = fgetc(file);
	c[3] = fgetc(file);
	
	if (!strcmp("fmt ",&c[0]))
	{
		// we are in fmt chunk
		ReadFormatChunk(file);
		return FMT_CHNK;
	}
	if (!strcmp("data",&c[0]))
	{
		// we are in fmt chunk
		ReadDataChunk(file);
		return DATA_CHNK;
	}
	
}


int main(int argc, char **argv)
{
	if (sizeof(short) < 2)
	{
		char yn;
		printf ("unsigned short is < 2 bytes wide. Program will likely fail.\n");
		printf ("Continue? (y/n): ");
		scanf("%c\n", &yn);
		if (yn == 'n' || yn == 'N')
			exit(0);
	}
	// no error checks we sUCK
	char *out_filename;
	//char *out_filename;
	audiofile = fopen(argv[1], "rb");
	
	if (argc >= 3)
	{
		out = fopen(argv[2], "wb");
		if (!out)
		{
			printf ("Something wrong opening output file %s", argv[2]);
		}
	}
	else
	{
		char *ptr;
		ptr = argv[1];
		int i=0;
		bool no_extension = false;
		
		while (1)
		{
			if (ptr[++i] == '\0')
				break;
		}
		while (1)
		{
			if (ptr[--i] == '.')
				break;
			else if (i <= 0)
			{
				no_extension = true;
				break;
			}
		}
		
		if (no_extension == true)
		{
			out_filename = malloc(sizeof(char)*(strlen(argv[1])+5));
			strcpy(out_filename,argv[1]);
			out = fopen(strcat(out_filename,".brr"),"wb");
			free(out_filename);
		}
		else
		{
			out_filename = malloc(sizeof(char)*(i+4));
			int y;
			for (y=0; y<=i; y++)
			{
				out_filename[y] = argv[1][y];
			}
			out = fopen(strcat(out_filename,"brr"),"wb");
			free(out_filename);
		}
		
		if (!out)
		{
			printf("Something wrong opening Output File: %s\n",out_filename);
			exit(-1);
		}
	}
	
	if (audiofile)
	{
		// Let's Read Wav header
		char c;
		c = fgetc(audiofile);
		if (c != 'R')
			error();
		c = fgetc(audiofile);
		if (c != 'I')
			error();
		
		c = fgetc(audiofile);
		if (c != 'F')
			error();
		c = fgetc(audiofile);
		if (c != 'F')
			error();
		
		// Read chunk size
		//;
		wav.MainChunkSize = fgetc(audiofile);
		wav.MainChunkSize |= fgetc(audiofile)<<8;
		wav.MainChunkSize |= fgetc(audiofile)<<16;
		wav.MainChunkSize |= fgetc(audiofile)<<24;
		
		printf ("Main Chunk Size: 0x%x\n",wav.MainChunkSize);
		
		c = fgetc(audiofile);
		if (c != 'W')
			error();
		c = fgetc(audiofile);
		if (c != 'A')
			error();
		
		c = fgetc(audiofile);
		if (c != 'V')
			error();
		c = fgetc(audiofile);
		if (c != 'E')
			error();
		
		// Read Next Chunk
		// Could be anything , format chunk or Data chunk for instance
		int returncode;
		int processed[10];
		while (1)
		{
			returncode = ReadNextChunk(audiofile);
		
			if (returncode == FMT_CHNK)
			{
				// Do format processing here
				processed[FMT_CHNK] = 1;
			}
			else if (returncode == DATA_CHNK)
			{
				// finish up processing dATA
				if (processed[FMT_CHNK])
					break;
			}
		}
	}
		
	fclose(audiofile);
	fclose(out);
	//free(out_filename);
	return 0;
	
}
