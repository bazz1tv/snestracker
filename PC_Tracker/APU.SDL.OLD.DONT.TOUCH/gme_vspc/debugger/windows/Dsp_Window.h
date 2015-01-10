#pragma once
#include "BaseD.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "sdl_dblclick.h"
#include "Colors.h"
#include "sdlfont.h"
#include "Experience.h"
#include "gui/Clickable_Text.h"
#include "gui/cursor.h"

#define MAX_VOICES 8
// These should get moved to DSP_Register_Map_Interface class later...
#define MAX_VOICE_REGS 10*MAX_VOICES
#define MAX_GEN_DSP_REGS 25

int mute_solo_voice(void *data);


struct Dsp_Window : public BaseD, public Render_Context, public Player_Context,
public Experience
{
  void run();
  void draw();
  void receive_event(SDL_Event &ev);

  enum modes 
  { 
    MODE_NAV=0,
    MODE_EDIT_ADDR,
    MODE_EDIT_APU_PORT 
  };

  
  Cursor cursor;
  
  int mode=MODE_NAV;
  // this just covers the mute/solo of voices
  Clickable_Text voice_title[MAX_VOICES];
  //
  // Gen_Dsp clickable text enum array entry index table
  enum {
    mvol_l=0,
    mvol_r,
    evol_l,
    evol_r,
    esa,edl,efb,
    // dir, 
    c0,c1,c2,c3,c4,c5,c6,c7,
    SIZEOF_GEN_DSP_ENUM
  };
  enum {
    flg,
    kon,koff,non,pmon,
    eon,endx,
    SIZEOF_8BIT_GEN_DSP_ENUM
  };
  Clickable_Text clickable_8bit_gen_dsp[SIZEOF_8BIT_GEN_DSP_ENUM];
  Clickable_Text clickable_gen_dsp[SIZEOF_GEN_DSP_ENUM]; // dir-to-be is included see below commented out dir enum entry
  //
  enum {
    vol_l=0,
    vol_r,
    plo,
    phi,
    srcn,
    adsr1,
    adsr2,
    gain,
    envx,
    outx,
    SIZE_OF_VOICE_ENUM
  };
  struct Voice_Clickable_Text
  {
    Clickable_Text e[SIZE_OF_VOICE_ENUM];
  };
  Voice_Clickable_Text voice_clickable_text[MAX_VOICES];

  //
  unsigned char gen_dsp_vals[MAX_GEN_DSP_REGS]; // num of DSP regs
  unsigned char voice_dsp_vals[10*(MAX_VOICES)]; 
  //
  uint8_t dir_offset=0; // allows scrolling directory
  //
  bool is_first_run=true;

private:
  void init_gen_dsp_clickable(char *str, int&, int&);
};