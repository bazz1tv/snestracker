#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "Debugger.h"
#include "Midi_Context.h"
#include "File_System.h"
#include "File_System_Context.h"
#include "App_Settings_Context.h"
#include "Audio.h"

class App : public Render_Context, public Player_Context, public Midi_Context, public App_Settings_Context
{
public:
  App(int &argc, char **argv, int samplerate=44100);
  ~App();
  void run();
  
private:
  //int parse_args(int argc, char **argv);
  Tracker tracker;
  //Debugger debugger;
  Music_Player player;
  App_Settings app_settings;
  File_System file_system;
  Midi midi;
  Audio audio;

private:
};