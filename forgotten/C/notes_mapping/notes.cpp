
// Currently All for Sample Rate 32Khz

// basic file operations
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
using namespace std;

struct Key
{
	//unsigned char keynum;
	string name;
	float freq;
};

string ushort2hex(unsigned int inshort)
{
  ostringstream oss (ostringstream::out);
  oss << setw(2) << setfill('0') << hex << (int)(inshort);
  return oss.str();
}


int main () 
{
	float SampleRate = 32000; // Hz
	Key notes[88];
  	ifstream freqfile, notesfile;
  	freqfile.open ("freq.txt");
  	notesfile.open("notes.txt");
  
  	//float buffer;
  	//string no
	
	cout << "Welcome" << endl;
	cout << "SampleRate (Hz): ";
	cin >> SampleRate;
  	for (int i=87; i>=0; i--)
  	{
		freqfile >> notes[i].freq;
		notesfile >> notes[i].name;
	}
  
	freqfile.close();
	notesfile.close();
	
	for (int base=0; base < 88; base++)
	{
		for (int interval=0; interval < 88; interval++)
		{
			// Formula F = F0 * P/$1000
			// F = notes[interval]
			// F0 = notes[base]
			// P = (4096F)/(F0)
			float Pf = ((SampleRate * 0.128))*(notes[interval].freq)/notes[base].freq;
			unsigned int P = floor(Pf+0.5);
			
			//if (base == 40-1)
			//{
				if ( P == 16384 )
					P = 16383;
				if (!(P > 16384))
					cout << "Base: " << notes[base].name << " " << base << " " << notes[base].freq << "Hz, to " << notes[interval].name << ", " << Pf << "Hz "<< "0x" << ushort2hex(P) << endl;
					//; // don't use this value
				//}
			
		}
		
		//cout << "Key " << i << ": " << keys[i].notename << " " << keys[i].freq << endl;
	}
	return 0;
}