#include "mouse_hexdump.h"
#include "gme/player/Music_Player.h"

int MOUSE_HEXDUMP_START_Y;
extern uint8_t* IAPURAM;
extern Music_Player *player;

namespace mouse_hexdump
{
  int tmp_ram; char draw_tmp_ram;
  int rel_x;
  int rel_y;

  int res_x;
  int res_y;
  Uint8 highnibble;
  Uint8 horizontal=1;

  char locked=0;
  Uint16 address=0x0000, addr_being_edited=0x0000;
  Uint8 address_remainder=0;

  void set_addr(int i)
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
  void set_addr_from_cursor(int x, int y)
  {
    x-= MEMORY_VIEW_X;
    y -= MEMORY_VIEW_Y;
    x /= 2;
    y /= 2;
    set_addr(y*256+x);
  }
  void add_addr(int i)
  {
    set_addr(address+i);
  }

  void dec_cursor_row();
  void inc_cursor_row();

  void lock(char l/*=1*/, int x/*=0*/, int y/*=0*/, uint8_t rx/*=0*/, uint8_t ry/*=0*/)
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

      mode = MODE_EDIT_MOUSE_HEXDUMP;
      cursor::start_timer();

      memcursor::start_timer();
      mouse_hexdump::addr_being_edited = mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x;
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
      report::restore_color(mouse_hexdump::addr_being_edited);

      mode = MODE_NAV;
      cursor::stop_timer();
      memcursor::stop_timer();
    }
    
  }
  void toggle_lock(int x/*=0*/, int y/*=0*/)
  {
    lock(!locked, x, y);
  }
  
  void unlock()
  {
    lock(0);
  }
  void update_editing_address()
  {
    report::restore_color(mouse_hexdump::addr_being_edited);
    mouse_hexdump::addr_being_edited = mouse_hexdump::address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x;
    if ( (addr_being_edited == 0xf3 && (IAPURAM[0xf2] == 0x4c || IAPURAM[0xf2] == 0x5c) ) || addr_being_edited==0xf1 || addr_being_edited == 0xf0)
    {
      // only update the buffer the first time.. if we haven't started writing in a new value
      if (!mouse_hexdump::draw_tmp_ram)
      {
        if (addr_being_edited == 0xf3)
          mouse_hexdump::tmp_ram = player->spc_read(0xf2);
        else mouse_hexdump::tmp_ram = player->spc_read(addr_being_edited);
      }
    }
  }
  void inc_cursor_row()
  {
    cursor::toggle=1;
    
    if (res_y == 15)
    {
      add_addr(+8); //mouse_hexdump::address += 8;
    }
    else res_y++;    
    update_editing_address();
  }
  void dec_cursor_row()
  {
    cursor::toggle=1;
    
    if (res_y == 0)
    {
      add_addr(-8);
    }
    else res_y--;    
    update_editing_address();
  }
  void inc_cursor_pos()
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
  void dec_cursor_pos()
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

  void draw_cursor(SDL_Surface *screen, Uint32 color)
  {
    if (mouse_hexdump::highnibble)
    {
      cursor::draw(screen, MOUSE_HEXDUMP_START_X + (mouse_hexdump::res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT), MOUSE_HEXDUMP_START_Y + (mouse_hexdump::res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), color);
    }
    else
    {
      cursor::draw(screen, MOUSE_HEXDUMP_START_X + (mouse_hexdump::res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT + (7)), MOUSE_HEXDUMP_START_Y + (mouse_hexdump::res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), color);
    }
  }



}

