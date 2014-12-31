#include "report.h"

namespace report
{
  int bcolor=0; // backup color

  int backup_color(int addr)
  {
    int r,g,b;
    int idx = (((addr)&0xff00)<<4); idx+= ((addr)%256)<<3; 
    r=memsurface_data[idx];
    g=memsurface_data[idx+1];
    b=memsurface_data[idx+2];
    bcolor = (r << 16) | (g << 8) | b;
    //fprintf(stderr, "bcol = %03x\n", bcolor);
    return bcolor;
  }


void restore_color(int addr)
{
  int idx = (((addr)&0xff00)<<4); 
  idx+= ((addr)%256)<<3;
  int c[3];
  c[0] = (bcolor >> 16);
  c[1] = (bcolor >> 8) & 0xff;
  c[2] = bcolor & 0xff;

  //fprintf(stderr, "r = %d, g = %d, b = %d\n", c[0],c[1],c[2]);
  //SDL_GetRGB(bcolor, memsurface->format, &c[0], &c[1], &c[2]); 
  //Uint8 c = r1
  for (int i=0; i < 3; i++) 
  { 
    memsurface_data[idx+i]=c[i]; 
    memsurface_data[idx+2048+i]=c[i]; 
    memsurface_data[idx+4+i]=c[i]; 
    memsurface_data[idx+4+2048+i]=c[i]; 
  }
}
}