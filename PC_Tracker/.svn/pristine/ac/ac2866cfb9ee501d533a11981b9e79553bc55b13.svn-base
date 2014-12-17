#pragma once
#include "TestComponent.h"
#include "TestHelperFunctions.h"
#include "TestDefinitions.h"
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

struct TwoVarComparison
{
	string name;
	void *var1,*var2;
	boost::any value;
	TwoVarComparison *next;
};



class cSettingsPreferencesTest : TS::TestComponent
{
public:
	cSettingsPreferencesTest() : TestComponent() { }
	int Test();
	int init();
	void Push(string name, boost::any value, void *orig);
	void Push2(string name, void *var1, void* var2, boost::any value);

	int ParsePreferences();
	int CompareValues();
	
	int LoadFile();
	int SaveFile();
	int Apply();
	int Update();
	
	template <typename T>
	int dothething1(TwoVarComparison *comp)
	{
		T ptr1 = *((T *)comp->var1);
		T ptr2 = *((T *)comp->var2);
		if ( ptr1 == ptr2 )
		{
			OK();
			return TEST_SUCCESSFUL;
		}
		else
		{
			PrintTestFailed();
			SomethingFailed = SDL_TRUE;
			return TEST_FAILED;
		}
	}
	template <typename T>
	int dothething2(property *pref_entry)
	{
		T ptr = *((T *)pref_entry->orig_variable);
		if ( ptr == boost::any_cast<T>(pref_entry->value) )
		{
			OK();
			return TEST_SUCCESSFUL;
		}
		else
		{
			PrintTestFailed();
			SomethingFailed = SDL_TRUE;
			return TEST_FAILED;
		}
	}
		
	property *prefs_head;
	TwoVarComparison *twovar;
};