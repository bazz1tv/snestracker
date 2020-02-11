#include "Main_Memory_Area.h"
#include "gme/player/Music_Player.h"
#include "report.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include <nfd.h>
#include "DEBUGLOG.h"
#include "utility.h"

int write_brri_to_file(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data; 
  
  BRR::write_brri_to_file(&mma->brr);

  return 0;
}

int write_brrp_to_file(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data; 
  
  BRR::write_brrp_to_file(&mma->brr);

  return 0;
}

int write_plain_brr_to_file(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data;
  BRR::write_plain_brr_to_file(&mma->brr);

  return 0;
}

int Main_Memory_Area::Context::play_sample(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data;
  fprintf(stderr, "one_solo = %d\n", mma->brr.one_solo);
  BaseD::play_sample(mma->brr.one_solo);
  return 0;
}

int solo_sample(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data;
  mma->brr.solo_sample();
  return 0;
}



void Main_Memory_Area::log_the_fucking_address_for_the_fucking_context_window()
{
  context.addr_when_user_right_clicked = mouse_addr;
}

Main_Memory_Area::Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area, uint16_t *dir) :
mouse_hexdump_area(mouse_hexdump_area), dir(dir),
context(this)
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
        res_x = 0;
      }
    }

    cursor.start_timer();

    memcursor.start_timer();
    report::restore_color(addr_being_edited);
    addr_being_edited = address+(res_y*8)+res_x;
  }
  else
  {
    DEBUGLOG("unlocked form lcok");
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
  DEBUGLOG("unlock");
  lock(0);
}
