#include "gui/Tab.h"

Tab::Tab(int x, int y, int pad_size, std::string str, 
  int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) : 
str(str), 
Clickable_Rect(action,data)
{
  rect.x = x;
  rect.y = y;
  rect.w = pad_size + str.length()*CHAR_WIDTH + pad_size;
  rect.h = pad_size + CHAR_HEIGHT + pad_size;
}

Tab::Tab(int x, int y, int pad_w_size, int pad_h_size, std::string str,
  int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) : 
str(str),
Clickable_Rect(action,data)
{
  rect.x = x;
  rect.y = y;
  rect.w = pad_w_size + str.length()*CHAR_WIDTH + pad_w_size;
  rect.h = pad_h_size + CHAR_HEIGHT + pad_h_size;
}