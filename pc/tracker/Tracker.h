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

#include "Samples.h"
#include "Instruments.h"

#include "gui/MouseCursors.h"

#include "shared/DrawRenderer.h"
#include <unordered_set>

#include "gme_m/Spc_Report.h"

#include "SongSettings.h"

#include "shared/fps.h"

#include "Song.h"

class SpcReport : public Spc_Report
{
public:
	void report(Type type, unsigned addr, unsigned opcode);
};


typedef Uint32 WindowID;
struct TrackerApuRam;

struct Tracker
{
public:
  Tracker(int &argc, char **argv);
  ~Tracker();
  void run();
  void handle_events();

	void inc_patlen();
	void dec_patlen();

  void render_to_apu(bool repeat_pattern = false);
	void reset();
	int read_from_file(SDL_RWops *file);
	void save_to_file(SDL_RWops *file);
  int DialogUnsavedChanges();

	/* Tracker APU RAM mapping */
	TrackerApuRam *apuram;
  /* TRACKER CORE -- Here is located the "model" data structures, not GUI
   * */
	Song song;

/* The Main_Window, particularly, uses Tracker Core components to
 * initialize certain GUI elements, so we need to ensure that the
 * Main_Window declaration comes after the above Tracker Core */

//////////////////////////////////////

  Menu_Bar menu_bar;
  Main_Window main_window;

  SDL_DisplayMode monitor_display_mode;

  Experience *sub_window_experience = NULL;

  Options_Window options_window;
  Spc_Export_Window spc_export_window;

  MouseCursors *mousecursors;

  static std::unordered_set<DrawRenderer *> prerenders, postrenders;

  bool playback = false; // is tracker playback happening?

  FPS frame;  // framerate control


private:
	SpcReport spcreport;
};
