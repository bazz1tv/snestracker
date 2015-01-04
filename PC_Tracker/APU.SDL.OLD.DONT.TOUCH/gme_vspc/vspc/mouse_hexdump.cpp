#include "Main_Memory_Area.h"
#include "gme/player/Music_Player.h"


//extern uint8_t* IAPURAM;
//extern Music_Player *player;


MainMemoryArea::MainMemoryArea(SDL_Surface *screen, Music_Player *player) :
    screen(screen), player(player), IAPURAM(player->spc_emu()->ram())
{

}


void Main_Memory_Area::set_addr(int i)
{
  if (memcursor::is_active())
  {
    report::restore_color(addr_being_edited);
    memcursor::start_timer();
  }
  address = i;
  //if (memcursor::is_active())
    //report::backup_color(addr_being_edited);
  update_editing_address();
}
void Main_Memory_Area::set_addr_from_cursor(int x, int y)
{
  x-= MEMORY_VIEW_X;
  y -= MEMORY_VIEW_Y;
  x /= 2;
  y /= 2;
  set_addr(y*256+x);
}
void Main_Memory_Area::add_addr(int i)
{
  set_addr(address+i);
}

//void dec_cursor_row();
//void inc_cursor_row();

void Main_Memory_Area::lock(char l/*=1*/, int x/*=0*/, int y/*=0*/, uint8_t rx/*=0*/, uint8_t ry/*=0*/)
{
  
  locked = l;
  if (locked)
  {
     res_y = 0;
    if (submode != EASY_EDIT)
    {
      if (rx || ry)
      {
        res_x = rx; res_y = ry;
      }
      else
      {
        address_remainder = address % 8;
        add_addr(-address_remainder);
        res_x = 0; // address_remainder;
      }
    }

    mode = MODE_EDIT_Main_Memory_Area;
    cursor::start_timer();

    memcursor::start_timer();
    Main_Memory_Area::addr_being_edited = Main_Memory_Area::address+(Main_Memory_Area::res_y*8)+Main_Memory_Area::res_x;
  }
  else
  {
    if (x && y)
    {
      if (  x >= MEMORY_VIEW_X && 
                  x < MEMORY_VIEW_X + 512 &&
                  y >= MEMORY_VIEW_Y &&
                  y < MEMORY_VIEW_Y + 512 )
      {
        set_addr_from_cursor(x, y);
      }
    }
    report::restore_color(Main_Memory_Area::addr_being_edited);

    mode = MODE_NAV;
    cursor::stop_timer();
    memcursor::stop_timer();
  }
  
}
void Main_Memory_Area::toggle_lock(int x/*=0*/, int y/*=0*/)
{
  lock(!locked, x, y);
}

void Main_Memory_Area::unlock()
{
  lock(0);
}
void Main_Memory_Area::update_editing_address()
{
  report::restore_color(Main_Memory_Area::addr_being_edited);
  Main_Memory_Area::addr_being_edited = Main_Memory_Area::address+(Main_Memory_Area::res_y*8)+Main_Memory_Area::res_x;
  if ( (addr_being_edited == 0xf3 && (IAPURAM[0xf2] == 0x4c || IAPURAM[0xf2] == 0x5c) ) || addr_being_edited==0xf1 || addr_being_edited == 0xf0)
  {
    // only update the buffer the first time.. if we haven't started writing in a new value
    if (!Main_Memory_Area::draw_tmp_ram)
    {
      if (addr_being_edited == 0xf3)
        Main_Memory_Area::tmp_ram = player->spc_read(0xf2);
      else Main_Memory_Area::tmp_ram = player->spc_read(addr_being_edited);
    }
  }
}
void Main_Memory_Area::inc_cursor_row()
{
  cursor::toggle=1;
  
  if (res_y == 15)
  {
    add_addr(+8); //Main_Memory_Area::address += 8;
  }
  else res_y++;    
  update_editing_address();
}
void Main_Memory_Area::dec_cursor_row()
{
  cursor::toggle=1;
  
  if (res_y == 0)
  {
    add_addr(-8);
  }
  else res_y--;    
  update_editing_address();
}
void Main_Memory_Area::inc_cursor_pos()
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
void Main_Memory_Area::dec_cursor_pos()
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

void Main_Memory_Area::draw_cursor(SDL_Surface *screen, Uint32 color)
{
  if (Main_Memory_Area::highnibble)
  {
    cursor::draw(screen, Main_Memory_Area_START_X + (Main_Memory_Area::res_x * Main_Memory_Area_ENTRY_X_INCREMENT), Main_Memory_Area_START_Y + (Main_Memory_Area::res_y * Main_Memory_Area_ENTRY_Y_INCREMENT), color);
  }
  else
  {
    cursor::draw(screen, Main_Memory_Area_START_X + (Main_Memory_Area::res_x * Main_Memory_Area_ENTRY_X_INCREMENT + (7)), Main_Memory_Area_START_Y + (Main_Memory_Area::res_y * Main_Memory_Area_ENTRY_Y_INCREMENT), color);
  }
}


