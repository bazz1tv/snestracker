#include "Misc.h"



void UniversalEventHandler(SDL_Event *event)
{
	switch(event->type)
	{
		case SDL_QUIT:
		{
			mode = MODE_QUIT;
			break;
		}
		case SDL_WINDOWEVENT:
			{
				switch (event->window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						{
							
							break;
						}
				}
				break;
			}
		case SDL_KEYDOWN:
		{
			if( event->key.keysym.sym == SDLK_BACKSLASH && (SDL_GetModState() & KMOD_ALT) )
			{
				SDL_ToggleFS(Window);
			}
			break;
		}

	}
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

		//SDL_SetWindowSize(Window,window_width,window_height);
		
        return 0;
    } 
    
    // Swith to FULLSCREEN mode
	
    if (SDL_SetWindowFullscreen(win, SDL_TRUE) < 0)
	{
		std::cout<<"Setting fullscreen failed : "<<SDL_GetError()<<std::endl;
		return -1;
	}

	//SDL_SetWindowSize(Window,window_width,window_height);

	// Code here to redo all graphics textures ( Windows Only ) 
	
	return 1;
}



/// Check for Collision with the Mouse
/// @returnsSDL_TRUE for collision
SDL_bool MouseCollidesWith(SDL_Rect *Crect)
{
	return pMouse->CollisonCheck( Crect );
}