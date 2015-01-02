#pragma once

#include "SDL.h"
#include "sdl_userevents.h"
#include "gui/cursor.h"
#include "mode.h"
#include "report.h"



#define MOUSE_HEXDUMP_START_X 584
//#define MOUSE_HEXDUMP_START_Y 229 + 30
#define MOUSE_HEXDUMP_END_X 740
#define MOUSE_HEXDUMP_END_Y 364 + 9 + 30
#define MOUSE_HEXDUMP_ENTRY_X_INCREMENT 20 // but i think i like 16 programmatically
#define MOUSE_HEXDUMP_ENTRY_Y_INCREMENT 9  // dunno what i like programmatically yet







extern int MOUSE_HEXDUMP_START_Y;
//extern Uint16 mouse_hexdump::address;
//extern struct SIAPU IAPU;
namespace mouse_hexdump
{
  enum submodes { HARD_EDIT=0, EASY_EDIT=1 };
  extern int tmp_ram; 
  extern char draw_tmp_ram;
  extern int rel_x; // = te->motion.x - MOUSE_HEXDUMP_START_X;
  //rel_x+=2;
  extern int rel_y; // = te->motion.y - MOUSE_HEXDUMP_START_Y;

  extern int res_x;
  extern int res_y;
  extern Uint8 highnibble;
  extern Uint8 horizontal; //=1;

  extern char locked;//=0;
  extern Uint16 address;//=0x0000;

  void set_addr(int i);
  void set_addr_from_cursor(int x, int y);
  void add_addr(int i);

  void dec_cursor_row();
  void inc_cursor_row();

  void lock(char l=1, int x=0, int y=0);
  void toggle_lock(int x=0, int y=0);
  
  void unlock();

  void inc_cursor_row();
  void dec_cursor_row();
  void inc_cursor_pos();
  void dec_cursor_pos();

  void draw_cursor(SDL_Surface *screen, Uint32 color);

}

