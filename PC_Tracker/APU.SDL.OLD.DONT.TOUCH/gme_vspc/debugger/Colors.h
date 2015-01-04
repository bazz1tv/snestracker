#pragma once

#include "SDL.h"
#include "Render_Context.h"
struct Colors : public Render_Context
{
  //Colors(SDL_Surface *cscreen);
  //static SDL_Surface *screen;

  static Uint32 white, black, cyan, magenta, yellow, red;
  static Uint32 green, blue, nearblack;
  static Uint32 dark_magenta, dark_cyan, dark_yellow;
  static Uint32 gray;
  static Uint32 colorscale[12];

  static void precompute(SDL_Surface *cscreen);
  // belongs in color namespace
  static Uint32 subtract(Uint32 *c, Uint8 subval);
  static void subtractp(Uint32 *c, Uint8 subval);
};