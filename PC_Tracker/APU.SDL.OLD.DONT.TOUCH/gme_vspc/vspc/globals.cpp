#include "SDL.h"
#include "report.h"
#include "globals.h"

bool filter_is_active=false;

uint8_t *IAPURAM;
bool paused;

Uint32 time_last=0, time_cur=0;

namespace mouse
{
  int x,y;
  char show=0;
}

void handle_error( const char* error )
{
  if ( error )
  {
    // put error in window title
    char str [256];
    sprintf( str, "Error: %s", error );
    fprintf( stderr, "Error: %s", error );
    SDL_WM_SetCaption( str, str );
    
    // wait for keyboard or mouse activity
    SDL_Event e;
    do
    {
      while ( !SDL_PollEvent( &e ) ) { }
    }
    while ( e.type != SDL_QUIT && e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN );

    exit( EXIT_FAILURE );
  }
}


namespace screen_pos
{
  // 8 is the char width in pixels
  //typedef SDL_Rect coord;
  #define NO_INIT 0
  SDL_Rect voice0vol = {NO_INIT, NO_INIT,+8+125,10};
  SDL_Rect voice0pitch = {NO_INIT,NO_INIT, 2*8,8 };
  SDL_Rect locked = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  SDL_Rect echoE = { MEMORY_VIEW_X+520, NO_INIT, 2*8, 10 };
  #undef UN_INIT
}


int last_pc=-1;

// raph: those are bigger so I dont have to do a range test
// each time I want to log the PC address (where I assume
// a 5 byte instruction)
unsigned char used[0x10006];
unsigned char used2[0x101];

unsigned char g_cfg_filler = 0x00;
int g_cfg_apply_block = 0;
int g_cfg_statusline = 0;
int g_cfg_nice = 0;
int g_cfg_extratime = 0;
int g_cfg_ignoretagtime = 0;
int g_cfg_defaultsongtime = DEFAULT_SONGTIME;
int g_cfg_autowritemask = 0;
int g_cfg_nosound = 0;
int g_cfg_novideo = 0;
int g_cfg_update_in_callback = 0;
int g_cfg_num_files = 0;
char **g_cfg_playlist = NULL;
int g_paused = 0;
int g_cur_entry = 0;
char *g_real_filename=NULL; // holds the filename minus path

int mode=0; int submode = 0;
 
SDL_Event event;



const char* path;
char tmpbuf[500];
SDL_Surface *screen=NULL;


Mem_Surface memsurface;



bool is_first_run=true;