
#include "Globals.h"
#include "Game.h"
#include "MainMenu.h"

using namespace std;



// SDL_ep classes
cFramerate *pFramerate;
cImageManager *IMan;
cSoundManager *SMan;
cFont *pFont;
cAudio *pAudio;
cTCPNet *pTCP;



// Main window Icon
SDL_Surface *icon;

/// ifSDL_TRUE the game exits
int done;

/// ifSDL_TRUE the game if paused
SDL_bool paused;

// Fonts
TTF_Font *Menu_Font;
TTF_Font *bold_16;

/// the Screen
SDL_Surface *Screen;
SDL_Window *Window;
int window_width;
int window_height;
SDL_Renderer *Renderer;
SDL_Texture *ScreenTexture;

/// Keyboard Keystate
Uint8 *keys;

/// Input Events
SDL_Event event;

/// ifSDL_TRUE the fps display is active
SDL_bool fps_display =SDL_FALSE;

/// Magneta (default) Colorkey
Uint32 colorkey = 0;

/// MeatBall
cPreferences *pPreferences;
cSettings *pGameSettings;
cMouseCursor *pMouse;
cCamera *pCamera;
cPlayer *pPlayer;
cLevel *pLevel;
cLevelEditor *pLevelEditor;
TextObjectEngine *TOE;

/// The Console
cConsole *pConsole;

/// The Current mode
Uint8 mode, oldmode;

/// The screen settings 
//const SDL_VideoInfo* screeninfo;

/// Initializes the basic
/// Init some Fonts, set default colorkey, Add mouse to IMan
/// Init the mouse,camera,player,level,leveleditor, and Console
/// Preload bulletImages
void InitObjects( void )
{
	Menu_Font = pFont->CreateFont( FONT_DIR "Bluehigd.ttf", 27, TTF_STYLE_BOLD );
	bold_16 = pFont->CreateFont( FONT_DIR "bluebold.ttf", 16, TTF_STYLE_BOLD );

	// Set Magneta (default) Colorkey
	//colorkey = SDL_MapRGB( Screen->format, 255, 0, 255 );
	colorkey = 0xffff00ff; // GHETTO STYLE <- 
	
	IMan->Add( LoadImage( PIXMAPS_DIR "internal/Mouse.png", colorkey ), "Mouse_Cursor" );

	// MeatBall
	pMouse = new cMouseCursor( Renderer,0, 0, IMan->GetPointer( "Mouse_Cursor" ) );
	pCamera = new cCamera();
	pPlayer = new cPlayer();
	pLevel = new cLevel();
	pLevelEditor = new cLevelEditor();
	pConsole = new cConsole();

	// Do not show the Hardware Cursor
	SDL_ShowCursor( 0 );
	
	PreloadBulletimages();
}

/// Quits the game
void QuitGame( void )
{
	if( pPreferences )
	{
		delete pPreferences;
	}
	
	DeleteAllBullets();
	DeleteAllParticleEmitter();
	DeleteAllEnemies();

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

	if( pLevelEditor ) 
	{
		delete pLevelEditor;
	}

	if ( pConsole )
	{
		delete pConsole;
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

	if( Menu_Font ) 
	{
		TTF_CloseFont( Menu_Font );
	}

	if( bold_16 ) 
	{
		TTF_CloseFont( bold_16 );
	}
	

	if( pFont )
	{
		delete pFont;
	}

	//SDL_FreeSurface(icon);
	
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);

	//SDL_FreeSurface(Screen);
	icon = NULL;

	QuitSDL();
}


SDL_bool FileValid( string filename )
{
	ifstream ifs;
	ifs.open( filename.c_str(), ios :: out );

	if( ifs ) 
	{
		ifs.close();
		return SDL_TRUE;
	}

	return SDL_FALSE;
}

SDL_bool is_valid_number( char *c )
{
	SDL_bool floatnumber = SDL_FALSE;
	if (*c == '-') 
		c++;
	for(;*c;c++)
	{
		if (*c == '.')
		{
			if (!floatnumber)
			{
				floatnumber = SDL_TRUE;
				c++;
			} 
			else { return SDL_FALSE; }
		}
		
		if ((*c < '0' || *c > '9') && *c != '\r')
		{
			return SDL_FALSE;
	
		}
	}
	return SDL_TRUE;
}

void FramerateDraw( SDL_Surface *target, double posx /** = 5.0 */, double posy /** = 5.0  */)
{
	if ( !fps_display )
	{
		return;
	}

	char fps_string[100]; 
	sprintf( fps_string, "FPS : %d", (int)pFramerate->fps );
	
	SDL_Surface *image = pFont->CreateText( fps_string, bold_16 );
	
	if( !image ) 
	{
		return;
	}
	
	SDL_Rect rect = SetRect( (int)posx, (int)posy, image->w, image->h );

	SDL_BlitSurface( image, NULL, target, &rect );

	SDL_FreeSurface( image );
}


