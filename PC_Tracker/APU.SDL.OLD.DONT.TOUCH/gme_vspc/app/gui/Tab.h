#pragma once
#include "gui/Clickable_Rect.h"
#include <string>
struct Tab : public Clickable_Rect
{
  Tab(int x, int y, int pad_size, std::string str, 
    int (*action)(void *data)=NULL, void *data=NULL);
  Tab(int x, int y, int pad_w_size, int pad_h_size, std::string str,
    int (*action)(void *data)=NULL, void *data=NULL);
  void draw();
  

  //Clickable_Rect clickable;
  std::string str;
};

/*
 ------
| TEXT |
 ------
*/