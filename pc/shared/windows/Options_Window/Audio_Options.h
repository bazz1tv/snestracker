#pragma once
#include "Experience.h"
#include "SDL.h"
#include "Audio.h"
#include "gui/Context_Menu.h"
#include "gui/Button.h"
#include "gui/Text.h"
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

  // Audio Buffer Size
#define AUDIO_BUFFER_MIN 2
#define AUDIO_BUFFER_MAX 32768
  struct BufferSize
  {
    BufferSize();
    void update();
    void setCoords(int x, int y);
    char cbuf[sizeof("131072")];
    Text title, valtext;
    Button decbtn, incbtn;
    static int inc(void *i);
    static int dec(void *i);
  };
  BufferSize bufferSize;

private:
  bool openDeviceonClick;
};
