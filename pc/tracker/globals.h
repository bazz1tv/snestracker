#pragma once
#ifndef __SNESTRACKER_GLOBALS_H__
#define __SNESTRACKER_GLOBALS_H__

#include "SDL.h"
#include "gme/player/Music_Player.h"
#include "shared/My_Nfd.h"
#include "shared/Experience.h"
#include "shared/Voice_Control.h"
#include "shared/gui/MouseCursors.h"

extern bool quitting;
extern My_Nfd nfd;
extern Experience *cur_exp;
extern Voice_Control voice_control;
extern uint8_t *IAPURAM;
extern track_info_t tag;
extern MouseCursors *mousecursors;

struct Tracker;
extern Tracker *tracker;

void handle_error( const char* );

// 5 minutes default
#define DEFAULT_SONGTIME  (60*5) 

#define PROG_NAME_VERSION_STRING "SNES Tracker v%s"
#define CREDITS "APU emulation code from Blargg's GME and Snes_Spc 0.9.0"

#endif /* __SNESTRACKER_GLOBALS_H__ */