#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *fp = fopen(argv[2],"r");
	char a;
	
	if (!strcmp(argv[1], "cpu"))
		goto cpu;
	else if (!strcmp(argv[1], "apu"))
		goto apu;
	
cpu:
	while((a = fgetc(fp)) != EOF)
	{
		char b,c;
		if (a == '\n')
		{
			if ((b = fgetc(fp)) == '\t')
			{
				if ((c = fgetc(fp)) == '\t')
					printf ( " | $(CPU_OBJDIR)\n\t\t" );
				else if (c == EOF) { printf("%c%c",a,b); break; }
				else printf("%c%c%c", a,b,c);
			}
			else if (b == EOF) { printf("%c",a); break; }
			else printf("%c%c",a,b);
		}
		else printf("%c",a);
	}
	fclose(fp);
	return 0;
	
apu:
	while((a = fgetc(fp)) != EOF)
	{
		char b,c;
		if (a == '\n')
		{
			if ((b = fgetc(fp)) == '\t')
			{
				if ((c = fgetc(fp)) == '\t')
					printf ( " | $(APU_OBJDIR)\n\t\t" );
				else if (c == EOF) { printf("%c%c",a,b); break; }
				else printf("%c%c%c",a,b,c);
				
			}
			else if (b == EOF) { printf("%c",a); break; }
			else printf("%c%c",a,b);
			
		}
		else printf("%c",a);
	}
	fclose(fp);
	return 0;
}
