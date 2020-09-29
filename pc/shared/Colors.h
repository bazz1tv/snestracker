#pragma once

#include "SDL.h"
#include "Render.h"
struct Colors
{
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
      button_bg,
      button_pressed_bg,
      button_fg,
      button_pressed_fg,
      selections,
      recording,
      subselections,
      note,
      instr,
      vol,
      fx,
      fxparam,

      //v0.2.1
      patseqpanelBG,
      songpanelBG,
      instrpanelBG,
      samplepanelBG,
      SIZEOF
    };
    static Uint32 color[Type::SIZEOF];
  };

  static Uint32 white, black, cyan, magenta, yellow, red;
  static Uint32 orange;
  static Uint32 green, blue, nearblack;
  static Uint32 dark_magenta, dark_cyan, dark_yellow;
  static Uint32 gray;
  static Uint32 colorscale[12];
  static Uint32 voice[8];
  static Uint32 transparent;

  static void precompute(SDL_Surface *screen);
  // belongs in color namespace
  static Uint32 subtract(Uint32 *c, Uint8 subval);
  static void subtractp(Uint32 *c, Uint8 subval);
};
