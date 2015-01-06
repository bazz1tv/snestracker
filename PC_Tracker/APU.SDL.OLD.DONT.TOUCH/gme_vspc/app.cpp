#include "app.h"
#include "report.h"
App::App(int &argc, char **argv, int samplerate/*=44100*/) : 
debugger(argc,argv)
{
  for (int i=0; i < report::BRR_HEADER_MAX; i++)
    report::BRR_Headers[i] = 0xffff;  // init to ROM address, brr would never be there
  for (int i=0; i < report::SRCN_MAX; i++)
    report::SRCN_used[i] = 0xffff;  // init to ROM address, brr would never be there
  //SDL_SetColorKey(screen, SDL_TRUE, 0);
  Player_Context::player = &player;
  handle_error(player.init(samplerate) );
}

void App::run()
{
  // For now..
  debugger.run();
}

App::~App()
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

