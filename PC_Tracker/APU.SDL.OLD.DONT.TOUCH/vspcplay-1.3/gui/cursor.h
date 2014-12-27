#pragma once

#include "sdlfont.h"

#define CURSOR_TOGGLE_TIMEWINDOW 500

namespace cursor
{
  Uint8 toggle; // 1 = show cursor, 0 = hide
  SDL_TimerID timerid;
  
  Uint32 cursor_timer(Uint32 interval, void *param)
  {
    Uint8 *p = (Uint8 *) param;
    *p = !*p;
    return interval;
  }

  void start_timer()
  {
    toggle = 1;
    // is there a problem Removing an invalid timerid?? I think not..
    SDL_RemoveTimer(timerid);
    timerid = SDL_AddTimer(CURSOR_TOGGLE_TIMEWINDOW, &cursor_timer, &toggle);
  }
  void stop_timer()
  {
    SDL_RemoveTimer(timerid);
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
