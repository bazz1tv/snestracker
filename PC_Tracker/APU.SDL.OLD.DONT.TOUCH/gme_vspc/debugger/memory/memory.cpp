#include "memory.h"

Cursor Memory::cursor;

// Cannot decide if this belongs in BaseD
//Music_Player *Memory::player=NULL;
//SDL_Surface *Memory::screen=NULL;
//uint8_t *Memory::IAPURAM=NULL;

Uint16 Memory::address=0; //=0x0000;
Uint16 Memory::addr_being_edited=0;
int Memory::rel_x=0;
int Memory::rel_y=0;

int Memory::res_x=0;
int Memory::res_y=0;

Memory::Memory()
{
  //player = cplayer;
  //screen = cscreen;
  address = 0;
  addr_being_edited = 0;
}

void Memory::set_addr(int i)
{
  address = i;
}
void Memory::add_addr(int i)
{
  set_addr(address+i);
}