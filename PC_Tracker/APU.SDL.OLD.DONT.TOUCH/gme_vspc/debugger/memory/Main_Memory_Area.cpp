#include "Main_Memory_Area.h"
#include "gme/player/Music_Player.h"
#include "report.h"

//extern uint8_t* IAPURAM;
//extern Music_Player *player;

int demo(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data;

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

int Main_Memory_Area::check_brr()
{
  uint16_t *address = &context.addr_when_user_right_clicked;
  fprintf(stderr, "address = %04X", context.addr_when_user_right_clicked);
  assert (address != 0);
  uint8_t lowest_srcn_index=0x0;
  

  // get closest SRCN address before clicked address
  uint16_t lowest_closest_srcn_address=0xffff, lowest_offset=0xffff;
  for (int i=0; i < report::SRCN_MAX; i++)
  {
    if (report::SRCN_used[i] < *address)
    {
      uint16_t offset = *address - report::SRCN_used[i];
      if (offset < lowest_offset)
      {
        lowest_closest_srcn_address=report::SRCN_used[i];
        //fprintf(stderr, "lowest = %04X, %04X\n", lowest_closest_srcn_address, report::SRCN_used[i]);
        lowest_offset = offset;
      }
    }
  }

  if (lowest_closest_srcn_address == 0xffff)
  {
    fprintf(stderr, "no appropriate SRCN was found :(\n");
    return -1;
  }

  for (int x=0; x < MAX_VOICES; x++)
  {
    if (srcn[x] == lowest_closest_srcn_address)
    {
      lowest_srcn_index |= 1<<x;
      //break;
    }
  }

  BaseD::voice_control.solo_bits(lowest_srcn_index);

  // get closest LOOP Address before clicked address
  uint16_t lowest_closest_brrloopstart_address_from_click=0xffff;
  lowest_offset=0xffff;
  for (int i=0; i < report::SRCN_MAX; i++)
  {
    if (report::LOOP_used[i] == 0xffff) break;

    if (report::LOOP_used[i] < *address)
    {
      uint16_t offset = *address - report::LOOP_used[i];
      if (offset < lowest_offset)
      {
        lowest_closest_brrloopstart_address_from_click = report::LOOP_used[i];
        lowest_offset = offset;
      }
    }
  }

  if (lowest_closest_brrloopstart_address_from_click == 0xffff)
  {
    fprintf(stderr, "no appropriate LOOP point was found :(\n");
    //return -1;
  }

  // Find closest BRR end block after closest SRCN
  uint16_t lowest_closest_brrend_address_from_srcn = 0xffff;
  uint16_t p = lowest_closest_srcn_address;
  while(1)
  {
    if (BaseD::IAPURAM[p] & 1)
    {
      p+=8;
      break;
    }
    p+=9;
  }
  lowest_closest_brrend_address_from_srcn = p;

  if (lowest_closest_brrend_address_from_srcn == 0xffff)
  {
    fprintf(stderr, "no appropriate BRR_END after SRCN was found :(\n");
    //return -1;
  }

  // Find closest BRR end block after clicked address
  uint16_t lowest_closest_brrend_address_from_click=0xffff;
  lowest_offset=0xffff;
  for (int i=0; i < report::BRR_HEADER_MAX; i++)
  {
    if (report::BRR_Headers[i] == 0xffff) break;

    if (report::BRR_Headers[i] > *address)
    {
      uint16_t offset = report::BRR_Headers[i] - *address;
      if (offset < lowest_offset)
      {
        lowest_closest_brrend_address_from_click = report::BRR_Headers[i];
        lowest_offset = offset;
      }
    }
  }

  if (lowest_closest_brrend_address_from_click == 0xffff)
  {
    fprintf(stderr, "no appropriate BRR_END was found :(\n");
    //return -1;
  }



  if (lowest_closest_brrloopstart_address_from_click > lowest_closest_brrend_address_from_srcn && 
    *address > lowest_closest_brrloopstart_address_from_click)
  {
    fprintf(stderr,"You clicked on an external Loop sample\n");
  }
  else fprintf(stderr, "BRR @ 0x%04X-0x%04X\n", lowest_closest_srcn_address, lowest_closest_brrend_address_from_srcn);

  fprintf(stderr, "lowest_closest_srcn_address = %04X\n", lowest_closest_srcn_address);
  fprintf(stderr, "lowest_closest_brrend_address_from_srcn = %04X\n", lowest_closest_brrend_address_from_srcn);
  fprintf(stderr, "lowest_closest_brrloopstart_address_from_click = %04X\n", lowest_closest_brrloopstart_address_from_click);
  fprintf(stderr, "lowest_closest_brrend_address_from_click = %04X\n", lowest_closest_brrend_address_from_click);
  
}

void Main_Memory_Area::log_the_fucking_address_for_the_fucking_context_window()
{
  context.addr_when_user_right_clicked = mouse_addr;
}

Main_Memory_Area::Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area) :
mouse_hexdump_area(mouse_hexdump_area),
context(this)
{
  for (int i=0; i < MAX_VOICES; i++)
  {
    srcn[i]=0xffff;
  }
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