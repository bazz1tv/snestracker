#include "app.h"
#include "report.h"
#include "DEBUGLOG.h"
#include "Audio_Context.h"

static int create_midi(void *vapp)
{
  App *app = (App *)vapp;
  app->midi = new Midi;
  return 0;
}

App::App(int &argc, char **argv, int samplerate/*=44100*/) : 
app_settings(new App_Settings(&file_system)),
audio(new Audio)
{
  
  Player_Context::player = &player;

  handle_error(player.init(samplerate) );
  
  //App_Settings_Context::app_settings = &app_settings;

  debugger = new Debugger(argc, argv);

  SDL_Thread *thread = SDL_CreateThread(create_midi, "create_midi", this);
  SDL_DetachThread(thread);

}

void App::run()
{

  // For now..
  debugger->run();
}

App::~App()
{
  delete debugger;

  if (screen)
    SDL_FreeSurface(screen);
  if (sdlTexture)
    SDL_DestroyTexture(sdlTexture);
  if (sdlRenderer)
    SDL_DestroyRenderer(sdlRenderer);
  if(sdlWindow)
    SDL_DestroyWindow(sdlWindow);

  delete app_settings;
  delete midi;
  delete audio;
}

