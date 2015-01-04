#pragma once

class MainMemoryArea
{
  MainMemoryArea(SDL_Surface *screen, Music_Player *player);

  void set_addr(int i);
  void set_addr_from_cursor(int x, int y);
  void add_addr(int i);

  void dec_cursor_row();
  void inc_cursor_row();

  void lock(char l=1, int x=0, int y=0, uint8_t rx=0, uint8_t ry=0);
  void toggle_lock(int x=0, int y=0);
  
  void unlock();

  void inc_cursor_row();
  void dec_cursor_row();
  void inc_cursor_pos();
  void dec_cursor_pos();
  

  Music_Player *player;
  SDL_Surface *screen;
  uint8_t *IAPURAM;
  

  
  

  char locked=0;
  Uint16 address=0x0000, addr_being_edited=0x0000;
  Uint8 address_remainder=0;
};