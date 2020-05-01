#ifndef _report_h__
#define _report_h__

#include "SDL.h"
#include "Screen.h"
#include "gme_m/Spc_Report.h"
#include "gme_m/Spc_Dsp_Register_Map_Interface.h"

/* 
I cannot encapsulate this because the DSP needs to be able to access it
*/

struct Mem_Surface
{
public:
  static SDL_Rect memrect;

  Mem_Surface();
  ~Mem_Surface();
  void draw(SDL_Surface *screen);
  void init();
  void clear();
  void init_video();
  void fade_arrays();
  
  unsigned char *data, *cdata;
  SDL_Surface *sdl_surface, *sdl_csurface;
};

class Sdl_Spc_Report : public Spc_Report
{
  void report(Type type, unsigned addr, unsigned opcode);
};

void report_cursor(int addr);


// Memsurface and the other entities are coupled together OK!! 
// ie. memsurface.clear() will also clear BRR_Headers SRCN LOOP_used arrays..
// it doesn't make sense to call one but not the other!! 
namespace report
{
  static const int BRR_HEADER_MAX=100, SRCN_MAX=0x200;
  extern int last_pc;
  extern int bcolor; // backup color
  extern Mem_Surface memsurface;
  extern unsigned char used2[0x101];
  extern unsigned char used[0x10006];
  extern Sdl_Spc_Report sdlSpcReport;

  int backup_color(int addr);

  void restore_color(int addr);
}
#endif




