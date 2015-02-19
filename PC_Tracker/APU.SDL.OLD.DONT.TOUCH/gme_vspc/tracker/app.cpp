#include "app.h"
#include "report.h"
#include "DEBUGLOG.h"
#include "Audio_Context.h"
App::App(int &argc, char **argv, int samplerate/*=44100*/) : 
//debugger(argc,argv),
app_settings(&file_system)
{
  Player_Context::player = &player;
  Audio_Context::audio = &audio;
  handle_error(player.init(samplerate) );
  file_system.init();
  //App_Settings_Context::app_settings = &app_settings;
  Midi_Context::midi = &midi;
  App_Settings_Context::app_settings = &app_settings;
  File_System_Context::file_system = &file_system;
  
}

void App::run()
{

  // For now..
  //debugger.run();
  tracker.run();
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

