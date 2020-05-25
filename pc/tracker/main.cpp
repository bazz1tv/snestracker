#include "app.h"
#include "Screen.h"
#include "Colors.h"
#include "Organization.h"

#include "Render.h"
#include <time.h>
#include "shared/SdlNfd.h"
#include "DEBUGLOG.h"
#include "sdl_dblclick.h"
#include "globals.h"

int init_sdl(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer,
             SDL_Texture **sdlTexture, SDL_Surface **screen,
             int width, int height, Uint32 flags=0);

int main(int argc, char **argv)
{
  time_t t = time(NULL);
  srand((unsigned int)t);

  Render render;
  ::render = &render;

  init_sdl(&render.sdlWindow, &render.sdlRenderer,
    &render.sdlTexture, &render.screen, SCREEN_WIDTH, SCREEN_HEIGHT);

  render.windowID = SDL_GetWindowID(render.sdlWindow);
  SdlNfd::init(render.sdlWindow);


  App app(argc, argv, 32000);
  app.run();
}


int init_sdl(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer, SDL_Texture **sdlTexture, 
  SDL_Surface **screen, int width, int height, Uint32 flags/*=0*/)
{
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

  *sdlWindow = SDL_CreateWindow("snes tracker", SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
  if (*sdlWindow == NULL)
  {
    fprintf(stderr, "Couldn't create the main window! %s\n", SDL_GetError());
    return -1;
  }

  *sdlRenderer = SDL_CreateRenderer(*sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (*sdlRenderer == NULL)
  {
    DEBUGLOG("No Renderer: %s\n", SDL_GetError());
    *sdlRenderer = SDL_CreateRenderer(*sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (*sdlRenderer == NULL)
    {
      DEBUGLOG("No Renderer: %s\n", SDL_GetError());
      *sdlRenderer = SDL_CreateRenderer(*sdlWindow, -1, 0);
      if (*sdlRenderer == NULL)
      {
        fprintf(stderr,
            "\nUnable to initialize SDL:  %s\n",
            SDL_GetError()
            );
        return -1;
      }
    }
  }

  // make the scaled rendering look smoother.
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
  SDL_RenderSetLogicalSize(*sdlRenderer, width, height);
  SDL_RenderSetIntegerScale(*sdlRenderer, SDL_TRUE);

  *screen = SDL_CreateRGBSurface(0, width, height, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
  SDL_SetColorKey(*screen, SDL_TRUE, 0);
  SDL_SetSurfaceBlendMode(*screen, SDL_BLENDMODE_BLEND);

  *sdlTexture = SDL_CreateTexture(*sdlRenderer,
                                          SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          width, height);

  SDL_SetTextureBlendMode(*sdlTexture, SDL_BLENDMODE_BLEND);

  if (*screen == NULL || *sdlTexture == NULL)
  {
    fprintf(stderr, "MAn we have SDL init problems :\\. I couldn't allocate a screen or Texture : %s\n", SDL_GetError());
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
