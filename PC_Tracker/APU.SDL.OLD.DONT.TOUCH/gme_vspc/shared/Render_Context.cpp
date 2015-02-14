#include "Render_Context.h"
//SDL_Surface  *  Render_Context::screen       =NULL;
SDL_Window   *  Render_Context::sdlWindow         =NULL;
//SDL_Window   *  Render_Context::tracker_sdlWindow =NULL;
SDL_Renderer *  Render_Context::sdlRenderer       =NULL;
SDL_Texture  *  Render_Context::sdlTexture        =NULL;
SDL_Surface  *  Render_Context::screen            =NULL;
char Render_Context::tmpbuf[500];

void Render_Context::clear_screen()
{
  SDL_FillRect(screen, NULL, 0);
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Render_Context::sdl_draw()
{
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}