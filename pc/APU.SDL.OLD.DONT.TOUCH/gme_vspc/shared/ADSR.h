#pragma once
#include "SDL.h"
#include "utility.h"
#include "types.h"

// GUI stuffz
#include "gui/Expanding_List.h"



// ADSR Stuffz
struct ADSR
{
  static const char * get_attack_str(Uint8 adsr1);
  static int get_attack_index(Uint8 adsr1);
  static Uint8 reverse_attack_index(Uint8 index);
  //static const char * get_decay_str(Uint8 adsr1);   
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
  static const IntToCharStar decay_map[SUSTAIN_LEVEL_MAP_SIZE][DECAY_MAP_SIZE];
  static const IntToCharStar sustain_level_map[SUSTAIN_LEVEL_MAP_SIZE];
  static const IntToCharStar sustain_release_map[SUSTAIN_LEVEL_MAP_SIZE][SUSTAIN_RELEASE_MAP_SIZE];

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

  struct Sustain_Level_Context
  {
    // Take parent class here
    Sustain_Level_Context() : menu(menu_items)
    {
      
    }

    

    Expanding_List menu;
    Context_Menu_Item menu_items[ADSR::SUSTAIN_LEVEL_MAP_SIZE+1] = 
    {
      {ADSR::sustain_level_map[7].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[6].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[5].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[4].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[3].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[2].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[1].str, true,  NULL,  NULL},
      {ADSR::sustain_level_map[0].str, true,  NULL,  NULL},
      {"",            false, NULL,  NULL}
    };

  };

  struct Decay_Context
  {
    Decay_Context(Sustain_Level_Context *sustain_level_context) :  
      sustain_level_context(sustain_level_context),
      menu(menu_items)
    {

    }

    Sustain_Level_Context *sustain_level_context;
    Uint8 currently_used_sustain_level_index=0;
    

    void update()
    {
      // defined in external "collector class"
      Uint8 sustain_level_index = 7-sustain_level_context->menu.currently_selected_item_index;
      // index into big array
      if (sustain_level_index == currently_used_sustain_level_index)
        return;

      for (int i=0; i < ADSR::DECAY_MAP_SIZE; i++)
      {
        //fill up menu_items
        menu_items[i].clickable_text.str = ADSR::decay_map[sustain_level_index][i].str;
      }
      currently_used_sustain_level_index = sustain_level_index;
    }

    Expanding_List menu;
    Context_Menu_Item menu_items[ADSR::DECAY_MAP_SIZE+1] = 
    {
      {ADSR::decay_map[0][0].str, true,  NULL,  NULL},
      {ADSR::decay_map[0][1].str, true,  NULL,  NULL},
      {ADSR::decay_map[0][2].str, true,  NULL,  NULL},
      {ADSR::decay_map[0][3].str, true,  NULL,  NULL},
      {ADSR::decay_map[0][4].str, true,  NULL,  NULL},
      {ADSR::decay_map[0][5].str, true,  NULL,  NULL},
      {ADSR::decay_map[0][6].str, true,  NULL,  NULL},
      {ADSR::decay_map[0][7].str, true,  NULL,  NULL},
      {"",            false, NULL,  NULL}
    };

  };

  struct Sustain_Release_Context
  {
    // Take parent class here
    Sustain_Release_Context(Sustain_Level_Context *sustain_level_context) : 
      sustain_level_context(sustain_level_context), menu(menu_items)
    {
      
    }

    Sustain_Level_Context *sustain_level_context;
    Uint8 currently_used_sustain_level_index=0;

    void update()
    {
      // defined in external "collector class"
      Uint8 sustain_level_index = 7-sustain_level_context->menu.currently_selected_item_index;
      // index into big array
      if (sustain_level_index == currently_used_sustain_level_index)
        return;

      for (int i=0; i < ADSR::SUSTAIN_RELEASE_MAP_SIZE; i++)
      {
        //fill up menu_items
        menu_items[i].clickable_text.str = ADSR::sustain_release_map[sustain_level_index][i].str;
      }
      currently_used_sustain_level_index = sustain_level_index;
    }

    Expanding_List menu;
    Context_Menu_Item menu_items[ADSR::SUSTAIN_RELEASE_MAP_SIZE+1] = 
    {
      {ADSR::sustain_release_map[0][0].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][1].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][2].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][3].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][4].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][5].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][6].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][7].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][8].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][9].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][10].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][11].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][12].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][13].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][14].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][15].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][16].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][17].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][18].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][19].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][20].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][21].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][22].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][23].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][24].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][25].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][26].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][27].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][28].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][29].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][30].str, true,  NULL,  NULL},
      {ADSR::sustain_release_map[0][31].str, true,  NULL,  NULL},
      {"",            false, NULL,  NULL}
    };

  };

  struct Context_Menus
  {
    Context_Menus() : decay_context(&sustain_level_context),
      sustain_release_context(&sustain_level_context) {}
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
    Sustain_Level_Context   sustain_level_context;
    Decay_Context           decay_context;
    Sustain_Release_Context sustain_release_context;
  };
};