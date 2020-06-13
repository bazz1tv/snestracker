#pragma once
#include <SDL.h>
#include <string>
#include "sdlfont.h"
#include "Render.h"
#include "Clickable_Rect.h"
#include "Colors.h"

/* TODO: Strip out Colors.h and friends from this class */
struct Clickable_Text : public Clickable_Rect
{
  Clickable_Text(const char * str="", int (*action)(void *data)=NULL, void *data=NULL,
    Uint32 color=Colors::Interface::color[Colors::Interface::text_fg]);
  Clickable_Text(const char * str, int x, int y, int (*action)(void *data)=NULL, void *data=NULL);

  void draw(Uint32 color, bool prefill=true, bool Vflip=false, bool Hflip=false, SDL_Surface *screen=::render->screen);
  void draw(bool prefill=true, bool Vflip=false, bool Hflip=false, SDL_Surface *screen=::render->screen);
  
  void init_width_height();
  void setup(int x, int y, bool center_x=false);
  
  const char *str;
  Uint32 color=Colors::Interface::color[Colors::Interface::text_fg];
};
