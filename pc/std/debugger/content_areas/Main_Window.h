
#pragma once
#include "SDL.h"
#include "globals.h"

#include "report.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "Port_Tool.h"
#include "Mouse_Hexdump_Area.h"
#include "Main_Memory_Area.h"
#include "Colors.h"
#include "platform.h"
#include "Render.h"
#include "gme/player/Music_Player.h"
#include "Experience.h"
#include "gui/Context_Menu.h"
#include "gui/Slider/Slider.h"
#include "gui/Tab.h"
#include "shared/Scroll_Tag.h"

struct Main_Window : public BaseD, public Experience
{
  void scroll_track_tags();

  struct
  {
    Scroll_Tag game, song, author, dumper, comment;
    
    void scroll_draw()
    {
      song.scroll_draw();
      author.scroll_draw();
      dumper.scroll_draw();
      comment.scroll_draw(); 
    }
  } scroll_tags;


  int echo_on_x=0, echo_on_y=0;

  void maybe_write_to_mem(bool force=false); // maybe write value to memory
  struct Gain
  {
    ~Gain() { if (slider) delete slider; }
    Slider<double> *slider=NULL;
    static int change(void *dblnewgain);
  } gain;

  struct Tempo
  {
    ~Tempo() { if (slider) delete slider; }
    Slider<double> *slider=NULL;
    static int change(void *dblnewtempo);
  } tempo;

  enum modes 
  { 
    MODE_NAV=0,
    MODE_EDIT_MOUSE_HEXDUMP,
    MODE_EDIT_APU_PORT,
    MODE_EDIT_TIME
  };

  Main_Window(int &argc, char **argv);

  void run();
  void check_quit(SDL_Event &ev);

  int receive_event(SDL_Event &ev);
  void draw();

  void one_time_draw();

  void draw_memory_outline();
  void draw_program_counter();
  void draw_voices_pitchs();
  void draw_voices_volumes();
  void draw_global_volumes();
  void draw_main_volume();
  void draw_echo_volume();
  void draw_mouseover_hexdump();
  void draw_porttool();
  void draw_time_and_echo_status(int *x=NULL, int *y=NULL);
  void draw_mouse_address();

  void draw_block_usage_bar();
  void do_scroller(int elaps_milli);
  void fade_arrays();
  

  void lock(char l=1, int x=0, int y=0, uint8_t rx=0, uint8_t ry=0);
  void toggle_lock(int x=0, int y=0);
  void unlock();
  bool locked() { return main_memory_area.locked; }
  
  void exit_edit_mode();
  void inc_ram(int addr, int i);
  void dec_ram(int addr, int i);
  void reload();

  // external deps

  // How to package this as an entity?
  uint16_t dir;
  Main_Memory_Area main_memory_area;
  Mouse_Hexdump_Area mouseover_hexdump_area;
  Port_Tool port_tool;
  
  void update_window_title();
  void draw_track_tag();

  int mode=MODE_NAV;

  bool is_onetime_draw_necessary=true;

  int tmp=0, i=0;
  SDL_Rect tmprect;

  Uint32 time_last=0, time_cur=0;
  
  bool is_first_run=true;
  
  char *cur_marquee = NULL;
  int cur_marquee_id = 0;
  char *marquees[3] = { (char*)CREDITS, now_playing, NULL };
};

