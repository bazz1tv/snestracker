#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "Tracker.h"
#include "Midi.h"
#include "File_System.h"
#include "App_Settings.h"
#include "Audio.h"
#include "gui/Cursors.h"
#include "Render.h"

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

private:
};
