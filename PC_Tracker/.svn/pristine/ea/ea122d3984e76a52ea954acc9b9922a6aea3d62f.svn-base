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
	/*#ifndef _DEBUG
		
		#ifndef DEMO
			pAudio->FadeOutMusic( 2000 );
		#endif
		//FadeOutBG();
	#endif
	*/	
	
	
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

void MasterBlaster::Loop()
{
	while( mode != MODE_QUIT )
	{
		
		
		if (mode == MODE_MAINMENU)
			mode = MainMenu();
		
		else if (mode == MODE_GAME)
		{
			oldmode = MODE_GAME;
			mode = Game();
		}
		#ifdef INGAME_LEVEL_EDITOR
			else if (mode == MODE_LEVELEDITOR)
			{
				oldmode = MODE_LEVELEDITOR;
				mode = pLevelEditor->Do();
			}
		#endif
		else if (mode == MODE_CONSOLE)
			mode = Console();
	}
}