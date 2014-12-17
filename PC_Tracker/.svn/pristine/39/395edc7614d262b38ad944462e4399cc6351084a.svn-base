#include "SettingsPreferencesTest.h"
#include "TestDefinitions.h"
#include "TestStream.h"

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

void cSettingsPreferencesTest::Push2(string name, void *var1, void* var2, boost::any value)
{
	TwoVarComparison *newcomp = new TwoVarComparison;
	newcomp->name = name;
	newcomp->value = value;
	newcomp->var1 = var1;
	newcomp->var2 = var2;
	newcomp->next = twovar;
	twovar = newcomp;
}


#define TEST_PREF_FILE "Test/Preferences/Preferences.ini"
#define TEST_PREF_SAVEFILE "Test/Preferences/Saved_Preferences.ini"

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
	
	
	
	Push2("Music",				&pGameSettings->Music,				&pPreferences->pSettings->Music,			(SDL_bool)SDL_TRUE);
	Push2("Sounds",				&pGameSettings->Sounds,				&pPreferences->pSettings->Sounds,			(SDL_bool)SDL_TRUE);
	Push2("Fullscreen",			&pGameSettings->Fullscreen,			&pPreferences->pSettings->Fullscreen,		(SDL_bool)SDL_FALSE);
	Push2("Screen_H",			&pGameSettings->Screen_H,			&pPreferences->pSettings->Screen_H,			(Uint16)600);
	Push2("Screen_W",			&pGameSettings->Screen_W,			&pPreferences->pSettings->Screen_W,			(Uint16)800);
	Push2("Svol",				&pGameSettings->svol,				&pPreferences->pSettings->svol,				(Uint8)113);
	Push2("Mvol",				&pGameSettings->mvol,				&pPreferences->pSettings->mvol,				(Uint8)116);
	Push2("Key_walk_left",		&pGameSettings->Key_walk_left,		&pPreferences->pSettings->Key_walk_left,	(SDL_Keycode)97 );
	Push2("Key_walk_right",		&pGameSettings->Key_walk_right,		&pPreferences->pSettings->Key_walk_right,	(SDL_Keycode)100);
	Push2("Key_jump",			&pGameSettings->Key_jump,			&pPreferences->pSettings->Key_jump,			(SDL_Keycode)1073742054);
	Push2("Key_shoot_primary",	&pGameSettings->Key_shoot_primary,	&pPreferences->pSettings->Key_shoot_primary,(SDL_Keycode)1073742055 );
	
	// after apply
	/*pAudio->bMusic = Music;
	 pAudio->bSounds = Sounds;
	 pAudio->Sound_Volume = svol;
	 pAudio->Music_Volume = mvol;*/
	
	Push2("pAudio->Music",		&pAudio->bMusic,					&pPreferences->pSettings->Music,			(SDL_bool)SDL_TRUE);
	Push2("pAudio->bSounds",	&pAudio->bSounds,					&pPreferences->pSettings->Sounds,			(SDL_bool)SDL_TRUE);
	Push2("pAudio->Sound_Volume",		&pAudio->Sound_Volume,		&pPreferences->pSettings->svol,				(Uint8)113);
	Push2("pAudio->Music_Volume",		&pAudio->Music_Volume,		&pPreferences->pSettings->mvol,				(Uint8)116);
	
	
	
	return 0;
}

int cSettingsPreferencesTest::Test()
{
	init();
	
	// Note: Must check Apply before Save, because Save uses Update
	// This means we Apply Preferences to GameSettings, so when Save updates preferences from GameSettings, we will have
	// those Preferences Get Loaded ;) 
	
	PrintTestNumber("Load File");
	test << endl;
	test.IncrementTabLevel();
	LoadFile();
	test.DecrementTabLevel();
	
	PrintTestNumber("Apply");
	test.IncrementTabLevel();
	test << endl;
	Apply();
	test.DecrementTabLevel();
	
	
	PrintTestNumber("Update");
	test << endl;
	test.IncrementTabLevel();
	Update();
	test.DecrementTabLevel();
	
	
	PrintTestNumber("Save File");
	test.IncrementTabLevel();
	test << endl;
	SaveFile();
	test.DecrementTabLevel();
	
	if (SomethingFailed)
	{
		return TEST_FAILED;
	}
	else return TEST_SUCCESSFUL;
}

