
#pragma once
#include "SDL.h"
#include "globals.h"

#include "report.h"
#include "Voice_Control.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "Port_Tool.h"
#include "mode.h"
#include "MouseOver_HexDump_Area.h"
#include "Main_Memory_Area.h"
#include "colors.h"
#include "platform.h"

struct Debugger
{
  Debugger(int &argc, char **argv, Music_Player *player, SDL_Window *, SDL_Renderer *, SDL_Texture *, SDL_Surface*);
  void run();

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

  // How to package this as an entity?
  Voice_Control voice_control;
  Port_Tool port_tool;
  Main_Memory_Area main_memory_area;
  MouseOver_HexDump_Area mouseover_hexdump_area;
  //Uint16 address, addr_being_edited; // shared variable between main_memory_area and mouseover_hexdump_area

  // TRACK STUFFZZ  
  char now_playing[1024];
  int song_time;
  track_info_t tag;
  void update_window_title();
  void start_track( int track, const char* path );
  void update_track_tag();


// external deps
  Music_Player *player=NULL;
  SDL_Window *sdlWindow=NULL;
  SDL_Renderer *sdlRenderer=NULL;
  SDL_Texture *sdlTexture=NULL;
  SDL_Surface *screen;

  

  // trim later. Get working now
  struct {
    unsigned char g_cfg_filler = 0x00;
    int g_cfg_apply_block = 0;
    int g_cfg_statusline = 0;
    int nice = 0;
    int extratime = 0;
    int ignoretagtime = 0;
    int defaultsongtime = DEFAULT_SONGTIME;
    int autowritemask = 0;
    int nosound = 0;
    int novideo = 0;
    int update_in_callback = 0;
    int num_files = 0;
    char **g_cfg_playlist = NULL;
  } g_cfg;

  int g_paused = 0;
  int g_cur_entry = 0;
  char *g_real_filename=NULL; // holds the filename minus path

  int tmp=0, i=0;
  SDL_Rect tmprect;
  unsigned char packed_mask[32];
  uint16_t mouse_addr=0; 

  uint8_t *IAPURAM;
  bool paused;
  int last_pc;
  Uint32 time_last=0, time_cur=0;
  int mode =0;
  int submode =  0;
  bool is_first_run=true;
  const char* path;
  char tmpbuf[500];

  char *marquees[3] = { (char*)CREDITS, now_playing, NULL };
  char *cur_marquee = NULL;
  int cur_marquee_id = 0;

};

namespace screen_pos
{
  //typedef SDL_Rect coord;
  #define NO_INIT 0
  extern SDL_Rect voice0vol;   // = {NO_INIT, NO_INIT,+8+125,10};
  extern SDL_Rect voice0pitch; //= {NO_INIT,NO_INIT, 2*8,8 };
  extern SDL_Rect locked;      // = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  extern SDL_Rect echoE;
  #undef UN_INIT
}