#pragma once

#include "SDL.h"
#include "Render_Context.h"
struct Colors : public Render_Context
{
  //Colors(SDL_Surface *cscreen);
  //static SDL_Surface *screen;

  struct Interface
  {
    enum Type
    { 
      bg=0,
      text_bg,
      text_fg,
      voice_volume_left,
      voice_volume_right,
      voice_gain,
      lock,
      inactive_tab_bg,
      inactive_tab_fg,
      active_tab_bg,
      active_tab_fg,
      SIZEOF
    };
    static Uint32 color[Type::SIZEOF];
  };// interface;

  static Uint32 white, black, cyan, magenta, yellow, red;
  static Uint32 green, blue, nearblack;
  static Uint32 dark_magenta, dark_cyan, dark_yellow;
  static Uint32 gray;
  static Uint32 colorscale[12];
  static Uint32 voice[8];

  static void precompute(SDL_Surface *cscreen);
  // belongs in color namespace
  static Uint32 subtract(Uint32 *c, Uint8 subval);
  static void subtractp(Uint32 *c, Uint8 subval);
};