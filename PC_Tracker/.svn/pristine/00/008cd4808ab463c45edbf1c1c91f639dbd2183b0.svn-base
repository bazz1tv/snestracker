#include "TestSuite.h"
#include <stdio.h>
#include <iostream>
#include "Os.h"
#include <string>

using namespace std;

TestSuite :: TestSuite()
{
	numtabs = 0;
}


void TestSuite :: Begin()
{
	int input;
	
	printf ("Test Mode!\n----------\n");
	
	printf("Make a Selection\n");
	printf("1) Test All\n");
	printf("2) Test a specific Component\n");
	printf(" : ");
	
	cin >> input;
	
	printf("\n");
	
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
		printf("\t");
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
		// printf("posnewline = %ld\n",posnewline);
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
			//str.append("\n");
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

void TestSuite :: testprintnts(string format, ...)
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
		// printf("posnewline = %ld\n",posnewline);
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
			str.append("\n");
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
	CollisionTest();
}

int TestSuite :: CollisionTest()
{
	// All that really has to be done here to Checking for different rects in RectIntersect
	SDL_Rect r1,r2;
	int testnum=1;
	
	testprint("Collision Test\n---------------\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(30,0,20,20);
	if (RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: Rect Intersect!\n");
		return -1;
	}
	testprintnts("OK\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,20,20,20);
	if (RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: Rect Intersect!\n");
		return -1;
	}
	testprintnts ("OK\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,20,20,20);
	if (RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: Rect Intersect!\n");
		return -1;
	}
	testprintnts ("OK\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,19,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: NO Rect Intersect!\n");
		return -1;
	}
	testprintnts ("OK\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(18,18,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: NO Rect Intersect!\n");
		return -1;
	}
	testprintnts ("OK\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(10,10,10,10);
	r2 = SetRect(8,0,20,30);
	if (!RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: NO Rect Intersect!\n");
		return -1;
	}
	testprintnts ("OK\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(0,20,20,20);
	r2 = SetRect(20,0,20,20);
	if (RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: Rect Intersect!\n");
		return -1;
	}
	testprintnts ("OK\n");
	
	testprint("Test%d: ", testnum++);
	r1 = SetRect(0,0,20,20);
	r2 = SetRect(0,18,20,20);
	if (!RectIntersect(&r1, &r2))
	{
		testprint(" ERROR: NO Rect Intersect!\n");
		return -1;
	}
	testprintnts ("OK\n");
	
	numtabs--;
	return 0;
}
