#include "Colors.h"

//SDL_Surface *Colors::screen=NULL;
Uint32 Colors::white, Colors::black, Colors::cyan, Colors::magenta, Colors::yellow, Colors::red;
Uint32 Colors::green, Colors::blue, Colors::nearblack;
Uint32 Colors::dark_magenta, Colors::dark_cyan, Colors::dark_yellow;
Uint32 Colors::gray;
Uint32 Colors::colorscale[12];
Uint32 Colors::voice[8];

Uint32 Colors::Interface::color[Colors::Interface::Type::SIZEOF];
//Uint32 Colors::Interface::text_bg, Colors::Interface::text_fg;

void Colors::precompute(SDL_Surface *cscreen)
{
  screen = cscreen;
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

  // white
  voice[0] = SDL_MapRGB(screen->format,153,153,255);
  // orange
  voice[1] = SDL_MapRGB(screen->format, 255, 160, 0);
  // yellow
  voice[2] = yellow;
  // geen
  voice[3] = green;
  // blue
  voice[4] = cyan;
  // magenta
  voice[5] = magenta;
  // red
  voice[6] = red;
  // gray
  voice[7] = SDL_MapRGB(screen->format, 0,250,154);


  Interface::color[Interface::Type::lock] = red;

  Interface::color[Interface::Type::voice_volume_left] = yellow;
  Interface::color[Interface::Type::voice_volume_right] = cyan;
  Interface::color[Interface::Type::voice_gain] = magenta;

  Interface::color[Interface::Type::text_bg] = black;
  Interface::color[Interface::Type::text_fg] = white;
  Interface::color[Interface::Type::bg] = black;

  Interface::color[Interface::Type::inactive_tab_bg] = gray;
  Interface::color[Interface::Type::active_tab_bg] = SDL_MapRGB(screen->format, 0x20, 0x20, 0x20);
  Interface::color[Interface::Type::inactive_tab_fg] = Interface::color[Interface::Type::active_tab_fg] = white;
}

Uint32 Colors::subtract(Uint32 *c, Uint8 subval)
{
  Uint8 r,g,b;
  SDL_GetRGB(*c, screen->format, &r, &b, &g);
      // CAP at ZERO
  
      return SDL_MapRGB(screen->format,
        r-subval >= 0x10 ? (r-subval):0x10,
        g-subval >= 0x10 ? (g-subval):0x10,
        b-subval >= 0x10 ? (b-subval):0x10);
}

void Colors::subtractp(Uint32 *c, Uint8 subval)
{
  Uint8 r,g,b;
  SDL_GetRGB(*c, screen->format, &r, &b, &g);
      // CAP at ZERO
      *c = SDL_MapRGB(screen->format,
        r-subval >= 0x10 ? (r-subval):0x10,
        g-subval >= 0x10 ? (g-subval):0x10,
        b-subval >= 0x10 ? (b-subval):0x10);
}

