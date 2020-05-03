#pragma once
#include "SDL.h"

struct Clickable_Rect
{
  // Constructors
  Clickable_Rect(int (*action)(void *data)=NULL, void *data=NULL);
  Clickable_Rect(int x, int y, int w, int h,
                 int (*action)(void *data)=NULL, void *data=NULL);

  void set_rect(int x, int y, int w, int h);
  bool check_mouse_and_execute(int x, int y, void *newdata=NULL);
  bool check_mouse_and_execute(int x, int y, SDL_Rect *rect, void *newdata=NULL);

  void do_thing(void *newdata=NULL);

  SDL_Rect rect;
  int (*action)(void* data);
  void *data;
};
