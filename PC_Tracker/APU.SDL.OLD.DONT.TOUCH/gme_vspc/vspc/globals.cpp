#include "SDL.h"
#include "report.h"
#include "globals.h"
//namespace globals
//{
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
  //typedef SDL_Rect coord;
  #define NO_INIT 0
  SDL_Rect voice0vol = {NO_INIT, NO_INIT,+8+125,10};
  SDL_Rect voice0pitch = {NO_INIT,NO_INIT, 2*8,8 };
  SDL_Rect locked = { MEMORY_VIEW_X+520+24*8, 0 ,(strlen(LOCKED_STR)*8)+1, 9};
  #undef UN_INIT
}


int last_pc=-1;



/*SPC_Config spc_config = {
    44100,
    16,
    2,
    0, // interpolation
    0 // echo
};*/

// those are bigger so I dont have to do a range test
// each time I want to log the PC address (where I assume
// a 5 byte instruction)
// bazZ: dunno if we need this
unsigned char used[0x10006];
unsigned char used2[0x101];

//extern struct SAPU APU;
//extern struct SIAPU IAPU;

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

//SDL_Surface *memsurface=NULL;
//unsigned char *memsurface_data=NULL;
//#define BUFFER_SIZE 65536
///static unsigned char audiobuf[BUFFER_SIZE];
//static int audio_buf_bytes=0, spc_buf_size;

Uint32 color_screen_white, color_screen_black, color_screen_cyan, color_screen_magenta, color_screen_yellow, color_screen_red;
Uint32  color_screen_green, color_screen_blue, color_screen_nearblack;
Uint32 color_screen_dark_magenta, color_screen_dark_cyan, color_screen_dark_yellow;
Uint32 color_screen_gray;
Uint32 colorscale[12];

int cur_mouse_address=0x0000;
SDL_Rect tmprect;

bool is_first_run=true;