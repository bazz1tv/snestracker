#pragma once
#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "memory.h" 
#include "gui/cursor.h"

struct Main_Memory_Area : Memory
{
public:
  //Main_Memory_Area(SDL_Surface *screen, Music_Player *player/*, Uint16 &address*/);

  void set_addr(int i);
  void set_addr_from_cursor(int x, int y);
  //void add_addr(int i);

  void lock(char l=1, int x=0, int y=0, uint8_t rx=0, uint8_t ry=0);
  void toggle_lock(int x=0, int y=0);
  
  void unlock();
  //void update_editing_address();
  

  
  Mem_Cursor memcursor;
  

  
  

  char locked=0;
  Uint8 address_remainder=0;
};