#include "colors.h"

extern SDL_Surface *screen;

namespace colors
{
Uint32 white, black, cyan, magenta, yellow, red;
Uint32  green, blue, nearblack;
Uint32 dark_magenta, dark_cyan, dark_yellow;
Uint32 gray;
Uint32 colorscale[12];

void precompute()
{
  // precompute some colors
  black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  nearblack = SDL_MapRGB(screen->format, 0x50, 0x50, 0x50);
  white = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
  yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
  cyan = SDL_MapRGB(screen->format, 0x00, 0xff, 0xff);
  magenta = SDL_MapRGB(screen->format, 0xff, 0x00, 0xff);
  gray = SDL_MapRGB(screen->format, 0x7f, 0x7f, 0x7f);
  red = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
  green = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);
  blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);

  dark_yellow = SDL_MapRGB(screen->format, 0x50,0x50,0x00);
  dark_cyan = SDL_MapRGB(screen->format, 0x00, 0x50, 0x50);
  dark_magenta = SDL_MapRGB(screen->format, 0x50, 0x00, 0x50);

  colorscale[0] = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
  colorscale[1] = SDL_MapRGB(screen->format, 0xff, 0x7f, 0x00);
  colorscale[2] = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
  colorscale[3] = SDL_MapRGB(screen->format, 0x7f, 0xff, 0x00);
  colorscale[4] = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);
  colorscale[5] = SDL_MapRGB(screen->format, 0x00, 0xff, 0x7f);
  colorscale[6] = SDL_MapRGB(screen->format, 0x00, 0xff, 0xff);
  colorscale[7] = SDL_MapRGB(screen->format, 0x00, 0x7f, 0xff);
  colorscale[8] = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);
  colorscale[9] = SDL_MapRGB(screen->format, 0x7f, 0x00, 0xff);
  colorscale[10] = SDL_MapRGB(screen->format, 0xff, 0x00, 0xff);
  colorscale[11] = SDL_MapRGB(screen->format, 0xff, 0x00, 0x7f);
}

}