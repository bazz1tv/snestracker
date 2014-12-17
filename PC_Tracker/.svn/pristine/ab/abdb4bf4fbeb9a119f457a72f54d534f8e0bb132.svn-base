#include "Globals.h"
#include "Game.h"
#include "MainMenu.h"
//#include "main.h"



int DoGame();
void initEngine();


int main (int argc, char **argv)
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
	
	//
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	pPreferences	= new cPreferences();
	pGameSettings	= new cSettings();

	pPreferences->Load();
	pPreferences->Apply();
	
	// Declare structures to be filled in.
	SDL_DisplayMode target, closest;
	
	// Set the target properties.
	target.w = pGameSettings->Screen_W;     // weird request, but ok
	target.h = pGameSettings->Screen_H;
	target.format = 0;  // don't care
	target.refresh_rate = 0; // don't care
	target.driverdata   = 0; // initialize to 0
	
	DEBUGLOG("Requesting: \t%dx%dpx @ %dhz \n", target.w, target.h, target.refresh_rate);
	
	// Pass the display mode structures by reference to SDL_GetClosestDisplay
	// and check that the result is not a null pointer.
	if(SDL_GetClosestDisplayMode(0, &target, &closest)==NULL){
		
		// If the returned pointer is null ...
		DEBUGLOG("\nNo match was found!\n\n");
		SDL_Quit();
		return;
		
	}
	// Otherwise, a display mode close to the target is available.
	// Access the SDL_DisplayMode structure to see what was received.
	DEBUGLOG("  Received: \t%dx%dpx @ %dhz \n", closest.w, closest.h, closest.refresh_rate);
	
	Window = GetWindow(APP_TITLE, closest.w, closest.h, pGameSettings->Screen_Bpp, pPreferences->pSettings->Fullscreen ? SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_GRABBED : SDL_WINDOW_SHOWN);
	Renderer = GetRenderer(Window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	window_height = pGameSettings->Screen_H;
	window_width = pGameSettings->Screen_W;
	
	//SDL_SetWindowBordered(Window, SDL_FALSE);
	SDL_SetWindowMaximumSize(Window, window_width, window_height);
	SDL_SetWindowMinimumSize(Window, window_width, window_height);
	SDL_DisableScreenSaver();
	
	SDL_Surface *icon = SDL_LoadBMP("data/favicon.bmp");
	Uint32 ckey = SDL_MapRGB(icon->format, 128, 128, 128);
	SDL_SetColorKey(icon, SDL_TRUE, ckey);
	
#if SDL1
	SDL_WM_SetIcon(icon, NULL);
#else
	// The icon is attached to the window pointer
	SDL_SetWindowIcon(Window, icon);
#endif
	
	// ...and the surface containing the icon pixel data is no longer required.
	SDL_FreeSurface(icon);

	/* new code */
	//screeninfo	= SDL_GetVideoInfo();
	//Screen		= InitScreen( pGameSettings->Screen_W, pGameSettings->Screen_H, pGameSettings->Screen_Bpp, pGameSettings->Fullscreen, MEATBALL_CUSTOM_FLAGS );
	//keys		= SDL_GetKeyState( NULL );
	
	/* wflags:
	 Values
	 
	 SDL_WINDOW_FULLSCREEN
	 fullscreen window
	 SDL_WINDOW_FULLSCREEN_DESKTOP
	 fullscreen window at the current desktop resolution
	 SDL_WINDOW_OPENGL
	 window usable with OpenGL context
	 SDL_WINDOW_SHOWN
	 window is visible
	 SDL_WINDOW_HIDDEN
	 window is not visible
	 SDL_WINDOW_BORDERLESS
	 no window decoration
	 SDL_WINDOW_RESIZABLE
	 window can be resized
	 SDL_WINDOW_MINIMIZED
	 window is minimized
	 SDL_WINDOW_MAXIMIZED
	 window is maximized
	 SDL_WINDOW_INPUT_GRABBED
	 window has grabbed input focus
	 SDL_WINDOW_INPUT_FOCUS
	 window has input focus
	 SDL_WINDOW_MOUSE_FOCUS
	 window has mouse focus
	 SDL_WINDOW_FOREIGN
	 window not created by SDL
	 
	 rflags:
	 SDL_RENDERER_SOFTWARE
	 the renderer is a software fallback
	 SDL_RENDERER_ACCELERATED
	 the renderer uses hardware acceleration
	 SDL_RENDERER_PRESENTVSYNC
	 present is synchronized with the refresh rate
	 SDL_RENDERER_TARGETTEXTURE
	 the renderer supports rendering to texture
	 
	 
	 */

	
	
	keys = SDL_GetKeyboardState(NULL);

	//int InitWindow( SDL_Window *win, SDL_Renderer *Screen, const char *title, int width, int height, int bpp, SDL_SDL_bool fullscreen, Uint32 wflags, Uint32 rflags)
	
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