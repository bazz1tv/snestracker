//sdl_userevents.h
#pragma once

namespace UserEvents
{
  enum Events
  {
    #include "sdl_dblclick_events.enum"
    mouse_ani,
    mouse_ani_aux,

    play_pitch,
    keyoff,
    sound_stop
  };
}
