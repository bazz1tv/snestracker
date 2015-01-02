#include "gui/porttool.h"

//#pragma once
//#include "gui/cursor.h"

//extern struct SIAPU IAPU;
//extern Music_Player* player;
namespace porttool
{
  Uint8 highnibble, portnum, portaddress, tmp[4];
  Uint8 horizontal=0;
  int x,y;
  Uint8 portdata[4] = {0,0,0,0};

  void inc_port(int port)
  {
    player->spc_emu()->write_port(port, ++portdata[port]);
    tmp[port] = portdata[port];
  }
  void dec_port(int port)
  {
    player->spc_emu()->write_port(port, --portdata[port]);
    tmp[port] = portdata[port];
  }

  void draw_cursor(SDL_Surface *screen, Uint32 color)
  {
    //fprintf(stderr, "myx = %d", x);
    if (highnibble)
      cursor::draw(screen, x, PORTTOOL_Y+8, color);
    else
      cursor::draw(screen, x, PORTTOOL_Y+8, color);
  }

  void write(int i)
  {
    player->spc_emu()->write_port(i, tmp[i]);
    portdata[i] = tmp[i];
  }
  void write()
  {
    int i = portnum;
    player->spc_emu()->write_port(i, tmp[i]);
    portdata[i] = tmp[i];
  }
  void writeall()
  {
    for (int i=0; i < 4; i++)
    {
      //player->spc_write(0xf4 + i, tmp[i]);
      player->spc_emu()->write_port(i, tmp[i]);
      portdata[i] = tmp[i];
      //IAPU.RAM[0xf4 + i] = tmp[i];
    }
  }

  void reset_port()
  {
    tmp[portnum] = portdata[portnum];
  }
  void reset_port(Uint8 num)
  {
    tmp[num] = portdata[num];
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
      tmp[i] = portdata[i]; //player->spc_emu()->write_port(i);
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