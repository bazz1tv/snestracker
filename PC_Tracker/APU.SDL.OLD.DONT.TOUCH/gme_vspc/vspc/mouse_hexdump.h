#pragma once

#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "sdl_userevents.h"
#include "gui/cursor.h"
#include "mode.h"
#include "report.h"
#include "gui/cursor.h"



#define MOUSE_HEXDUMP_START_X 584
//#define MOUSE_HEXDUMP_START_Y 229 + 30
#define MOUSE_HEXDUMP_END_X 740
#define MOUSE_HEXDUMP_END_Y 364 + 9 + 30
#define MOUSE_HEXDUMP_ENTRY_X_INCREMENT 20 // but i think i like 16 programmatically
#define MOUSE_HEXDUMP_ENTRY_Y_INCREMENT 9  // dunno what i like programmatically yet







//extern int MOUSE_HEXDUMP_START_Y;
//extern Uint16 mouse_hexdump::address;
//extern struct SIAPU IAPU;
class MainMemoryArea
{
  Memory(SDL_Surface *screen, Music_Player *player, uint8_t* IAPURAM);

  void set_addr(int i);
  void set_addr_from_cursor(int x, int y);
  void add_addr(int i);

  void dec_cursor_row();
  void inc_cursor_row();

  void lock(char l=1, int x=0, int y=0, uint8_t rx=0, uint8_t ry=0);
  void toggle_lock(int x=0, int y=0);
  
  void unlock();

  void inc_cursor_row();
  void dec_cursor_row();
  void inc_cursor_pos();
  void dec_cursor_pos();
  

  Music_Player *player;
  SDL_Surface *screen;
  uint8_t *IAPURAM;
  

  
  

  char locked=0;
  Uint16 address=0x0000, addr_being_edited=0x0000;
  Uint8 address_remainder=0;
};

struct MouseOverHexDumpArea
{
  MouseOverHexdumpArea(Main_Memory_Area *main_mem_area, Music_Player *player, SDL_Surface *screen, uint8_t *IAPURAM);

  void update_editing_address();
  void draw_cursor(SDL_Surface *screen, Uint32 color);


  enum submodes { HARD_EDIT=0, EASY_EDIT=1 };


  //deps
  Music_Player *player;
  SDL_Surface *screen;
  uint8_t *IAPURAM;
  //unique
  Cursor cursor;
  int tmp_ram; char draw_tmp_ram;
  int rel_x;
  int rel_y;

  int res_x;
  int res_y;
  Uint8 highnibble;
  Uint8 horizontal=1;
  int MOUSE_HEXDUMP_START_Y;
}; 


