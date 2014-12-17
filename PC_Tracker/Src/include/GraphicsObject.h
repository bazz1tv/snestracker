#pragma once

#include "OS.h"
extern SDL_Renderer *Renderer;

/// A simple collection of Surface/Texture/Rect
/// for most basic graphics objects
class GraphicsObject
{
public:
	GraphicsObject()
	{
		surface = NULL;
		texture = NULL;
		rect = SetRect(0,0,0,0);
	}
	~GraphicsObject() 
	{ 
		freeSurface();
		freeTexture();
	}
	
	/// Set Rect X,Y to x,y
	void SetPos(int x, int y);
		
	/// Updates Rect with x,y coordinate and surface height/width
	void UpdateRect(int x, int y);
	
	// just Updates Rect from a new surface height/width
	void UpdateRect();
	
	void Draw(SDL_Renderer *renderer);
	void Draw();
	
	
	void freeSurface()
	{
	
		if (surface)
		{
			SDL_FreeSurface(surface);
			surface = NULL;
		}
			
	}
	
	void freeTexture()
	{
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
	}
	
	SDL_Surface *surface;
	SDL_Rect	 rect;
	SDL_Texture *texture;
};