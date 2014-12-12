#include <stdio.h>

int main(int argc,char **argv)
{
	FILE *fp = fopen(argv[1], "rb");
	FILE *out = fopen("fix.fig", "wb");
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
	
	rewind(fp);	
	for (count=0; count < size; count++)
	{
		in = fgetc(fp);
		if (count == 0xffdc)
		{
			unsigned char lo,hi;
			lo = (sum&0xff);
			lo ^= 0xff; 
			hi = (sum&0xff00)>>8;
			hi ^= 0xff;
			fputc(lo, out);
			fputc(hi,out);	
			fgetc(fp);
			count++;
		}
		else if (count == 0xffde)
		{
			unsigned char lo,hi;
			lo = sum&0xff;
			hi = (sum&0xff00)>>8;
			fputc(lo, out);
			fputc(hi,out);
			fgetc(fp);
			count++;
		}
		else
		{
			fputc(in, out);
		}
	}
	//fseek(fp,OL,0xffdc);
	fclose(fp);
	fclose(out);
	
	return 0;
}
