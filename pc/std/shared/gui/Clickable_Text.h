#pragma once
#include <SDL.h>
#include <string>
#include "sdlfont.h"
#include "Render.h"
#include "Clickable_Rect.h"
#include "Colors.h"


struct Clickable_Text : Clickable_Rect
{
  Clickable_Text(); // if we must post-init
  Clickable_Text(char * str, int (*action)(void *data)=NULL, void *data=NULL,
    Uint32 color=Colors::Interface::color[Colors::Interface::text_fg]);
  Clickable_Text(char * str, int x, int y, int (*action)(void *data)=NULL, void *data=NULL);

  void draw(Uint32 &color, bool prefill=true, bool Vflip=false, bool Hflip=false, SDL_Surface *screen=::render->screen);
  void draw(bool prefill=true, bool Vflip=false, bool Hflip=false, SDL_Surface *screen=::render->screen);
  
  void init_width_height();
  void setup(int x, int y, bool center_x=false);
  
  char *str;
  Uint32 color=Colors::Interface::color[Colors::Interface::text_fg];
};
