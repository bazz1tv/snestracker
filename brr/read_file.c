/* File Padder

// Pad beginning or End of file with X bytes of Y 

*/

// Usage: ./a.out filename #bytes byte

#include <stdio.h>

int main(int argc, char **argv)
{
	// no error checks we sUCK 
	FILE *audiofile, *out;
	short sample_data;
	
	audiofile = fopen(argv[1], "rb");
	out = fopen("derp.raw","wb");
	if (audiofile)
	{
		while ((sample_data = fgetc(audiofile)) != EOF)
		{
			sample_data = (fgetc(audiofile)<<8)|sample_data;
			printf ("0x%x, ", sample_data&0x0000ffff);
			fputc(sample_data,out);
			fputc(sample_data>>8,out);
			//fputc(sample_data>>8,out);
		}
	}
	fclose(audiofile);
	fclose(out);
	
	return 0;
	
}

