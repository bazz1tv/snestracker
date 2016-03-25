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

App::App(int &argc, char **argv, int samplerate/*=44100*/)
//main_window(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME_VERSION)
{
  //Colors::precompute(*main_window->screen);
  //Render_Context::load_from_window(main_window);
  file_system = new File_System;
  app_settings=new App_Settings(file_system);
  audio=new Audio;
  player=new Music_Player;
  Player_Context::player = player;

  handle_error(player->init(samplerate, app_settings->vars.audio_out_dev) );
  
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
  DEBUGLOG("~App;");
  delete debugger;

  delete player;

  delete app_settings;
  delete midi;
  delete audio;

  delete file_system;

  if (screen)
    SDL_FreeSurface(screen);
  if (sdlTexture)
    SDL_DestroyTexture(sdlTexture);
  if (sdlRenderer)
    SDL_DestroyRenderer(sdlRenderer);
  if(sdlWindow)
    SDL_DestroyWindow(sdlWindow);
}

