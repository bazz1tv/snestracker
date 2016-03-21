#pragma once
#include "report.h"

#define SCREEN_X_OFFSET 0
#define SCREEN_Y_OFFSET 50
//ld be inside of Main_Memory_View .. but keeping it here for now while I do other things
#define MEMORY_VIEW_X  16 + SCREEN_X_OFFSET
#define MEMORY_VIEW_Y  40 + SCREEN_Y_OFFSET

extern const int SCREEN_WIDTH;// 800
extern const int SCREEN_HEIGHT;// 600
// to adjust for 800x600 -> 1024x768
/*#define SCREEN_X_OFFSET 90
#define SCREEN_Y_OFFSET 100*/
//extern int SCREEN_X_OFFSET, SCREEN_Y_OFFSET;
//
// Should be inside of Main_Memory_View .. but keeping it here for now while I do other things
//extern const int MEMORY_VIEW_X;//= 16 + SCREEN_X_OFFSET
//extern const int MEMORY_VIEW_Y;//= 40 + SCREEN_Y_OFFSET
//
extern const int PORTTOOL_X;//= 540 + SCREEN_X_OFFSET
extern const int PORTTOOL_Y;//= 380 + 40 + SCREEN_Y_OFFSET

extern const int PORTTOOL_PORT0_X;// PORTTOOL_X//= + (8*7) + SCREEN_X_OFFSET
extern const int PORTTOOL_PORT3_X_RIGHTMOST_LOBYTE;// PORTTOOL_X;//= + (8*23) + SCREEN_X_OFFSET
//
extern const int MOUSE_HEXDUMP_START_X;//= 584 + SCREEN_X_OFFSET
//extern const int MOUSE_HEXDUMP_START_Y 229 + 30
extern const int MOUSE_HEXDUMP_END_X;//= 740 + SCREEN_X_OFFSET
extern const int MOUSE_HEXDUMP_END_Y;//= 364 + 9 + 30 + SCREEN_Y_OFFSET
extern const int MOUSE_HEXDUMP_ENTRY_X_INCREMENT;//= 20 // but i think i like 16 programmatically
extern const int MOUSE_HEXDUMP_ENTRY_Y_INCREMENT;//= 9  // dunno what i like programmatically yet
//
extern const int INFO_X;//=      540 + SCREEN_X_OFFSET
extern const int INFO_Y;//=      420 + 40 + SCREEN_Y_OFFSET                                             
//


#define LOCKED_STR "locked"

namespace Screen
{
  //typedef SDL_Rect coord;
  #define NO_INIT 0
  extern SDL_Rect voice0vol;   // = {NO_INIT, NO_INIT,+8+125,10};
  extern SDL_Rect voice0pitch; // = {NO_INIT,NO_INIT, 2*8,8 };
  extern SDL_Rect locked;      // = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  extern SDL_Rect echoE;
  #undef UN_INIT
}