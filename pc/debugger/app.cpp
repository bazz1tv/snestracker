#include "app.h"
#include "report.h"
#include "DEBUGLOG.h"
#include "Audio.h"

static int create_midi(void *vapp)
{
  App *app = (App *)vapp;
  app->midi = new Midi;
  return 0;
}

App::App(int &argc, char **argv, int samplerate/*=44100*/)
{
  file_system = new File_System;
  audio=new Audio;
  player=new Music_Player;
  ::player = player;
  app_settings=new App_Settings(file_system);

  handle_error(player->init(samplerate, app_settings->vars.audio_out_dev) );
  
  debugger = new Debugger(argc, argv);

  SDL_Thread *thread = SDL_CreateThread(create_midi, "create_midi", this);
  SDL_DetachThread(thread);
}

void App::run()
{
  debugger->run();
}

App::~App()
{
  DEBUGLOG("~App;");
  delete debugger;

  delete player;

  delete app_settings;
  delete midi;
  delete audio;

  delete file_system;
}

