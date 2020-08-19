#include "app.h"
#include "Screen.h"
#include "Colors.h"
#include "Organization.h"

#include "Render.h"
#include <time.h>
#include "shared/SdlNfd.h"
#include "DEBUGLOG.h"
#include "sdl_dblclick.h"
#include "globals.h"
#include "Screen.h"
#include "RecentFiles.h"
#include "platform.h"

static int init_sdl(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer,
             SDL_Texture **sdlTexture, SDL_Surface **screen,
             int width, int height, Uint32 flags=0);
static void SetIcon(SDL_Window* window);


int main(int argc, char **argv)
{
  time_t t = time(NULL);
  srand((unsigned int)t);

  Render render(SCREEN_WIDTH, SCREEN_HEIGHT);
  ::render = &render;

  init_sdl(&render.sdlWindow, &render.sdlRenderer,
    &render.sdlTexture, &render.screen, SCREEN_WIDTH, SCREEN_HEIGHT);

  render.windowID = SDL_GetWindowID(render.sdlWindow);

/* Fixes the issue on certain Windows platforms that render a black icon (WINE)
or half black icon (Win7). The problem probably stems from the ICO file, but since
we cannot identify it yet, this is the fix.

Be sure NOT to use this function on Mac because it will cause the Dock to use a
lower resolution icon! */
#ifdef PLATFORM_WINDOWS
  SetIcon(::render->sdlWindow);
#endif

  SdlNfd::init(render.sdlWindow);
  RecentFiles::init();

  App app(argc, argv, 32000);
  app.run();
}

#ifdef PLATFORM_WINDOWS
void SetIcon(SDL_Window* window)
{
#include "st_icon32_rle.h"

  DEBUGLOG("SETTING ICON\n");

  // these masks are needed to tell SDL_CreateRGBSurface(From)
  // to assume the data it gets is byte-wise RGB(A) data
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int shift = (ST_ICON_BYTES_PER_PIXEL == 3) ? 8 : 0;
  rmask = 0xff000000 >> shift;
  gmask = 0x00ff0000 >> shift;
  bmask = 0x0000ff00 >> shift;
  amask = 0x000000ff >> shift;
#else // little endian, like x86
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = (ST_ICON_BYTES_PER_PIXEL == 3) ? 0 : 0xff000000;
#endif

  /* Extract RLE-encoded data */
  unsigned char *st_icon_pixel_buf =
    (unsigned char *) SDL_malloc( sizeof(unsigned char) * (32 * 32 * 3 + 1) );

  if (st_icon_pixel_buf == NULL)
    return;

  ST_ICON_RUN_LENGTH_DECODE(st_icon_pixel_buf, ST_ICON_RLE_PIXEL_DATA,
    ST_ICON_WIDTH * ST_ICON_HEIGHT, ST_ICON_BYTES_PER_PIXEL);

  SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*)st_icon_pixel_buf,
    ST_ICON_WIDTH, ST_ICON_HEIGHT, ST_ICON_BYTES_PER_PIXEL*8,
    ST_ICON_BYTES_PER_PIXEL * ST_ICON_WIDTH, rmask, gmask, bmask, amask);

  if (icon == NULL)
    goto _free_buf;

  SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 255, 0, 255));
  SDL_SetWindowIcon(window, icon);

  SDL_FreeSurface(icon);
_free_buf:
  SDL_free(st_icon_pixel_buf);
}
#endif /* PLATFORM_WINDOWS */



int init_sdl(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer, SDL_Texture **sdlTexture, 
  SDL_Surface **screen, int width, int height, Uint32 flags/*=0*/)
{
  flags |= SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER; 

  if (SDL_Init(flags) != 0) 
  {
    fprintf(stderr,
            "\nUnable to initialize SDL:  %s\n",
            SDL_GetError()
           );
    return -1;
  }
  atexit(SDL_Quit);

  *sdlWindow = SDL_CreateWindow("snes tracker", SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
  if (*sdlWindow == NULL)
  {
    fprintf(stderr, "Couldn't create the main window! %s\n", SDL_GetError());
    return -1;
  }

  *sdlRenderer = SDL_CreateRenderer(*sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (*sdlRenderer == NULL)
  {
    DEBUGLOG("No Renderer: %s\n", SDL_GetError());
    *sdlRenderer = SDL_CreateRenderer(*sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (*sdlRenderer == NULL)
    {
      DEBUGLOG("No Renderer: %s\n", SDL_GetError());
      *sdlRenderer = SDL_CreateRenderer(*sdlWindow, -1, 0);
      if (*sdlRenderer == NULL)
      {
        fprintf(stderr,
            "\nUnable to initialize SDL:  %s\n",
            SDL_GetError()
            );
        return -1;
      }
    }
  }

  // nearest neighbor
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  //SDL_RenderSetLogicalSize(*sdlRenderer, width, height);
  /* pretty hackish to set this here but whatever */
  ::render->logical = false;
  //SDL_RenderSetIntegerScale(*sdlRenderer, SDL_TRUE);

  *screen = SDL_CreateRGBSurface(0, width, height, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
  SDL_SetColorKey(*screen, SDL_TRUE, 0);
  SDL_SetSurfaceBlendMode(*screen, SDL_BLENDMODE_BLEND);

  *sdlTexture = SDL_CreateTexture(*sdlRenderer,
                                          SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          width, height);

  SDL_SetTextureBlendMode(*sdlTexture, SDL_BLENDMODE_BLEND);

  if (*screen == NULL || *sdlTexture == NULL)
  {
    fprintf(stderr, "MAn we have SDL init problems :\\. I couldn't allocate a screen or Texture : %s\n", SDL_GetError());
    return -1;
  }

  // needs to be more permanent eh!
  char tmpbuf[100];
  sprintf(tmpbuf, PROG_NAME_VERSION_STRING, APP_VERSION);
  SDL_SetWindowTitle(*sdlWindow, tmpbuf);
  
  Colors::precompute(*screen);

  dblclick::init();

  return 0; 
}
