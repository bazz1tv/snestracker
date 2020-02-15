#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *fp;
	long long size_bytes=0;
	
	fp = fopen(argv[1], "rb");
	fseek(fp, 0L, SEEK_END);
	size_bytes = ftell(fp);
	
	printf ("%s is 0x%llx bytes\n", argv[1], size_bytes);
	fclose(fp);
	return 0;
}
