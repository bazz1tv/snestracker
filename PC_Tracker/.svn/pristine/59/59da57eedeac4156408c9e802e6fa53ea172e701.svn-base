#include "SettingsPreferencesTest.h"
#include "TestDefinitions.h"
//#include <list>

// Pushes a new Command onto the linked list of aLL commands
void cSettingsPreferencesTest::Push(string name, boost::any value, void *orig)
{
	property* newprop = new property;
	newprop->name = name;
	newprop->value = value;
	newprop->orig_variable = orig;
	//newprop->command.add(new string(command));
	newprop->next = prefs_head;
	prefs_head = newprop;
}

#define TEST_PREF_FILE "Test/Preferences/Preferences.ini"


int cSettingsPreferencesTest::init()
{
	Push("Music",				(SDL_bool)SDL_TRUE,		&pPreferences->pSettings->Music );
	Push("Sounds",				(SDL_bool)SDL_TRUE,		&pPreferences->pSettings->Sounds);
	Push("Fullscreen",			(SDL_bool)SDL_FALSE,	&pPreferences->pSettings->Fullscreen);
	Push("Screen_H",			(Uint16)600,			&pPreferences->pSettings->Screen_H);
	Push("Screen_W",			(Uint16)800,			&pPreferences->pSettings->Screen_W);
	Push("Svol",				(Uint8)113,				&pPreferences->pSettings->svol);
	Push("Mvol",				(Uint8)116,				&pPreferences->pSettings->mvol);
	Push("Key_walk_left",		(SDL_Keycode)97,		&pPreferences->pSettings->Key_walk_left );
	Push("Key_walk_right",		(SDL_Keycode)100,		&pPreferences->pSettings->Key_walk_right);
	Push("Key_jump",			(SDL_Keycode)1073742054,&pPreferences->pSettings->Key_jump);
	Push("Key_shoot_primary",	(SDL_Keycode)1073742055,&pPreferences->pSettings->Key_shoot_primary);
	
	return 0;
}

int cSettingsPreferencesTest::Test()
{
	init();
	
	PrintTestNumber("Load File");
	cout << endl;
	LoadFile();
	PrintTestNumber("Save File");
	cout << endl;
	SaveFile();
	PrintTestNumber("Apply");
	cout << endl;
	Apply();
	PrintTestNumber("Update");
	cout << endl;
	Update();
	
	cout << "OverAll: ";
	
	return 1;
}

int cSettingsPreferencesTest::LoadFile()
{
	// We are going to use a Sample Preferences file and compare it's output with expected output
	
	/* 
	 Music 1
	 Sounds 1
	 Fullscreen 0
	 Screen_H 600
	 Screen_W 800
	 Screen_Bpp 32
	 svol 113
	 mvol 116
	 WalkLeft 97
	 WalkRight 100
	 Jump 1073742054
	 FirePrimary 1073742055
	 */
	
	// Load the TestPrefFile
	pPreferences->Load(TEST_PREF_FILE);
	// Set our table to the HEAD
	property *pref_entry = prefs_head;
	
	// loop through all entries
	while (pref_entry != NULL)
	{
		// Prepare output string
		string check = "Checking " + pref_entry->name;
		cout << StringPadding(check, 30) << " | ";
		
		
		// check for different types
		if (pref_entry->value.type() == typeid(SDL_bool))
		{
			SDL_bool ptr = *((SDL_bool *)pref_entry->orig_variable);
			if ( ptr == boost::any_cast<SDL_bool>(pref_entry->value))
			{
				OK();
			}
			else
			{
				PrintTestFailed();
				SomethingFailed = SDL_TRUE;
			}
			
		}
		else if (pref_entry->value.type() == typeid(Uint16))
		{
			int ptr = *((Uint16 *)pref_entry->orig_variable);
			if ( ptr == boost::any_cast<Uint16>(pref_entry->value) )
			{
				OK();
			}
			else
			{
				PrintTestFailed();
				SomethingFailed = SDL_TRUE;
			}
		}
		else if (pref_entry->value.type() == typeid(Uint8))
		{
			int ptr = *((Uint8 *)pref_entry->orig_variable);
			if ( ptr == boost::any_cast<Uint8>(pref_entry->value) )
			{
				OK();
			}
			else
			{
				PrintTestFailed();
				SomethingFailed = SDL_TRUE;
			}
		}
		else if (pref_entry->value.type() == typeid(SDL_Keycode))
		{
			int ptr = *((SDL_Keycode *)pref_entry->orig_variable);
			if ( ptr == boost::any_cast<SDL_Keycode>(pref_entry->value) )
			{
				OK();
			}
			else
			{
				PrintTestFailed();
				SomethingFailed = SDL_TRUE;
			}
		}
		else
		{
			// this should not happen ;)
			cout<<"Undefined Type??" << "Entry Name: " << pref_entry->name;
		}
		
		// rinse repeat
		pref_entry = pref_entry->next;
	}
		
	if (SomethingFailed)
		return TEST_FAILED;
	
	return TEST_SUCCESSFUL;
}

int cSettingsPreferencesTest::SaveFile()
{
	Stub();
	return 1;
}

int cSettingsPreferencesTest::Apply()
{
	Stub();
	return 1;
}

int cSettingsPreferencesTest::Update()
{
	Stub();
	return 1;
}