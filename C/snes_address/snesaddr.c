#include <stdio.h>

unsigned long addr=0;
int input=1;

int main()
{

	printf ("1) 32Mb ROM\n");
	printf ("2) 64Mb ROM\n");
	printf ("3) Help\n");
	scanf("%d", &input);
	
	if (input == 2 || input == 1)
	{
		printf ("Enter SNES Address: 0x");
		scanf ("%lx", &addr);
	}
	
	if (input == 2)
	{
		if (addr < 0x400000 || (addr < 0xc00000 && addr >= 0x800000) )
		{
			printf ("This Address is not in the proper 64Mb mapped region\n");
			return 0;
		}
		if (addr >= 0x7e0000 && addr <= 0x7fffff)
		{
			printf ("System RAM is mapped here\n");
			return 0;
		}
	
		addr &= 0xBFFFFF;
		if (addr & 0x800000)
		{
			addr |= 0x400000;
			addr &= 0x7fffff;
		}
	
		printf ("\n64Mb ROM Address = 0x%06lx", addr);
	}
	else if (input == 1)
	{
		if ( (addr >= 0x400000 && addr <= 0x7fffff) || (addr >= 0xc00000 ) )
		{
			printf ("This Address is not in the proper 32Mb mapped region\n");
			return 0;
		}
		
		unsigned long buffer = addr & 0xFF7FFF; // get rid of A15
		
		unsigned long high = buffer & 0xFF0000;
		unsigned long derp = (high & 0x3F0000)>>1;
		
		if (high & 0x800000)
		{
			derp |= 0x200000;
		}
		
		derp |= (addr & 0x7fff);
		printf ("\n32Mb ROM Address = 0x%06lx", derp);
	}
	else
	{
		printf ("This program automatically finds the corresponding ROM address from a SNES address for the specific 96Mb Cartridge as depicted on my website: http://www.cs.umb.edu/~bazz/snes/96MbCart/\n\n");
		printf ("64Mb ROM is connected to $40-7d:0000-FFFF and $c0-ff:0000-ffff. Don't use any values outside of these specified ranges. This is because of the decoder logic that will shut off the 64Mb ROM in the areas not specified.\n\n 32Mb ROM is connected to 00-3f:8000-ffff and 80-bf:8000-ffff\n\n Once again only use the addresses in the range provided.\n"); 
		
	}
	
	return 0;
	
}