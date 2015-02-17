#include "app.h"
#include "report.h"
#include "DEBUGLOG.h"
#include "Audio_Context.h"
App::App(int &argc, char **argv, int samplerate/*=44100*/) : 
app_settings(new App_Settings(&file_system)),
debugger(argc,argv)
{
  App_Settings_Context::app_settings = app_settings;
  Player_Context::player = &player;
  audio = new Audio;
  Audio_Context::audio = audio;
  handle_error(player.init(samplerate) );
  file_system.init();
  //App_Settings_Context::app_settings = &app_settings;
  
  midi = new Midi;
  Midi_Context::midi = midi;
  
  File_System_Context::file_system = &file_system;
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

  delete app_settings;
  delete midi;
  delete audio;
}

