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


/// Queries the Screen if it's set to Full of Not
/// @return 0 if windowed, 1 if fullscreen
SDL_bool IsFullScreen(SDL_Window *win)
{
	Uint32 flags = SDL_GetWindowFlags(win);
	
    if (flags & SDL_WINDOW_FULLSCREEN) return SDL_TRUE; // return SDL_TRUE if surface is fullscreen
    
	return SDL_FALSE; // Return SDL_FALSE if surface is windowed
} 

/// Toggles On/Off FullScreen
/// @returns SDL_FALSE on error, SDL_TRUE on Set fullscreen successful, SDL_FALSE on Set Windowed successful
SDL_bool SDL_ToggleFS(SDL_Window *win)
{ 
    //Uint32 flags = surface->flags; // Get the video surface flags
    
    if (IsFullScreen(win))
    { 
        // Swith to WINDOWED mode 
        if (SDL_SetWindowFullscreen(win, SDL_FALSE) < 0)
		{
			std::cout<<"Setting windowed failed : "<<SDL_GetError()<<std::endl;
			return SDL_FALSE;
		}
		
        return SDL_FALSE;
    } 
    
    // Swith to FULLSCREEN mode
	//SDL_LockSurface(surface);
    if (SDL_SetWindowFullscreen(win, SDL_TRUE) < 0)
	{
		std::cout<<"Setting fullscreen failed : "<<SDL_GetError()<<std::endl;
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}

/// Check for Collision with the Mouse
/// @returnsSDL_TRUE for collision
SDL_bool MouseCollidesWith(SDL_Rect *Crect)
{
	return pMouse->CollisonCheck( Crect );
}