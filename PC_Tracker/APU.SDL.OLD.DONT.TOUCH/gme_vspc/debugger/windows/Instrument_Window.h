#pragma once
#include "BaseD.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "sdl_dblclick.h"
#include "Colors.h"
#include "sdlfont.h"
#include "Experience.h"
#include "gui/Clickable_Text.h"
#include "gui/cursor.h"
#include "utility.h"
#include "ADSR.h"
#include "gui/Expanding_List.h"





struct Instrument_Window : public BaseD,
public Experience
{

  Instrument_Window();
  void run();
  void draw();
  void receive_event(SDL_Event &ev);

  void play_pitch(int p, bool abs=false);
  void pause_spc();
  void restore_spc(bool resume=true);

  void set_voice(unsigned char v);
  void inc_voice();
  void dec_voice();
  void inc_octave();
  void dec_octave();

  void keyoff_current_voice();

  struct Start_Stop
  {
    Start_Stop() : startc("Start", 10, 10), stopc("Stop ", 10, 10) {}
    Clickable_Text startc, stopc;
    bool is_started=false;
  } start_stop;
  

  struct AdjustableUD
  {
    AdjustableUD(const char *label) : up_arrow("\x5c"), down_arrow("\x5c"), label(label) {}
    Clickable_Text up_arrow,down_arrow;
    Clickable_Text label;
    uint8_t n=4;
    int n_x,n_y;
  };
  struct AdjustableLR
  {
    AdjustableLR(const char *label) : right_arrow("\x5d"), left_arrow("\x5d"), label(label) {}
    Clickable_Text right_arrow,left_arrow;
    Clickable_Text label;
    uint8_t n=0;
    int n_x,n_y;
  };

  AdjustableUD octave;
  AdjustableLR voice;
  uint16_t pc_backup;
  uint16_t *pc_ptr=NULL;
  int scancode=0; // there's a reason for saving the scancode.. not sure if it's purposed anymore
  int song_time_backup;
  //track_info_t track_info_backup, track_info_backup2; // cause of pass by reference

  struct
  {
    int x,y;

  } adsr, attack, decay;

  struct Attack_Context
  {
    // Take parent class here
    Attack_Context() : menu(menu_items)
    {
      
    }
    // for tcontext menu

    enum {
      
    };

    Expanding_List menu;
    Context_Menu_Item menu_items[ADSR::ATTACK_MAP_SIZE+1] = 
    {
      {ADSR::attack_map[0].str, true,  NULL,  NULL},
      {ADSR::attack_map[1].str, true,  NULL,  NULL},
      {ADSR::attack_map[2].str, true,  NULL,  NULL},
      {ADSR::attack_map[3].str, true,  NULL,  NULL},
      {ADSR::attack_map[4].str, true,  NULL,  NULL},
      {ADSR::attack_map[5].str, true,  NULL,  NULL},
      {ADSR::attack_map[6].str, true,  NULL,  NULL},
      {ADSR::attack_map[7].str, true,  NULL,  NULL},
      {ADSR::attack_map[8].str, true,  NULL,  NULL},
      {ADSR::attack_map[9].str, true,  NULL,  NULL},
      {ADSR::attack_map[10].str, true,  NULL,  NULL},
      {ADSR::attack_map[11].str, true,  NULL,  NULL},
      {ADSR::attack_map[12].str, true,  NULL,  NULL},
      {ADSR::attack_map[13].str, true,  NULL,  NULL},
      {ADSR::attack_map[14].str, true,  NULL,  NULL},
      {ADSR::attack_map[15].str, true,  NULL,  NULL},
      {"",            false, NULL,  NULL}
    };

  } attack_context;

  struct Decay_Context
  {
    // Take parent class here
    Decay_Context() : menu(menu_items)
    {
      
    }
    // for tcontext menu

    enum {
      
    };

    Expanding_List menu;
    Context_Menu_Item menu_items[ADSR::DECAY_MAP_SIZE+1] = 
    {
      {ADSR::decay_map[0].str, true,  NULL,  NULL},
      {ADSR::decay_map[1].str, true,  NULL,  NULL},
      {ADSR::decay_map[2].str, true,  NULL,  NULL},
      {ADSR::decay_map[3].str, true,  NULL,  NULL},
      {ADSR::decay_map[4].str, true,  NULL,  NULL},
      {ADSR::decay_map[5].str, true,  NULL,  NULL},
      {ADSR::decay_map[6].str, true,  NULL,  NULL},
      {ADSR::decay_map[7].str, true,  NULL,  NULL},
      {"",            false, NULL,  NULL}
    };

  } decay_context;

  enum modes 
  { 
    MODE_NAV=0,
  };
  int mode=MODE_NAV;
  enum submodes
  {
    NONE=0,
  };
  int submode=NONE;
  
  bool is_first_run=true;
};

