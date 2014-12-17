#pragma once

#include <OS.h>
#include <iostream>
#include "Misc.h"



namespace TS
{
	extern SDL_bool verbose;
	
/** The TestComponent Abstracts the role of TestComponents.
	 Namely, to test themselves! */
class TestComponent
{
public:
	TestComponent() { testnum=1; SomethingFailed = SDL_FALSE; Description = "Undocumented Component\n"; }
	~TestComponent() { }
	
	virtual int Test() { return 0; } // Tests the Component
	std::string GetDescription() { return Description; }
	void SetDescription(std::string desc) { Description = desc; }
	
	void PrintTestNumber(std::string t="")
	{
		if (verbose == SDL_FALSE)
			t = "";
		ostringstream testnumss;
		testnumss << "Test" << testnum++ << ": ";
		
		std::cout << StringPadding(testnumss.str(),10) << StringPadding(t,20);
	}
	int testnum;
	SDL_bool SomethingFailed;
	std::string Description;
};
	
}

//#include "TestSuite.h"

//using namespace TS;

