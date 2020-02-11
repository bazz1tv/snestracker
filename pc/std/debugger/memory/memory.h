#pragma once

#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "BaseD.h"
#include "gui/Cursor.h"
#include "Render.h"
#include "gme/player/Music_Player.h"

#define IS_SPECIAL_ADDRESSES(addr) ( (addr == 0xf3 && (IAPURAM[0xf2] == 0x4c || IAPURAM[0xf2] == 0x5c) ) ||\
addr==0xf1 || addr == 0xf0 || (addr >= 0xf4 && addr <= 0xf7) || (addr >= 0xfa && addr <= 0xfc)  )

struct Memory : public BaseD
{
  Memory();

  void set_addr(int i);
  void add_addr(int i);

  // All of the static variables are stuff I want shared between the 
  // Main Memory Area and Mouse Hexdump area
  static Cursor cursor;

  static Uint16 address; //=0x0000;
  static Uint16 addr_being_edited;
  static int rel_x;
  static int rel_y;

  static int res_x;
  static int res_y;
};
