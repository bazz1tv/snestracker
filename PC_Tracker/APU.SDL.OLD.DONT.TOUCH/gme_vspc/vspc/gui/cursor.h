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
  const int interval = 300;
  Uint8 flags=0; // 1 = show cursor, 0 = hide
  SDL_TimerID timerid=0;

  
  
  // timer callback, dont worry about this
  Uint32 cursor_timer(Uint32 interval, void *param)
  {
    Uint8 *p = (Uint8 *) param;
    *p ^= FLAG_TOGGLED;
    return interval;
  }

  void start_timer()
  {
    if (flags & FLAG_DISABLED)
      return;
    //fprintf(stderr, "%d %d", flags_flags, flags_ACTIVE);
    // always start with the cursor showing
    
    // is there a problem Removing an invalid timerid?? I think not..
    SDL_RemoveTimer(timerid);
    // i remove it when starting so I can have simple repeat-logic in the 
    // double click code
    timerid = SDL_AddTimer(interval, &cursor_timer, &flags);
    flags=FLAG_ACTIVE | FLAG_TOGGLED;
  }

  void stop_timer()
  {
    //report::restore_color(mouse_hexdump::address);
    flags &= ~(FLAG_TOGGLED | FLAG_ACTIVE);
    SDL_RemoveTimer(timerid);
    // flags must be set to 0 to prevent drawing to screen
    
  }
  char is_active() { return flags & FLAG_ACTIVE; }
  char is_disabled() { return flags & FLAG_DISABLED; }
  char is_toggled() { return flags & FLAG_TOGGLED; }
  void disable(char c=1)
  {
    if (c)
    {
      if (is_active())
        flags |= FLAG_WAS_ACTIVE_BEFORE_DISABLING;
      else flags &= ~FLAG_WAS_ACTIVE_BEFORE_DISABLING;
      stop_timer();
      flags |= FLAG_DISABLED;
    }
    else
    {
      flags &= ~FLAG_DISABLED;
      if (flags & FLAG_WAS_ACTIVE_BEFORE_DISABLING)
        start_timer();
    }
    
  }
  void toggle_disable()
  {
    disable(!is_disabled());
  }
  
  
} 


namespace cursor
{
  Uint8 toggle=0; // 1 = show cursor, 0 = hide
  SDL_TimerID timerid=0;
  
  // timer callback, dont worry about this
  Uint32 cursor_timer(Uint32 interval, void *param)
  {
    Uint8 *p = (Uint8 *) param;
    *p = !*p;
    return interval;
  }

  void start_timer()
  {
    // always start with the cursor showing
    toggle = 1;
    //if (timerid)
      //return;
    // is there a problem Removing an invalid timerid?? I think not..
    SDL_RemoveTimer(timerid);
    // i remove it when starting so I can have simple repeat-logic in the 
    // double click code
    timerid = SDL_AddTimer(CURSOR_TOGGLE_TIMEWINDOW, &cursor_timer, &toggle);
    //memcursor::start_timer();
  }
  void stop_timer()
  {
    //memcursor::stop_timer();
    SDL_RemoveTimer(timerid);
    // toggle must be set to 0 to prevent drawing to screen
    toggle = 0;
  }

  void draw(SDL_Surface *screen, int x, int y, Uint32 color)
  {
    if (cursor::toggle)
    {
        sdlfont_drawString(screen, x, y, "\x5B", color);
    }
  }

  Uint8 scancode_to_hex(int &scancode)
  {
      if ((scancode >= '0') && (scancode <= '9'))
        return scancode - '0';
      else if ((scancode >= 'A') && (scancode <= 'F'))
        return (scancode - 'A') + 0x0a;
      else if ((scancode >= 'a') && (scancode <= 'f'))
        return (scancode - 'a') + 0x0a;

    return 0xff; 
  }
} 

