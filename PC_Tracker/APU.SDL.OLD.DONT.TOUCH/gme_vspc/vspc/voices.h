#pragma once
#include "SDL.h"
#include "globals.h"
namespace voices
{
  extern Uint8 was_keyed_on;//=0;
  extern Uint8 muted_toggle_protect;//;//=0; // for toggle protection during MouseMotion event
  extern Uint8 muted;//=0;

  void checkmouse_mute(Uint16 &x,Uint16 &y);
  
  void checkmouse_solo(Uint16 &x,Uint16 &y);
  
  void checkmouse(Uint16 &x, Uint16 &y, Uint8 &b);

  Uint8 is_muted(int i);
}