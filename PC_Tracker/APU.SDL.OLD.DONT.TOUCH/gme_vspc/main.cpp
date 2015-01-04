#include "app.h"
#include "Screen.h"
#include "Colors.h"

#include "Render_Context.h"

int init_sdl(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer, SDL_Texture **sdlTexture,
  SDL_Surface **screen, int width, int height);

int main(int argc, char **argv)
{
  init_sdl(&Render_Context::sdlWindow, &Render_Context::sdlRenderer, 
    &Render_Context::sdlTexture, &Render_Context::screen, SCREEN_WIDTH, SCREEN_HEIGHT);

  //fprintf(stderr, "screen = %d\n", Render_Context::screen);
  App app(argc, argv);
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

  SDL_CreateWindowAndRenderer(width, height, 0, sdlWindow, sdlRenderer);
  if (*sdlWindow == NULL || *sdlRenderer == NULL)
  {
    fprintf(stderr, "FCK\n");
    return -1;
  }
  //sdlWindow = *sdlWindow;

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

  SDL_SetWindowTitle(*sdlWindow, PROG_NAME_VERSION_STRING);
  
  Colors::precompute(*screen);

  dblclick::init();

  return 0; 
}










