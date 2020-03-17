#pragma once
#include "SDL.h"
#include "Render.h"
#include "Colors.h"

/* A graph helps plot an array of Points. It should help map this array of
 * points from another interpretation of data via scaling factors. */

struct Graph
{
  void draw_border();
  void draw_bg();
  void draw_lines();

  Uint32 fg_color, bg_color, border_color;
  int xinterval, yinterval;
  SDL_Rect bounds;
  SDL_Point *points;
  int pcount;
};
