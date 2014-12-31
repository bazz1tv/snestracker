#pragma once

#include "SDL.h"
#include "sdl_userevents.h"
#include "gui/cursor.h"
#include "mode.h"
#include "report.h"

#define MEMORY_VIEW_X 16
#define MEMORY_VIEW_Y 40

#define MOUSE_HEXDUMP_START_X 584
//#define MOUSE_HEXDUMP_START_Y 229 + 30
#define MOUSE_HEXDUMP_END_X 740
#define MOUSE_HEXDUMP_END_Y 364 + 9 + 30
#define MOUSE_HEXDUMP_ENTRY_X_INCREMENT 20 // but i think i like 16 programmatically
#define MOUSE_HEXDUMP_ENTRY_Y_INCREMENT 9  // dunno what i like programmatically yet



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



int MOUSE_HEXDUMP_START_Y;
//extern Uint16 mouse_hexdump::address;
//extern struct SIAPU IAPU;
namespace mouse_hexdump
{
  enum submodes { HARD_EDIT=0, EASY_EDIT=1 };
  int tmp_ram; char draw_tmp_ram;
  int rel_x; // = te->motion.x - MOUSE_HEXDUMP_START_X;
  //rel_x+=2;
  int rel_y; // = te->motion.y - MOUSE_HEXDUMP_START_Y;

  int res_x;
  int res_y;
  Uint8 highnibble;
  Uint8 horizontal=1;

  char locked=0;
  Uint16 address=0x0000;

  void set_addr(int i)
  {
    if (memcursor::toggle & memcursor::TOGGLE_ACTIVE)
    {
      report::restore_color(address);
      memcursor::start_timer();
    }
    address = i;
    if (memcursor::toggle & memcursor::TOGGLE_ACTIVE)
      report::backup_color(address);
  }
  void set_addr(int x, int y)
  {
    x-= MEMORY_VIEW_X;
    y -= MEMORY_VIEW_Y;
    x /= 2;
    y /= 2;
    set_addr(y*256+x);
  }
  void add_addr(int i)
  {
    set_addr(address+i);
  }

  void dec_cursor_row();
  void inc_cursor_row();

  void lock(char l=1)
  {
    locked = l;
    if (locked)
    {
      
      int derp = (mouse_hexdump::address % 8);
                    //if (derp >= 4) mouse_hexdump::address += (8-derp);
                    /*else*/ add_addr(-derp);
      memcursor::start_timer();
    }
    else
    {
      report::restore_color(mouse_hexdump::address);
      memcursor::stop_timer();
    }
    
  }
  void toggle_lock()
  {
    lock(!locked);
  }
  
  void unlock()
  {
    lock(0);
  }

  void inc_cursor_row()
  {
    cursor::toggle=1;
    
    if (res_y == 15)
    {
      //res_y;
      add_addr(+8); //mouse_hexdump::address += 8;
    }
    else res_y++;    
  }
  void dec_cursor_row()
  {
    cursor::toggle=1;
    
    if (res_y == 0)
    {
      //res_y;
      add_addr(-8);
    }
    else res_y--;    
  }
  void inc_cursor_pos()
  {
    cursor::toggle=1;
    if (highnibble)
    {
      highnibble = 0;
    }
    else
    {
      if (res_x == 7)
      {
        res_x = 0;
        if (res_y == 15)
        {
          //res_y;
          add_addr(8);
        }
        else res_y++;


      }
      else
        res_x++;

      highnibble = 1;
    }
  }
  void dec_cursor_pos()
  {
    cursor::toggle=1;
    if (!highnibble)
    {
      highnibble = 1;
    }
    else
    {
      if (res_x == 0)
      {
        res_x = 7;
        if (res_y == 0)
        {
          //res_y;
          add_addr(-8);
        }
        else res_y--;
      }
      else
        res_x--;

      highnibble = 0;
    }
  }

  void draw_cursor(SDL_Surface *screen, Uint32 color)
  {
    if (mouse_hexdump::highnibble)
    {
      cursor::draw(screen, MOUSE_HEXDUMP_START_X + (mouse_hexdump::res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT), MOUSE_HEXDUMP_START_Y + (mouse_hexdump::res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), color);
    }
    else
    {
      cursor::draw(screen, MOUSE_HEXDUMP_START_X + (mouse_hexdump::res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT + (7)), MOUSE_HEXDUMP_START_Y + (mouse_hexdump::res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), color);
    }
  }

//unused
  void enter_edit_mode()
  {
    mode = MODE_EDIT_MOUSE_HEXDUMP;
  }
  void exit_edit_mode()
  {

  }

}

