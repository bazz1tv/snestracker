#pragma once
#include <SDL.h>
#include <string>
#include "sdlfont.h"

struct Clickable_Text 
{
  Clickable_Text(); // if we must post-init
  Clickable_Text(std::string str, int (*action)(void *data)=NULL, void *data=NULL);
  Clickable_Text(std::string str, int x, int y, int (*action)(void *data)=NULL, void *data=NULL);

  void check_mouse_and_execute(int x, int y, void *newdata);

  void set_rect(int x, int y, int w, int h);
  SDL_Rect rect;
  std::string str;
  void *data;
  // maybz function pointer?
  // must be static class function, or reg. global function, or somethin.
  // it can be in namespace
  void do_thing(void *newdata=NULL);
  int (*action)(void* data);
};