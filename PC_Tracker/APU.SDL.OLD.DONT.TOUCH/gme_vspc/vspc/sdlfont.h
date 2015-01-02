#pragma once
#include "SDL.h"
#include "colors.h"

void sdlfont_drawString(SDL_Surface *dst, int x, int y, const char *string, Uint32 color);
void sdlfont_drawString2c(SDL_Surface *dst, int x, int y, const char *string, Uint32 color1, Uint32 Color2);
