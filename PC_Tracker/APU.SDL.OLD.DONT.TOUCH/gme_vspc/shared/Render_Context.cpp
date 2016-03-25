#include "Render_Context.h"

//SDL_Surface  *  Render_Context::screen       =NULL;
SDL_Window   *  Render_Context::sdlWindow         =NULL;
Uint32          Render_Context::windowID;
//SDL_Window   *  Render_Context::tracker_sdlWindow =NULL;
SDL_Renderer *  Render_Context::sdlRenderer       =NULL;
SDL_Texture  *  Render_Context::sdlTexture        =NULL;
SDL_Surface  *  Render_Context::screen            =NULL;

#include "Screen.h"
// used for SDL_ResizeWinwow events
int Render_Context::w=SCREEN_WIDTH,Render_Context::h=SCREEN_HEIGHT;

void Render_Context::clear_screen(SDL_Surface *screen/*=screen*/, SDL_Texture *sdlTexture/*=sdlTexture*/,
  SDL_Renderer *sdlRenderer/*=sdlRenderer*/)
{
  SDL_FillRect(screen, NULL, 0);
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Render_Context::sdl_draw(SDL_Surface *screen/*=screen*/, SDL_Texture *sdlTexture/*=sdlTexture*/,
  SDL_Renderer *sdlRenderer/*=sdlRenderer*/)
{
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Render_Context::load_from_window(Window *win)
{
  sdlWindow = win->sdlWindow;
  windowID = win->windowID;
  sdlRenderer = win->sdlRenderer;
  sdlTexture = win->sdlTexture;
  screen = win->screen;
}