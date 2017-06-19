#include "Clickable_Rect.h"
#include "utility.h"
#include <assert.h>

Clickable_Rect::Clickable_Rect(int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) : 
action(action), data(data)
{

}

Clickable_Rect::Clickable_Rect(int x, int y, int w, int h, 
  int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) : 
rect({x,y,w,h}), 
action(action),
data(data)
{

}

bool Clickable_Rect::check_mouse_and_execute(int x, int y, void *newdata/*=NULL*/)
{
  if (Utility::coord_is_in_rect(x,y, &rect))
  {
    do_thing(newdata);
    return true;
  }
  return false;
}

void Clickable_Rect::set_rect(int x, int y, int w, int h)
{
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
}

void Clickable_Rect::do_thing(void *newdata/*=NULL*/)
{
  assert(action);
  if (!newdata)
    action(data);
  else action(newdata);
}