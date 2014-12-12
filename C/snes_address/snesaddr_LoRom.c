#include <stdio.h>

unsigned long addr=0;
int input=1;

int main()
{

	printf ("1) Map SNES to LoRom ROM Address\n");
	printf ("2) Help\n");
	scanf("%d", &input);
	
	if (input == 1)
	{
		printf ("Enter SNES Address: 0x");
		scanf ("%lx", &addr);
	}
	
	
	if (input == 1)
	{
		if ( (addr >= 0x7e0000 && addr <= 0x7fffff))
		{
			printf ("This Address is not in the proper 32Mb mapped region\n");
			return 0;
		}
		
		unsigned long buffer = addr & 0xFF7FFF; // get rid of A15
		
		unsigned long high = buffer & 0xFF0000;
		unsigned long derp = (high & 0x7F0000)>>1;
		
		/*if (high & 0x800000)
		{
			derp |= 0x200000;
		}*/
		
		derp |= (addr & 0x7fff);
		printf ("\n32Mb ROM Address = 0x%06lx", derp);
	}
	else
	{
		printf ("This program automatically finds the corresponding LoROM ROM address from a SNES address\n\n");
	}
	
	return 0;
	
}