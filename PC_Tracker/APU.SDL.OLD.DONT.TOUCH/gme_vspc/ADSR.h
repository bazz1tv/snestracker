#pragma once
#include "SDL.h"
#include "utility.h"
#include "types.h"

// GUI stuffz
#include "utility.h"
#include "gui/Expanding_List.h"



// ADSR Stuffz
struct ADSR
{
  static const char * get_attack_str(Uint8 adsr1);
  static int get_attack_index(Uint8 adsr1);
  static Uint8 reverse_attack_index(Uint8 index);
  static const char * get_decay_str(Uint8 adsr1);   
  static int get_decay_index(Uint8 adsr1);
  static Uint8 reverse_decay_index(Uint8 index);
  static const char * get_sustain_level_str(Uint8 adsr2);   
  static int get_sustain_level_index(Uint8 adsr2); 
  static Uint8 reverse_sustain_level_index(Uint8 index);
  static const char * get_sustain_release_str(Uint8 adsr2);  
  static int get_sustain_release_index(Uint8 adsr2);
  static Uint8 reverse_sustain_release_index(Uint8 index);

  static const int ATTACK_MASK = 0x0f;
  static const int DECAY_MASK = 0x70;
  static const int SUSTAIN_LEVEL_MASK = 0xe0;
  static const int SUSTAIN_RELEASE_MASK = 0x1f;

  static const int ATTACK_MAP_SIZE = 16;
  static const int DECAY_MAP_SIZE = 8;
  static const int SUSTAIN_LEVEL_MAP_SIZE = 8;
  static const int SUSTAIN_RELEASE_MAP_SIZE = 0x20;

  static const IntToCharStar attack_map[ATTACK_MAP_SIZE];
  static const IntToCharStar decay_map[DECAY_MAP_SIZE];
  static const IntToCharStar sustain_level_map[SUSTAIN_LEVEL_MAP_SIZE];
  static const IntToCharStar sustain_release_map[SUSTAIN_RELEASE_MAP_SIZE];

  struct Attack_Context
  {
    // Take parent class here
    Attack_Context() : menu(menu_items)
    {
      
    }

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
  };

  struct Decay_Context
  {
    // Take parent class here
    Decay_Context() : menu(menu_items)
    {
      
    }

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

  };

  struct Sustain_Level_Context
  {
    // Take parent class here
    Sustain_Level_Context() : menu(menu_items)
    {
      
    }

    Expanding_List menu;
    Context_Menu_Item menu_items[ADSR::SUSTAIN_LEVEL_MAP_SIZE+1] = 
    {
      {ADSR::sustain_level_map[0].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[1].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[2].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[3].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[4].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[5].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[6].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[7].str, true,  NULL,  NULL},
      {"",            false, NULL,  NULL}
    };

  };

  struct Sustain_Release_Context
  {
    // Take parent class here
    Sustain_Release_Context() : menu(menu_items)
    {
      
    }

    Expanding_List menu;
    Context_Menu_Item menu_items[ADSR::SUSTAIN_RELEASE_MAP_SIZE+1] = 
    {
      {ADSR::sustain_release_map[0].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[1].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[2].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[3].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[4].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[5].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[6].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[7].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[8].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[9].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[10].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[11].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[12].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[13].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[14].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[15].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[16].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[17].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[18].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[19].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[20].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[21].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[22].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[23].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[24].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[25].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[26].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[27].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[28].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[29].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[30].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[31].str, true,  NULL,  NULL},
      {"",            false, NULL,  NULL}
    };

  };

  struct Context_Menus
  {
    enum
    {
      EVENT_INACTIVE=0,
      EVENT_ACTIVE=1,
      ATTACK_CHANGED,
      DECAY_CHANGED,
      SUSTAIN_LEVEL_CHANGED,
      SUSTAIN_RELEASE_CHANGED
    };
    bool check_left_click_activate(int &x, int &y);
    int receive_event(SDL_Event &ev);
    void draw(SDL_Surface *screen);
    void update(Uint8 adsr, Uint8 adsr2);
    Attack_Context          attack_context;
    Decay_Context           decay_context;
    Sustain_Level_Context   sustain_level_context;
    Sustain_Release_Context sustain_release_context;
  };
};