#pragma once
#include "SDL.h"

struct Window;
struct Render
{
public:
  Render(int w, int h);
  ~Render();

  void sdl_draw();
  void clear_screen();

  void load_from_window(Window *win);

  SDL_Renderer *sdlRenderer;
  SDL_Texture *sdlTexture;
  SDL_Surface *screen;

  SDL_Window *sdlWindow;
  Uint32 windowID;

  int w,h; // the native size
  float sx, sy; // raw scaling factors when not using SDL's logical scaling
  bool logical; // using SDL's logical scaling?
};

extern Render *render;
