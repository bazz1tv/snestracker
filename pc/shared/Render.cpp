#include "Render.h"
#include "Colors.h"
#include "utility.h"
#include "gui/Window.h"

Render *render;

Render::Render(int w, int h) : w(w), h(h)
{
  sdlWindow         =NULL;

  sx = sy = 1.0;
  logical = false;
}

Render::~Render()
{
  if (screen)
    SDL_FreeSurface(screen);
  if (sdlTexture)
    SDL_DestroyTexture(sdlTexture);
  if (sdlRenderer)
    SDL_DestroyRenderer(sdlRenderer);
  if(sdlWindow)
    SDL_DestroyWindow(sdlWindow);
}
void Render::clear_screen()
{

  SDL_FillRect(screen, NULL, 0);
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  Utility::set_render_color_rgba(sdlRenderer, Colors::transparent);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Render::sdl_draw()
{
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_SetRenderDrawColor (sdlRenderer, 0, 0, 0, 0);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Render::load_from_window(Window *win)
{
  sdlWindow = win->render.sdlWindow;
  windowID = win->render.windowID;
  sdlRenderer = win->render.sdlRenderer;
  sdlTexture = win->render.sdlTexture;
  screen = win->render.screen;
}
