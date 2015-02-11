#pragma once
#include "sdlfont.h"
#include "Render_Context.h"


struct Clickable_Rect : public Render_Context
{
  Clickable_Rect(int (*action)(void *data)=NULL, void *data=NULL);
  Clickable_Rect();
  Clickable_Rect(int x, int y, int w, int h, int (*action)(void *data)=NULL, void *data=NULL);
  void set_rect(int x, int y, int w, int h);
  bool check_mouse_and_execute(int x, int y, void *newdata=NULL);
  SDL_Rect rect;
  void do_thing(void *newdata=NULL);
  int (*action)(void* data);
  void *data;
};