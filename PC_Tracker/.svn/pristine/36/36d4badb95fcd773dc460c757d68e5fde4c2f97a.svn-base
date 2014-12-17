#include "Globals.h"


void FillRectAlpha(SDL_Renderer *renderer, int x, int y, int w, int h, Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
	Uint8 oor,og,ob,oa;
	SDL_Rect rect = {x,y,w,h};
	SDL_GetRenderDrawColor(renderer, &oor, &og, &ob, &oa);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, oor, og, ob, oa);
	//SDL_FillRect(surface, &rect, color);
}

void FillRect(SDL_Renderer *renderer, int x, int y, int w, int h, Uint8 r,Uint8 g,Uint8 b)
{
	FillRectAlpha(renderer, x, y, w, h, r, g, b, 255);
}

void FillRectAlpha(SDL_Renderer *renderer, int x, int y, int w, int h, Uint32 rgba)
{
	Uint8 r = (rgba>>16) & 0xff;
	Uint8 g = (rgba>>8) & 0xff;
	Uint8 b = (rgba) & 0xff;
	Uint8 a	= (rgba >> 24) &0xff;
	FillRectAlpha(renderer, x, y, w, h, r, g, b, a);
}

void FillRectAlpha(SDL_Renderer *renderer, SDL_Rect *rect, Uint32 rgba)
{
	Uint8 r = (rgba>>16) & 0xff;
	Uint8 g = (rgba>>8) & 0xff;
	Uint8 b = (rgba) & 0xff;
	Uint8 a	= (rgba >> 24) &0xff;
	FillRectAlpha(renderer, rect->x, rect->y, rect->w, rect->h, r, g, b, a);
}

//////// FOR SURFACES

void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color)
{
	SDL_Rect rect = {x,y,w,h};
	SDL_FillRect(surface, &rect, color);
}
void FillRectAlpha(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color)
{
	Uint8 alpha = color>>24;

	SDL_Surface *sfc = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, surface->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_FillRect(sfc, NULL, color);
	SDL_SetSurfaceAlphaMod(sfc, alpha);
	//SDL_SetAlpha(sfc, SDL_SRCALPHA, alpha);

	SDL_Rect rect = {x,y,w,h};
	SDL_BlitSurface(sfc,NULL,surface,&rect);
	SDL_FreeSurface(sfc);
}

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
	
	SDL_DestroyRenderer(Renderer);

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