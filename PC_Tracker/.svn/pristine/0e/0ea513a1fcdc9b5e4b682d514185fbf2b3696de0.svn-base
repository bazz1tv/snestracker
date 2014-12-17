
#include "SDL_ep.h"

int InitSDL( Uint32 flags /* = SDL_INIT_EVERYTHING  */ )
{
	if ( !SDL_WasInit( NULL ) && ( SDL_Init( flags ) == -1 ) )
	{
		printf ( "Error : Can't init SDL\n" );
		return 0;
	}

	return 1;
}

SDL_Surface *InitScreen( int width, int height, int bpp, bool Fullscreen /* = 0 */, 
						Uint32 flags /* = SDL_HWSURFACE | SDL_HWACCEL | SDL_RLEACCEL | SDL_DOUBLEBUF  */ )
{
	SDL_Surface *target = NULL;

	if( Fullscreen ) 
	{
		flags |= SDL_FULLSCREEN;
	}

	target = SDL_SetVideoMode( width, height, bpp, flags );

	if( !target )
	{
		printf( "Error : Can't create Video Screen" );
		return NULL;
	}

	return target;
}

void SetWindowCaption( const char *Caption, const char *iconpath /* = NULL  */ )
{
	SDL_WM_SetCaption( Caption, iconpath );	
}

void SetWindowIcon( SDL_Surface *Icon, Uint8 *Imask /* = NULL  */ )
{
	if( !Icon )
	{
		return;
	}

	SDL_WM_SetIcon( Icon, Imask );
}

void QuitSDL( void )
{
	SDL_Quit();
}