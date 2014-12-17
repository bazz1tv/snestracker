
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

//  * * * Tagged for mods to SDL2
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
int InitWindow( SDL_Window *win, SDL_Renderer *Screen, const char *title, int width, int height, int bpp, Uint32 wflags, Uint32 rflags, SDL_bool fullscreen /* = SDL_FALSE */)
{
	if (fullscreen == SDL_TRUE)
	{
		wflags |= SDL_WINDOW_FULLSCREEN;
	}
	
	win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, wflags);
	
	if( win == NULL )
	{
		printf( "Error : Can't create Window\n" );
		return -1;
	}
	
	Screen = SDL_CreateRenderer(win, -1, rflags);
	
	if (Screen == NULL)
	{
		printf("Error : Can't create Renderer\n");
		return -1;
	}
	
	return 0; // You did it! 
}

SDL_Window *GetWindow(const char *title, int width, int height, int bpp, Uint32 wflags)
{	
	return SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, wflags);
}

SDL_Renderer *GetRenderer(SDL_Window *win, Uint32 rflags)
{
	return SDL_CreateRenderer(win, -1, rflags);
}

void SetWindowCaption( SDL_Window *win, const char *Caption )
{
	SDL_SetWindowTitle(win,Caption);
}

void SetWindowIcon( SDL_Window *win, SDL_Surface *Icon )
{
	if( !Icon )
	{
		return;
	}

	SDL_SetWindowIcon( win, Icon);
}

void QuitSDL( void )
{
	SDL_Quit();
}