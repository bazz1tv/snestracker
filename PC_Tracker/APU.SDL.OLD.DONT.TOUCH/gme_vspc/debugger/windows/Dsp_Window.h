#pragma once
#include "BaseD.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "sdl_dblclick.h"
#include "Colors.h"
#include "sdlfont.h"
#include "Experience.h"
struct Dsp_Window : public BaseD, public Render_Context, public Player_Context,
public Experience
{
  void run();
  void draw();
  void receive_event(SDL_Event &ev);

  //char derp;
  unsigned char vals[30 + (10*8)]; // num of DSP regs
};