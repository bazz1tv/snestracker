#pragma once

//#include "Spc_Emu.h"
#include "Snes_Spc.h"

//spc_addr_t dsp


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
  
  // should i make this inline?
  void dsp_write(const spc_addr_t a, const int v)  // a addr, v value
  {
    p->write(0xf2, a);
    p->write(0xf3, v);
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
/*int  read( spc_addr_t );
  void write( spc_addr_t, int );*/
private:
  //spc_emu *p;
  Snes_Spc *p;
};



