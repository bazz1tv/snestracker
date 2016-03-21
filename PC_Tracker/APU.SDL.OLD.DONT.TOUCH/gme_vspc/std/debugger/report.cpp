#include "report.h"

void Sdl_Spc_Report::report(Spc_Report::Type type, unsigned addr, unsigned opcode)
{
    switch (type)
    {
      int idx;
    case Spc_Report::Execute:
      //std::cerr << "report_execution: $" << addr << ", $" << opcode << std::endl;
      int i;
      if ((addr)!=report::last_pc)
      {
        for (i=0; i<5; i++)
        {
          idx = (((addr)+i)&0xff00)<<4; idx += (((addr)+i)%256)<<3;
          report::memsurface.data[idx]=0xff;
          report::memsurface.data[idx+2048]=0xff;
          report::memsurface.data[idx+4]=0xff;
          report::memsurface.data[idx+4+2048]=0xff;
          report::used2[((addr)&0xff00)>>8]=1;
        }
        report::used[(addr)]=1;
        report::used[(addr)+1]=1;
        report::used[(addr)+2]=1;
        report::used[(addr)+3]=1;
        report::used[(addr)+4]=1;
        report::used[(addr)+5]=1;
      }
      break;
    case Spc_Report::Read:
        //std::cerr << "report_memread: $" << addr << std::endl;
        idx = (((addr)&0xff00)<<4)+2;
        idx += ((addr)%256)<<3;
        report::memsurface.data[idx]=0xff;
        report::memsurface.data[idx+2048]=0xff;
        report::memsurface.data[idx+4]=0xff;
        report::memsurface.data[idx+4+2048]=0xff;
        report::used2[((addr)&0xff00)>>8]=1;
        report::used[(addr)&0xffff]=1;
        break;
    case Spc_Report::Echo:
        //std::cerr << "report_echomem: $" << addr << std::endl;
        idx = (((addr)&0xff00)<<4);
        idx+= ((addr)%256)<<3;
        for (int i=0; i < 2; i++)
        {
          report::memsurface.data[idx+i]=0xff;
          report::memsurface.data[idx+2048+i]=0xff;
          report::memsurface.data[idx+4+i]=0xff;
          report::memsurface.data[idx+4+2048+i]=0xff;
        }
        break;
    case Spc_Report::Write:
        //std::cerr << "report_memwrite: $" << addr << std::endl;
        idx = (((addr)&0xff00)<<4)+1;
        idx += ((addr) % 256)<<3;
        report::memsurface.data[idx] = 0xff;
        report::memsurface.data[idx+4] = 0xff;
        report::memsurface.data[idx+2048] = 0xff;
        report::memsurface.data[idx+2048+4] = 0xff;
        break;
   }
}


SDL_Rect Mem_Surface::memrect = {MEMORY_VIEW_X, MEMORY_VIEW_Y,0,0};

Mem_Surface::Mem_Surface()
{
  sdl_csurface = NULL;
  sdl_surface = NULL;
  data = NULL;
  cdata = NULL;
}

Mem_Surface::~Mem_Surface()
{
  if (sdl_surface)
    SDL_FreeSurface(sdl_surface);
  if (sdl_csurface)
    SDL_FreeSurface(sdl_csurface);
  if (data)
    free(data);
  if (cdata)
    free(cdata);
}

