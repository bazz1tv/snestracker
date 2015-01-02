#pragma once

enum modes { MODE_NAV=0,
  MODE_EDIT_MOUSE_HEXDUMP,
  MODE_EDIT_APU_PORT,
  MODE_DSP_MAP  

};
/*#define MODE_NAV 0
#define MODE_EDIT_MOUSE_HEXDUMP 1
#define MODE_EDIT_APU_PORT 2
*/
extern int mode;
extern int submode;

