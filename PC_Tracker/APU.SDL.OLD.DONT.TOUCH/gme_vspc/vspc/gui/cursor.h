#pragma once

#include "sdlfont.h"

#define CURSOR_TOGGLE_TIMEWINDOW 300 // ms

namespace memcursor
{
  const int interval = 800;
  Uint8 toggle; // 1 = show cursor, 0 = hide
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
    toggle=1;
    // is there a problem Removing an invalid timerid?? I think not..
    SDL_RemoveTimer(timerid);
    // i remove it when starting so I can have simple repeat-logic in the 
    // double click code
    timerid = SDL_AddTimer(interval, &cursor_timer, &toggle);
  }
  void stop_timer()
  {
    SDL_RemoveTimer(timerid);
    // toggle must be set to 0 to prevent drawing to screen
    toggle = 0;
  }



  
} 


namespace cursor
{
  Uint8 toggle; // 1 = show cursor, 0 = hide
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

