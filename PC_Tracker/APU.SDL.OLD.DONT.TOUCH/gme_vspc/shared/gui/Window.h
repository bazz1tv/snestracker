#pragma once
#include "SDL.h"
#include "Experience.h"

struct Window : public Experience
{
  Window(int width, int height, const char *title);
  ~Window();
  SDL_Window *sdlWindow = NULL;
  SDL_Renderer *sdlRenderer = NULL;
  SDL_Texture *sdlTexture = NULL;
  SDL_Surface *screen = NULL;
  Uint32 windowID=0;

  SDL_Rect rect;
  

  bool is_focused=false;

  void clear_screen();
  void update_screen();

  void show();
  void raise();
  void hide();
};