void Mem_Surface::draw(SDL_Surface *screen)
{
  // draw the memory read/write display area
 // SDL_Rect memrect;
  //memrect.x = MEMORY_VIEW_X; memrect.y = MEMORY_VIEW_Y;

  //fprintf(stderr, "%d %d %d %d, ", memrect.x, memrect.y, memrect.w, memrect.h);
  
  SDL_BlitSurface(sdl_surface, NULL, screen, &memrect);  
  SDL_BlitSurface(sdl_csurface, NULL, screen, &memrect);
}
void Mem_Surface::init()
{
  fprintf(stderr,"YIPEE\n");
  data = (unsigned char *)malloc(512*512*4);
  memset(data, 0, 512*512*4);
  cdata = (unsigned char *)malloc(512*512*4);
  memset(cdata, 0, 512*512*4);
  init_video();
}
void Mem_Surface::clear()
{
  memset(data, 0, 512*512*4);
  memset(cdata, 0, 512*512*4);
  for (int i=0; i < MAX_SRCN_ENTRIES; i++)
  {
    report::src[i].brr_start = 0xffff;  // init to ROM address, brr would never be there
    report::src[i].brr_loop_start = 0xffff;
    report::src[i].brr_end = 0xffff;
    report::src[i].brr_loop_end = 0xffff;
  }
  /*memset(used, 0, sizeof(used));
  memset(used2, 0, sizeof(used2));*/
}
void Mem_Surface::init_video()
{
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      sdl_surface = SDL_CreateRGBSurfaceFrom(data,512,512,32,2048,0xFF000000,0x00FF0000,0x0000FF00,0x0);
      sdl_csurface = SDL_CreateRGBSurfaceFrom(cdata,512,512,32,2048,0xFF000000,0x00FF0000,0x0000FF00,0x0);
  #else
      sdl_surface = SDL_CreateRGBSurfaceFrom(data,512,512,32,2048,0xFF,0xFF00,0xFF0000,0x0);    
      sdl_csurface = SDL_CreateRGBSurfaceFrom(cdata,512,512,32,2048,0xFF,0xFF00,0xFF0000,0x0);    
  #endif

  //SDL_SetAlpha(sdl_csurface, 0, 0);
  //SDL_SetAlpha(sdl_surface, 0, 0);
  //SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key);
  SDL_SetColorKey(sdl_csurface, SDL_TRUE, 0);
}
void Mem_Surface::fade_arrays()
{
  int i;
  for (i=0; i<512*512*4; i++)
  {
    if (data[i] > 0x40) { data[i]--; }
  }
}



void report_cursor(int addr)
{
  int idx = (((addr)&0xff00)<<4);
  idx+= ((addr)%256)<<3;
  report::memsurface.cdata[idx]=0x0;
  report::memsurface.cdata[idx+2048]=0x0;
  report::memsurface.cdata[idx+4]=0x00;
  report::memsurface.cdata[idx+4+2048]=0x00;
  report::memsurface.cdata[idx+1]=0xff;
  report::memsurface.cdata[idx+2048+1]=0xff;
  report::memsurface.cdata[idx+4+1]=0xff;
  report::memsurface.cdata[idx+4+2048+1]=0xff;
  report::memsurface.cdata[idx+2]=0x0;
  report::memsurface.cdata[idx+2048+2]=0x0;
  report::memsurface.cdata[idx+4+2]=0x0;
  report::memsurface.cdata[idx+4+2048+2]=0x0;
 
}

namespace report
{
  //const int BRR_HEADER_MAX, SRCN_MAX;
  unsigned char used2[0x101];
  unsigned char used[0x10006];
  Mem_Surface memsurface;
  int last_pc = -1;
  int bcolor=0; // backup color
  Src src[MAX_SRCN_ENTRIES];
  Sdl_Spc_Report sdlSpcReport;

  // backup colors is no longer an issue
  int backup_color(int addr)
  {
    /*int r,g,b;
    int idx = (((addr)&0xff00)<<4); idx+= ((addr)%256)<<3; 
    r=report::memsurface.data[idx];
    g=report::memsurface.data[idx+1];
    b=report::memsurface.data[idx+2];
    bcolor = (r << 16) | (g << 8) | b;
    //fprintf(stderr, "bcol = %03x\n", bcolor);
    return bcolor;*/
    return 0;
  }


void restore_color(int addr)
{
  int idx = (((addr)&0xff00)<<4); 
  idx+= ((addr)%256)<<3;
  /*
  int c[3];
  c[0] = (bcolor >> 16);
  c[1] = (bcolor >> 8) & 0xff;
  c[2] = bcolor & 0xff;*/

  //fprintf(stderr, "r = %d, g = %d, b = %d\n", c[0],c[1],c[2]);
  //SDL_GetRGB(bcolor, sdl_surface->format, &c[0], &c[1], &c[2]); 
  //Uint8 c = r1
  for (int i=0; i < 3; i++) 
  { 
    report::memsurface.cdata[idx+i]=0; 
    report::memsurface.cdata[idx+2048+i]=0; 
    report::memsurface.cdata[idx+4+i]=0; 
    report::memsurface.cdata[idx+4+2048+i]=0; 
  }
}
}