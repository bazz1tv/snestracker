#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "Debugger.h"

class App : public Render_Context
{
public:
  App(SDL_Window **sdlWindow, int &argc, char **argv, int w, int h, int samplerate=44100);
  //~App();
  void run();
  int init();
  int init_sdl(SDL_Window **, int,int);
  
private:
  //int parse_args(int argc, char **argv);
  // Tracker tracker;
  Debugger *debugger=NULL;
  Music_Player player;
  

private:
  int init_sdl();
};