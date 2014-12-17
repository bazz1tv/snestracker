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
#include "TestStream.h"

using namespace std;
class cConsoleTest;
class cCollisionTest;
class cSettingsPreferencesTest;

namespace TS
{
	
	void Begin();
	
	void ComponentView();
	void init();
	
	void TestAll();
	void TestComponents();
	
	void tabprint();
	
	// Components Testing
	int ConsoleTest();
	int MiniEngineTest();
	int CollisionTest();
	int SettingsPreferencesTest();
	
	
	extern cConsoleTest TC_console;
	extern cCollisionTest TC_collision;
	extern cSettingsPreferencesTest TC_settingsPreferences;
	
	extern SDL_bool verbose;
	extern int numtabs;
	
	
	
	
	struct SingleComponent
	{
		string component_name;
		int ( *TestComponent )( void );
	};
	
	
	//extern Component *ComponentList;
};




