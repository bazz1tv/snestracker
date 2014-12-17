#pragma once
#include "TestComponent.h"
#include "TestHelperFunctions.h"
#include "TestDefinitions.h"
#include "preferences.h"
#include "TestStream.h"

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
	
	// Pushes Everything to the Linked Lists
	int init();
	
	// Push for name,value,variable association
	void Push(string name, boost::any value, void *orig);
	// Push for 2var comparison w/ value and ID string
	void Push2(string name, void *var1, void* var2, boost::any value);

	int ParsePreferences();
	int CompareValues();
	
	int LoadFile();
	int SaveFile();
	int Apply();
	int Update();
	
	// Compare 2 Variables and expected Value to be equal
	// for the Update/Apply pPreferences/pGameSettings
	template <typename T>
	int comptwo(TwoVarComparison *comp)
	{
		T ptr1 = *((T *)comp->var1);
		T ptr2 = *((T *)comp->var2);
		if ( ptr1 == ptr2 && ptr1 == boost::any_cast<T>(comp->value) )
		{
			OK();
			return TEST_SUCCESSFUL;
		}
		else
		{
			PrintTestFailed();
			test << "Var1 == " << ptr1 << " Var2 == " << ptr2 << " value == " << boost::any_cast<T>(comp->value) << endl;
			SomethingFailed = SDL_TRUE;
			return TEST_FAILED;
		}
	}
	// For the Comparison of Variables and expected Values on Loading File / Saving
	template <typename T>
	int compone(property *pref_entry)
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