#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <fcntl.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "SDL.h"
#include "vspc/report.h"
#include "gme/player/Music_Player.h"

#include "sdlfont.h"

#include "sdl_dblclick.h"
#include "mode.h"
#include "mouse_hexdump.h"
#include "gui/porttool.h"
#include "globals.h"

#include "events.h"
#include "track.h"
#include "colors.h"

void prev_track();
void next_track();
void toggle_pause();
void restart_track();

Music_Player* player;


// DEPRECATED
typedef struct SPC_Config
{
  int sampling_rate;
  int resolution;
  int channels;
  int is_interpolation;
  int is_echo;
} SPC_Config;

SPC_Config spc_config = {
    44100,
    16,
    2,
    0, // interpolation
    0 // echo
};





void inc_ram(int addr, int i=1)
{
	player->spc_write(addr, (player->spc_read(addr))+i);
}
void dec_ram(int addr, int i=1)
{
	player->spc_write(addr, (player->spc_read(addr))-i);
}



int parse_args(int argc, char **argv)
{
	int res;
	static struct option long_options[] = {
		{"nosound", 0, 0, 0},
		{"novideo", 0, 0, 1},
		{"update_in_callback", 0, 0, 2},
		{"echo", 0, 0, 3},
		{"interpolation", 0, 0, 4},
		{"savemask", 0, 0, 5},
		{"default_time", 1, 0, 6},
		{"ignore_tag_time", 0, 0, 7},
		{"extra_time", 1, 0, 8},
		{"yield", 0, 0, 9},
		{"auto_write_mask", 0, 0, 10},
		{"status_line", 0, 0, 11},
		{"help", 0, 0, 'h'},
		{"apply_mask_block", 0, 0, 12},
		{"apply_mask_byte", 0, 0, 13},
		{"filler", 1, 0, 14},
		{0,0,0,0}
	};


	while ((res=getopt_long(argc, argv, "h",
				long_options, NULL))!=-1)
	{
		switch(res)
		{
			case 0:
				g_cfg_nosound = 1;
				break;
			case 1:
				g_cfg_novideo = 2;
				break;
			case 2:
				g_cfg_update_in_callback = 1;
				break;
			case 4:
				spc_config.is_interpolation = 1;
				break;
			case 3:
				spc_config.is_echo = 1;
				break;
			case 5:
				g_cfg_autowritemask = 1;
				break;
			case 6:
				g_cfg_defaultsongtime = atoi(optarg);
				break;
			case 7:
				g_cfg_ignoretagtime = 1;
				break;
			case 8:
				g_cfg_extratime = atoi(optarg);
				break;
			case 9:
				g_cfg_nice = 1;
				break;
			case 10:
				g_cfg_autowritemask = 1;
				break;
			case 11:
				g_cfg_statusline = 1;
				break;
			case 12:
				g_cfg_apply_block = 1;
				break;
			case 14:
				g_cfg_filler = strtol(optarg, NULL, 0);
				break;
			case 'h':
				printf("Usage: ./vspcplay [options] files...\n");
				printf("\n");
				printf("Valid options:\n");
				printf(" -h, --help     Print help\n");
				printf(" --nosound      Dont output sound\n");
				printf(" --novideo      Dont open video window\n");
				printf(" --update_in_callback   Update spc sound buffer inside\n");
				printf("                        sdl audio callback\n");
				printf(" --interpolation  Use sound interpolatoin\n");
				printf(" --echo           Enable echo\n");
				printf(" --auto_write_mask   Write mask file automatically when a\n");
				printf("                     tune ends due to playtime from tag or\n");
				printf("                     default play time.\n");
				printf(" --default_time t    Set the default play time in seconds\n");
				printf("                     for when there is not id666 tag. (default: %d\n", DEFAULT_SONGTIME);
				printf(" --ignore_tag_time   Ignore the time from the id666 tag and\n");
				printf("                     use default time\n");
				printf(" --extra_time t      Set the number of extra seconds to play (relative to\n");
				printf("                     the tag time or default time).\n");
				printf(" --nice              Try to use less cpu for graphics\n");
				printf(" --status_line       Enable a text mode status line\n");
				printf("\n!!! Careful with those!, they can ruin your sets so backup first!!!\n");
				printf(" --apply_mask_block  Apply the mask to the file (replace unused blocks(256 bytes) with a pattern)\n");
				printf(" --filler val        Set the pattern byte value. Use with the option above. Default 0\n");
				printf("\n");
				printf("The mask will be applied when the tune ends due to playtime from tag\n");
				printf("or default playtime.\n");
				exit(0);
				break;
		}
	}

	g_cfg_num_files = argc-optind;
	g_cfg_playlist = &argv[optind];

	return 0;
}


SDL_Window *sdlWindow=NULL;
SDL_Renderer *sdlRenderer=NULL;
SDL_Texture *sdlTexture=NULL;
int init_sdl()
{
	Uint32 flags=0;
	
	/* SDL initialisation */
	// can keep these variables i guess
	if (!g_cfg_novideo) { flags |= SDL_INIT_VIDEO; }
	if (!g_cfg_nosound) { flags |= SDL_INIT_AUDIO; }

	if (SDL_Init(flags) != 0) {
        fprintf(stderr,
                "\nUnable to initialize SDL:  %s\n",
                SDL_GetError()
               );
        return 1;
    }
    atexit(SDL_Quit);

	if (!g_cfg_novideo) {
		// video
		/*screen = SDL_SetVideoMode(800, 600, 0, SDL_SWSURFACE); //SDL_HWSURFACE | SDL_DOUBLEBUF);
		if (screen == NULL) {
			printf("Failed to set video mode\n");
			return 0;
		}*/
		/*sdlWindow = SDL_CreateWindow(PROG_NAME_VERSION_STRING,
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          800, 600,
                          0);
		if (sdlWindow == NULL)
		{
			fprintf(stderr, "FUCK!");
			exit(0);
		}*/

		SDL_CreateWindowAndRenderer(800, 600, 0, &sdlWindow, &sdlRenderer);
		if (sdlWindow == NULL || sdlRenderer == NULL)
		{
			fprintf(stderr, "FCK\n");
			exit(1);
		}

		screen = SDL_CreateRGBSurface(0, 800, 600, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);
		sdlTexture = SDL_CreateTexture(sdlRenderer,
                                            SDL_PIXELFORMAT_RGB888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            800, 600);

		SDL_SetWindowTitle(sdlWindow, PROG_NAME_VERSION_STRING);
		colors::precompute();
	}

	dblclick::init();
	//SDL_EnableKeyRepeat( 500, 80 );

	return 0;	
}


void reload();
int main(int argc, char **argv)
{
	printf("%s\n", PROG_NAME_VERSION_STRING);
	parse_args(argc, argv);

	if (g_cfg_num_files < 1) {
		printf("No files specified\n");
		return 1;
	}
	
	init_sdl();

	time_cur = time_last = SDL_GetTicks();

	memsurface.init();

	// Create player
	player = new Music_Player;
	if ( !player )
		handle_error( "Out of memory" );
	handle_error( player->init(44100) );

  if (mode <= MODE_EDIT_APU_PORT)
  {
  	base_mode_game_loop();
  }
  else if (mode == MODE_DSP_MAP)
  {

  }

clean:
	delete player;
	SDL_Quit();

  return 0;
}




