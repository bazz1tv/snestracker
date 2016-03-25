#include "app.h"
#include "Screen.h"
#include "Colors.h"
#include "Organization.h"

#include "Render_Context.h"

int init_sdl(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer, SDL_Texture **sdlTexture,
  SDL_Surface **screen, int width, int height);

int main(int argc, char **argv)
{
  init_sdl(&Render_Context::sdlWindow, &Render_Context::sdlRenderer, 
    &Render_Context::sdlTexture, &Render_Context::screen, SCREEN_WIDTH, SCREEN_HEIGHT);

  Render_Context::windowID = SDL_GetWindowID(Render_Context::sdlWindow);
  //SDL_SetWindowSize(Render_Context::sdlWindow, 1024+100, 768+100);

  //fprintf(stderr, "screen = %d\n", Render_Context::screen);
  App app(argc, argv, 32000);
  app.run();
}


int init_sdl(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer, SDL_Texture **sdlTexture, 
  SDL_Surface **screen, int width, int height)
{
  Uint32 flags=0;
  flags |= SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER; 

  if (SDL_Init(flags) != 0) 
  {
    fprintf(stderr,
            "\nUnable to initialize SDL:  %s\n",
            SDL_GetError()
           );
    return -1;
  }
  atexit(SDL_Quit);

  SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN /*| SDL_WINDOW_BORDERLESS*/, sdlWindow, sdlRenderer);
  if (*sdlWindow == NULL || *sdlRenderer == NULL)
  {
    fprintf(stderr, "FCK\n");
    return -1;
  }
  //sdlWindow = *sdlWindow;
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");  // make the scaled rendering look smoother.
  SDL_RenderSetLogicalSize(*sdlRenderer, width, height);

  *screen = SDL_CreateRGBSurface(0, width, height, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
  *sdlTexture = SDL_CreateTexture(*sdlRenderer,
                                          SDL_PIXELFORMAT_RGB888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          width, height);

  if (*screen == NULL || *sdlTexture == NULL)
  {
    fprintf(stderr, "MAn we have SDL init problems :\\. I couldn't allocate a screen or Texture :\\\n");
    return -1;
  }

  // needs to be more permanent eh!
  char tmpbuf[100];
  sprintf(tmpbuf, PROG_NAME_VERSION_STRING, APP_VERSION);
  SDL_SetWindowTitle(*sdlWindow, tmpbuf);
  
  Colors::precompute(*screen);

  dblclick::init();

  return 0; 
}










