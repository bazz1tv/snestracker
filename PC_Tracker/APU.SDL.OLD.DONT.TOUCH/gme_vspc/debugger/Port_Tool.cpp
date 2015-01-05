#include "Port_Tool.h"

//#pragma once
//#include "gui/cursor.h"

//extern struct SIAPU IAPU;
//extern Music_Player* player;
Port_Tool::Port_Tool(Cursor *cursor) :
cursor(cursor)
{

}




void Port_Tool::inc_port(int port)
{
  player->spc_emu()->write_port(port, ++portdata[port]);
  tmp[port] = portdata[port];
}
void Port_Tool::dec_port(int port)
{
  player->spc_emu()->write_port(port, --portdata[port]);
  tmp[port] = portdata[port];
}

void Port_Tool::draw_cursor(SDL_Surface *screen, Uint32 color)
{
  if (highnibble)
    cursor->draw(screen, x, PORTTOOL_Y+8, color);
  else
    cursor->draw(screen, x, PORTTOOL_Y+8, color);
}

void Port_Tool::write(int i, uint8_t data)
{
  player->spc_emu()->write_port(i, data);
  portdata[i] = tmp[i] = data;
}
void Port_Tool::write(int i)
{
  player->spc_emu()->write_port(i, tmp[i]);
  portdata[i] = tmp[i];
}
void Port_Tool::write()
{
  int i = portnum;
  player->spc_emu()->write_port(i, tmp[i]);
  portdata[i] = tmp[i];
}
void Port_Tool::writeall()
{
  for (int i=0; i < 4; i++)
  {
    player->spc_emu()->write_port(i, tmp[i]);
    portdata[i] = tmp[i];
  }
}

void Port_Tool::reset_port()
{
  tmp[portnum] = portdata[portnum];
}
void Port_Tool::reset_port(Uint8 num)
{
  tmp[num] = portdata[num];
}

void Port_Tool::switch_port(Uint8 num)
{
  // don't read from RAM like in set_port()
  // 
  portaddress = 0xf4 + num;
  portnum = num;
}
void Port_Tool::set_port(Uint8 num)
{
  portaddress = 0xf4 + num;
  portnum = num;
  for (int i=0; i < 4; i++)
    tmp[i] = portdata[i]; 
}

void Port_Tool::inc_cursor_pos()
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

void Port_Tool::dec_cursor_pos()
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


