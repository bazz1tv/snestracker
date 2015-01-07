#pragma once
#include "Render_Context.h"
#include "Player_Context.h"
#include "gme/player/Music_Player.h"
#include "globals.h"
//#include "Main_Window.h"
//#include "Dsp_Window.h"
#include "Experience.h"
#include "Voice_Control.h"

struct Main_Window;
struct Dsp_Window;
struct BaseD : public Render_Context, public Player_Context
{
  enum GrandMode
  {
    MAIN=0,
    DSP_MAP
  };
  void pack_mask(unsigned char packed_mask[32]);
  void applyBlockMask(char *filename);
  void write_mask(unsigned char packed_mask[32]);
  void reload();
  void start_track( int track, const char* path );
  void switch_mode(int mode);
  void draw_menu_bar();
  void toggle_pause();
  void restart_track(); // goes to beginning of playlist too
  void prev_track();
  void next_track();
  void restart_current_track();
  void update_track_tag();

  static int g_cur_entry;// = 0;
  static bool paused;
  static const char* path;

  static int song_time;
  static track_info_t tag;
  static char *g_real_filename;//=NULL;


  //static bool new_track_started;
  static Voice_Control voice_control;
  static unsigned char packed_mask[32];
  static char now_playing[1024];
  
  static int grand_mode;// =0;
  //static int submode;// =  0;
  
  // trim later. Get working now
  static struct Cfg {
    unsigned char filler = 0x00;
    int apply_block = 0;
    int statusline = 0;
    int nice = 0;
    int extratime = 0;
    int ignoretagtime = 0;
    int defaultsongtime = DEFAULT_SONGTIME;
    int autowritemask = 0;
    int nosound = 0;
    int novideo = 0;
    int update_in_callback = 0;
    int num_files = 0;
    char **playlist = NULL;
  } g_cfg;

  static Experience *exp;
  static Main_Window *main_window;
  static Dsp_Window *dsp_window;

  static int g_paused;// = 0;
  static bool quitting;
  // CAREFUL WITH THIS ONE:
  static uint8_t *IAPURAM;
  // IT CAN ONLY BE VALID AFTER THE MUSIC_PLAYER STARTS A TRACK!!

  // IT IS CURRENT INIT'D IN MAINWINDOW`
};