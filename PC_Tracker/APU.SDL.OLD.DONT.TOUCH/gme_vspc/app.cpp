#include "app.h"

App::App(int &argc, char **argv, int samplerate/*=44100*/) : 
debugger(argc,argv)
{
  Player_Context::player = &player;
  handle_error(player.init(samplerate) );
}

void App::run()
{
  // For now..
  debugger.run();
}

