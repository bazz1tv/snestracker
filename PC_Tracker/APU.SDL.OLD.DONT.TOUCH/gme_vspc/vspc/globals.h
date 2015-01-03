#pragma once
#include "SDL.h"
#include "report.h"
#include "gme/player/Music_Player.h"

// TRY NOT TO USE THIS!!! 
// just for prototyping!
//namespace globals

extern bool filter_is_active;
extern uint8_t *IAPURAM;
extern bool paused;

#define LOCKED_STR "locked"

void handle_error( const char* );
void start_track( int track, const char* path );

namespace mouse {
  extern int x,y;
  extern char show;
}

namespace screen_pos
{
  //typedef SDL_Rect coord;
  #define NO_INIT 0
  extern SDL_Rect voice0vol;   // = {NO_INIT, NO_INIT,+8+125,10};
  extern SDL_Rect voice0pitch; //= {NO_INIT,NO_INIT, 2*8,8 };
  extern SDL_Rect locked;      // = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  extern SDL_Rect echoE;
  #undef UN_INIT
}


extern int last_pc; //=-1;

#define PACKAGE "spcview"
#define VERSION "0.01"



#define INFO_X      540
#define INFO_Y      420 + 40

// 5 minutes default
#define DEFAULT_SONGTIME  (60*5) 

#define PROG_NAME_VERSION_STRING "vspcplay v1.32 by bazz & Raph Assenat!"
#define CREDITS "APU emulation code from Blargg's GME and Snes_Spc 0.9.0"


// those are bigger so I dont have to do a range test
// each time I want to log the PC address (where I assume
// a 5 byte instruction)
// bazZ: dunno if we need this
extern unsigned char used[0x10006];
extern unsigned char used2[0x101];

//extern struct SAPU APU;
//extern struct SIAPU IAPU;

extern unsigned char g_cfg_filler;// = 0x00;
extern int g_cfg_apply_block;// = 0;
extern int g_cfg_statusline;// = 0;
extern int g_cfg_nice;// = 0;
extern int g_cfg_extratime;// = 0;
extern int g_cfg_ignoretagtime;// = 0;
extern int g_cfg_defaultsongtime;// = DEFAULT_SONGTIME;
extern int g_cfg_autowritemask;// = 0;
extern int g_cfg_nosound;// = 0;
extern int g_cfg_novideo;// = 0;
extern int g_cfg_update_in_callback;// = 0;
extern int g_cfg_num_files;// = 0;
extern char **g_cfg_playlist;// = NULL;
//extern int g_paused = 0;
extern int g_cur_entry;// = 0;
extern char *g_real_filename;//=NULL; // holds the filename minus path

extern int mode;//=0;
extern int submode;// = 0;
 
extern SDL_Event event;



extern const char* path;
extern char tmpbuf[500];
extern SDL_Surface *screen;//=NULL;


extern Mem_Surface memsurface;

extern Uint32 time_last, time_cur;






extern bool is_first_run;//=true;
extern Music_Player *player;