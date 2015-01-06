#pragma once
#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "memory.h" 
#include "gui/cursor.h"
#include "memory/Mouse_Hexdump_Area.h"
#include "gui/Context_Menu.h"

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
  

  
  Mem_Cursor memcursor;
  Mouse_Hexdump_Area *mouse_hexdump_area;
  char locked=0;
  Uint8 address_remainder=0;

  Context_Menu context_menu;
  Context_Menu_Item context_menu_items[5] = 
  {
    {"RIP BRR",true},
    {"Rip BRR+",true},
    {"Rip Instrument",true},
    {"Play Sample",true},
    {"",false}
  };
};