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

// clickable function
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
  int mode=MODE_NAV;
  enum submodes
  {
    NONE=0,
    EDIT_GEN_DSP_ADDR=1,
    EDIT_VOICE_ADDR
  };
  int submode=NONE;
  int current_edit_addr=0;
  bool highnibble=false;
  
  Cursor cursor;
  
  
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

  struct Gen_Dsp_Map
  {
    const char* format_str;
    int addr;
  };
  Gen_Dsp_Map gen_dsp_map[SIZEOF_GEN_DSP_ENUM] = 
  {
    {"Mvol_L: $%02X",0x0c},
    {"Mvol_R: $%02X",0x1c},
    {"Evol_L: $%02X",0x2c},
    {"Evol_R: $%02X",0x3c},
    {"ESA...: $%02X",0x6d},
    {"EDL...: $%02X",0x7d},
    {"EFB...: $%02X",0x0d},
    {"C0: $%02X",0x0f},
    {"C1: $%02X",0x1f},
    {"C2: $%02X",0x2f},
    {"C3: $%02X",0x3f},
    {"C4: $%02X",0x4f},
    {"C5: $%02X",0x5f},
    {"C6: $%02X",0x6f},
    {"C7: $%02X",0x7f}
  };


  // below is better detail of the above
  /*Voice_Map voice_map[SIZEOF_GEN_DSP_ENUM] = 
    { {mvol_l,  0x0c},
      {mvol_r,  0x1c},
      {evol_l,  0x2c},
      {evol_r,  0x3c},
      {esa,     0x6d},
      {edl,     0x7d},
      {efb,     0x0d},
      {c0,      0x0f},
      {c1,      0x1f},
      {c2,      0x2f},
      {c3,      0x3f},
      {c4,      0x4f},
      {c5,      0x5f},
      {c6,      0x6f},
      {c7,      0x7f} };*/

  // Voice map is not required
  struct 
  {
    SDL_Rect bits[8];
  } byte[SIZEOF_8BIT_GEN_DSP_ENUM];

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
    SIZEOF_VOICE_ENUM
  };
  const char *voice_map[SIZEOF_VOICE_ENUM] = 
  { 
    "vol_L: $%02X",
    "vol_R: $%02X",
    "p_lo.: $%02X",
    "p_hi.: $%02X",
    "srcn.: $%02X",
    "adsr1: $%02X",
    "adsr2: $%02X",
    "gain.: $%02X",
    "envx.: $%02X",
    "outx.: $%02X"
  };
  Clickable_Text clickable_voice[MAX_VOICES][SIZEOF_VOICE_ENUM];
  uint8_t tmp_ram; // plan on changing this to tmp_byte after adding inclusions form main_window.cpp

  //
  //unsigned char gen_dsp_vals[MAX_GEN_DSP_REGS]; // num of DSP regs
  //unsigned char voice_dsp_vals[10*(MAX_VOICES)]; 
  //
  uint8_t dir_offset=0; // allows scrolling directory
  //
  bool is_first_run=true;

private:
  void init_gen_dsp_clickable(char *str, int&, int&);
  void init_voice_clickable(char *str, int&, int&);
  void enter_edit_mode();
  void exit_edit_mode();
};