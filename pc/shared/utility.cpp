#include "utility.h"
#include <fstream>
#include <sstream>

namespace Utility
{
const char * getFileName(const char *s)
{
  if (s == NULL)
    return ("");

  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif
  const char *whoami;
  (whoami = strrchr(s, sep)) ? ++whoami : (whoami = NULL);
  return whoami;
}

char * getDirectoryName(const char *s)
{
  static char *newstr = NULL;
  if (s == NULL)
    return ("");

  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  if (newstr != NULL)
    SDL_free(newstr);

  newstr = (char *) SDL_malloc( sizeof(char) * ( strlen(s) + 1 ) );
  strcpy(newstr, s);

  char *filename = (char *) getFileName(newstr);
  if (filename == NULL)
    return NULL;

  *(filename - 1) = 0; // turn the '/' to NULL char
  char *whoami;
  (whoami = strrchr(newstr, sep)) ? ++whoami : (whoami = NULL);
  return whoami;
}


  extern int random(int min, int max);

SDL_Texture *load_texture_bmp( const char *path, SDL_Renderer *r )
{
  SDL_Texture *tex = NULL;

  SDL_Surface *surface = SDL_LoadBMP(path);
  if (surface == NULL)
  {
    printf("couldn't load %s: %s\n", path, SDL_GetError());
    return tex;
  }

  tex = SDL_CreateTextureFromSurface( r, surface);
  if (tex == NULL)
  {
    printf("Unabled to create texture from %s: %s\n",
        path, SDL_GetError());
  }

  return tex;
}
char nibble_to_ascii(uint8_t nibble)
{
  nibble &= 0x0f;
  if ( nibble < 0x0a )
    return '0' + nibble;
  return 'A' + (nibble - 0x0a);
}

void DrawRect(const SDL_Rect *r, int thickness/*=1*/,
              Uint32 color/*=Colors::white*/,
              SDL_Surface *screen/*=::render->screen*/)
{
  SDL_Rect tmp;
  /* Turns a rect that looks like
   *  --
   * |  |
   *  --
   *
   *  into its left,top,right,button components for drawing a non-filled
   *  rect*/

  // Top
  tmp.x = r->x;
  tmp.y = r->y;
  tmp.w = r->w;
  tmp.h = thickness;
  SDL_FillRect(screen, &tmp, color);

  // bottom
  tmp.y += r->h;
  SDL_FillRect(screen, &tmp, color);

  // left
  tmp.y = r->y;
  tmp.h = r->h + thickness;
  tmp.w = thickness;
  SDL_FillRect(screen, &tmp, color);

  // right
  tmp.x += r->w;
  SDL_FillRect(screen, &tmp, color);
}

extern bool coord_is_in_rect(int x, int y, SDL_Rect *r);
extern Uint32 scancode2ascii (int scancode);
extern bool scancode_is_hex(int scancode);

void set_render_color_rgb(SDL_Renderer *r, Uint32 color)
{
  SDL_SetRenderDrawColor(r,
      (Uint8) ((color >> 16) & 0xff),
      (Uint8) ((color >> 8)  & 0xff),
      (Uint8) ((color)       & 0xff),
      255);
}

void set_render_color_rgba(SDL_Renderer *r, Uint32 color)
{
  SDL_SetRenderDrawColor(r,
      (Uint8) ((color >> 24) & 0xff),
      (Uint8) ((color >> 16) & 0xff),
      (Uint8) ((color >> 8)  & 0xff),
      (Uint8) ((color)       & 0xff));
}

const char* rstrstr(const char* haystack, const char* needle)
{
  unsigned int needle_length = strlen(needle);
  const char* haystack_end = haystack + strlen(haystack) - needle_length;
  const char* p;
  size_t i;

  for(p = haystack_end; p >= haystack; --p)
  {
    for(i = 0; i < needle_length; ++i) {
      if(p[i] != needle[i])
        goto next;
    }
    return p;

    next:;
  }
  return NULL;
}

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

SDL_bool file_is_valid( std::string filename )
{
  std::ifstream ifs;
  ifs.open( filename.c_str(), std :: ios :: out );
  
  if( ifs )
  {
    ifs.close();
    return SDL_TRUE;
  }
  
  return SDL_FALSE;
}

void clearsstream(std::stringstream &s)
{
  s.clear();//clear any bits set
  s.str(std::string());
}
}
