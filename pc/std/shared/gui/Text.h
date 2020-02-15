#pragma once
#include "SDL.h"
#include "Render.h"
#include "Colors.h"

struct Text
{
  Text(const char *str = "");
  void draw(Uint32 color=Colors::Interface::color[Colors::Interface::Type::text_fg],
      bool prefill=true, bool Vflip=false,
      bool Hflip=false, SDL_Surface *screen=::render->screen);
  /* defines the rect of this text entity. Only the coords are used
   * currently; the width and height are merely informational */
  SDL_Rect rect;
  const char *str;
  // Maybe give this entity its own color
  //Uint32 color;
};
