#include "app.h"
#include "DEBUGLOG.h"
#include "gme/player/Music_Player.h"
#include "Tracker.h"
#include "Midi.h"
#include "File_System.h"
#include "App_Settings.h"
#include "Audio.h"

static int create_midi(void *vapp)
{
  App *app = (App *)vapp;
  app->midi = new Midi;

  SDL_SemPost(app->sem);
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

  sem = SDL_CreateSemaphore(0);
  midithread = SDL_CreateThread(create_midi, "create_midi", this);
  SDL_DetachThread(midithread);
}

void App::run()
{
  tracker->run();

  // We're finished (quitting), now we may have to wait for the slow Midi thread
  // to finish up. (slow on Mac sometimes)
  int alpha = 255; // set this to 0 and uncomment below lines to do a fadeout
  bool thread_finished = false;

  DEBUGLOG("Waiting on Midi Thread (it could take several seconds)");
  while (!thread_finished || alpha != 255)
  {
    if (!thread_finished)
      thread_finished = !(SDL_SemWaitTimeout(sem, 15) == SDL_MUTEX_TIMEDOUT);
    /*SDL_SetRenderDrawBlendMode(::render->sdlRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0);
    SDL_RenderClear(::render->sdlRenderer);
    SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);

    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, alpha);
    SDL_RenderFillRect(::render->sdlRenderer, NULL);

    SDL_RenderPresent(::render->sdlRenderer);*/
    // Fade out the screen
    DEBUGLOG(".");
    alpha += 4;
    if (alpha >= 255)
      alpha = 255;

    if (thread_finished)
      SDL_Delay(15);
  }
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

