#pragma once

struct Debugger_Base 
{
  enum modes 
  { 
    MODE_NAV=0,
    MODE_EDIT_MOUSE_HEXDUMP,
    MODE_EDIT_APU_PORT,
    MODE_DSP_MAP  
  };
  static int mode;// =0;
  static int submode;// =  0;
};