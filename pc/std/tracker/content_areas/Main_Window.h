#pragma once
#include "SDL.h"
#include "globals.h"

#include <math.h>
#include "sdl_dblclick.h"
#include "Colors.h"
#include "platform.h"
#include "Render.h"
#include "gme/player/Music_Player.h"
#include "Experience.h"
#include "gui/Context_Menu.h"
#include "gui/Slider/Slider.h"
#include "gui/Tab.h"
#include "globals.h"
#include "shared/gui/Text_Edit_Rect.h"
#include "Instruments.h"

struct Tracker;

struct Main_Window : public Experience
{
  char tmpbuf[100];

  Text song_title_label;
  Text_Edit_Rect song_title;
  char song_title_str[22];

  struct Gain
  {
    ~Gain() { if (slider) delete slider; }
    Slider<double> *slider=NULL;
    static int change(void *dblnewgain);
  } gain;
  
  enum modes 
  { 
    MODE_NAV=0,
    MODE_EDIT_MOUSE_HEXDUMP,
    MODE_EDIT_APU_PORT,
    MODE_EDIT_TIME
  };

  Main_Window(int &argc, char **argv, Tracker *tracker);

  void run();
  void check_quit(SDL_Event &ev);

  int receive_event(SDL_Event &ev);
  void draw();

  void one_time_draw();

  void draw_memory_outline();
  void draw_pattern_table_outline();
  void draw_pattern();
  
  void draw_voices_pitchs();
  void draw_voices_volumes();
  void draw_main_volume();
  void draw_echo_volume();
    
  // external deps

  // How to package this as an entity?
  uint16_t dir;
  void draw_track_tag();

  int mode=MODE_NAV;

  bool is_onetime_draw_necessary=true;

  int tmp=0, i=0;
  SDL_Rect tmprect;

  Uint32 time_last=0, time_cur=0;
  
  bool is_first_run=true;

  /* Handle to the tracker to access core components (instrument and
   * pattern data */
  Tracker *tracker;
  Instrument_Panel instrpanel;
  Sample_Panel samplepanel;
};
