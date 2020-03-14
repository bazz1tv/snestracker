#include "app.h"
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
  app_settings=new App_Settings(file_system);
  audio=new Audio;
  player=new Music_Player;
  ::player = player;

  handle_error(player->init(samplerate, app_settings->vars.audio_out_dev) );
  
  tracker = new Tracker(argc, argv);
	::tracker = tracker;

  SDL_Thread *thread = SDL_CreateThread(create_midi, "create_midi", this);
  SDL_DetachThread(thread);
}

void App::run()
{
  tracker->run();
}

App::~App()
{
  DEBUGLOG("~App;");
  delete tracker;

  delete player;

  delete app_settings;
  delete midi;
  delete audio;

  delete file_system;
}

