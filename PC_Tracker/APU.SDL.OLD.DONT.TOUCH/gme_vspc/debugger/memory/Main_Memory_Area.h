#pragma once
#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "memory.h" 
#include "gui/cursor.h"
#include "memory/Mouse_Hexdump_Area.h"
#include "gui/Context_Menu.h"

int demo(void *data);
int play_sample(void *data);

struct Main_Memory_Area : Memory
{
public:
  //friend class Mouse_Hexdump_Area;
  Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area);

  void set_addr(int i);
  void set_addr_from_cursor(int x, int y);
  //void add_addr(int i);

  void lock(char l=1, int x=0, int y=0, uint8_t rx=0, uint8_t ry=0);
  void toggle_lock(int x=0, int y=0);
  
  void unlock();
  //void update_editing_address();
  
  //void processContextEvents(SDL_Event &ev);
  
  Mem_Cursor memcursor;
  Mouse_Hexdump_Area *mouse_hexdump_area;
  char locked=0;
  Uint8 address_remainder=0;

  void log_the_fucking_address_for_the_fucking_context_window();

  struct Context
  {
    Context() : menu(menu_items) {}
    // for tcontext menu
    uint16_t addr_when_user_right_clicked=0;
    uint8_t voice_to_play=0;

    Context_Menu menu;
    Context_Menu_Item menu_items[5] = 
    {
      {"RIP BRR",true, &demo, &addr_when_user_right_clicked},
      {"Rip BRR+",true},
      {"Rip Instrument",true},
      {"Play Sample",true, &play_sample},
      {"",false}
    };
  } context;
};