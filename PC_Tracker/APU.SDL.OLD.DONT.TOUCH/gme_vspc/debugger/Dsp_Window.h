#pragma once
#include "Debugger_Base.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "sdl_dblclick.h"
#include "Colors.h"
#include "sdlfont.h"
#include "Experience.h"
struct Dsp_Window : public Debugger_Base, public Render_Context, public Player_Context,
public Experience
{
  void run();
  void draw();
  void receive_event(SDL_Event &ev);

  uint8_t vals[41]; // num of DSP regs
};