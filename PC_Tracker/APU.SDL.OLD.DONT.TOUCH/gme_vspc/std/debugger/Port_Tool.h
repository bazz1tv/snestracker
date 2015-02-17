#pragma once
#include "gui/cursor.h"
#include "gme/player/Music_Player.h"
#include "Player_Context.h"
#include "Render_Context.h"
#include "Screen.h"

// 8 = tile width

//extern Music_Player* player;
struct Port_Tool : public Render_Context, public Player_Context
{
  Port_Tool(Cursor *cursor);

  Uint8 highnibble, portnum, portaddress, tmp[4];
  Uint8 horizontal=0;
  int x=0,y, last_x;
  Uint8 portdata[4] = {0,0,0,0};

  // deps
  //Music_Player *player;
  //SDL_Surface *screen;
  Cursor *cursor;

  void inc_port(int port);
  
  void dec_port(int port);

  void draw_cursor(SDL_Surface *screen, Uint32 color);

  void write();
  void write(int i);
  void write(int i, uint8_t data);
  void writeall();

  void reset_port();
  void reset_port(Uint8 num);

  void switch_port(Uint8 num);
  void set_port(Uint8 num);

  void inc_cursor_pos();

  void dec_cursor_pos();


};