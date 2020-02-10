#pragma once
#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "Screen.h"

struct Voice_Control
{
  Voice_Control(){}
  Uint8 muted_toggle_protect=0; // for toggle protection during MouseMoition event
  Uint8 muted=0;

  void checkmouse_mute(Uint16 &x,Uint16 &y);
  
  void checkmouse_solo(Uint16 &x,Uint16 &y);
  
  void checkmouse(Uint16 &x, Uint16 &y, int);

  Uint8 is_muted(int i);
  void toggle_mute(uint8_t m);  // channel num
  void toggle_mute_all();
  void mute_all();
  void unmute_all();
  void mute(uint8_t i);
  void solo(uint8_t i);
  void solo_bits(uint8_t i);
  void toggle_solo(uint8_t i);
};
