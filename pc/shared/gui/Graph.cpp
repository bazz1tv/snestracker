#include "Graph.h"

void Graph::draw_border()
{
  SDL_Rect scaled = {
    bounds.x * ::render->sx,
    bounds.y * ::render->sy,
    bounds.w * ::render->sx,
    bounds.h * ::render->sy
  };

  SDL_SetRenderDrawColor(::render->sdlRenderer,
      (Uint8) ((border_color >> 16) & 0xff),
      (Uint8) ((border_color >> 8)  & 0xff),
      (Uint8) ((border_color)       & 0xff),
      255);
  SDL_RenderDrawRect(::render->sdlRenderer, &scaled);
}
void Graph::draw_bg()
{
  SDL_Rect scaled = {
    bounds.x * ::render->sx,
    bounds.y * ::render->sy,
    bounds.w * ::render->sx,
    bounds.h * ::render->sy
  };
  SDL_SetRenderDrawColor(::render->sdlRenderer,
      (Uint8) ((bg_color >> 16) & 0xff),
      (Uint8) ((bg_color >> 8)  & 0xff),
      (Uint8) ((bg_color)       & 0xff),
      (Uint8) ((bg_color >> 24) & 0xff));
  SDL_RenderFillRect(::render->sdlRenderer, &scaled);
}
void Graph::draw_lines()
{
  SDL_Point *scaled = (SDL_Point *) SDL_malloc(sizeof(SDL_Point) * pcount);
  for (int i=0; i < pcount; i++)
  {
    scaled[i].x = points[i].x * ::render->sx;
    scaled[i].y = points[i].y * ::render->sy;
  }
  // todo: Remove this draw_bg() call (keep it separate)
  draw_bg();
  SDL_SetRenderDrawColor(::render->sdlRenderer,
      (Uint8) ((fg_color >> 16) & 0xff),
      (Uint8) ((fg_color >> 8)  & 0xff),
      (Uint8) ((fg_color)       & 0xff),
      255);

  SDL_RenderDrawLines(::render->sdlRenderer, scaled, pcount);
  SDL_free(scaled);
}
