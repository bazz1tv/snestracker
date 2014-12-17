#include "Globals.h"
#include "MiniEngine.h"
#include "MainMenu.h"
#include "Game.h"
#include "LevelEditor.h"
#include "Console.h"



class MasterBlaster
{
public:
	MasterBlaster();
	~MasterBlaster();
	
	
	void Loop();
	int Game();
	int MainMenu();
	int Console();
	
	Uint8 mode, oldmode;
	cLevelEditor *pLevelEditor;
	
	
	cGame *pGame;
	cMainMenu *pMainMenu;
};