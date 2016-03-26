#pragma once
#include <SDL.h>
#include <string>
#include "sdlfont.h"
#include "Render_Context.h"
#include "Clickable_Rect.h"
#include "Colors.h"


struct Clickable_Text : public Render_Context, Clickable_Rect
{
  Clickable_Text(); // if we must post-init
  Clickable_Text(std::string str, int (*action)(void *data)=NULL, void *data=NULL, 
    Uint32 color=Colors::Interface::color[Colors::Interface::text_fg]);
  Clickable_Text(std::string str, int x, int y, int (*action)(void *data)=NULL, void *data=NULL);

  void draw(Uint32 &color, bool prefill=true, bool Vflip=false, bool Hflip=false, SDL_Surface *screen=Render_Context::screen);
  void draw(bool prefill=true, bool Vflip=false, bool Hflip=false, SDL_Surface *screen=Render_Context::screen);
  
  void init_width_height();
  void setup(int x, int y, bool center_x=false);
  
  std::string str;
  Uint32 color=Colors::Interface::color[Colors::Interface::text_fg];
  // maybz function pointer?
  // must be static class function, or reg. global function, or somethin.
  // it can be in namespace
  
};