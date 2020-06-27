#pragma once
#include "Experience.h"
#include "SDL.h"
#include "Audio.h"
#include "gui/Context_Menu.h"
struct Audio_Options : public Experience
{
  Audio_Options(SDL_Surface *screen, SDL_Renderer *sdlRenderer, bool openDeviceonClick=true);
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
    Context(Audio_Options *ao);
    ~Context();
    // for tcontext menu
    SDL_Rect rect;
    Context_Menu *menu;
    Context_Menu_Item *menu_items;
    static int change_audio_out_device(void *item);
  private:
    Audio_Options *ao;
  } context;

private:
  bool openDeviceonClick;
};
