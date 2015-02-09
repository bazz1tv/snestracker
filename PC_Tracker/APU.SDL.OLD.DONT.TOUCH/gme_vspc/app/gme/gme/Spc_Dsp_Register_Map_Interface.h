#pragma once

//#include "Spc_Emu.h"
#include "Snes_Spc.h"

#define MAX_SRCN_ENTRIES 256
#define MAX_VOICES 8
// These should get moved to DSP_Register_Map_Interface class later...
#define MAX_VOICE_REGS 10*MAX_VOICES
#define MAX_GEN_DSP_REGS 25

namespace dsp_reg
{
  extern int how_many;
enum {
    vol_l=0,
  vol_r=1,
  plo=2,
  phi=3,
  srcn=4,
  adsr1=5,
  adsr2=6,
  gain=7,
  envx=8,
  outx=9,
  voice0_vol_l=0,
  voice0_vol_r=1,
  voice0_pitch_lo=2,
  voice0_pitch_hi=3,
  voice0_srcn=4,
  voice0_adsr1=5,
  voice0_adsr2=6,
  voice0_gain=7,
  voice0_envx=8,
  voice0_outx=9,

  voice1_vol_l=0x10,
  voice1_vol_r=0x11,
  voice1_pitch_lo=0x12,
  voice1_pitch_hi=0x13,
  voice1_srcn=0x14,
  voice1_adsr1=0x15,
  voice1_adsr2=0x16,
  voice1_gain=0x17,
  voice1_envx=0x18,
  voice1_outx=0x19,

  voice2_vol_l=0x20,
  voice2_vol_r=0x21,
  voice2_pitch_lo=0x22,
  voice2_pitch_hi=0x23,
  voice2_srcn=0x24,
  voice2_adsr1=0x25,
  voice2_adsr2=0x26,
  voice2_gain=0x27,
  voice2_envx=0x28,
  voice2_outx=0x29,

  voice3_vol_l=0x30,
  voice3_vol_r=0x31,
  voice3_pitch_lo=0x32,
  voice3_pitch_hi=0x33,
  voice3_srcn=0x34,
  voice3_adsr1=0x35,
  voice3_adsr2=0x36,
  voice3_gain=0x37,
  voice3_envx=0x38,
  voice3_outx=0x39,

  voice4_vol_l=0x40,
  voice4_vol_r=0x41,
  voice4_pitch_lo=0x42,
  voice4_pitch_hi=0x43,
  voice4_srcn=0x44,
  voice4_adsr1=0x45,
  voice4_adsr2=0x46,
  voice4_gain=0x47,
  voice4_envx=0x48,
  voice4_outx=0x49,

  voice5_vol_l=0x50,
  voice5_vol_r=0x51,
  voice5_pitch_lo=0x52,
  voice5_pitch_hi=0x53,
  voice5_srcn=0x54,
  voice5_adsr1=0x55,
  voice5_adsr2=0x56,
  voice5_gain=0x57,
  voice5_envx=0x58,
  voice5_outx=0x59,

  voice6_vol_l=0x60,
  voice6_vol_r=0x61,
  voice6_pitch_lo=0x62,
  voice6_pitch_hi=0x63,
  voice6_srcn=0x64,
  voice6_adsr1=0x65,
  voice6_adsr2=0x66,
  voice6_gain=0x67,
  voice6_envx=0x68,
  voice6_outx=0x69,

  voice7_vol_l=0x70,
  voice7_vol_r=0x71,
  voice7_pitch_lo=0x72,
  voice7_pitch_hi=0x73,
  voice7_srcn=0x74,
  voice7_adsr1=0x75,
  voice7_adsr2=0x76,
  voice7_gain=0x77,
  voice7_envx=0x78,
  voice7_outx=0x79,

  mvol_l=0x0c,
  mvol_r=0x1c,
  evol_l=0x2c,
  evol_r=0x3c,
  kon=0x4c,
  koff=0x5c,
  flg=0x6c,
  endx=0x7c,

  efb=0x0d,
  //
  pmon=0x2d,
  non=0x3d,
  eon=0x4d,
  dir=0x5d,
  esa=0x6d,
  edl=0x7d,

