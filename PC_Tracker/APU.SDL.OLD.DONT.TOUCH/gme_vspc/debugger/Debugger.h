
#pragma once
#include "SDL.h"
#include "globals.h"

#include "report.h"
#include "Voice_Control.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "Port_Tool.h"
#include "mode.h"
#include "Mouse_Hexdump_Area.h"
#include "Main_Memory_Area.h"
#include "Colors.h"
#include "platform.h"
#include "Main_Window.h"

struct Debugger : Debugger_Base
{
public:
  Debugger(int &argc, char **argv); // , Music_Player *player, SDL_Window *, SDL_Renderer *, SDL_Texture *, SDL_Surface*);
  void run();
  void handle_events();

  Main_Window main_window;
};

