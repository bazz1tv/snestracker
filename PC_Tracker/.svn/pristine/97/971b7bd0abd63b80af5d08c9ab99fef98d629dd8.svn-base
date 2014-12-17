#include "SettingsPreferencesTest.h"


#define TEST_PREF_FILE "Test/Preferences/Preferences.ini"

int cSettingsPreferencesTest::Test()
{
	//cout << "Settings&Preferences Test\n---------------" << endl;
	
	PrintTestNumber("Load File");
	LoadFile();
	PrintTestNumber("Save File");
	SaveFile();
	PrintTestNumber("Apply");
	Apply();
	PrintTestNumber("Update");
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
	
	pPreferences->Load(TEST_PREF_FILE);
	
	cout << "Checking Music: ";
	if (pPreferences->pSettings->Music == SDL_TRUE)
	{
		OK();
	}
	else
	{
		return PrintTestFailed();
	}
	
	cout << "Checking Sounds: " ; 
	if (pPreferences->pSettings->Sounds == SDL_TRUE)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Fullscreen: ";
	if (pPreferences->pSettings->Fullscreen == SDL_FALSE)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Screen_H: ";
	if (pPreferences->pSettings->Screen_H == 600)
		OK();
	else
	{
		return PrintTestFailed();
	}
	
	cout << "Checking Screen_W: ";
	if (pPreferences->pSettings->Screen_W == 800)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Svol: ";
	if (pPreferences->pSettings->svol == 113)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Mvol: ";
	if (pPreferences->pSettings->mvol == 116)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Key_walk_left: ";
	if (pPreferences->pSettings->Key_walk_left == 97)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Key_walk_right: ";
	if (pPreferences->pSettings->Key_walk_right == 100)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Key_jump: ";
	if (pPreferences->pSettings->Key_jump == 1073742054)
		OK();
	else
	{
		return PrintTestFailed();
	}
	cout << "Checking Key_shoot_primary: ";
	if (pPreferences->pSettings->Key_shoot_primary == 1073742055)
		OK();
	else
	{
		return PrintTestFailed();
	}
	
	
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
	
	return 1;
	
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