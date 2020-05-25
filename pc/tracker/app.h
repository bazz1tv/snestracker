#include "SDL.h"

class App_Settings;
struct Tracker;
class Music_Player;
struct File_System;
struct Midi;
struct Audio;


class App
{
public:
  App(int &argc, char **argv, int samplerate=44100);
  ~App();
  void run();
  
  App_Settings *app_settings;
  Tracker *tracker;
  Music_Player *player;
  
  File_System *file_system;
  Midi *midi;
  Audio *audio;

  SDL_sem *sem;
private:
	SDL_Thread *midithread;
};
