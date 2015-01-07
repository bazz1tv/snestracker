
#pragma once
#include "SDL.h"
#include "globals.h"

#include "report.h"
#include "Voice_Control.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "Port_Tool.h"
#include "Mouse_Hexdump_Area.h"
#include "Main_Memory_Area.h"
#include "Colors.h"
#include "platform.h"
#include "Render_Context.h"
#include "Player_Context.h"
#include "Experience.h"
#include "gui/Context_Menu.h"




struct Main_Window : public BaseD, public Render_Context, 
public Player_Context,
public Experience
{
  enum modes 
  { 
    MODE_NAV=0,
    MODE_EDIT_MOUSE_HEXDUMP,
    MODE_EDIT_APU_PORT
    //MODE_QUIT  
  };
  Main_Window(int &argc, char **argv);
  void run();
  void receive_event(SDL_Event &ev);
  void draw();

  void one_time_draw();

  void draw_program_counter();
  void draw_voices_pitchs();
  void draw_voices_volumes();
  void draw_global_volumes();
  void draw_main_volume();
  void draw_echo_volume();
  void draw_mouseover_hexdump();
  void draw_porttool();
  void draw_time_and_echo_status();
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
  
  Main_Memory_Area main_memory_area;
  Mouse_Hexdump_Area mouseover_hexdump_area;
  Port_Tool port_tool;
  
  //Uint16 address, addr_being_edited; // shared variable between main_memory_area and mouseover_hexdump_area

  // TRACK STUFFZZ  
  
  
  void update_window_title();
  //void start_track( int track, const char* path );
  void draw_track_tag();

  int mode=0;

  //bool main_memory_context_menu_is_active=false;
  

  // trim later. Get working now
  

 
  bool is_onetime_draw_necessary=true;
   // holds the filename minus path

  int tmp=0, i=0;
  SDL_Rect tmprect;
  
  uint16_t mouse_addr=0; 

  
  //int last_pc;
  Uint32 time_last=0, time_cur=0;
  
  bool is_first_run=true;
  

  
  char *cur_marquee = NULL;
  int cur_marquee_id = 0;
  char *marquees[3] = { (char*)CREDITS, now_playing, NULL };

};

