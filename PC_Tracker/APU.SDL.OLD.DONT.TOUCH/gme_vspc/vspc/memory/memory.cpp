#include "memory.h"

Cursor Memory::cursor;

Music_Player *Memory::player;
SDL_Surface *Memory::screen;
uint8_t *Memory::IAPURAM;
Uint16 Memory::address; //=0x0000;
Uint16 Memory::addr_being_edited;
int Memory::rel_x;
int Memory::rel_y;

int Memory::res_x;
int Memory::res_y;

Memory::Memory(Music_Player *cplayer, SDL_Surface *cscreen)
{
  player = cplayer;
  screen = cscreen;
  IAPURAM = player->spc_emu()->ram();
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