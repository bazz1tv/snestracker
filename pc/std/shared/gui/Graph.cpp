#include "Graph.h"

void Graph::draw_border()
{
  SDL_SetRenderDrawColor(::render->sdlRenderer,
      (Uint8) ((border_color >> 16) & 0xff),
      (Uint8) ((border_color >> 8)  & 0xff),
      (Uint8) ((border_color)       & 0xff),
      255);
  SDL_RenderDrawRect(::render->sdlRenderer, &bounds);
}
void Graph::draw_bg()
{
  SDL_SetRenderDrawColor(::render->sdlRenderer,
      (Uint8) ((bg_color >> 16) & 0xff),
      (Uint8) ((bg_color >> 8)  & 0xff),
      (Uint8) ((bg_color)       & 0xff),
      (Uint8) ((bg_color >> 24) & 0xff));
  SDL_RenderFillRect(::render->sdlRenderer, &bounds);
}
void Graph::draw_lines()
{
  draw_bg();
  SDL_SetRenderDrawColor(::render->sdlRenderer,
      (Uint8) ((fg_color >> 16) & 0xff),
      (Uint8) ((fg_color >> 8)  & 0xff),
      (Uint8) ((fg_color)       & 0xff),
      255);

  SDL_RenderDrawLines(::render->sdlRenderer, points, pcount);
}
