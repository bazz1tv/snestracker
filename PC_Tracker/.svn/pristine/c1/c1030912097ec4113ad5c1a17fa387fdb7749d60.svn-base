#pragma once
#include "TestComponent.h"
#include "TestHelperFunctions.h"

#include "preferences.h"

#include <boost/any.hpp>

// homogenous
struct property
{
	std::string name;
	boost::any value;
	void *orig_variable;
	property *next;
};



class cSettingsPreferencesTest : TS::TestComponent
{
public:
	cSettingsPreferencesTest() : TestComponent() { }
	int Test();
	int init();
	void Push(string name, boost::any value, void *orig);

	
	int LoadFile();
	int SaveFile();
	int Apply();
	int Update();
	
		
	property *prefs_head;
};