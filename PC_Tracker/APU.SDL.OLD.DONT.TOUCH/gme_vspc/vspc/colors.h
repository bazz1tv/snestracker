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
}