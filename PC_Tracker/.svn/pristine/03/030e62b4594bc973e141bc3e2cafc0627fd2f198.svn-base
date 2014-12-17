#include "ConsoleTest.h"
#include "player.h"
#include "TestSuite.h"
#include "TestDefinitions.h"
#include "Misc.h"

	/* Command List
	 SDL_bool clearcon( string &str );	///< Clears the console
	 // nothing to test here
	 SDL_bool loadmap( string &str );	///< Load a new Level from @link LEVEL_DIR @endlink
	 SDL_bool SetMx( string &str );		///< Set Meatball's X Coordinate
	 SDL_bool SetMy( string &str );		///< Set Meatball's Y Coordinate
	 SDL_bool SetMxy( string &str );		///< Set Meatball (X,Y) coordinate
	 SDL_bool play( string &str );		///< Play a song from the @link MUSIC_DIR @endlink
	 SDL_bool QuitAll( string &str);		///< Quit the Game
	 SDL_bool ShowFPS( string & str);	///< Toggle FPS framerate to be displayed
	 SDL_bool help( string &str );		///< bring up help on all commands or specific command
	 SDL_bool soundVol(string &str);		///< change the volume of a sound (0-128)
	 SDL_bool allSoundsVol(string &str);	///< Change the volume of All sounds (0-128)
	 SDL_bool musicVol(string &str);		///< Change the music volume (0-128)
	 SDL_bool cd(string &str);			///< change the current directory
	 SDL_bool ls(string &str);			///< list the directory
	 SDL_bool SetScreenScaleXY(string &str);
	 SDL_bool SetScreenScaleX(string &str);
	 SDL_bool SetScreenScaleY(string &str);
	 SDL_bool SaveLevel(string &str);
	 SDL_bool camx(string &str);
	 SDL_bool camy(string &str);
	 */

string cConsoleTest::cmdstr = "";
string cConsoleTest::matchstr = "";
string cConsoleTest::parm = "";
int cConsoleTest::rval = 1;

// Here is our list of Full Test Commands to Run
string commands_to_test[] = {
	"cls",
	"ls",
	"/ls",
	"mx 100",
	"my 100",
	"loadmap test.lvl",
	"mxy 100 100",
	"playmus rockin.xm",
	"fps",
	"svol pistol 20",
	"allsvol 80",
	"mvol 127",
	"cd data",
	"sx 2",
	"sy 2",
	"sxy 2 2",
	"savelevel bla.lvl",
	"camx 10",
	"camy 10",
	""
};

// A Command -> Function Mapping System
CmdtoFunc CommandToFunctionMap[] = {
	{"clear", cConsoleTest::TestClear},
	{"ls", cConsoleTest::TestLS},
	{"mx",cConsoleTest::TestMx},
	{"my",cConsoleTest::TestMy},
	{"loadmap",cConsoleTest::TestLoadMap},
	{"mxy", cConsoleTest::TestMxy},
	{"playmus", cConsoleTest::TestPlayMus},
	{"fps", cConsoleTest::TestFPS},
	{"svol", cConsoleTest::TestSvol},
	{"allsvol", cConsoleTest::TestAllSvol},
	{"mvol", cConsoleTest::TestMvol},
	{"cd", cConsoleTest::TestCD},
	{"sx", cConsoleTest::TestSx},
	{"sy", cConsoleTest::TestSy},
	{"sxy", cConsoleTest::TestSxy},
	{"savelevel", cConsoleTest::TestSaveLevel},
	{"camx", cConsoleTest::TestCamX},
	{"camy", cConsoleTest::TestCamY},
	{"quit", cConsoleTest::TestQuit},
	{"", NULL}
	
};

cConsoleTest::cConsoleTest() :
TestComponent()
{
	
}

