#include "MouseOver_HexDump_Area.h"
int MouseOver_HexDump_Area::MOUSE_HEXDUMP_START_Y;


MouseOver_HexDump_Area::MouseOver_HexDump_Area(/*Main_Memory_Area *main_mem_area,*/ 
Music_Player *player, SDL_Surface *screen/*, Uint16 &addr_being_edited*/) :
Memory(player,screen)
{
  
}


void MouseOver_HexDump_Area::update_editing_address()
{
  report::restore_color(addr_being_edited);
  addr_being_edited = address+(res_y*8)+res_x;
  if ( (addr_being_edited == 0xf3 && (IAPURAM[0xf2] == 0x4c || IAPURAM[0xf2] == 0x5c) ) || addr_being_edited==0xf1 || addr_being_edited == 0xf0)
  {
    // only update the buffer the first time.. if we haven't started writing in a new value
    if (!draw_tmp_ram)
    {
      if (addr_being_edited == 0xf3)
        tmp_ram = player->spc_read(0xf2);
      else tmp_ram = player->spc_read(addr_being_edited);
    }
  }
}
void MouseOver_HexDump_Area::inc_cursor_row()
{
  cursor.toggle=1;
  
  if (res_y == 15)
  {
    add_addr(+8); //address += 8;
  }
  else res_y++;    
  update_editing_address();
}
void MouseOver_HexDump_Area::dec_cursor_row()
{
  cursor.toggle=1;
  
  if (res_y == 0)
  {
    add_addr(-8);
  }
  else res_y--;    
  update_editing_address();
}
void MouseOver_HexDump_Area::inc_cursor_pos()
{
  cursor.toggle=1;
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
        add_addr(8);
      }
      else res_y++;


    }
    else
      res_x++;

    highnibble = 1;
  }
  update_editing_address();
}
void MouseOver_HexDump_Area::dec_cursor_pos()
{
  cursor.toggle=1;
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
        add_addr(-8);
      }
      else res_y--;
    }
    else
      res_x--;

    highnibble = 0;
  }
  
  update_editing_address();
}

void MouseOver_HexDump_Area::draw_cursor(SDL_Surface *screen, Uint32 color)
{
  if (highnibble)
  {
    cursor.draw(screen, MOUSE_HEXDUMP_START_X + (res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT), MOUSE_HEXDUMP_START_Y + (res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), color);
  }
  else
  {
    cursor.draw(screen, MOUSE_HEXDUMP_START_X + (res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT + (7)), MOUSE_HEXDUMP_START_Y + (res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), color);
  }
}


