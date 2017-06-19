#pragma once

#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "BaseD.h"
#include "gui/cursor.h"
#include "Render_Context.h"
#include "Player_Context.h"

#define IS_SPECIAL_ADDRESSES(addr) ( (addr == 0xf3 && (IAPURAM[0xf2] == 0x4c || IAPURAM[0xf2] == 0x5c) ) ||\
addr==0xf1 || addr == 0xf0 || (addr >= 0xf4 && addr <= 0xf7) || (addr >= 0xfa && addr <= 0xfc)  )

struct Memory : public BaseD, public Render_Context, public Player_Context
{
  Memory();

  void set_addr(int i);
  void add_addr(int i);

  

  // All of the static variables are stuff I want shared between the 
  // Main Memory Area and Mouse Hexdump area
  //static Mem_Cursor memcursor;
  static Cursor cursor;

  //static Music_Player *player;
  //uint8_t *IAPURAM;
  
  static Uint16 address; //=0x0000;
  static Uint16 addr_being_edited;
  static int rel_x;
  static int rel_y;

  static int res_x;
  static int res_y;
};