#include <stdio.h>

void main(int argc,char **argv)
{
	FILE *fp = fopen(argv[1], "rb");
	long count,size;
	unsigned short in,sum;

	//int
     //fseek(FILE *stream, long offset, int whence);
	//SEEK_SET
	// SEEK_SET
	fseek(fp,0L,SEEK_END);
	size = ftell(fp);
	rewind(fp);

	sum=0;
	for (count =0; count < size; count++)
	{
		in = fgetc(fp);
		if (count >= 0xffdc && count <= 0xffdf)
			continue;
		else sum += in;
	}

	sum += 2*0xff;

	printf ("check sum = 0x%x\n", sum);
	printf ("inverted checksum = 0x%x\n", sum^0xFFFF);
}
