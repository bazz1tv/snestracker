#pragma once
#include "BaseD.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "sdl_dblclick.h"
#include "Colors.h"
#include "sdlfont.h"
#include "Experience.h"
#include "gui/Clickable_Text.h"

#define MAX_VOICES 8

int mute_solo_voice(void *data);


struct Dsp_Window : public BaseD, public Render_Context, public Player_Context,
public Experience
{
  void run();
  void draw();
  void receive_event(SDL_Event &ev);

  Clickable_Text voice_title[MAX_VOICES];

  //
  unsigned char gen_dsp_vals[30]; // num of DSP regs
  unsigned char voice_dsp_vals[10*(MAX_VOICES)]; 
  //
  uint8_t dir_offset=0; // allows scrolling directory
  //
  bool is_first_run=true;
};