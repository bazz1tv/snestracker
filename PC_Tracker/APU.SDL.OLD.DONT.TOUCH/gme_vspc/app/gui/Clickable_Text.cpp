#include "gui/Clickable_Text.h"
#include "utility.h"
#include "sdlfont.h"
#include <assert.h>

Clickable_Text::Clickable_Text() : 
str(""),
Clickable_Rect(action,data)
{}

Clickable_Text::Clickable_Text(std::string str, int (*action)(void *data)/*=NULL*/, 
void *data/*=NULL*/) : 
str(str),
Clickable_Rect(action,data)
{
  rect.w = str.length()*CHAR_WIDTH; // could add padding
  rect.h = CHAR_HEIGHT;
}
Clickable_Text::Clickable_Text(std::string str, int x, int y, 
int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) : 
str(str),
Clickable_Rect(action,data)
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