int cConsoleTest :: Test()
{
	//cout << "Console Test\n---------------" << endl;
	int i=0;
	while (1)
	{
		PrintTestNumber(commands_to_test[i]);
		// get current command into easy-access variable
		cmdstr = commands_to_test[i++];
		
		if (TestCommand(cmdstr) == TEST_FAILED)
			SomethingFailed = SDL_TRUE;
		
		if (commands_to_test[i] == "")
			break;
	}
	
	if (SomethingFailed)
		return TEST_FAILED;
		
	return TEST_SUCCESSFUL;
}

int cConsoleTest :: TestCommand(string cmdstr)
{
	string base = pConsole->ParseBase(cmdstr);
	parm = pConsole->ParseParm(cmdstr);
	
	if ( base.empty() )
	{
		cout<<"base empty\n";
		return SDL_FALSE;
	}
	
	
	cCMD *cmd_match = pConsole->FindMatch(base,matchstr);
	
	if (TS::verbose)
	{
		string bla = " | original command name: "+ *cmd_match->command.objects[0];
		cout << StringPadding(bla, 40) << " | ";
	}
	
	// Automated Mapping of Command-Name to its proper TestFunction
	for (int i=0; CommandToFunctionMap[i].commandname != ""; i++ )
	{
		if (*cmd_match->command.objects[0] == CommandToFunctionMap[i].commandname)
		{
			return CommandToFunctionMap[i].TestFunction();
		}
	}
	
	cout << "No Command->Function match for " << *cmd_match->command.objects[0];
	
	return -2;
}



int cConsoleTest::TestClear()
{
	if (pConsole->CMDHandler(cmdstr) == SDL_TRUE)
	{
		if (pConsole->consoleInput_str.empty())
		{
			for ( int i = 0; i < HISTORY_LINES; i++ )
			{
				if (!pConsole->strcpy[i].empty())
				{
					PrintTestFailed();
					return -1;
				}
			}
		}
		else {
			PrintTestFailed();
			return -1;
		}
		
	}
	
	cout << "OK" << endl;
	return 0;
}

int cConsoleTest::TestMx()
{
	if (pConsole->CMDHandler(cmdstr) == SDL_TRUE)
	{
		istringstream parameter(parm);
		double val;
		parameter >> val;
		if (pPlayer->posx == val)
		{
			PrintOK();
			return 0;
		}
	}
	
	return -1;
	
}

int cConsoleTest::TestMy()
{
	
	if (pConsole->CMDHandler(cmdstr) == SDL_TRUE)
	{
		istringstream parameter(parm);
		double val;
		parameter >> val;
		
		if (pPlayer->posy == val)
		{
			OK();
			return 0;
		}
	}
	
	PrintTestFailed();
	cout << "pPlayer->posy = " << pPlayer->posy << endl;
	cout << "parm = " << parm << endl;
	return -1;
}

int cConsoleTest::TestLS()
{
	Stub(); return 0;
}
int cConsoleTest::TestLoadMap()
{
	Stub(); return 0;
}
int cConsoleTest::TestMxy()
{
	Stub(); return 0;
}
int cConsoleTest::TestPlayMus()
{
	Stub(); return 0;
}
int cConsoleTest::TestFPS()
{
	Stub(); return 0;
}
int cConsoleTest::TestSvol()
{
	Stub(); return 0;
}
int cConsoleTest::TestAllSvol()
{
	Stub(); return 0;
}
int cConsoleTest::TestMvol()
{
	Stub(); return 0;
}
int cConsoleTest::TestCD()
{
	Stub(); return 0;
}
int cConsoleTest::TestSx()
{
	Stub(); return 0;
}
int cConsoleTest::TestSy()
{
	Stub(); return 0;
}
int cConsoleTest::TestSxy()
{
	Stub(); return 0;
}
int cConsoleTest::TestSaveLevel()
{
	Stub(); return 0;
}
int cConsoleTest::TestCamX()
{
	Stub(); return 0;
}
int cConsoleTest::TestCamY()
{
	Stub(); return 0;
}
int cConsoleTest::TestQuit()
{
	cout << "Test Quit Yourself :P"<<endl;
	return 1;
}


