#pragma once
#include "TestComponent.h"
#include "TestHelperFunctions.h"
#include "console.h"
//class TestSuite;

using namespace std;



class cConsoleTest : TS::TestComponent
{
	
public:
	cConsoleTest();
	int Test();
	int TestCommand(string);
	
	// all the different Commands to Test
	static int TestClear();
	static int TestMx();
	static int TestMy();
	static int TestLS();
	static int TestLoadMap();
	static int TestMxy();
	static int TestPlayMus();
	static int TestFPS();
	static int TestSvol();
	static int TestAllSvol();
	static int TestMvol();
	static int TestCD();
	static int TestSx();
	static int TestSy();
	static int TestSxy();
	static int TestSaveLevel();
	static int TestCamX();
	static int TestCamY();
	static int TestQuit();
	
	static int rval;
	static string matchstr;
	static string cmdstr,parm;
	
	
	//static TestCmd CommandsToTest[];
};

struct CmdtoFunc
{
	string commandname;
	int (*TestFunction)( void );
};