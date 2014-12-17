#include "TestSuite.h"

using namespace std;


/*TestSuite :: TestSuite()
{
	numtabs = 0;
}*/
namespace TS {

	cConsoleTest TC_console;
	cCollisionTest TC_collision;
	
	SDL_bool verbose=SDL_FALSE;
	int numtabs=0;

void Begin()
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
void TestComponents()
{
	
}

void tabprint()
{
	for (register Uint8 i=0; i < numtabs; i++)
		cout << "\t";
}

void testprint(string format, ...)
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

void testprintnts( string format, ...)
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

void TestAll()
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

int CollisionTest()
{
	return TC_collision.Test();
}




int ConsoleTest()
{
	return TC_console.Test();

}
	
// END NAMESPACE
}
