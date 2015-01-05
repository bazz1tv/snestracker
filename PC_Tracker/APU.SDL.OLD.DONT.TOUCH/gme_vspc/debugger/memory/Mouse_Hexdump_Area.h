#pragma once

#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "sdl_userevents.h"
#include "gui/cursor.h"
#include "report.h"
#include "gui/cursor.h"
#include "memory.h"
#include "Screen.h"







//extern int MOUSE_HEXDUMP_START_Y;
//extern Uint16 mouse_hexdump::address;
//extern struct SIAPU IAPU;


struct Mouse_Hexdump_Area : Memory
{
  //friend class Main_Memory_Area;
  //Mouse_Hexdump_Area(Music_Player *player, SDL_Surface *screen/*, Uint16 &address, Uint16 &address_being_edited*/);
enum submodes { HARD_EDIT=0, EASY_EDIT=1 };
  void update_editing_address();
  void draw_cursor(SDL_Surface *screen, Uint32 color);
  void inc_cursor_row();
  void dec_cursor_row();
  void inc_cursor_pos();
  void dec_cursor_pos();
  void add_addr(int i);

  


  //deps
  /*Music_Player *player;
  SDL_Surface *screen;
  uint8_t *IAPURAM;*/
  //unique
  
  int tmp_ram=0; char draw_tmp_ram=0;
  int submode=0;
  
  Uint8 highnibble=1;
  Uint8 horizontal=1;
  int old_addr=0;
static  int MOUSE_HEXDUMP_START_Y;
  //Uint16 &address, &addr_being_edited;
}; 


