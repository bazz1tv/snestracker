#ifndef _report_h__
#define _report_h__

#define MEMORY_VIEW_X 16
#define MEMORY_VIEW_Y 40

#include "SDL.h"

#ifdef __cplusplus
extern "C" unsigned char *memsurface_data;
extern "C" unsigned char used2[0x101];
extern "C" unsigned char used[0x10006];
extern "C" int last_pc;
#else
extern unsigned char *memsurface_data;
extern unsigned char used2[0x101];
extern unsigned char used[0x10006];
extern int last_pc;
#endif

#if 0
#define report_memread(addr) do { int idx = (((addr)&0xff00)<<4)+2; idx+= ((addr)%256)<<3; memsurface_data[idx]=0xff; memsurface_data[idx+2048]=0xff; memsurface_data[idx+4]=0xff; memsurface_data[idx+4+2048]=0xff; used2[((addr)&0xff00)>>8]=1; } while(0)

#define report_memread2(addr, opcode) do { int i, idx; if ((addr)==last_pc) { break; } else { for (i=0; i<5; i++) { idx = (((addr)+i)&0xff00)<<4; idx+= (((addr)+i)%256)<<3; memsurface_data[idx]=0xff; memsurface_data[idx+2048]=0xff; memsurface_data[idx+4]=0xff; memsurface_data[idx+4+2048]=0xff; used2[((addr)&0xff00)>>8]=1;  } } } while(0)

#define report_memwrite(addr) do { int idx = (((addr)&0xff00)<<4)+1; idx += ((addr) % 256)<<3; memsurface_data[idx]=0xff; memsurface_data[idx+4] = 0xff; memsurface_data[idx+2048]=0xff; memsurface_data[idx+2048+4] = 0xff; } while(0)
#endif

struct Mem_Surface
{
  static const SDL_Rect memrect;

  Mem_Surface();
  ~Mem_Surface();
  void draw(SDL_Surface *screen);
  void init();
  void clear();
  void init_video();
  void fade_arrays();
  SDL_Surface *memsurface;
  unsigned char *memsurface_data;
};




#define report_off(addr) do { int idx = (((addr)&0xff00)<<4); idx+= ((addr)%256)<<3; for (int i=0; i < 3; i++) { memsurface_data[idx+i]=0x00; memsurface_data[idx+2048+i]=0x00; memsurface_data[idx+4+i]=0x0; memsurface_data[idx+4+2048+i]=0x0; } } while(0)


#define report_memread(addr) do { int idx = (((addr)&0xff00)<<4)+2; idx+= ((addr)%256)<<3; memsurface_data[idx]=0xff; memsurface_data[idx+2048]=0xff; memsurface_data[idx+4]=0xff; memsurface_data[idx+4+2048]=0xff; used2[((addr)&0xff00)>>8]=1; used[(addr)&0xffff]=1; } while(0)
#define report_echomem(addr) do { int idx = (((addr)&0xff00)<<4); idx+= ((addr)%256)<<3; for (int i=0; i < 2; i++) { memsurface_data[idx+i]=0xff; memsurface_data[idx+2048+i]=0xff; memsurface_data[idx+4+i]=0xff; memsurface_data[idx+4+2048+i]=0xff; } } while(0)
#define report_cursor(addr) do\
{\
  int idx = (((addr)&0xff00)<<4);\
  idx+= ((addr)%256)<<3;\
  memsurface_data[idx]=0x0;\
  memsurface_data[idx+2048]=0x0;\
  memsurface_data[idx+4]=0x00;\
  memsurface_data[idx+4+2048]=0x00;\
  memsurface_data[idx+1]=0xff;\
  memsurface_data[idx+2048+1]=0xff;\
  memsurface_data[idx+4+1]=0xff;\
  memsurface_data[idx+4+2048+1]=0xff;\
  memsurface_data[idx+2]=0x0;\
  memsurface_data[idx+2048+2]=0x0;\
  memsurface_data[idx+4+2]=0x0;\
  memsurface_data[idx+4+2048+2]=0x0;\
  } while(0)

#define report_memread2(addr, opcode) do { int i, idx; if ((addr)==last_pc) { break; } else { for (i=0; i<5; i++) { idx = (((addr)+i)&0xff00)<<4; idx+= (((addr)+i)%256)<<3; memsurface_data[idx]=0xff; memsurface_data[idx+2048]=0xff; memsurface_data[idx+4]=0xff; memsurface_data[idx+4+2048]=0xff; used2[((addr)&0xff00)>>8]=1;  } used[(addr)]=1; used[(addr)+1]=1; used[(addr)+2]=1; used[(addr)+3]=1; used[(addr)+4]=1; used[(addr)+5]=1; } } while(0)

#define report_memwrite(addr) do { int idx = (((addr)&0xff00)<<4)+1; idx += ((addr) % 256)<<3; memsurface_data[idx]=0xff; memsurface_data[idx+4] = 0xff; memsurface_data[idx+2048]=0xff; memsurface_data[idx+2048+4] = 0xff; } while(0)


/*#define report_memread(addr) 
#define report_echomem(addr) 
#define report_memread2(addr, opcode) 
#define report_memwrite(addr)*/


namespace report
{
  extern int bcolor; // backup color

  int backup_color(int addr);

  void restore_color(int addr);
}
#endif




