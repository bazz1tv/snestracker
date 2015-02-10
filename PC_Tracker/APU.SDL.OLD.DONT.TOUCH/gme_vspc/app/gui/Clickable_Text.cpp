#include "gui/Clickable_Text.h"
#include "utility.h"
#include "sdlfont.h"
#include <assert.h>

Clickable_Text::Clickable_Text() : 
str(""),
action(NULL),
data(NULL)
{}

Clickable_Text::Clickable_Text(std::string str, int (*action)(void *data)/*=NULL*/, 
void *data/*=NULL*/) : 
str(str),
action(action),
data(data)
{
  rect.w = str.length()*CHAR_WIDTH; // could add padding
  rect.h = CHAR_HEIGHT;
}
Clickable_Text::Clickable_Text(std::string str, int x, int y, 
int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) : 
str(str),
action(action),
data(data)
{
  setup(x,y);
}

void Clickable_Text::setup(int x, int y)
{
  rect.w = str.length()*CHAR_WIDTH; // could add padding
  rect.h = CHAR_HEIGHT;
  rect.x = x;
  rect.y = y;
}

void Clickable_Text::draw(Uint32 &color, bool prefill/*=true*/, 
  bool Vflip/*=false*/, bool Hflip/*=false*/)
{
  sdlfont_drawString(screen, rect.x, rect.y, str.c_str(), color, prefill, Vflip, Hflip);
}

bool Clickable_Text::check_mouse_and_execute(int x, int y, void *newdata/*=NULL*/)
{
  if (Utility::coord_is_in_rect(x,y, &rect))
  {
    do_thing(newdata);
    return true;
  }
  return false;
}

void Clickable_Text::set_rect(int x, int y, int w, int h)
{
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
}

void Clickable_Text::do_thing(void *newdata/*=NULL*/)
{
  assert(action);
  if (!newdata)
    action(data);
  else action(newdata);
}