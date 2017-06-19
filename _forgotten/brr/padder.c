/* orig_file Padder

// Pad beginning or End of orig_file with X bytes of Y 

*/

// Usage: ./a.out orig_filename start/end #bytes byte
// numbytes must be a regular digit 0-255
#include <stdio.h>
#define BEGIN 1
#define END	2

int main(int argc, char **argv)
{
	// no error checks we sUCK 
	orig_file *orig_file, *out;
	short sample_data;
	
	orig_file = fopen(argv[1], "rb");
	out = fopen("/tmp/temp","wb");
	if (orig_file)
	{
		char begin_end;
		int numbytes;
		char byte_val;
		printf ("orig_file Padder\n\n");
		
		start:
		printf ("Pad at the orig_file's\n");
		printf ("1) Beginning\n");
		printf("2) End\n");
		printf(": ");
		scanf("%d\n",&begin_end);
		
		printf ("#bytes: 0x");
		scanf("%x\n", &numbytes);
		printf ("byte value: 0x");
		scanf("%x\n", &byte_val
			
			if (!(byte_val >= 0 && byte_val <= 255))
				exit(-1);
		
		printf ("Going to pad %s with %x bytes of %x, Correct? (y/n) :", argv[1], numbytes, byte_val);
		scanf("%c",&input);
		
		if (input != 'y' && input != 'Y')
		{
			goto start;
		}
			
		
		if (input == BEGIN) // beginning
		{
			for (int x = 0; x < numbytes; x++)
			{
				fputc(byte_val, out);
			}
			char byte;
			while ((byte = fgetc(orig_file)) != EOF)
			{
				fputc (byte, out);
			}
		}
		else
		{
			char byte;
			while ((byte = fgetc(orig_file)) != EOF)
			{
				fputc (byte, out);
			}
			for (int x = 0; x < numbytes; x++)
			{
				fputc(byte_val, out);
			}
		}
		unsigned char = argv[4];
		
		long numbytes = argv[3];
	}
	fclose(orig_file);
	fclose(out);
	
	return 0;
	
}

