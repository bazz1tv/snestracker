#pragma once
#include "SDL.h"

#include <string>
#include <nfd.h>
#include "Colors.h"
#include "Render.h"

namespace Utility
{
  const char * getFileName(const char *s);
  char * getDirectoryName(const char *s);

  inline int random(int min, int max) {  return rand() % (max - min + 1) + min; }
  SDL_Texture *load_texture_bmp( const char *path, SDL_Renderer *r );

  char nibble_to_ascii(uint8_t nibble);

  void DrawRect(const SDL_Rect *rect, int thickness=1, Uint32 color=Colors::white,
                SDL_Surface *screen=::render->screen);

  inline bool coord_is_in_rect(int x, int y, const SDL_Rect *r)
  {
    return (x >= r->x && x < (r->x + r->w) &&
            y >= r->y && y < (r->y+r->h) );
  }

//////////////// SCANCODE BASED FUNCS /////////////////////////
  inline Uint32 scancode2ascii (int scancode)
  {
    Uint32 i;

    if ((scancode >= '0') && (scancode <= '9'))
      i = scancode - '0';
    else if ((scancode >= 'A') && (scancode <= 'F'))
      i = (scancode - 'A') + 0x0a;
    else if ((scancode >= 'a') && (scancode <= 'f'))
      i = (scancode - 'a') + 0x0a;
    return i;
  }

  inline bool scancode_is_hex(int scancode)
  {
    return ( ((scancode >= '0') && (scancode <= '9')) || ((scancode >= 'A') && (scancode <= 'F'))
      || ((scancode >= 'a') && (scancode <= 'f')) ) ? true : false;
  }
/////////////////////////////////////////////////////////////////
  void set_render_color_rgb(SDL_Renderer *r, Uint32 color);
  void set_render_color_rgba(SDL_Renderer *r, Uint32 color);

  const char* rstrstr(const char* haystack, const char* needle);

  Uint8 scancode_to_hex(int &scancode);

  SDL_bool file_is_valid( std::string filename );

  void clearsstream(std::stringstream &s);
}
