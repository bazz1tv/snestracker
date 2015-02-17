//sdl_userevents.h
#pragma once

namespace UserEvents
{
  enum Events
  {
    #include "sdl_dblclick_events.h"
    play_pitch,
    keyoff,
    sound_stop
  };
}