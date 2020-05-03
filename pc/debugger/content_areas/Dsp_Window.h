#pragma once
#include "BaseD.h"
#include "gme_m/Spc_Dsp_Register_Map_Interface.h"
#include "sdl_dblclick.h"
#include "Colors.h"
#include "sdlfont.h"
#include "Experience.h"
#include "gui/Clickable_Text.h"
#include "gui/Cursor.h"

static const Uint8 NUM_TIMERS=3;

struct Dsp_Window : public BaseD, public Experience
{

  bool is_srcn_used(Uint8 dirnum);
  void do_loop_point_color(int index, Uint8* brr_sample, Uint32 active_color, Uint32 inactive_color);
  Dsp_Window();
  void run();
  void draw();
  int receive_event(SDL_Event &ev);

  void reset_screw();
  Clickable_Text screw_clickable;
  bool is_screwing=false;
  static int toggle_screw(void *dsp_win);

  struct Timers
  {
    static int toggle_timer(void *n)
    {
      Uint8 *num = (Uint8*)n;
      Uint8 t;
      if (BaseD::IAPURAM[0xf1] & (1<<*num))
      {
        t = BaseD::IAPURAM[0xf1] &= ~(1<<*num);
      }
      else 
      {
        t = BaseD::IAPURAM[0xf1] |= (1<<*num);
      }
      ::player->spc_write(0xf1, t);
      return 0;
    }
    Clickable_Text label[3] = 
    {
      {"Timer 0", toggle_timer, &num},
      {"Timer 1", toggle_timer, &num},
      {"Timer 2", toggle_timer, &num} 
    };
    Clickable_Text value[3] = 
    {
      {"00"},
      {"00"},
      {"00"}
    };
    Uint8 num;
  } timers;

  static const int NUM_DIR_ENTRIES_DISPLAYED = 8*4*2;
  uint16_t dir_ram_addr;
  static uint16_t *dir;
  static uint16_t dir_index;

	static int dir_rect_clicked(void *idx);
	Clickable_Rect dir_rects[NUM_DIR_ENTRIES_DISPLAYED];

	struct Loop_Clickable
  {
    Loop_Clickable() : clickable_text("L", toggle_loop, &index) {}
    static int toggle_loop(void *index);
    Clickable_Text clickable_text;
    Uint8 index;
  } loop_clickable[NUM_DIR_ENTRIES_DISPLAYED];
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
    EDIT_GEN_DSP_ADDR,
    EDIT_GEN_DSP_8BIT_ADDR,
    EDIT_VOICE_ADDR,
    EDIT_DIR,
    EDIT_TIMER
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
  
  SDL_Rect dir_rect;
 
  

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


  enum {
    flg,
    kon,koff,non,pmon,
    eon,endx,
    SIZEOF_8BIT_GEN_DSP_ENUM
  };

  struct 
  {
    SDL_Rect bits[8];
  } byte[SIZEOF_8BIT_GEN_DSP_ENUM];
    // for the addresses represented by 8bits
  
  Gen_Dsp_Map gen_8bit_dsp_map [SIZEOF_8BIT_GEN_DSP_ENUM] =
  {
    {"FLG...: %%",dsp_reg::flg},
    {"KON...: %%",dsp_reg::kon},
    {"KOFF..: %%",dsp_reg::koff},
    {"NON...: %%",dsp_reg::non},
    {"PMON..: %%",dsp_reg::pmon},
    {"EON...: %%",dsp_reg::eon},
    {"ENDX..: %%",dsp_reg::endx}
  };

  //Clickable_Text clickable_8bit_gen_dsp[SIZEOF_8BIT_GEN_DSP_ENUM];
  Clickable_Text clickable_gen_dsp[SIZEOF_GEN_DSP_ENUM]; // dir-to-be is included see below commented out dir enum entry
  Uint8 selected_bit=0;
  Uint32 selected_index=0, selected_sub_index=0; // last one is for voice 2d array subscript

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
  static uint8_t dir_offset; // allows scrolling directory
  uint8_t used_srcn[MAX_SRCN_ENTRIES/8]; // = 0x20
  uint8_t used_srcn_voice[MAX_SRCN_ENTRIES];
  //
  bool is_first_run=true;

private:
  void init_gen_dsp_clickable(char *str, int&, int&);
  void init_voice_clickable(char *str, int&, int&);
  void enter_edit_mode();
  void exit_edit_mode();
  void clear_used_srcn();
};
