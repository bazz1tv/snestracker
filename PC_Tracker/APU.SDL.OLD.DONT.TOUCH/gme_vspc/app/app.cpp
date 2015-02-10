#include "app.h"
#include "report.h"
#include "DEBUGLOG.h"
App::App(int &argc, char **argv, int samplerate/*=44100*/) : 
debugger(argc,argv),
app_settings(&file_system)
{
  Player_Context::player = &player;
  handle_error(player.init(samplerate) );
  file_system.init();
  //App_Settings_Context::app_settings = &app_settings;
  Midi_Context::midi = &midi;
  App_Settings_Context::app_settings = &app_settings;
  File_System_Context::file_system = &file_system;
}

void App::run()
{
  //the value ranges from 0 to SDL_GetNumAudioDevices() - 1
  int num_audio_devices = SDL_GetNumAudioDevices(0);
  DEBUGLOG("num audio devices: %d\n", num_audio_devices);
  for (int i = 0; i < num_audio_devices; ++i) {
    printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
}
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

