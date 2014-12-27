#pragma once

// defined in main.cpp
extern Uint32 color_screen_white, color_screen_black, color_screen_cyan, color_screen_magenta, color_screen_yellow, color_screen_red;
extern Uint32  color_screen_green;
extern Uint32 color_screen_gray;
extern Uint32 colorscale[12];

void sdlfont_drawString(SDL_Surface *dst, int x, int y, const char *string, Uint32 color);
