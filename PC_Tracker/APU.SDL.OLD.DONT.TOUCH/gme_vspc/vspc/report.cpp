#include "report.h"

const SDL_Rect Mem_Surface::memrect = {MEMORY_VIEW_X, MEMORY_VIEW_Y,0,0};

Mem_Surface::Mem_Surface()
{
  memsurface = NULL;
  memsurface_data = NULL;
}

Mem_Surface::~Mem_Surface()
{
  SDL_FreeSurface(memsurface);
  free(memsurface_data);
}

void Mem_Surface::draw(SDL_Surface *screen)
{
  // draw the memory read/write display area
  SDL_BlitSurface(memsurface, NULL, screen, (SDL_Rect*)&memrect);  
}
void Mem_Surface::init()
{
  memsurface_data = (unsigned char *)malloc(512*512*4);
  memset(memsurface_data, 0, 512*512*4);
}
void Mem_Surface::clear()
{
  memset(memsurface_data, 0, 512*512*4);
  /*memset(used, 0, sizeof(used));
  memset(used2, 0, sizeof(used2));*/
}
void Mem_Surface::init_video()
{
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      memsurface = SDL_CreateRGBSurfaceFrom(memsurface_data,512,512,32,2048,0xFF000000,0x00FF0000,0x0000FF00,0x0);
  #else
      memsurface = SDL_CreateRGBSurfaceFrom(memsurface_data,512,512,32,2048,0xFF,0xFF00,0xFF0000,0x0);    
  #endif
}
void Mem_Surface::fade_arrays()
{
  int i;
  for (i=0; i<512*512*4; i++)
  {
    if (memsurface_data[i] > 0x40) { memsurface_data[i]--; }
  }
}

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