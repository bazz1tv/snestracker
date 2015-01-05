#pragma once
#include "Render_Context.h"
#include "Player_Context.h"
#include "gme/player/Music_Player.h"
#include "globals.h"
//#include "Main_Window.h"
//#include "Dsp_Window.h"
#include "Experience.h"

struct Main_Window;
struct Dsp_Window;
struct Debugger_Base
{
  enum GrandMode
  {
    MAIN=0,
    DSP_MAP
  };
  void change_grand_mode(int mode);
  
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