
// 440->432 differential conversion

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
	Key notes[13];
	Key notes440[13];
  	//ifstream freqfile, notesfile;
  	//freqfile.open ("freq.txt");
  	//notesfile.open("notes.txt");
  
  	//float buffer;
  	//string no
	
	notes[0].freq = 256;
	notes[1].freq = 270;
	notes[2].freq = 288;
	notes[3].freq = 300;
	notes[4].freq = 324;
	notes[5].freq = 337.5;
	notes[6].freq = 360;
	notes[7].freq = 384;
	notes[8].freq = 405;
	notes[9].freq = 432;
	notes[10].freq = 455.111111;
	notes[11].freq = 486;
	notes[12].freq = 512;
	
	notes440[0].freq = 261.626;
	notes440[1].freq = 277.183;
	notes440[2].freq = 293.665;
	notes440[3].freq = 311.127;
	notes440[4].freq = 329.628;
	notes440[5].freq = 349.228;
	notes440[6].freq = 369.994;
	notes440[7].freq = 391.995;
	notes440[8].freq = 415.305;
	notes440[9].freq = 440.000;
	notes440[10].freq = 466.164;
	notes440[11].freq = 493.883;
	notes440[12].freq = 523.251;
	
	cout << "Welcome" << endl;
	cout << "SampleRate (Hz): ";
	cin >> SampleRate;
  	for (int i=0; i<13; i++)
  	{
		float Pf = ((SampleRate * 0.128))*(notes[i].freq)/notes440[0].freq;
		unsigned int P = floor(Pf+0.5);
		
		//if (base == 40-1)
		//{
			if ( P == 16384 )
				P = 16383;
			if (!(P > 16384))
				cout << "Base: " << " "  << " " << notes[i].freq << "Hz, to " << notes440[i].freq << ", " << Pf << "Hz "<< "0x" << ushort2hex(P) << endl;
				//; // don't use this value
			//}
	}
  

	return 0;
}