int cSettingsPreferencesTest::LoadFile()
{
	// We are going to use a Sample Preferences file and compare it's output with expected output
	
	// contents of TEST_PREF_FILE
	// Hard-coded into Array @ top of this file
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
	
	return ParsePreferences();
}

int cSettingsPreferencesTest::ParsePreferences()
{
	// Set our table to the HEAD
	property *pref_entry = prefs_head;
	
	// loop through all entries
	while (pref_entry != NULL)
	{
		// Prepare output string
		string check = "Checking " + pref_entry->name;
		test << StringPadding(check, 30) << " | ";
		
		
		// check for different types
		if (pref_entry->value.type() == typeid(SDL_bool))
		{
			compone<SDL_bool>(pref_entry);
			
		}
		else if (pref_entry->value.type() == typeid(Uint16))
		{
			compone<Uint16>(pref_entry);
		}
		else if (pref_entry->value.type() == typeid(Uint8))
		{
			compone<Uint8>(pref_entry);
		}
		else if (pref_entry->value.type() == typeid(SDL_Keycode))
		{
			compone<SDL_Keycode>(pref_entry);
		}
		else
		{
			// this should not happen ;)
			test<<"Undefined Type??" << "Entry Name: " << pref_entry->name;
		}
		
		// rinse repeat
		pref_entry = pref_entry->next;
	}
	
	if (SomethingFailed)
		return TEST_FAILED;
	
	return TEST_SUCCESSFUL;

}

// Define Template Function Check
/*typeid(T)
T ptr1 = *((T *)comp->var1);*/


int cSettingsPreferencesTest::CompareValues()
{
	// Set our table to the HEAD
	//property *comp = prefs_head;
	TwoVarComparison *comp = twovar;
	// loop through all entries
	while (comp != NULL)
	{
		// Prepare output string
		string check = "Checking " + comp->name;
		test << StringPadding(check, 30) << " | ";
		
		/*for (int i=0; types[i] != NULL; i++)
		{
			if (comp->value.type() == *types[i])
			{
				test << "Testing";
				///comptwo(comp);
				break;
			}
		}*/
		if (comp->value.type() == typeid(SDL_bool))
		{
			comptwo<SDL_bool>(comp);
		}
		else if (comp->value.type() == typeid(Uint8))
		{
			comptwo<Uint8>(comp);
		}
		else if (comp->value.type() == typeid(Uint16))
		{
			comptwo<Uint16>(comp); 
		}
		else if (comp->value.type() == typeid(SDL_Keycode))
		{
			comptwo<SDL_Keycode>(comp);
		}
		
		
		// check for different types
		comp = comp->next;
	}
			
	if (SomethingFailed)
		return TEST_FAILED;
	
	return TEST_SUCCESSFUL;
	
}

int cSettingsPreferencesTest::SaveFile()
{
	
	//Stub();
	
	// Save File
	
	// Write the File
	pPreferences->Save(TEST_PREF_SAVEFILE);
	
	
	// Check for File Existence
	if (!FileValid(TEST_PREF_SAVEFILE))
	{
		PrintTestFailed();
		test << TEST_PREF_SAVEFILE << " is not a valid file! ";
		return TEST_FAILED;
	}
	
	pPreferences->Load(TEST_PREF_SAVEFILE);
	// Reparse the File and Compare results with stored expected information
	if (ParsePreferences() == TEST_FAILED )
	{
		PrintTestFailed();
		test << "ParsePreferences Failed!";
	}
	
	
	return TEST_SUCCESSFUL;
}

int cSettingsPreferencesTest::Apply()
{
	//Stub();
	// we already know Loadfile Works ;) (it's tested First)
	pPreferences->Load(TEST_PREF_FILE);
	pPreferences->Apply();
	
	// Now lets check the values
	CompareValues();
	return TEST_SUCCESSFUL;
}

int cSettingsPreferencesTest::Update()
{
	// Based on Apply Test, pGameSettings should match TEST_PREF_FILE values
	pPreferences->Update();
	
	CompareValues();
	
	//Stub();
	return TEST_SUCCESSFUL;
}