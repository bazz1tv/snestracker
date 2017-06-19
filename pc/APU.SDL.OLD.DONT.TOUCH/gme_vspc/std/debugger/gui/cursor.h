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

  virtual void draw(SDL_Surface *screen, int x, int y, Uint32 color);
  virtual void draw(SDL_Surface *screen, Uint32 color);

  ~Cursor() { stop_timer(); }
}; 

struct Mem_Cursor : public Cursor
{
  enum { FLAG_TOGGLED =1,
          FLAG_ACTIVE=(1 << 1),
          FLAG_DISABLED=(1 << 2),
          FLAG_WAS_ACTIVE_BEFORE_DISABLING=(1 << 3)
        };
  const int interval= 300;
  Uint8 flags=0; // 1 = show cursor, 0 = hide
  SDL_TimerID timerid=0;

  
  
  // timer callback, dont worry about this
  static Uint32 cursor_timerCB(Uint32 interval, void *param);
  Uint32 cursor_timer(Uint32 interval);

  void start_timer();

  void stop_timer();
  char is_active();
  char is_disabled(); 
  char is_toggled();
  void disable(char c=1);
  void toggle_disable();
  
  
};

