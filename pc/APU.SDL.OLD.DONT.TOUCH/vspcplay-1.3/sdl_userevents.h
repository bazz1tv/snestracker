//sdl_userevents.h
#pragma once

namespace UserEvents
{
  enum Events
  {
    #include "sdl_dblclick_events.h"
    #include "mouse_hexdump_events.h"
    derp
  };
}