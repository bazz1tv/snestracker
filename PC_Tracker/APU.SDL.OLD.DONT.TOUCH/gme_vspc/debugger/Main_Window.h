
#pragma once
#include "SDL.h"
#include "globals.h"

#include "report.h"
#include "Voice_Control.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "Port_Tool.h"
#include "mode.h"
#include "Mouse_Hexdump_Area.h"
#include "Main_Memory_Area.h"
#include "Colors.h"
#include "platform.h"
#include "Render_Context.h"
#include "Player_Context.h"

struct Main_Window : public Debugger_Base, public Render_Context, public Player_Context
{
  Main_Window(int &argc, char **argv);
  void run();
  void receive_event(SDL_Event &ev);
  void draw();

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
  void write_mask(unsigned char packed_mask[32]);
  void applyBlockMask(char *filename);
  void fade_arrays();
  void pack_mask(unsigned char packed_mask[32]);



  void toggle_pause();
  void restart_track();
  void prev_track();
  void next_track();
  void exit_edit_mode();
  void inc_ram(int addr, int i);
  void dec_ram(int addr, int i);
  void reload();

  // external deps

  // How to package this as an entity?
  
  Main_Memory_Area main_memory_area;
  Mouse_Hexdump_Area mouseover_hexdump_area;
  Port_Tool port_tool;
  Voice_Control voice_control;
  
  //Uint16 address, addr_being_edited; // shared variable between main_memory_area and mouseover_hexdump_area

  // TRACK STUFFZZ  
  char now_playing[1024];
  int song_time;
  track_info_t tag;
  void update_window_title();
  void start_track( int track, const char* path );
  void update_track_tag();




  

  // trim later. Get working now
  

 
  int g_cur_entry = 0;
  char *g_real_filename=NULL; // holds the filename minus path

  int tmp=0, i=0;
  SDL_Rect tmprect;
  unsigned char packed_mask[32];
  uint16_t mouse_addr=0; 

  bool paused;
  //int last_pc;
  Uint32 time_last=0, time_cur=0;
  
  bool is_first_run=true;
  const char* path;
  char tmpbuf[500];

  char *marquees[3] = { (char*)CREDITS, now_playing, NULL };
  char *cur_marquee = NULL;
  int cur_marquee_id = 0;

};

