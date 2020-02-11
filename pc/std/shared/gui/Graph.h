#include "SDL.h"
#include "Render.h"
#include "Colors.h"

/* A graph helps plot an array of Points. It should help map this array of
 * points from another interpretation of data via scaling factors. */

struct Graph
{
  void draw_border()
  {
    SDL_SetRenderDrawColor(::render->sdlRenderer,
        (Uint8) ((border_color >> 16) & 0xff),
        (Uint8) ((border_color >> 8)  & 0xff),
        (Uint8) ((border_color)       & 0xff),
        255);
    SDL_RenderDrawRect(::render->sdlRenderer, &bounds);
  }
  void draw_bg()
  {
    SDL_SetRenderDrawColor(::render->sdlRenderer,
        (Uint8) ((bg_color >> 16) & 0xff),
        (Uint8) ((bg_color >> 8)  & 0xff),
        (Uint8) ((bg_color)       & 0xff),
        255);
    SDL_RenderFillRect(::render->sdlRenderer, &bounds);
  }
  void draw_lines()
  {
    draw_bg();
    SDL_SetRenderDrawColor(::render->sdlRenderer,
        (Uint8) ((fg_color >> 16) & 0xff),
        (Uint8) ((fg_color >> 8)  & 0xff),
        (Uint8) ((fg_color)       & 0xff),
        255);

    SDL_RenderDrawLines(::render->sdlRenderer, points, pcount);
  }

  Uint32 fg_color, bg_color, border_color;
  int xinterval, yinterval;
  SDL_Rect bounds;
  SDL_Point *points;
  int pcount;
};
