#include "Screen.h"
namespace Screen
{
  // 8 is the char width in pixels
  //typedef SDL_Rect coord;
  #define NO_INIT 0
  SDL_Rect voice0vol = {NO_INIT, NO_INIT,+8+125,10};
  SDL_Rect voice0pitch = {NO_INIT,NO_INIT, 2*8,8 };
  SDL_Rect locked = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  SDL_Rect echoE = { MEMORY_VIEW_X+520, NO_INIT, 2*8, 10 };
  #undef UN_INIT
}