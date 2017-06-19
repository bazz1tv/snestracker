#pragma once
#include "SDL.h"
#include "gui/Window.h"
struct Render_Context
{
public:
  //Render_Context()

  static void sdl_draw(SDL_Surface *screen=Render_Context::screen, 
    SDL_Texture *sdlTexture=Render_Context::sdlTexture,
    SDL_Renderer *sdlRenderer=Render_Context::sdlRenderer);
  static void clear_screen(SDL_Surface *screen=Render_Context::screen, 
    SDL_Texture *sdlTexture=Render_Context::sdlTexture, 
    SDL_Renderer *sdlRenderer=Render_Context::sdlRenderer);

  static void load_from_window(Window *win);

  static SDL_Window *sdlWindow;//      =NULL;
  static Uint32 windowID;
  //static SDL_Window *tracker_sdlWindow;
  static SDL_Renderer *sdlRenderer;//  =NULL;
  static SDL_Texture *sdlTexture;//    =NULL;
  static SDL_Surface *screen;//        = NULL;

  static int w,h;
};

