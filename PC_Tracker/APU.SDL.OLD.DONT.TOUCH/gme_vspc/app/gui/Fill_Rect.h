/// @addtogroup Engine_Gfx
// @{

/* 
  @file Fill_Rect.h  Graphic functions
  
  (c) Bazz
  
  V 1.3
*/


#pragma once

#include <SDL.h>

void FillRectAlpha(SDL_Renderer *renderer, const SDL_Rect *rect, const SDL_Color *color);
void FillRectAlpha(SDL_Renderer *renderer, int x, int y, int w, int h, Uint8 r,Uint8 g,Uint8 b,Uint8 a);
void FillRectAlpha(SDL_Renderer *renderer, int x, int y, int w, int h, Uint32 rgba);
void FillRect(SDL_Renderer *renderer, int x, int y, int w, int h, Uint8 r,Uint8 g,Uint8 b);
void FillRectAlpha(SDL_Renderer *renderer, SDL_Rect *rect, Uint32 rgba);
void FillRectAlpha(SDL_Renderer *renderer, SDL_Rect *rect, Uint8 r,Uint8 g,Uint8 b,Uint8 a);

void DrawPixel(SDL_Renderer *renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

/// Todo ! :)
// @}