#include "Globals.h"
#include "Game.h"
#include "MainMenu.h"
//#include "main.h"



int DoGame();
void initEngine();


int SDL_main (int argc, char **argv)
{
	initEngine();
	DoGame();

	// After having MUCH FUN.....
	QuitGame();
	return 0;
}

/// Runs the actual Game
int DoGame()
{
	Game game;
	MainMenu mainmenu;

	mode = MODE_MAINMENU;

	while( 1 )
	{
		if (mode == MODE_MAINMENU)
			mainmenu.Do();

		else if (mode == MODE_GAME)
			game.Do();

		else if (mode == MODE_LEVELEDITOR)
			pLevelEditor->Do();

		else if (mode == MODE_CONSOLE)
			pConsole->Do();

		else if (mode == MODE_QUIT)
			break;
	}

	return 0;
}


void initEngine()
{
	InitEP();
	InitSDL( SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE );
	SetWindowCaption( "MeatBall - Vegetable Destruction" );
#ifdef _WIN32
	icon = SDL_LoadBMP("data/favicon.bmp");
	Uint32 colorkey = SDL_MapRGB(icon->format, 128, 128, 128);
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
	SDL_WM_SetIcon(icon, NULL);
#endif


	pPreferences	= new cPreferences();
	pGameSettings	= new cSettings();

	pPreferences->Load();
	pPreferences->Apply();

	/* new code */
	screeninfo	= SDL_GetVideoInfo();
	Screen		= InitScreen( pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp, pGameSettings->Fullscreen, MEATBALL_CUSTOM_FLAGS );
	keys		= SDL_GetKeyState( NULL );

	pFramerate = new cFramerate( 60 );
	SetSpriteSpeedfactor( &pFramerate->speedfactor );

	IMan	= new cImageManager();
	SMan	= new cSoundManager();
	
	pFont	= new cFont();
	
	pAudio = new cAudio();
	pAudio->bMusic		 = pGameSettings->Music;
	pAudio->bSounds		 = pGameSettings->Sounds;
	pAudio->Sound_Volume = pGameSettings->svol;
	pAudio->Music_Volume = pGameSettings->mvol;
	pAudio->InitAudio();

	InitObjects();
}