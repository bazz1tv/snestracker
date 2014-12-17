
#include "SDL_ep.h"

Screenscale ScreenScale;

int InitSDL( Uint32 flags /* = SDL_INIT_EVERYTHING  */ )
{
	if ( !SDL_WasInit( NULL ) && ( SDL_Init( flags ) == -1 ) )
	{
		printf ( "Error : Can't init SDL\n" );
		return 0;
	}
	
	ScreenScale.x = 1;
	ScreenScale.y = 1;

	return 1;
}

void SetScreenScale(int sx, int sy)
{
	ScreenScale.x = sx;
	ScreenScale.y = sy;
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


SDL_Window *GetWindow(const char *title, int width, int height, Uint32 wflags)
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


/// Queries the Screen to see if it's set to Fullscreen or Not
/// @returns SDL_FALSE if windowed, SDL_TRUE if fullscreen
SDL_bool IsFullScreen(SDL_Window *win)
{
	Uint32 flags = SDL_GetWindowFlags(win);
	
    if (flags & SDL_WINDOW_FULLSCREEN) return SDL_TRUE; // return SDL_TRUE if fullscreen
    
	return SDL_FALSE; // Return SDL_FALSE if windowed
}

/// Toggles On/Off FullScreen
/// @returns -1 on error, 1 on Set fullscreen successful, 0 on Set Windowed successful
int SDL_ToggleFS(SDL_Window *win)
{
	//SDL_DisplayMode mode;
	//SDL_GetWindowDisplayMode(Window, &mode);
	
	//SDL_DestroyRenderer(Renderer);
	
    if (IsFullScreen(win))
    {
        // Swith to WINDOWED mode
        if (SDL_SetWindowFullscreen(win, SDL_FALSE) < 0)
		{
			std::cout<<"Setting windowed failed : "<<SDL_GetError()<<std::endl;
			return -1;
		}
		
		//SDL_SetWindowSize(Window,window.w,window.h);
		
        return 0;
    }
    
    // Swith to FULLSCREEN mode
	
    if (SDL_SetWindowFullscreen(win, SDL_TRUE) < 0)
	{
		std::cout<<"Setting fullscreen failed : "<<SDL_GetError()<<std::endl;
		return -1;
	}
	
	//SDL_SetWindowSize(Window,window.w,window.h);
	
	// Code here to redo all graphics textures ( Windows Only )
	
	return 1;
}

void QuitSDL( void )
{
	SDL_Quit();
}