//sdl_userevents.h
#pragma once

namespace UserEvents
{
  enum Events
  {
    #include "sdl_dblclick_events.enum"
    mouse_ani,

    play_pitch,
    keyoff,
    sound_stop
  };
}
