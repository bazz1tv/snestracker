#pragma once
#include "SDL.h"
struct Window
{
  Window(int width, int height, const char *title);
  ~Window();
  SDL_Window *sdlWindow = NULL;
  SDL_Renderer *sdlRenderer = NULL;
  SDL_Texture *sdlTexture = NULL;
  SDL_Surface *screen = NULL;

  void clear_screen();
  void draw();   
};