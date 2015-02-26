#pragma once
#include "Experience.h"
#include "SDL.h"
#include "Audio_Context.h"
#include "gui/Context_Menu.h"
struct Audio_Options : public Experience, public Audio_Context
{
  Audio_Options(SDL_Surface *screen, SDL_Renderer *sdlRenderer);
  void one_time_draw();
  void draw();
  void run();
  int receive_event(SDL_Event &ev);

  void preload(SDL_Rect *rect);

  SDL_Surface *screen;
  SDL_Renderer *renderer;
  bool is_first_run=true;
  SDL_Rect rect;

  struct Context
  {
    Context();
    ~Context();
    // for tcontext menu
    SDL_Rect rect;
    Context_Menu *menu;
    Context_Menu_Item *menu_items;
    static int change_audio_out_device(void *item);

  } context;
};