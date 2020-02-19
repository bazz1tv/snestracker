#include "utility.h"
#include <fstream>
#include <sstream>

namespace Utility
{
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
  if ( ((nibble) & 0x0f) < 0x0a )
    return '0' + nibble;
  return 'a' + (nibble - 0x0a);
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

nfdresult_t get_file_write_handle(nfdchar_t **outPath, SDL_RWops **file, const char *filter_list/*=NULL*/)
{
  char tmpbuf[200];
  *outPath=NULL;
  nfdresult_t result = NFD_SaveDialog( filter_list, NULL, outPath );
  //SDL_RaiseWindow(BaseD::sdlWindow);
    if ( result == NFD_OKAY )
    {
      //puts("Success!");
      //puts(outPath);
      //SDL_RWops* SDL_RWFromFile(const char* file,
        //                const char* mode)
      *file = SDL_RWFromFile(*outPath, "wb");
      
      if (*file == NULL)
      {
        sprintf(tmpbuf, "Warning: Unable to open file!\n %s", SDL_GetError() );
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Could not open FILE!",
                       tmpbuf,
                       NULL);
        return NFD_ERROR;
      }
      return result;
    }
    else if ( result == NFD_CANCEL ) 
    {
      if (*outPath)
        free(*outPath);
      puts("User pressed cancel.");
      return result;
    }
    else
    {
      if (*outPath)
      free(*outPath);
      printf("Error: %s\n", NFD_GetError() );
      return NFD_ERROR;
    }
}

nfdresult_t get_file_read_path(nfdchar_t **outPath, const char *filter_list/*=NULL*/)
{
  *outPath=NULL;
  nfdresult_t result = NFD_OpenDialog( filter_list, NULL, outPath );
  //SDL_RaiseWindow(BaseD::sdlWindow);
    if ( result == NFD_OKAY )
    {
      //puts("Success!");
      //puts(outPath);
      //SDL_RWops* SDL_RWFromFile(const char* file,
        //                const char* mode)
      //SDL_RaiseWindow(BaseD::sdlWindow);
      return result;
    }
    else if ( result == NFD_CANCEL ) 
    {
      if (*outPath)
        free(*outPath);
      puts("User pressed cancel.");
      return result;
    }
    else
    {
      if (*outPath)
      free(*outPath);
      printf("Error: %s\n", NFD_GetError() );
      return NFD_ERROR;
    }
}

//untested
nfdresult_t get_file_read_handle(nfdchar_t **outPath, SDL_RWops **file, const char *filter_list/*=NULL*/)
{
  char tmpbuf[200];
  *outPath=NULL;
  nfdresult_t result = NFD_OpenDialog( filter_list, NULL, outPath );
  //SDL_RaiseWindow(BaseD::sdlWindow);
    if ( result == NFD_OKAY )
    {
      //puts("Success!");
      //puts(outPath);
      //SDL_RWops* SDL_RWFromFile(const char* file,
        //                const char* mode)
      *file = SDL_RWFromFile(*outPath, "rb");
      if (*file == NULL)
      {
        sprintf(tmpbuf, "Warning: Unable to open file!\n %s", SDL_GetError() );
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Could not open FILE!",
                       tmpbuf,
                       NULL);
        return NFD_ERROR;
      }
      return result;
    }
    else if ( result == NFD_CANCEL ) 
    {
      if (*outPath)
        free(*outPath);
      puts("User pressed cancel.");
      return result;
    }
    else
    {
      if (*outPath)
      free(*outPath);
      printf("Error: %s\n", NFD_GetError() );
      return NFD_ERROR;
    }
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

void
draw_rect(SDL_Surface * screen, const SDL_Rect * r, int depth, Uint32 color)
{
  SDL_Rect tmprect;
  tmprect.x = r->x-depth;
  tmprect.y = r->y-depth;
  tmprect.w = depth;
  tmprect.h = r->h+(2*depth);
  SDL_FillRect(screen, &tmprect, color);
  tmprect.x = r->x-depth;
  tmprect.y = r->y-depth;
  tmprect.w = r->w+(2*depth);
  tmprect.h = depth;
  SDL_FillRect(screen, &tmprect, color);
  tmprect.x = r->x-depth + r->w + depth;
  tmprect.y = r->y-depth;
  tmprect.w = depth;
  tmprect.h = r->h+(2*depth);
  SDL_FillRect(screen, &tmprect, color);
  tmprect.x = r->x-depth;
  tmprect.y = r->y-depth + r->h + depth;
  tmprect.w = r->w+(2*depth);
  tmprect.h = depth;
  SDL_FillRect(screen, &tmprect, color);
}

}
