#pragma once
#include "sdlfont.h"

#define CURSOR_TOGGLE_TIMEWINDOW 300 // ms

struct Cursor
{
  Uint8 toggle=0; // 1 = show cursor, 0 = hide
  SDL_TimerID timerid=0;

  SDL_Rect rect = {0,0,CHAR_WIDTH,CHAR_HEIGHT};
  
  // timer callback, dont worry about this
  static Uint32 cursor_timerCB(Uint32 interval, void *param);
  Uint32 cursor_timer(Uint32 interval);
 
  virtual void start_timer();
  virtual void stop_timer();

  virtual void draw(SDL_Surface *screen, int x, int y, Uint32 color,
      bool draw_transparent = false);
  virtual void draw(SDL_Surface *screen, Uint32 color,
      bool draw_transparent = false);

  ~Cursor() { stop_timer(); }
}; 
