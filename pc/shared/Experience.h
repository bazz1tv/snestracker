#pragma once
#include "SDL.h"
struct Experience
{
  virtual void run() {};
  virtual void one_time_draw() {}
  virtual void draw() {};
  virtual int receive_event(SDL_Event &ev) {return 0;};
};