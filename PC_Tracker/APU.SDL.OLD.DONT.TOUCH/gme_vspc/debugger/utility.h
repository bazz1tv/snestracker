#pragma once
#include "SDL.h"

namespace Utility
{
  Uint8 scancode_to_hex(int &scancode);
  bool is_coord_in_rect(int x, int y, SDL_Rect *r);
}