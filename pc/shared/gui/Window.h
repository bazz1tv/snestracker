#pragma once
#include "SDL.h"
#include "Experience.h"
#include <string>
#include "Render.h"
struct Window : public Experience
{
  void init();
  Window(int width, int height, const char *title);
  ~Window();
  Render render;

  SDL_Rect rect;
  std::string title;

  bool is_focused=false;

  void clear_screen();
  void update_screen();

  void show();
  void raise();
  void hide();
  void destroy();

  bool oktoshow = false;

  // TODO: Have the map be a vector or unordered_map and get rid of NUM_WINDOWS
  static const int constexpr NUM_WINDOWS = 2;
  static Window *map[NUM_WINDOWS+1];
  static Window * getWindow(Uint32 windowID);
  static Window * getWindow(SDL_Window *sdlWindow);

};