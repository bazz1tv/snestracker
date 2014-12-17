#include "TestSuite.h"
#include <stdio.h>
#include <iostream>
#include "Os.h"
#include <string>

using namespace std;

void PrintTestNumber(int &testnum)
{
	cout << "Test" << testnum++ << ": ";
}

TestSuite :: TestSuite()
{
	numtabs = 0;
}


void TestSuite :: Begin()
{
	int input;
	
	cout << "Test Mode!\n----------" << endl;
	
	cout << "Make a Selection" << endl;
	cout << "1) Test All" << endl;
	cout << "2) Test a specific Component" << endl;
	cout << " : ";
	
	cin >> input;
	
	cout << "" << endl;
	
	if (input == 1)
		TestAll();
	else if (input == 2)
	{
		TestComponents();
	}
}

// Stub
void TestSuite::TestComponents()
{
	
}

void TestSuite :: tabprint()
{
	for (register Uint8 i=0; i < numtabs; i++)
		cout << "\t";
}

void TestSuite :: testprint(string format, ...)
{
	//SDL_bool finished = SDL_FALSE;
	char* tabs = (char*) SDL_malloc(sizeof(char)*numtabs+1);
	register Uint8 i;
	for (i=0; i < numtabs; i++)
	{
		tabs[i] = '\t';
	}
	tabs[i] = '\0';
	
	string str = tabs;
	//str.append(tabs);
	
	
	size_t oldpos=0;
	size_t posnewline=0;
	while (1)
	{
		posnewline = format.find('\n', posnewline);
		// cout << "posnewline = %ld\n",posnewline);
		str.append(format.substr(oldpos, posnewline));
		if (posnewline == string::npos)
		{
			break;
		}
		else if (posnewline != format.length()-1)
		{
			str.append("\n");
			str.append(tabs);
			oldpos = ++posnewline;
		}
		else if (posnewline == format.length()-1)
		{
			//str.append("" << endl;
			//str.append(tabs);
			break;
		}
	}
	
	//str.append(formatstr);
	
	va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, str.c_str(), argptr);
    va_end(argptr);
	
	SDL_free(tabs);
}

void TestSuite :: testprintnts( string format, ...)
{
	//SDL_bool finished = SDL_FALSE;
	char* tabs = (char*) SDL_malloc(sizeof(char)*numtabs+1);
	register Uint8 i;
	for (i=0; i < numtabs; i++)
	{
		tabs[i] = '\t';
	}
	tabs[i] = '\0';
	
	string str = "";
	//str.append(tabs);
	
	size_t oldpos=0;
	size_t posnewline=0;
	while (1)
	{
		posnewline = format.find('\n', posnewline);
		// cout << "posnewline = %ld\n",posnewline);
		str.append(format.substr(oldpos, posnewline));
		if (posnewline == string::npos)
		{
			break;
		}
		else if (posnewline != format.length()-1)
		{
			str.append("\n"); // << endl;
			str.append(tabs);
			oldpos = ++posnewline;
		}
		else if (posnewline == format.length()-1)
		{
			str.append("\n"); // << endl;
			//str.append(tabs);
			break;
		}
	}
	
	//str.append(formatstr);
	
	va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, str.c_str(), argptr);
    va_end(argptr);
	
	SDL_free(tabs);
}

void TestSuite :: TestAll()
{
	if (!CollisionTest())
		cout << "Collision Test Successful!\n" << endl;
	else cout << "Collision Test Failed!\n" << endl;
	
	if (!ConsoleTest())
	{
		cout << "Console Test Successful!!\n" << endl;
	}
	else cout << "Console Test Failed!\n" << endl;
}

int TestSuite :: CollisionTest()
{
	// All that really has to be done here to Checking for different rects in RectIntersect
	SDL_Rect r1,r2;
	int testnum=1;
	
	cout << "Collision Test\n---------------" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(30,0,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,20,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,20,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,19,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(18,18,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(10,10,10,10);
	r2 = SetRect(8,0,20,30);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,20,20,20);
	r2 = SetRect(20,0,20,20);
	if (RectIntersect(&r1, &r2))
	{
		cout << " ERROR: Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	cout << "Test" << testnum++ << ": ";
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,18,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		cout << " ERROR: NO Rect Intersect!" << endl;
		return -1;
	}
	cout << "OK" << endl;
	
	numtabs--;
	return 0;
}


int TestSuite::TestCommand(string cmdstr)
{
	cCMD *cmd = pConsole->FindMatch(cmdstr);
	if (cmd == NULL)
	{
		cout << "Test Failed" << endl;
		return -1;
	}
	else
	{
		for (Uint8 cmdcount=0; cmdcount < cmd->command.objcount; cmdcount++)
		{
			if (cmdstr == *cmd->command.objects[cmdcount])
			{
				cout << "OK" << endl;
				return 0;
			}
		}
		
		cout << "Got a command, but not the right one! Fail" << endl;
		return -2;
		
	}
	
	
}
int TestSuite::ConsoleTest()
{
	cout << "Console Test\n---------------" << endl;
	
	int rval=0;
	int testnum=0;
	
	/*try {
		Console = new cConsole();
	} catch (int e)
	{
		switch (e)
		{
			case 1:
			{
				cout << "Exception Caught: IMan not loaded yet!" << endl;
				return 1;
			}
			case 2:
			{
				cout << "Exception Caught: pFont not loaded yet!" << endl;
				return 2;
			}
		}
	}*/
	
	
	
	// First let's make sure the Command Parse is working correctly
	PrintTestNumber(testnum);
	cout << "Testing LS: ";
	TestCommand("ls");
	//cmdstr = "ls";
	//cmd = Console->FindMatch("ls");
	
	// Test for Correct Command Parsing
	// A command that takes parameters
/*		Should test for correct format of parameters
		Test for correct parameter value in the context of the function
			if incorrect value, make best decision
	*/
	// Call functions to test each command
	// or whatever
	/*
	SDL_bool clearcon( string &str );	///< Clears the console
	 // nothing to test here
	SDL_bool loadmap( string &str );	///< Load a new Level from @link LEVEL_DIR @endlink
	SDL_bool SetMx( string &str );		///< Set Meatball's X Coordinate
	SDL_bool SetMy( string &str );		///< Set Meatball's Y Coordinate
	SDL_bool SetMxy( string &str );		///< Set Meatball (X,Y) coordinate
	SDL_bool play( string &str );		///< Play a song from the @link MUSIC_DIR @endlink
	SDL_bool QuitAll( string &str);		///< Quit the Game
	SDL_bool ShowFPS( string & str);	///< Toggle FPS framerate to be displayed
	SDL_bool help( string &str );		///< bring up help on all commands or specific command
	SDL_bool soundVol(string &str);		///< change the volume of a sound (0-128)
	SDL_bool allSoundsVol(string &str);	///< Change the volume of All sounds (0-128)
	SDL_bool musicVol(string &str);		///< Change the music volume (0-128)
	SDL_bool cd(string &str);			///< change the current directory
	SDL_bool ls(string &str);			///< list the directory
	SDL_bool SetScreenScaleXY(string &str);
	SDL_bool SetScreenScaleX(string &str);
	SDL_bool SetScreenScaleY(string &str);
	SDL_bool SaveLevel(string &str);
	SDL_bool camx(string &str);
	SDL_bool camy(string &str);
	*/
	
	
end:
	delete pConsole;
	
	return rval;
}
