#include "MasterBlaster.h"
#include "Camera.h"
cConsole *pConsole;
extern cCamera *pCamera;

MasterBlaster::MasterBlaster()
{
	mode = MODE_MAINMENU;
	
#ifdef INGAME_LEVEL_EDITOR
	pLevelEditor = new cLevelEditor();
#endif
	pConsole = new cConsole();
	
	pGame = new cGame();
	pMainMenu = new cMainMenu();
	
	// init Game
	pGame->Init();
}

MasterBlaster::~MasterBlaster()
{
				
#ifdef INGAME_LEVEL_EDITOR
	if( pLevelEditor )
	{
		delete pLevelEditor;
	}
#endif
	if ( pConsole )
	{
		delete pConsole;
	}
	if (pGame) {
		delete pGame;
	}
	if (pMainMenu)
	{
		delete pMainMenu;
	}

}

int MasterBlaster::Game()
{
	int mode = pGame->Do();
	
	return mode;
}

int MasterBlaster::MainMenu()
{
	int mode = pMainMenu->Do();
	
	return mode;
}

int MasterBlaster::Console()
{
	pConsole->Do();
	return oldmode;
}

int MasterBlaster::LevelEditor()
{
	return pLevelEditor->Do();
}

void MasterBlaster::Loop()
{
	Uint8 newmode = MODE_MAINMENU; // for first time
	while( mode != MODE_QUIT )
	{
		oldmode = mode;
		mode = newmode;
		
		if (mode == MODE_MAINMENU)
			newmode = MainMenu();
		
		else if (mode == MODE_GAME)
		{
			newmode = Game();
		}
		#ifdef INGAME_LEVEL_EDITOR
			else if (mode == MODE_LEVELEDITOR)
			{
				newmode = LevelEditor();
			}
		#endif
		else if (mode == MODE_CONSOLE)
			newmode = Console();
	}
}