#pragma once

#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "sdlfont.h"
#include "colors.h"

namespace track
{
  extern char now_playing[1024];
  extern int song_time;
  extern track_info_t tag;

void update_window_title();
void start( int track, const char* path );

void update_tag();
}