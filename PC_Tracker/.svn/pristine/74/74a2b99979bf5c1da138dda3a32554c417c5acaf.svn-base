
#include "Globals.h"
#include "preferences.h"
#include "player.h"
#include "Camera.h"
#include "level.h"
#include "leveleditor.h"
#include "console.h"
#include "bullet.h"
#include <fstream>

cMouseCursor *pMouse;		// The Mouse

/// ifSDL_TRUE the game exits
int done;

/// ifSDL_TRUE the game if paused
SDL_bool paused;
	
//extern cPreferences *pPreferences;	// The Preferences
//extern cSettings *pGameSettings;	// The Game Settings

cFramerate *pFramerate;
cImageManager *IMan;
cSoundManager *SMan;
	
cFont *pFont;
cAudio *pAudio;
cTCPNet *pTCP;
TTF_Font *bold_16;



/// the Screen
SDL_Window *Window;
int window_width;
int window_height;
SDL_Renderer *Renderer;

/// Keyboard Keystate
Uint8 *keys;

/// Input Events
SDL_Event event;

/// ifSDL_TRUE the fps display is active
SDL_bool fps_display =SDL_FALSE;

/// Magneta (default) Colorkey
Uint32 colorkey = 0;

/// The Current mode
//Uint8 mode, oldmode;
	



using namespace std;

void SetColorKey(Uint32 col)
{
	colorkey = col;
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
		
		if ((*c < '0' || *c > '9') && *c != '\r' && *c != '\n')
		{
			return SDL_FALSE;
	
		}
	}
	return SDL_TRUE;
}

void FramerateDraw( SDL_Renderer *renderer, double posx /** = 5.0 */, double posy /** = 5.0  */)
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
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, image);
	SDL_RenderCopy(renderer, tex, NULL, &rect);

	//SDL_BlitSurface( image, NULL, target, &rect );

	SDL_FreeSurface( image );
	SDL_DestroyTexture(tex);
}


