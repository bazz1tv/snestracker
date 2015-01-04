#include "app.h"
//#include <stdio.h>








App::App(SDL_Window **sdlWindow, int &argc, char **argv, int w, int h, int samplerate/*=44100*/)
{
  if (init_sdl(sdlWindow,w,h) < 0)
  {
    exit(1);
  }
  handle_error( player.init(samplerate) );
  if ( 
    ((debugger = new Debugger(argc, argv, &player, this->sdlWindow, sdlRenderer, sdlTexture, screen)) == NULL) ) 
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Uh-Oh", "Out of Memory?!?!", this->sdlWindow);
  }  
  

}

void App::run()
{
  // For now..
  debugger->run();
}

int App::init_sdl(SDL_Window **sdlWindow, int width, int height)
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

  // SDL 1.2 CODE
  // video
  /*screen = SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE); //SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (screen == NULL) {
    printf("Failed to set video mode\n");
    return 0;
  }*/

  SDL_CreateWindowAndRenderer(width, height, 0, sdlWindow, &sdlRenderer);
  if (sdlWindow == NULL || sdlRenderer == NULL)
  {
    fprintf(stderr, "FCK\n");
    return -1;
  }
  this->sdlWindow = *sdlWindow;

  screen = SDL_CreateRGBSurface(0, width, height, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
  sdlTexture = SDL_CreateTexture(sdlRenderer,
                                          SDL_PIXELFORMAT_RGB888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          width, height);

  if (screen == NULL || sdlTexture == NULL)
  {
    fprintf(stderr, "MAn we have SDL init problems :\\. I couldn't allocate a screen or Texture :\\\n");
    return -1;
  }

  SDL_SetWindowTitle(this->sdlWindow, PROG_NAME_VERSION_STRING);
  colors::precompute();

  dblclick::init();
  //SDL_EnableKeyRepeat( 500, 80 );

  return 0; 
}






