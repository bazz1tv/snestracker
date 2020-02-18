
#pragma once
#include "SDL.h"
#include "globals.h"

#include <math.h>
#include "sdl_dblclick.h"
#include "Colors.h"
#include "platform.h"
#include "Main_Window.h"
#include "Experience.h"
#include "Menu_Bar.h"
#include "gui/Window.h"

#include "Instruments.h"

#include "gui/Cursors.h"

typedef Uint32 WindowID;

struct Tracker
{
public:
  Tracker(int &argc, char **argv);
  ~Tracker();
  void run();
  void handle_events();

  Menu_Bar menu_bar;
  Main_Window main_window;

  SDL_DisplayMode monitor_display_mode;

  Experience *sub_window_experience = NULL;
  
  Options_Window options_window;
  Spc_Export_Window spc_export_window;
  static const int NUM_WINDOWS = 2;
  Window *window_map[NUM_WINDOWS+1];

  Cursors *cursors;

  void update_fps(int fps);
  int fps;
  Uint32 frame_tick_timeout, frame_tick_duration;
  ///Uint32 gframe = 0; // should worry about overflow

  // TRACKER CORE
  Instrument instruments[NUM_INSTR]; // also holds the samples
  // Of course Pattern datatype will be going here. Perhaps just like
  // there are Samples as part of Instruments, Patterns will be part of
  // PatternSequencer
};
