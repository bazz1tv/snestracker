#pragma once
#include "SDL.h"
#include "report.h"
#include "gme/player/Music_Player.h"

// TRY NOT TO USE THIS!!! 
// just for prototyping!
//namespace globals




void handle_error( const char* );

namespace mouse {
  extern int x,y;
  extern char show;
}



//extern  //=-1;

//#define PACKAGE "spcview"






// 5 minutes default
#define DEFAULT_SONGTIME  (60*5) 

#define PROG_NAME_VERSION_STRING "SNES Tracker Debugger v%s"
#define CREDITS "APU emulation code from Blargg's GME and Snes_Spc 0.9.0"