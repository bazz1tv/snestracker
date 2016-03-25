
#pragma once
#include "SDL.h"
#include "globals.h"

#include "report.h"
#include "Voice_Control.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "Port_Tool.h"
#include "Mouse_Hexdump_Area.h"
#include "Main_Memory_Area.h"
#include "Colors.h"
#include "platform.h"
#include "Main_Window.h"
#include "Dsp_Window.h"
#include "Experience.h"
#include "Instrument_Window.h"
#include "Menu_Bar.h"
#include "gui/Window.h"

typedef Uint32 SDL_WindowID_Type;

struct Debugger : public BaseD
{
public:
  Debugger(int &argc, char **argv); // , Music_Player *player, SDL_Window *, SDL_Renderer *, SDL_Texture *, SDL_Surface*);
  ~Debugger();
  void run();
  void handle_events();

  //
  //Options_Window *options_window;
  Menu_Bar menu_bar;
  Main_Window main_window;
  Dsp_Window dsp_window;
  Instrument_Window instr_window;
  //
  SDL_DisplayMode monitor_display_mode;
  //
  //std::map<SDL_WindowID_Type, Window *> window_map;
  Experience *sub_window_experience = NULL;
  
  Options_Window options_window;
  Spc_Export_Window spc_export_window;
  static const int NUM_WINDOWS = 2;
  Window *window_map[NUM_WINDOWS+1];

  Cursors cursors;
};

