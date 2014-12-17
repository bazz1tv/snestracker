#pragma once

#include "MiniEngine.h"
#include "Collision.h"
#include "Console.h"
#include <iostream>
#include <string>

#include "Os.h"
#include <string>
#include "player.h"

#include "TestHelperFunctions.h"

// Components
#include "ConsoleTest.h"
#include "CollisionTest.h"

using namespace std;

namespace TS
{
	
	void Begin();
	
	void TestAll();
	void TestComponents();
	
	void tabprint();
	void testprint(string, ...);
	void testprintnts(string, ...);
	
	// Components Testing
	int ConsoleTest();
	
	
	void MiniEngineTest();
	int CollisionTest();
	
	//friend class cConsoleTest;
	extern cConsoleTest TC_console;
	extern cCollisionTest TC_collision;
	
	extern SDL_bool verbose;
	extern int numtabs;
};


