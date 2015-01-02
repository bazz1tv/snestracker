#pragma once
#include "sdlfont.h"
//#include "../mouse_hexdump.h"

//extern int mouse_hexdump::address;

#define CURSOR_TOGGLE_TIMEWINDOW 300 // ms

namespace memcursor
{
  enum { FLAG_TOGGLED =1,
          FLAG_ACTIVE=(1 << 1),
          FLAG_DISABLED=(1 << 2),
          FLAG_WAS_ACTIVE_BEFORE_DISABLING=(1 << 3)
        };
  extern const int interval;// = 300;
  extern Uint8 flags;//=0; // 1 = show cursor, 0 = hide
  extern SDL_TimerID timerid;//=0;

  
  
  // timer callback, dont worry about this
  Uint32 cursor_timer(Uint32 interval, void *param);

  void start_timer();

  void stop_timer();
  char is_active();
  char is_disabled(); 
  char is_toggled();
  void disable(char c=1);
  void toggle_disable();
  
  
} 


namespace cursor
{
  extern Uint8 toggle;//=0; // 1 = show cursor, 0 = hide
  extern SDL_TimerID timerid;//=0;
  
  // timer callback, dont worry about this
  Uint32 cursor_timer(Uint32 interval, void *param);
 
  void start_timer();
  void stop_timer();

  void draw(SDL_Surface *screen, int x, int y, Uint32 color);

  Uint8 scancode_to_hex(int &scancode);
} 

