#pragma once

#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "sdl_userevents.h"
#include "gui/cursor.h"
#include "mode.h"
#include "report.h"
#include "gui/cursor.h"
#include "memory.h"

namespace screen_pos
{ 
  extern SDL_Rect voice0vol;   // = {NO_INIT, NO_INIT,+8+125,10};
  extern SDL_Rect voice0pitch; //= {NO_INIT,NO_INIT, 2*8,8 };
  extern SDL_Rect locked;      // = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  extern SDL_Rect echoE;
}


#define MOUSE_HEXDUMP_START_X 584
//#define MOUSE_HEXDUMP_START_Y 229 + 30
#define MOUSE_HEXDUMP_END_X 740
#define MOUSE_HEXDUMP_END_Y 364 + 9 + 30
#define MOUSE_HEXDUMP_ENTRY_X_INCREMENT 20 // but i think i like 16 programmatically
#define MOUSE_HEXDUMP_ENTRY_Y_INCREMENT 9  // dunno what i like programmatically yet







//extern int MOUSE_HEXDUMP_START_Y;
//extern Uint16 mouse_hexdump::address;
//extern struct SIAPU IAPU;


struct Mouse_Hexdump_Area : Memory
{
  //Mouse_Hexdump_Area(Music_Player *player, SDL_Surface *screen/*, Uint16 &address, Uint16 &address_being_edited*/);

  void update_editing_address();
  void draw_cursor(SDL_Surface *screen, Uint32 color);
  void inc_cursor_row();
  void dec_cursor_row();
  void inc_cursor_pos();
  void dec_cursor_pos();


  


  //deps
  /*Music_Player *player;
  SDL_Surface *screen;
  uint8_t *IAPURAM;*/
  //unique
  
  int tmp_ram=0; char draw_tmp_ram=0;
  
  Uint8 highnibble=1;
  Uint8 horizontal=1;
  int old_addr=0;
static  int MOUSE_HEXDUMP_START_Y;
  //Uint16 &address, &addr_being_edited;
}; 


