#pragma once

#include "Globals.h"

class GraphicsObject
{
public:
	~GraphicsObject() 
	{ 
		if (surface)
		{
			SDL_FreeSurface(surface);
			surface = NULL;
		}
	}
	SDL_Surface *surface;
	SDL_Rect rect;
};