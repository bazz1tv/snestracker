#pragma once
#include "SDL.h"
#include "report.h"
#include "gme/player/Music_Player.h"
#include "DVoiceControl.h"
#include "BrrContextMenu.h"

extern uint8_t *IAPURAM;
extern DVoiceControl voice_control;
extern track_info_t tag;
extern BrrContextMenu brrcontext;

void handle_error( const char* );

// 5 minutes default
#define DEFAULT_SONGTIME  (60*5) 

#define PROG_NAME_VERSION_STRING "SPC Debugger v%s"
#define CREDITS "APU emulation code from Blargg's GME and Snes_Spc 0.9.0"
