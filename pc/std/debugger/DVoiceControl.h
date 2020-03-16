#pragma once
#include "SDL.h"
#include "shared/Voice_Control.h"
struct DVoiceControl : public Voice_Control
{
  void checkmouse_mute(Uint16 &x,Uint16 &y);
  void checkmouse_solo(Uint16 &x,Uint16 &y);
  void checkmouse(Uint16 &x, Uint16 &y, int);
};
