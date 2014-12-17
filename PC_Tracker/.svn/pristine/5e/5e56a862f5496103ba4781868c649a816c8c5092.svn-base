
#include "Globals.h"
#include "preferences.h"
#include "player.h"
#include "Camera.h"
#include "level.h"
#include "leveleditor.h"
#include "console.h"
#include "bullet.h"
#include "MasterBlaster.h"
#include <fstream>

std::stringstream error;
void PrintErrorToConsole()
{
	std::cout << error.str();
}
void ClearErrorStream()
{
	error.str(std::string());
}

cMouseCursor *pMouse;		// The Mouse

/// ifSDL_TRUE the game exits
//int done;

/// ifSDL_TRUE the game if paused
SDL_bool paused;
	
//extern cPreferences *pPreferences;	// The Preferences
//extern cSettings *pGameSettings;	// The Game Settings

cFramerate *pFramerate;
cImageManager *IMan;
cSoundManager *SMan;
	
//cFont *pFont;
cAudio *pAudio;
cTCPNet *pTCP;
TTF_Font *bold_16;



/// the Screen
/*SDL_Window *Window;
int window.h;
int window.h;*/
Window window;
SDL_Renderer *Renderer;

/// Keyboard Keystate
Uint8 *keys;

/// Input Events
SDL_Event event;

/// ifSDL_TRUE the fps display is active
 //=SDL_FALSE;

/// Magneta (default) Colorkey
Uint32 colorkey = 0;

/// The Current mode
//Uint8 mode, oldmode;
	



using namespace std;

void SetColorKey(Uint32 col)
{
	colorkey = col;
}




void FramerateDraw( SDL_Renderer *renderer, double posx /** = 5.0 */, double posy /** = 5.0  */)
{
	if ( !MB->fps_display )
	{
		return;
	}

	char fps_string[100]; 
	sprintf( fps_string, "FPS : %d", (int)pFramerate->fps );
	
	SDL_Surface *image = cFont::CreateText( fps_string, bold_16 );
	
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


