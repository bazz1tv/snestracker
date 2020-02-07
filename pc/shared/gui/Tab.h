#pragma once
#include "gui/Clickable_Rect.h"
#include <string>
struct Tab : public Clickable_Rect
{
  Tab(bool active=false);
  Tab(int x, int y, int pad_size, std::string str, 
    int (*action)(void *data)=NULL, void *data=NULL, bool active=false);
  Tab(int x, int y, int pad_w_size, int pad_h_size, std::string str,
    int (*action)(void *data)=NULL, void *data=NULL, bool active=false);
  Tab(int pad_w_size, int pad_h_size, std::string str,
    int (*action)(void *data)=NULL, void *data=NULL, bool active=false);
  void draw(SDL_Surface *screen=::render->screen);
  int horiz_pixel_length();
  bool check_mouse_and_execute(int x, int y, void *newdata=NULL);
  
  std::string str;
  bool active=false;
  int pad_w_size, pad_h_size;  
};

/*
 ------
| TEXT |
 ------
*/
