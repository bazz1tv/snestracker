#include "Main_Memory_Area.h"
#include "gme/player/Music_Player.h"
#include "report.h"

//extern uint8_t* IAPURAM;
//extern Music_Player *player;


Main_Memory_Area::Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area) :
mouse_hexdump_area(mouse_hexdump_area)
{

}



void Main_Memory_Area::set_addr(int i)
{
  if (memcursor.is_active())
  {
    report::restore_color(addr_being_edited);
    memcursor.start_timer();
  }
  Memory::set_addr(i);
  // shared variables, but not all of them
  mouse_hexdump_area->update_editing_address();
}
void Main_Memory_Area::set_addr_from_cursor(int x, int y)
{
  x-= MEMORY_VIEW_X;
  y -= MEMORY_VIEW_Y;
  x /= 2;
  y /= 2;
  set_addr(y*256+x);
}


//void dec_cursor_row();
//void inc_cursor_row();

void Main_Memory_Area::lock(char l/*=1*/, int x/*=0*/, int y/*=0*/, uint8_t rx/*=0*/, uint8_t ry/*=0*/)
{
  
  locked = l;
  if (locked)
  {
     res_y = 0;
    if (mouse_hexdump_area->submode != Mouse_Hexdump_Area::EASY_EDIT)
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

    //mode = MODE_EDIT_MOUSE_HEXDUMP;
    cursor.start_timer();

    memcursor.start_timer();
    report::restore_color(addr_being_edited);
    addr_being_edited = address+(res_y*8)+res_x;
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
    
    report::restore_color(addr_being_edited);
    //mode = MODE_NAV;
    cursor.stop_timer();
    memcursor.stop_timer();
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