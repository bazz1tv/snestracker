#include "GraphicsObject.h"

void GraphicsObject::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

/// Updates Rect with x,y coordinate and surface height/width
void GraphicsObject::UpdateRect(int x, int y)
{
	rect = SetRect(x,y,surface->w, surface->h);
}

// just Updates Rect from a new surface height/width
void GraphicsObject::UpdateRect()
{
	rect = SetRect(rect.x,rect.y,surface->w, surface->h);
}

void GraphicsObject::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}
void GraphicsObject::Draw()
{
	Draw(Renderer);
}