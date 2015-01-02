/*
 * spcplay - main (program entry point)
 *
 * Copyright (C) 2000 AGAWA Koji <kaoru-k@self-core.104.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
/* $Id: main.c,v 1.32 2005/07/27 17:27:47 raph Exp $ */

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







void start_track( int track, const char* path )
{
	paused = false;
	//if (!player->is_paused())
	handle_error( player->start_track( track - 1 ) );
	// update window title with track info
	
	long seconds = player->track_info().length / 1000;
	const char* game = player->track_info().game;
	if ( !*game )
	{
		// extract filename
		game = strrchr( path, '\\' ); // DOS
		if ( !game )
			game = strrchr( path, '/' ); // UNIX
		if ( !game )
		{
			if (path)
				game = path;
			else game = "";
		}
		else
			game++; // skip path separator
	}
	
	char title [512];
	sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
			game, track, player->track_count(), player->track_info().song,
			seconds / 60, seconds % 60 );
	SDL_WM_SetCaption( title, title );
}



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



int init_sdl()
{
	Uint32 flags=0;
	
	/* SDL initialisation */
	// can keep these variables i guess
	if (!g_cfg_novideo) { flags |= SDL_INIT_VIDEO; }
	if (!g_cfg_nosound) { flags |= SDL_INIT_AUDIO; }

	SDL_Init(flags);	

	if (!g_cfg_novideo) {
		// video
		screen = SDL_SetVideoMode(800, 600, 0, SDL_SWSURFACE); //SDL_HWSURFACE | SDL_DOUBLEBUF);
		if (screen == NULL) {
			printf("Failed to set video mode\n");
			return 0;
		}

		SDL_WM_SetCaption(PROG_NAME_VERSION_STRING, NULL);
		
		// precompute some colors
		color_screen_black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		color_screen_nearblack = SDL_MapRGB(screen->format, 0x50, 0x50, 0x50);
		color_screen_white = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
		color_screen_yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
		color_screen_cyan = SDL_MapRGB(screen->format, 0x00, 0xff, 0xff);
		color_screen_magenta = SDL_MapRGB(screen->format, 0xff, 0x00, 0xff);
		color_screen_gray = SDL_MapRGB(screen->format, 0x7f, 0x7f, 0x7f);
		color_screen_red = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
		color_screen_green = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);
		color_screen_blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);

		color_screen_dark_yellow = SDL_MapRGB(screen->format, 0x50,0x50,0x00);
		color_screen_dark_cyan = SDL_MapRGB(screen->format, 0x00, 0x50, 0x50);
		color_screen_dark_magenta = SDL_MapRGB(screen->format, 0x50, 0x00, 0x50);

		colorscale[0] = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
		colorscale[1] = SDL_MapRGB(screen->format, 0xff, 0x7f, 0x00);
		colorscale[2] = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
		colorscale[3] = SDL_MapRGB(screen->format, 0x7f, 0xff, 0x00);
		colorscale[4] = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);
		colorscale[5] = SDL_MapRGB(screen->format, 0x00, 0xff, 0x7f);
		colorscale[6] = SDL_MapRGB(screen->format, 0x00, 0xff, 0xff);
		colorscale[7] = SDL_MapRGB(screen->format, 0x00, 0x7f, 0xff);
		colorscale[8] = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);
		colorscale[9] = SDL_MapRGB(screen->format, 0x7f, 0x00, 0xff);
		colorscale[10] = SDL_MapRGB(screen->format, 0xff, 0x00, 0xff);
		colorscale[11] = SDL_MapRGB(screen->format, 0xff, 0x00, 0x7f);
	}

	dblclick::init();
	SDL_EnableKeyRepeat( 500, 80 );

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




