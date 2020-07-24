#include "gui/Tab.h"
#include "Colors.h"
#include "DEBUGLOG.h"
#include "sdlfont.h"

bool Tab::check_mouse_and_execute(int x, int y, void *newdata/*=NULL*/)
{
  if (enabled)
    return Clickable_Rect::check_mouse_and_execute(x,y,newdata);
  else {return false;}
}

int Tab::horiz_pixel_length()
{
  return str.length()*CHAR_WIDTH + (pad_w * 2);
}

void Tab::draw(SDL_Surface *screen/*=Render_Context::screen*/)
{
  Uint32 *bg_color, *fg_color;

  if (enabled)
  {
    if (active)
    {
      bg_color = &Colors::Interface::color[Colors::Interface::active_tab_bg];
      fg_color = &Colors::Interface::color[Colors::Interface::active_tab_fg];
    }
    else
    {
      bg_color = &Colors::Interface::color[Colors::Interface::inactive_tab_bg];
      fg_color = &Colors::Interface::color[Colors::Interface::inactive_tab_fg];
    }
  }
  else
  {
    bg_color = &Colors::nearblack;
    fg_color = &Colors::gray;
  }

  SDL_FillRect(screen, &rect, *bg_color);
  sdlfont_drawString(screen, rect.x + pad_w, rect.y + pad_h,
                     str.c_str(), *fg_color, *bg_color);
}

Tab::Tab(bool active/*=false*/) : Clickable_Rect(NULL,NULL)
{ }

Tab::Tab(int x, int y, int pad, std::string str,
  int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/, bool active/*=false*/) : 
Clickable_Rect(action,data),
str(str),
active(active),
pad_w(pad),
pad_h(pad)

{
  rect.x = x;
  rect.y = y;
  rect.w = pad + str.length()*CHAR_WIDTH + pad;
  rect.h = pad + CHAR_HEIGHT + pad;
}

Tab::Tab(int x, int y, int pad_w, int pad_h, std::string str,
  int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/, bool active/*=false*/) : 
Clickable_Rect(action,data),
str(str),
active(active),
pad_w(pad_w),
pad_h(pad_h)
{
  rect.x = x;
  rect.y = y;
  rect.w = pad_w + str.length()*CHAR_WIDTH + pad_w;
  rect.h = pad_h + CHAR_HEIGHT + pad_h;
}

Tab::Tab(int pad_w, int pad_h, std::string str,
  int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/, bool active/*=false*/) : 
Clickable_Rect(action,data),
str(str),
active(active),
pad_w(pad_w),
pad_h(pad_h)
{
  rect.w = pad_w + str.length()*CHAR_WIDTH + pad_w;
  rect.h = pad_h + CHAR_HEIGHT + pad_h;
}
