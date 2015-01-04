#pragma once
#include "SDL.h"
#include "globals.h"
#include "Player_Context.h"
namespace screen_pos
{
  //typedef SDL_Rect coord;
  //#define NO_INIT 0
  extern SDL_Rect voice0vol;   // = {NO_INIT, NO_INIT,+8+125,10};
  extern SDL_Rect voice0pitch; //= {NO_INIT,NO_INIT, 2*8,8 };
  extern SDL_Rect locked;      // = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  extern SDL_Rect echoE;
  //#undef UN_INIT
}
struct Voice_Control : public Player_Context
{
  Voice_Control();

  //Music_Player *player;
  Uint8 was_keyed_on=0;
  Uint8 muted_toggle_protect=0; // for toggle protection during MouseMoition event
  Uint8 muted=0;


  void checkmouse_mute(Uint16 &x,Uint16 &y);
  
  void checkmouse_solo(Uint16 &x,Uint16 &y);
  
  void checkmouse(Uint16 &x, Uint16 &y, Uint8 &b);

  Uint8 is_muted(int i);
  void toggle_mute(uint8_t m);  // channel num
  void toggle_mute_all();
  void mute_all();
  void unmute_all();
};