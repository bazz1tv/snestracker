#pragma once

#include <SDL2/SDL.h>
#include <iostream>
using namespace std;
/* The TestComponent Abstracts the role of TestComponents.
 Namely, to test themselves! */



class TestComponent
{
public:
	TestComponent() { testnum=1; SomethingFailed = SDL_FALSE; }
	~TestComponent() { }
	
	virtual int Test() { return 0; } // Tests the Component
	virtual void Description() { cout << "Undocumented Component\n"; }
	void PrintTestNumber(string t="")
	{
		cout << "Test" << testnum++ << ": " << t << endl;
	}
	int testnum;
	SDL_bool SomethingFailed;
};