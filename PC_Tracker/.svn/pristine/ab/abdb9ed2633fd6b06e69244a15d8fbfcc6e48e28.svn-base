#pragma once

#include "Globals.h"

class GraphicsObject
{
public:
	GraphicsObject()
	{
		surface = NULL;
		texture = NULL;
	}
	~GraphicsObject() 
	{ 
		if (surface)
		{
			SDL_FreeSurface(surface);
			surface = NULL;
		}
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
	}
	SDL_Surface *surface;
	SDL_Rect rect;
	SDL_Texture *texture;
};