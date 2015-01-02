#pragma once
#include "gui/cursor.h"
#include "gme/player/Music_Player.h"
//#include "gme/player/Music_Player.h"
#define PORTTOOL_X    540
#define PORTTOOL_Y    380 + 40

#define PORTTOOL_PORT0_X PORTTOOL_X + (8*7)
#define PORTTOOL_PORT3_X_RIGHTMOST_LOBYTE PORTTOOL_X + (8*23)
// 8 = tile width

//extern struct SIAPU IAPU;
extern Music_Player* player;
namespace porttool
{
  extern Uint8 highnibble, portnum, portaddress, tmp[4];
  extern Uint8 horizontal;//=0;
  extern int x,y;
  extern Uint8 portdata[4];// = {0,0,0,0};

  void inc_port(int port);
  
  void dec_port(int port);

  void draw_cursor(SDL_Surface *screen, Uint32 color);

  void write(int i);
  void write();
  void writeall();

  void reset_port();
  void reset_port(Uint8 num);

  void switch_port(Uint8 num);
  void set_port(Uint8 num);

  void inc_cursor_pos();

  void dec_cursor_pos();


}