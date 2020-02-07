#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "Debugger.h"
#include "Midi.h"
#include "File_System.h"
#include "App_Settings.h"
#include "Audio.h"
#include "gui/Cursors.h"

class App : public Render_Context
{
public:
  App(int &argc, char **argv, int samplerate=44100);
  ~App();
  void run();
  
  App_Settings *app_settings;
  Debugger *debugger;
  Music_Player *player;
  
  File_System *file_system;
  Midi *midi;
  Audio *audio;

private:
};
