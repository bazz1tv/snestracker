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
#include "TestSuite.h"
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif


int DoGame();
void QuitGame();
void initEngine();
int processclargs(int, char **);
void Test(SDL_bool with_verbose);

void OS_Specific()
{
#ifdef __APPLE__
	
	CFURLRef resourcesURL = CFBundleCopyExecutableURL( CFBundleGetMainBundle() );
	char pathtofile[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)pathtofile, PATH_MAX))
	{
		// error!
	}
	CFRelease(resourcesURL);
	
	string longpath(pathtofile);
	string desiredpath;
	size_t pos;
	
	pos = longpath.rfind(".app/");
	desiredpath = longpath.substr(0,pos);
	// trim off the rest
	pos = desiredpath.find_last_of('/');
	desiredpath.erase(pos);
	
	chdir(desiredpath.c_str());
#endif

}

int main (int argc, char **argv)
{
	OS_Specific();
	initEngine();
	
	MB = new MasterBlaster();
	// Add Code to Check arguments for -Test
	// this will Run Test Cases code for All Components
	int rval = processclargs(argc, argv);
	if (rval == MODE_TEST)
	{
		QuitGame();
		return 0;
	}
	
	
	
	
	MB->Loop();
	
	// After having MUCH FUN.....
	delete MB;
	QuitGame();
	
	return 0;
}

void Test(SDL_bool with_verbose)
{
	//TestSuite TS;
	TS::verbose = with_verbose;
	TS::Begin();
}

int processclargs(int argc, char *argv[])
{
	int c;
	SDL_bool testmode=SDL_FALSE,verbosemode=SDL_FALSE;
	// parse for Verbose too
	
	printf ("Processing arguments!\n\n");
	
	while( --argc > 0 && (*++argv)[0] == '-')
		while( (c = *++argv[0]) ) //bug to investigate: what are the side effects of c == *++argv[0], which was a bug before the fix.
			switch (c) {
				case 't':
					//printf("Argv[1] = : %s\n", argv[1]);
					testmode = SDL_TRUE;
					continue;
				case 'v':
					verbosemode = SDL_TRUE;
					continue;
				default:
					//printf("illegal option %c\n", c);
					//argc = 0;
					//return -1;
					break;
			}
	
	if (testmode)
	{
		SDL_ShowCursor(1);
		Test(verbosemode);
		return MODE_TEST;
	}
	
    return 0;
}



// For InitEngine
void InitFramerate(int nframes_sec);
void InitImageManager();
void InitSoundManager();
void InitAudio();
void InitFont();
void InitCamera();
void InitPlayer();
void InitLevel();

void MakeWindow();
void SetupMouse();
void InitPreferences();
void InitObjects();
void SetupWindowAndDockIcon();

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
	
	MakeWindow();
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
void MakeWindow()
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
	
	window.sdlw = GetWindow(APP_TITLE, closest.w, closest.h, pPreferences->pSettings->Fullscreen ? SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_GRABBED : SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	//SDL_SetWindowMaximumSize(Window, window.h, window.h);
	//SDL_SetWindowMinimumSize(Window, window.h, window.h);
	
	Renderer = GetRenderer(window.sdlw, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	window.h = pGameSettings->Screen_H;
	window.w = pGameSettings->Screen_W;
	
	//SDL_SetWindowBordered(Window, SDL_FALSE);
	
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
	SDL_SetWindowIcon(window.sdlw, icon);
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
	
	SDL_DestroyWindow(window.sdlw);
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