  c0=0x0f,
  c1=0x1f,
  c2=0x2f,
  c3=0x3f,
  c4=0x4f,
  c5=0x5f,
  c6=0x6f,
  c7=0x7f
};

  struct reg_lut_t {
  int addr;
  const char *name;
  //char *altname;
};

// this lut is for automatic iteration in loops.. 
// like, im totally going to use it for the debugger page that details all
// DSP registers
extern reg_lut_t lut[];

}


// api needs a pointer to a spc_emu * wooooo
class Spc_Dsp_Register_Map_Interface  // name prone to change!! i fucking hate naming classes!
{
public:
  
  Spc_Dsp_Register_Map_Interface() { p = NULL; }
  Spc_Dsp_Register_Map_Interface(Snes_Spc *sp) : p(sp) {}
  void init(Snes_Spc *sp)
  { 
    if (p == NULL) 
      p = sp; 
  }
  
  uint8_t dsp_read(const int a)
  {
    //p->write(0xf2, a, 1);
    return p->get_dsp()->read(a);
  }
  uint8_t dsp_read2(const int a)
  {
    p->write(0xf2, a, 1);
    return p->read(0xf3, 1);
  }
  // should i make this inline?
  void dsp_write(const int a, const int v)  // a addr, v value
  {
    p->get_dsp()->write(a, v);
    //p->write(0xf3, v, 1);
  }
  void dsp_write2(const int a, const int v)  // a addr, v value
  {
    p->write(0xf2, a, 1);
    p->write(0xf3, v, 1);
  }

