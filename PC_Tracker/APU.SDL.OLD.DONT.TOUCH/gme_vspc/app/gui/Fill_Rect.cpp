
#include "gui/Fill_Rect.h"

void FillRectAlpha(SDL_Renderer *renderer, const SDL_Rect *r, const SDL_Color *c)
{
  FillRectAlpha(renderer, r->x, r->y, r->w, r->h, c->r, c->g, c->b, c->a);
}

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
  Uint8 a = (rgba >> 24) &0xff;
  FillRectAlpha(renderer, x, y, w, h, r, g, b, a);
}

void FillRectAlpha(SDL_Renderer *renderer, SDL_Rect *rect, Uint32 rgba)
{
  Uint8 r = (rgba>>16) & 0xff;
  Uint8 g = (rgba>>8) & 0xff;
  Uint8 b = (rgba) & 0xff;
  Uint8 a = (rgba >> 24) &0xff;
  FillRectAlpha(renderer, rect->x, rect->y, rect->w, rect->h, r, g, b, a);
}

void FillRectAlpha(SDL_Renderer *renderer, SDL_Rect *rect, Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
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
void DrawPixel(SDL_Renderer *renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
  Uint8 oor,og,ob,oa;
  
  // Preserve old color
  SDL_GetRenderDrawColor(renderer, &oor, &og, &ob, &oa);
  // Set new Color
  SDL_SetRenderDrawColor(renderer, (Uint8)red, (Uint8)green, (Uint8)blue, (Uint8)alpha);
  
  // Draw the point
  SDL_RenderDrawPoint(renderer, x, y);
  // Restore old color
  SDL_SetRenderDrawColor(renderer, oor, og, ob, oa);
}