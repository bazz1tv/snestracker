#pragma once

#include "SDL.h"

namespace colors
{
  extern Uint32 white, black, cyan, magenta, yellow, red;
  extern Uint32 green, blue, nearblack;
  extern Uint32 dark_magenta, dark_cyan, dark_yellow;
  extern Uint32 gray;
  extern Uint32 colorscale[12];

  void precompute();
  // belongs in color namespace
  Uint32 subtract(Uint32 *c, Uint8 subval);
  void subtractp(Uint32 *c, Uint8 subval);
}