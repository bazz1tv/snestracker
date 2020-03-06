
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

#include "gui/MouseCursors.h"

#include "shared/DrawRenderer.h"
#include <unordered_set>

typedef Uint32 WindowID;
struct TrackerApuRam;

struct Tracker
{
public:
  Tracker(int &argc, char **argv);
  ~Tracker();
  void run();
  void handle_events();

  /* This is stuff that will go into a Song struct, but for now just put
   * it here */
  unsigned int bpm :9;
  unsigned int spd :5;
  void inc_bpm();
  void dec_bpm();
  void inc_spd();
  void dec_spd();
  void render_to_apu();

  Menu_Bar menu_bar;
  Main_Window main_window;

  SDL_DisplayMode monitor_display_mode;

  Experience *sub_window_experience = NULL;
  
  Options_Window options_window;
  Spc_Export_Window spc_export_window;
  static const int NUM_WINDOWS = 2;
  Window *window_map[NUM_WINDOWS+1];

  MouseCursors *mousecursors;

  void update_fps(int fps);
  int fps;
  Uint32 frame_tick_timeout, frame_tick_duration;
  ///Uint32 gframe = 0; // should worry about overflow

  static std::unordered_set<DrawRenderer *> prerenders, postrenders;

	//
	bool playback = false; // is tracker playback happening?

	/* Tracker APU RAM mapping */
	TrackerApuRam *apuram;
  /* TRACKER CORE -- Here is located the "model" data structures, not GUI
   * */
  Instrument instruments[NUM_INSTR]; // also holds the samples
  PatternSequencer patseq;
};
