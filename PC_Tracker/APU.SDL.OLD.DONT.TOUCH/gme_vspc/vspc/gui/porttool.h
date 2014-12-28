#pragma once
#include "gui/cursor.h"
//#include "gme/player/Music_Player.h"
#define PORTTOOL_X    540
#define PORTTOOL_Y    380

#define PORTTOOL_PORT0_X PORTTOOL_X + (8*7)
#define PORTTOOL_PORT3_X_RIGHTMOST_LOBYTE PORTTOOL_X + (8*23)
// 8 = tile width

//extern struct SIAPU IAPU;
extern Music_Player* player;
namespace porttool
{
  Uint8 highnibble, portnum, portaddress, tmp[4];
  Uint8 horizontal=0;
  int x,y;

  void draw_cursor(SDL_Surface *screen, Uint32 color)
  {
    //fprintf(stderr, "myx = %d", x);
    if (highnibble)
      cursor::draw(screen, x, PORTTOOL_Y+8, color);
    else
      cursor::draw(screen, x, PORTTOOL_Y+8, color);
  }

  void write()
  {
    for (int i=0; i < 4; i++)
    {
      player->spc_write(0xf4 + i, tmp[i]);
      //IAPU.RAM[0xf4 + i] = tmp[i];
    }
  }

  void reset_port()
  {
    tmp[portnum] = player->spc_read(0xf4 + portnum);
  }
  void reset_port(Uint8 num)
  {
    tmp[num] = player->spc_read(0xf4 + num);
  }

  void switch_port(Uint8 num)
  {
    // don't read from RAM like in set_port()
    // 
    porttool::portaddress = 0xf4 + num;
    portnum = num;
  }
  void set_port(Uint8 num)
  {
    porttool::portaddress = 0xf4 + num;
    portnum = num;
    //tmp[portnum] = IAPU.RAM[0xf4 + portnum];
    for (int i=0; i < 4; i++)
      tmp[i] = player->spc_read(0xf4 + i);
  }

  void inc_cursor_pos()
  {
    if (highnibble)
    {
      highnibble = 0;
      x += 8;
    }
    else
    {
      highnibble = 1;
      if (portnum == 3)
      {
        switch_port(0);
        x = PORTTOOL_PORT0_X;
      }
      else
      {
        switch_port(++portnum);
        x += (8*4);
      }
    }
  }

  void dec_cursor_pos()
  {
    if (!highnibble)
    {
      highnibble = 1;
      x -= 8;
    }
    else
    {
      highnibble = 0;
      if (portnum == 0)
      {
        switch_port(3);
        x = PORTTOOL_PORT3_X_RIGHTMOST_LOBYTE;
      }
      else
      {
        switch_port(--portnum);
        x -= 8*4;
      }
    }
  }
}