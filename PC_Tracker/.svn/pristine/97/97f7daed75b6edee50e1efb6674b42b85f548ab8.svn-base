#pragma once

#include "MiniEngine.h"
#include "Collision.h"
#include "Console.h"
#include <iostream>
#include <string>
using namespace std;

class TestSuite
{
public:
	TestSuite();
	
	void Begin();
	
	void TestAll();
	void TestComponents();
	
	void tabprint();
	void testprint(string, ...);
	void testprintnts(string, ...);
	
	// Components Testing
	int ConsoleTest();
	int TestCommand(string);
	
	void MiniEngineTest();
	int CollisionTest();
	
	//cConsole *Console;
	
	int numtabs;
};