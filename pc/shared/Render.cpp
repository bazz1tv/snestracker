#include "Render.h"
#include "Screen.h"

Render *render;

SDL_Renderer *Render::sdlRenderer      =NULL;
SDL_Texture *Render::sdlTexture        =NULL;
SDL_Surface *Render::screen            =NULL;


Render::Render()
{
  ::render = this;
  sdlWindow         =NULL;

  w=SCREEN_WIDTH;
  h=SCREEN_HEIGHT;
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
void Render::clear_screen(SDL_Surface *screen/*=screen*/,
                          SDL_Texture *sdlTexture/*=sdlTexture*/,
                          SDL_Renderer *sdlRenderer/*=sdlRenderer*/)
{
  SDL_FillRect(screen, NULL, 0);
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Render::sdl_draw(SDL_Surface *screen/*=screen*/,
                      SDL_Texture *sdlTexture/*=sdlTexture*/,
                      SDL_Renderer *sdlRenderer/*=sdlRenderer*/)
{
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Render::load_from_window(Window *win)
{
  sdlWindow = win->sdlWindow;
  windowID = win->windowID;
  sdlRenderer = win->sdlRenderer;
  sdlTexture = win->sdlTexture;
  screen = win->screen;
}
