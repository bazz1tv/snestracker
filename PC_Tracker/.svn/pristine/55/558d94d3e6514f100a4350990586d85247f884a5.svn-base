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
#include "TestComponents/SettingsPreferences/SettingsPreferencesTest.h"

using namespace std;

namespace TS
{
	
	void Begin();
	
	void ComponentView();
	void init();
	
	void TestAll();
	void TestComponents();
	
	void tabprint();
	//void testprint(string, ...);
	//void testprintnts(string, ...);
	
	// Components Testing
	int ConsoleTest();
	
	
	int MiniEngineTest();
	int CollisionTest();
	int SettingsPreferencesTest();
	
	//friend class cConsoleTest;
	extern cConsoleTest TC_console;
	extern cCollisionTest TC_collision;
	extern cSettingsPreferencesTest TC_settingsPreferences;
	
	extern SDL_bool verbose;
	extern int numtabs;
	
	/*class Component
	{ public:
		Component() { next = NULL; }
		string component_name;
		int ( *handler )( void );
		Component *next;
	};*/
	struct SingleComponent
	{
		string component_name;
		int ( *TestComponent )( void );
	};
	//extern Component *ComponentList;
};


