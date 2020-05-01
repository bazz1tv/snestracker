#pragma once
#include "SDL.h"
#include "gui/Window.h"

struct Render
{
public:
  Render();
  ~Render();

  void sdl_draw(SDL_Surface *screen=Render::screen,
    SDL_Texture *sdlTexture=Render::sdlTexture,
    SDL_Renderer *sdlRenderer=Render::sdlRenderer);
  void clear_screen(SDL_Surface *screen=Render::screen,
    SDL_Texture *sdlTexture=Render::sdlTexture,
    SDL_Renderer *sdlRenderer=Render::sdlRenderer);

  void load_from_window(Window *win);

  static SDL_Renderer *sdlRenderer;
  static SDL_Texture *sdlTexture;
  static SDL_Surface *screen;

  SDL_Window *sdlWindow;
  Uint32 windowID;

  int w,h;
};

extern Render *render;