  // voice 0
  void write_voice0_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0, v);
  }
  void write_voice0_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(1, v);
  }
  void write_voice0_pitch_lo(int v)
  {
    dsp_write(2, v);
  }
  void write_voice0_pitch_hi(int v)
  {
    dsp_write(3, v);
  }
  void write_voice0_srcn(int v)
  {
    dsp_write(4, v);
  }
  void write_voice0_adsr1(int v)
  {
    dsp_write(5, v);
  }
  void write_voice0_adsr2(int v)
  {
    dsp_write(6, v);
  }
  void write_voice0_gain(int v)
  {
    dsp_write(7, v);
  }
  // i think this is output only
  /*
  void write_voice0_envx(int v)
  {
    dsp_write(8, v);
  }
  */
  // i think this is output only
  /*
  void write_voice0_outx(int v)
  {
    dsp_write(9, v);
  }
  */

  // voice 1
  void write_voice1_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x10, v);
  }
  void write_voice1_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x11, v);
  }
  void write_voice1_pitch_lo(int v)
  {
    dsp_write(0x12, v);
  }
  void write_voice1_pitch_hi(int v)
  {
    dsp_write(0x13, v);
  }
  void write_voice1_srcn(int v)
  {
    dsp_write(0x14, v);
  }
  void write_voice1_adsr1(int v)
  {
    dsp_write(0x15, v);
  }
  void write_voice1_adsr2(int v)
  {
    dsp_write(0x16, v);
  }
  void write_voice1_gain(int v)
  {
    dsp_write(0x17, v);
  }
  // i think this is output only
  /*
  void write_voice1_envx(int v)
  {
    dsp_write(0x18, v);
  }
  */
  // i think this is output only
  /*
  void write_voice1_outx(int v)
  {
    dsp_write(0x19, v);
  }
  */

  // voice2
  void write_voice2_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x20, v);
  }
  void write_voice2_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x21, v);
  }
  void write_voice2_pitch_lo(int v)
  {
    dsp_write(0x22, v);
  }
  void write_voice2_pitch_hi(int v)
  {
    dsp_write(0x23, v);
  }
  void write_voice2_srcn(int v)
  {
    dsp_write(0x24, v);
  }
  void write_voice2_adsr1(int v)
  {
    dsp_write(0x25, v);
  }
  void write_voice2_adsr2(int v)
  {
    dsp_write(0x26, v);
  }
  void write_voice2_gain(int v)
  {
    dsp_write(0x27, v);
  }
  // i think this is output only
  /*
  void write_voice2_envx(int v)
  {
    dsp_write(0x28, v);
  }
  */
  // i think this is output only
  /*
  void write_voice2_outx(int v)
  {
    dsp_write(0x29, v);
  }
  */

  // voice3
  void write_voice3_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x30, v);
  }
  void write_voice3_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x31, v);
  }
  void write_voice3_pitch_lo(int v)
  {
    dsp_write(0x32, v);
  }
  void write_voice3_pitch_hi(int v)
  {
    dsp_write(0x33, v);
  }
  void write_voice3_srcn(int v)
  {
    dsp_write(0x34, v);
  }
  void write_voice3_adsr1(int v)
  {
    dsp_write(0x35, v);
  }
  void write_voice3_adsr2(int v)
  {
    dsp_write(0x36, v);
  }
  void write_voice3_gain(int v)
  {
    dsp_write(0x37, v);
  }
  // i think this is output only
  /*
  void write_voice3_envx(int v)
  {
    dsp_write(0x38, v);
  }
  */
  // i think this is output only
  /*
  void write_voice3_outx(int v)
  {
    dsp_write(0x39, v);
  }
  */

  // voice4
  void write_voice4_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x40, v);
  }
  void write_voice4_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x41, v);
  }
  void write_voice4_pitch_lo(int v)
  {
    dsp_write(0x42, v);
  }
  void write_voice4_pitch_hi(int v)
  {
    dsp_write(0x43, v);
  }
  void write_voice4_srcn(int v)
  {
    dsp_write(0x44, v);
  }
  void write_voice4_adsr1(int v)
  {
    dsp_write(0x45, v);
  }
  void write_voice4_adsr2(int v)
  {
    dsp_write(0x46, v);
  }
  void write_voice4_gain(int v)
  {
    dsp_write(0x47, v);
  }
  // i think this is output only
  /*
  void write_voice4_envx(int v)
  {
    dsp_write(0x48, v);
  }
  */
  // i think this is output only
  /*
  void write_voice4_outx(int v)
  {
    dsp_write(0x49, v);
  }
  */

  // voice5
  void write_voice5_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x50, v);
  }
  void write_voice5_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x51, v);
  }
  void write_voice5_pitch_lo(int v)
  {
    dsp_write(0x52, v);
  }
  void write_voice5_pitch_hi(int v)
  {
    dsp_write(0x53, v);
  }
  void write_voice5_srcn(int v)
  {
    dsp_write(0x54, v);
  }
  void write_voice5_adsr1(int v)
  {
    dsp_write(0x55, v);
  }
  void write_voice5_adsr2(int v)
  {
    dsp_write(0x56, v);
  }
  void write_voice5_gain(int v)
  {
    dsp_write(0x57, v);
  }
  // i think this is output only
  /*
  void write_voice5_envx(int v)
  {
    dsp_write(0x58, v);
  }
  */
  // i think this is output only
  /*
  void write_voice5_outx(int v)
  {
    dsp_write(0x59, v);
  }
  */

  // voice6
  void write_voice6_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x60, v);
  }
  void write_voice6_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x61, v);
  }
  void write_voice6_pitch_lo(int v)
  {
    dsp_write(0x62, v);
  }
  void write_voice6_pitch_hi(int v)
  {
    dsp_write(0x63, v);
  }
  void write_voice6_srcn(int v)
  {
    dsp_write(0x64, v);
  }
  void write_voice6_adsr1(int v)
  {
    dsp_write(0x65, v);
  }
  void write_voice6_adsr2(int v)
  {
    dsp_write(0x66, v);
  }
  void write_voice6_gain(int v)
  {
    dsp_write(0x67, v);
  }
  // i think this is output only
  /*
  void write_voice6_envx(int v)
  {
    dsp_write(0x68, v);
  }
  */
  // i think this is output only
  /*
  void write_voice6_outx(int v)
  {
    dsp_write(0x69, v);
  }
  */

  // voice7
  void write_voice7_vol_l(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x70, v);
  }
  void write_voice7_vol_r(int v)
  {
    // will need rules to cap the values! see snesdevmanual.pdf
    // can i pass immediate into &a ??? no!
    dsp_write(0x71, v);
  }
  void write_voice7_pitch_lo(int v)
  {
    dsp_write(0x72, v);
  }
  void write_voice7_pitch_hi(int v)
  {
    dsp_write(0x73, v);
  }
  void write_voice7_srcn(int v)
  {
    dsp_write(0x74, v);
  }
  void write_voice7_adsr1(int v)
  {
    dsp_write(0x75, v);
  }
  void write_voice7_adsr2(int v)
  {
    dsp_write(0x76, v);
  }
  void write_voice7_gain(int v)
  {
    dsp_write(0x77, v);
  }
  // i think this is output only
  /*
  void write_voice7_envx(int v)
  {
    dsp_write(0x78, v);
  }
  */
  // i think this is output only
  /*
  void write_voice7_outx(int v)
  {
    dsp_write(0x79, v);
  }
  */

  void write_main_vol_l(int v)
  {
    dsp_write(0x0c, v);
  }
  void write_main_vol_r(int v)
  {
    dsp_write(0x1c, v);
  }
  //
  void write_echo_vol_l(int v)
  {
    dsp_write(0x2c, v);
  }
  void write_echo_vol_r(int v)
  {
    dsp_write(0x3c, v);
  }
  //
  void write_keyon(int v)  // d0-d7 correspond to voice0-voice7
  {
    dsp_write(0x4c, v);
  }
  void write_keyoff(int v)
  {
    dsp_write(0x5c, v);
  }

  //  designated on/off of reset, mute, echo, and noise clock
  void write_flg(int v)
  {
    dsp_write(0x6c, v);
  }

  //  output only?? indicates source end block
  /*
  void writeendx(int v)
  {
    dsp_write(0x7c, v);
  }
  */

  // echo feedback
  void write_echo_feedback(int v) { dsp_write(0x0d, v); }
  // 0x1d not used

  // 0x2d pmon -- pitch modulation of voice i with outx of voice (i-1)
  void write_pitch_mod_on(int v) { dsp_write(0x2d, v); }

  // 0x3d non -- noise on/off, d0-d7 corresponds to voice0-voice7
  void write_noise_on(int v) { dsp_write(0x3d, v); }

  // 0x4d eon -- echo on/off
  void write_echo_on(int v) { dsp_write(0x4d, v); }

  // 0x5d dir -- off-set address of source directory
  void write_dir(int v) { dsp_write(0x5d, v); }

  // 0x6d -- esa off-set address of echo-region, echo start address
  // this needs ram memory -> dsp reg value translation
  void write_echo_startaddr(int v) { dsp_write(0x6d, v); }

  // 0x7d -- edl echo delay, only lower 4 bits active
  void write_echo_delay(int v) { dsp_write(0x7d, v); }

  void write_filter_coefficient0(int v) { dsp_write(0x0f, v); }
  void write_filter_coefficient1(int v) { dsp_write(0x1f, v); }
  void write_filter_coefficient2(int v) { dsp_write(0x2f, v); }
  void write_filter_coefficient3(int v) { dsp_write(0x3f, v); }
  void write_filter_coefficient4(int v) { dsp_write(0x4f, v); }
  void write_filter_coefficient5(int v) { dsp_write(0x5f, v); }
  void write_filter_coefficient6(int v) { dsp_write(0x6f, v); }
  void write_filter_coefficient7(int v) { dsp_write(0x7f, v); }

  void write_filter_coefficients(int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7)
  {
    write_filter_coefficient0(c0);
    write_filter_coefficient1(c1);
    write_filter_coefficient2(c2);
    write_filter_coefficient3(c3);
    write_filter_coefficient4(c4);
    write_filter_coefficient5(c5);
    write_filter_coefficient6(c6);
    write_filter_coefficient7(c7);
  }

  // reference from snes_spc.h
/*int  read( int );
  void write( int, int );*/
private:
  //spc_emu *p;
  Snes_Spc *p;
};



