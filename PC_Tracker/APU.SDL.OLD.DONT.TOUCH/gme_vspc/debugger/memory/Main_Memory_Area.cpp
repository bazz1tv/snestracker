#include "Main_Memory_Area.h"
#include "gme/player/Music_Player.h"
#include "report.h"

//extern uint8_t* IAPURAM;
//extern Music_Player *player;

int demo(void *data)
{
  uint16_t *address = (uint16_t *)data;
  assert (address != 0);
  // determine if right-click happened on a BRR Sample
  // find the closest entry in array that is above the user's clicked address..
  uint16_t lowest_offset_above=0xffff, lowest_offset_below=0xffff;
  bool can_show_brr_context=false;
  uint8_t lowest_brr_array_index, lowest_srcn_array_index;
  for (uint8_t i=0; i < report::BRR_HEADER_MAX; i++)
  {
    if (report::BRR_Headers[i] == 0xffff)
      break;
    
    if (report::BRR_Headers[i] > *address)
    {
      uint16_t offset = report::BRR_Headers[i] - *address; //@init = 0xffff-0xffff
      if (offset < lowest_offset_above)
      {
        lowest_offset_above = offset;
        lowest_brr_array_index = i;
      }
    }
    
  }


  if (lowest_offset_above != 0xffff)
  {
    // POTENTIAL
    for (int i=0; i < report::SRCN_MAX; i++)
    {
      if (report::SRCN_used[i] == 0xffff)
      break;
    
      if (report::SRCN_used[i] < *address)
      {
        uint16_t offset =  *address - report::SRCN_used[i]; //@init = 0xffff-0xffff
        if (offset < lowest_offset_below)
        {
          lowest_offset_below = offset;
          lowest_srcn_array_index = i;
          can_show_brr_context = true;
        }
      }
    }

    if (can_show_brr_context)
    {
      fprintf(stderr, "address = %04X\nBRR sample located from %04X-%04X\n", 
        *address,
        report::SRCN_used[lowest_srcn_array_index], 
        report::BRR_Headers[lowest_brr_array_index]);
    }

    /*uint16_t dir = (uint16_t)player->spc_read_dsp(dsp_reg::dir)*0x100;
    // checking the voices' SRCN (*4)
    for (int i=0; i < 8; i++)
    {
      uint16_t srcn = player->spc_read_dsp(i*0x10+04);
      uint16_t dir_entry = srcn*4;
      if (IAPURAM[dir+dir_entry] < address)
      {
        // should have an array of ALL SRCN used during song
        // MUST reinitialize  BRR_Headers array at BaseD::reload()
      }
    }*/
  }
  // even if user clicks before a BRR sample.. this logic will just download the
  // next sample in memory
  return 0;
}


Main_Memory_Area::Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area) :
mouse_hexdump_area(mouse_hexdump_area),
context_menu(context_menu_items)
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