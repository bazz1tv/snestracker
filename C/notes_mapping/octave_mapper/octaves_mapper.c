#include <stdio.h>

unsigned short values[] = { 0x1000, 0x10f0, 0x11f0, 0x1300, 0x1430, 0x1560, 0x16a0, 0x1800, 0x1960, 0x1af0, 0x1c80, 0x1e30 };

int main()
{
	unsigned char shift=1;
	
	
	// up one octave: 
	printf ("; Octave +%d\n",shift);
	for (int i=0; i < 12; i++)
	{
		printf ("%x - %x\n", values[i], values[i]<<shift);
	}
	
	// SNES CANNOT PERFORM THIS OCTAVE
	/*shift++;
	// up 2 octave: 
	printf ("; Octave +%d\n",shift);
	for (int i=0; i < 12; i++)
	{
		printf ("%x - %x\n", values[i], values[i]<<shift);
	}*/
		
		// Down 12 Octaves
	for (shift=1; shift < 12; shift++)
	{
		printf ("; Octave -%d\n",shift);
		for (int i=0; i < 12; i++)
		{
			printf ("%x - %x\n", values[i], values[i]>>shift);
		}
	}


	return 0;
}