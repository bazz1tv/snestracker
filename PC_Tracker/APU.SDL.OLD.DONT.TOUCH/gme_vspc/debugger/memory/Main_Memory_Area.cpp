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
  uint16_t lowest_offset_above_addr=0xffff, lowest_offset_below_addr=0xffff;
  uint16_t lowest_loop_offset_below_addr=0xffff;
  bool can_show_brr_context=false, can_compare_brr_context=false;;
  uint16_t addr_of_closest_brrend_block_after_addr=0xffff, lowest_srcn_addr, lowest_loop_addr;
  uint16_t closest_brrend_block_before_addr_offset=0xffff, addr_of_closest_brrend_block_before_addr;
  for (uint8_t i=0; i < report::BRR_HEADER_MAX; i++)
  {
    uint16_t offset;
    uint16_t *b = &report::BRR_Headers[i];
    if ( *b == 0xffff)
      break;
    
    if (*b >= *address)
    {
      offset = *b - *address; //@init = 0xffff-0xffff
      if (offset < lowest_offset_above_addr)
      {
        lowest_offset_above_addr = offset;

        addr_of_closest_brrend_block_after_addr = *b;
        fprintf(stderr, "b = %04X, %04X\n", addr_of_closest_brrend_block_after_addr, *b);
      }
    }
    else
    {
      offset = *address - *b;
      if (offset < closest_brrend_block_before_addr_offset)
      {
        closest_brrend_block_before_addr_offset = offset;
        addr_of_closest_brrend_block_before_addr = *b;

      }
    }
    
  }

  fprintf(stderr, "lowestbrrendaddr = %04X", addr_of_closest_brrend_block_after_addr);


  if (addr_of_closest_brrend_block_after_addr == 0xffff)
  {
    addr_of_closest_brrend_block_after_addr = addr_of_closest_brrend_block_before_addr;
  }
    // POTENTIAL
    uint8_t flag=0;
    for (int i=0; i < report::SRCN_MAX; i++)
    {
      uint16_t *s = &report::SRCN_used[i];
      uint16_t *l = &report::LOOP_used[i];
      // 1
      if (*s == 0xffff && !(flag & 1) )
      {
        flag |= 1;
        if (flag == 3)
          break;
      }
      if (*s < *address)
      {
        uint16_t offset =  *address - *s; //@init = 0xffff-0xffff
        if (offset < lowest_offset_below_addr)
        {
          lowest_offset_below_addr = offset;
          lowest_srcn_addr = *s;
          can_show_brr_context = true;
        }
      }
      // 2
      if (*l == 0xffff && !(flag & 2))
      {
        flag |= 2;
        if (flag == 3)
          break;
      }
      if (*l < *address)
      {
        uint16_t offset =  *address - *l; //@init = 0xffff-0xffff
        if (offset < lowest_loop_offset_below_addr)
        {
          fprintf(stderr," loopused[%d] = %04X\n", i, *l);
          lowest_loop_offset_below_addr = offset;
          lowest_loop_addr = *l;
          can_compare_brr_context = true;
        }
      }
      
    }

    if (can_compare_brr_context && lowest_loop_addr > lowest_srcn_addr)
    {
      fprintf(stderr, "%04X-%04X is a Loop sample\n", lowest_loop_addr,
        addr_of_closest_brrend_block_after_addr);
      can_show_brr_context = false;
    }
    if (can_show_brr_context)
    {
      fprintf(stderr, "address = %04X\nBRR sample located from %04X-%04X\n", 
        *address,
        lowest_srcn_addr, 
        addr_of_closest_brrend_block_after_addr);
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
  
  // even if user clicks before a BRR sample.. this logic will just download the
  // next sample in memory
  return 0;
}

int play_sample(void *data)
{
  // steal sample routine from above
  // and a, #$ff -- A "DONOTHING" opcode
  // 28 ff 2f fc 8f 00 04
  // 2f fe // 8f 00 04
  // Enter instrument editor
  //
  // pause SPC
  //player->pause(1);
  // place into SPC the above opcodes at PC
  return 0;
}



void Main_Memory_Area::log_the_fucking_address_for_the_fucking_context_window()
{
  context.addr_when_user_right_clicked = Memory::address;
}

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