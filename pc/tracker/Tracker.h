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

  void renderCurrentInstrument();
  void render_to_apu(bool repeat_pattern = false, bool startFromPlayhead = false);
  int calcTicks();

	void reset();
	int read_from_file(SDL_RWops *file);
	void save_to_file(SDL_RWops *file);
  int DialogUnsavedChanges();

  void updateWindowTitle(const char *str);

	/* Tracker APU RAM mapping */
	TrackerApuRam *apuram;
  // Vars calculated at APU render time that are useful elsewhere
  struct {
    uint8_t highest_instr, highest_sample;
    uint16_t highest_pattern;
  } apuRender;
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
  bool instr_render = false;
  bool rendering();

  FPS frame;  // framerate control


private:
	SpcReport spcreport;

  /* The following variables are exclusively for the Alt-Return "Return playhead
   * to original position functionality" */
  int psp_currow_stash, pep_currow_stash;
  int psp_rows_scrolled_stash, pep_rows_scrolled_stash;
  bool alt_return_was_held;

  /////
  char windowStr[PATH_MAX];
};

uint8_t calcESAfromEDL(uint8_t edl);
