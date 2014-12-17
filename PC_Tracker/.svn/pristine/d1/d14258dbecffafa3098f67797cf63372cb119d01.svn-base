#include "Globals.h"
#include "Game.h"
#include "MainMenu.h"
#include "level.h"
#include "leveleditor.h"
#include "console.h"
#include "preferences.h"
#include "Camera.h"
#include "player.h"
#include "effects.h"
#include "bullet.h"
#include "enemy.h"
#include "MasterBlaster.h"

cSettings *pGameSettings;
cPreferences *pPreferences;

cCamera *pCamera;
cPlayer *pPlayer;
cLevel *pLevel;


int DoGame();
void initEngine();
void CreateWindow();
void QuitGame();
void SetupMouse();
void InitPreferences();
void InitObjects();
void SetupWindowAndDockIcon();

void InitFramerate(int nframes_sec);
void InitImageManager();
void InitSoundManager();
void InitAudio();
void InitFont();
void InitCamera();
void InitPlayer();
void InitLevel();

MasterBlaster *MB;

int main (int argc, char **argv)
{
	
	initEngine();
	
	MB = new MasterBlaster();
	MB->Loop();
	
	// After having MUCH FUN.....
	delete MB;
	QuitGame();
	
	return 0;
}


void initEngine()
{
	InitEP();
	InitSDL( SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE );
	
	SetColorKey(0xffff00ff);
	
	// Enable Dropfile Support
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	
	InitFramerate(60);
	InitPreferences();
	
	InitImageManager();
	InitSoundManager();
	
	InitAudio();
	InitFont();
	InitCamera();
	InitPlayer();
	InitLevel();
	
	CreateWindow();
	SetupMouse();
	keys = SDL_GetKeyboardState(NULL);
}

void InitFramerate(int nframes_sec)
{
	pFramerate = new cFramerate( nframes_sec );
	SetSpriteSpeedfactor( &pFramerate->speedfactor );
}

void InitPreferences()
{
	// InitPreferences
	pPreferences  = new cPreferences();
	pGameSettings = new cSettings();
	pPreferences->Load();
	pPreferences->Apply();
}

// To-Do: Expand this function to query available video modes and make best Selection.
void CreateWindow()
{
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
	
	Window = GetWindow(APP_TITLE, closest.w, closest.h, pPreferences->pSettings->Fullscreen ? SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_GRABBED : SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	Renderer = GetRenderer(Window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	window_height = pGameSettings->Screen_H;
	window_width = pGameSettings->Screen_W;
	
	//SDL_SetWindowBordered(Window, SDL_FALSE);
	//SDL_SetWindowMaximumSize(Window, window_width, window_height);
	//SDL_SetWindowMinimumSize(Window, window_width, window_height);
	SDL_DisableScreenSaver();
	SetupWindowAndDockIcon();
}

void SetupWindowAndDockIcon()
{
	SDL_Surface *icon = SDL_LoadBMP("data/favicon.bmp");
	// this ckey is hardcoded based on the bg color of favicon.bmp
	Uint32 ckey = SDL_MapRGB(icon->format, 128, 128, 128);
	SDL_SetColorKey(icon, SDL_TRUE, ckey);
	
	// The icon is attached to the window pointer
	SDL_SetWindowIcon(Window, icon);
	// ...and the surface containing the icon pixel data is no longer required.
	SDL_FreeSurface(icon);
}

void SetupMouse()
{
	IMan->Add( LoadImage( PIXMAPS_DIR "internal/Mouse.png", colorkey ), "Mouse_Cursor" );
	
	// MeatBall
	pMouse = new cMouseCursor( Renderer,0, 0, IMan->GetPointer( "Mouse_Cursor" ) );
	
	// Do not show the Hardware Cursor
	SDL_ShowCursor( 0 );
}

void QuitGame( void )
{
	DeleteAllBullets();
	DeleteAllParticleEmitter();
	DeleteAllEnemies();
	
	if( pPreferences )
	{
		delete pPreferences;
	}
	
	if( pPlayer )
	{
		delete pPlayer;
	}
	
	if( pLevel )
	{
		delete pLevel;
	}
	
	if( pMouse )
	{
		delete pMouse;
	}

	
	if( pAudio )
	{
		delete pAudio;
	}
	
	
	
	if( pFramerate )
	{
		delete pFramerate;
	}
	
	if( SMan )
	{
		delete SMan;
	}
	if( IMan )
	{
		delete IMan;
	}
	
	if( pCamera )
	{
		delete pCamera;
	}
	
	if( pGameSettings )
	{
		delete pGameSettings;
	}
	
	if( bold_16 )
	{
		TTF_CloseFont( bold_16 );
	}
	
	if( pFont )
	{
		delete pFont;
	}
	
	
	
	SDL_EnableScreenSaver();
	
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	
	//SDL_FreeSurface(Screen);
	//icon = NULL;
	
	QuitSDL();
}

void InitImageManager()
{	IMan	= new cImageManager();	}
void InitSoundManager()
{	SMan	= new cSoundManager();	}
void InitAudio()
{
	// Preferences must be loaded Already
	pAudio = new cAudio();
	pAudio->bMusic		 = pGameSettings->Music;
	pAudio->bSounds		 = pGameSettings->Sounds;
	pAudio->Sound_Volume = pGameSettings->svol;
	pAudio->Music_Volume = pGameSettings->mvol;
	pAudio->InitAudio();
}
void InitFont()
{
	pFont	= new cFont();
	bold_16 = pFont->CreateFont( FONT_DIR "bluebold.ttf", 16, TTF_STYLE_BOLD );
}
void InitCamera()
{
	pCamera = new cCamera();
}
void InitPlayer()
{
	pPlayer = new cPlayer();
}
void InitLevel()
{
	pLevel = new cLevel();
}

