#pragma once
#include "SDL.h"
#include "gui/Window.h"

struct Render
{
public:
  Render();
  ~Render();

  void sdl_draw(SDL_Surface *screen=screen, 
    SDL_Texture *sdlTexture=sdlTexture,
    SDL_Renderer *sdlRenderer=sdlRenderer);
  void clear_screen(SDL_Surface *screen=screen, 
    SDL_Texture *sdlTexture=sdlTexture, 
    SDL_Renderer *sdlRenderer=sdlRenderer);

  void load_from_window(Window *win);

  SDL_Window *sdlWindow;
  Uint32 windowID;
  static SDL_Renderer *sdlRenderer;
  static SDL_Texture *sdlTexture;
  static SDL_Surface *screen;

  int w,h;
};

extern Render *render;
