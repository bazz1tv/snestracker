#pragma once
#include "gui/Clickable_Rect.h"
#include <string>
#include "Render.h"

struct Tab : public Clickable_Rect
{
  Tab(bool active=false);
  Tab(int x, int y, int pad, std::string str,
    int (*action)(void *data)=NULL, void *data=NULL, bool active=false);
  Tab(int x, int y, int pad_w, int pad_h, std::string str,
    int (*action)(void *data)=NULL, void *data=NULL, bool active=false);
  Tab(int pad_w, int pad_h, std::string str,
    int (*action)(void *data)=NULL, void *data=NULL, bool active=false);
  void draw(SDL_Surface *screen=::render->screen);
  int horiz_pixel_length();
  bool check_mouse_and_execute(int x, int y, void *newdata=NULL);
  
  std::string str;
  bool active=false;
  int pad_w, pad_h;
};

/*
 ------
| TEXT |
 ------
*/
