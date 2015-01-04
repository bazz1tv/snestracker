#include "SDL.h"

namespace Utility
{
Uint8 scancode_to_hex(int &scancode)
{
    if ((scancode >= '0') && (scancode <= '9'))
      return scancode - '0';
    else if ((scancode >= 'A') && (scancode <= 'F'))
      return (scancode - 'A') + 0x0a;
    else if ((scancode >= 'a') && (scancode <= 'f'))
      return (scancode - 'a') + 0x0a;

  return 0xff; 
}
bool is_coord_in_rect(int x, int y, SDL_Rect *r)
{
  return (x >= (r->x) && x <= (r->x + r->w) &&
      y >= (r->y) && y < (r->y+r->h) ); 
}
}