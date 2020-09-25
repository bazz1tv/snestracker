#pragma once
#include "SDL.h"
#include "Render.h"
#include "Colors.h"

struct Text
{
  Text(const char *str = "");
  void init_width_height();
  void draw(SDL_Surface *screen=::render->screen,
      Uint32 fg_color=Colors::Interface::color[Colors::Interface::Type::text_fg],
      bool prefill=true, bool Vflip=false,
      bool Hflip=false,
      Uint32 bg_color=Colors::Interface::color[Colors::Interface::Type::text_bg]);
  /* defines the rect of this text entity. Only the coords are used
   * currently; the width and height are merely informational */
  SDL_Rect rect;
  const char *str;
  // Maybe give this entity its own color
  //Uint32 color;
};
