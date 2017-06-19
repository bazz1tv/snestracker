#include "Screen.h"



const int SCREEN_WIDTH=800;
const int SCREEN_HEIGHT=700;
//djust for 800x600 -> 1024x768




const int PORTTOOL_X= 540 + SCREEN_X_OFFSET;
const int PORTTOOL_Y= 380 + 40 + SCREEN_Y_OFFSET;
const int PORTTOOL_PORT0_X = PORTTOOL_X + (8*7) + SCREEN_X_OFFSET;
const int PORTTOOL_PORT3_X_RIGHTMOST_LOBYTE = PORTTOOL_X + (8*23) + SCREEN_X_OFFSET;

const int MOUSE_HEXDUMP_START_X= 584 + SCREEN_X_OFFSET;
//const int MOUSE_HEXDUMP_START_Y= 229 + 30
const int MOUSE_HEXDUMP_END_X= 740 + SCREEN_X_OFFSET;
const int MOUSE_HEXDUMP_END_Y= 364 + 9 + 30 + SCREEN_Y_OFFSET;
const int MOUSE_HEXDUMP_ENTRY_X_INCREMENT= 20; // but i think i like 16 programmatically
const int MOUSE_HEXDUMP_ENTRY_Y_INCREMENT= 9 ; // dunno what i like programmatically yet

const int INFO_X=      540 + SCREEN_X_OFFSET;
const int INFO_Y=      420 + 40 + SCREEN_Y_OFFSET;                                            

namespace Screen
{
  // 8 is the char width in pixels
  //typedef SDL_Rect coord;
  #define NO_INIT 0
  SDL_Rect voice0vol = {NO_INIT, NO_INIT,+8+125,10};
  SDL_Rect voice0pitch = {NO_INIT,NO_INIT, 2*8,8 };
  SDL_Rect locked = { MEMORY_VIEW_X+520+24*8, 0 ,static_cast<int>((strlen(LOCKED_STR)*8)+1), 9};
  SDL_Rect echoE = { MEMORY_VIEW_X+520, NO_INIT, 2*8, 10 };
  #undef NO_INIT
}