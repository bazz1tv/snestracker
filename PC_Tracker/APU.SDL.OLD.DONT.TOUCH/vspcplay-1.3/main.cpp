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
#include "libspc.h"
#include "id666.h"

#include "spc_structs.h"
#include "SDL.h"

#include "sdlfont.h"

#include "sdl_dblclick.h"
#include "mode.h"
#include "mouse_hexdump.h"

int last_pc=-1;

#define PACKAGE "spcview"
#define VERSION "0.01"

#define MEMORY_VIEW_X	16
#define MEMORY_VIEW_Y	40
#define PORTTOOL_X		540
#define PORTTOOL_Y		380
#define INFO_X			540
#define INFO_Y			420

// 5 minutes default
#define DEFAULT_SONGTIME	(60*5) 

#define PROG_NAME_VERSION_STRING "vspcplay v1.31 patched by bazz"
#define CREDITS "vspcplay v1.31 by Raphael Assenat (http://vspcplay.raphnet.net). APU emulation code from snes9x."

SPC_Config spc_config = {
    44100,
    16,
    2,
    0, // interpolation
    0 // echo
};

// those are bigger so I dont have to do a range test
// each time I want to log the PC address (where I assume
// a 5 byte instruction)
unsigned char used[0x10006];
unsigned char used2[0x101];

extern struct SAPU APU;
extern struct SIAPU IAPU;

static unsigned char g_cfg_filler = 0x00;
static int g_cfg_apply_block = 0;
static int g_cfg_statusline = 0;
static int g_cfg_nice = 0;
static int g_cfg_extratime = 0;
static int g_cfg_ignoretagtime = 0;
static int g_cfg_defaultsongtime = DEFAULT_SONGTIME;
static int g_cfg_autowritemask = 0;
static int g_cfg_nosound = 0;
static int g_cfg_novideo = 0;
static int g_cfg_update_in_callback = 0;
static int g_cfg_num_files = 0;
static char **g_cfg_playlist = NULL;
static int g_paused = 0;
static int g_cur_entry = 0;
static char *g_real_filename=NULL; // holds the filename minus path



int mode=0;
 
SDL_Event event;





SDL_Surface *screen=NULL;
SDL_Surface *memsurface=NULL;
unsigned char *memsurface_data=NULL;
#define BUFFER_SIZE 65536
static unsigned char audiobuf[BUFFER_SIZE];
static int audio_buf_bytes=0, spc_buf_size;

Uint32 color_screen_white, color_screen_black, color_screen_cyan, color_screen_magenta, color_screen_yellow, color_screen_red;
Uint32  color_screen_green;
Uint32 color_screen_gray;
Uint32 colorscale[12];

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void put4pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void report_memread(unsigned short address, unsigned char value);
void report_memwrite(unsigned short address, unsigned char value);

void fade_arrays()
{
	int i;
	for (i=0; i<512*512*4; i++)
	{
		if (memsurface_data[i] > 0x40) { memsurface_data[i]--; }
	}
}

static int audio_samples_written=0;

void applyBlockMask(char *filename)
{
	FILE *fptr;
	unsigned char nul_arr[256];
	int i;

	memset(nul_arr, g_cfg_filler, 256);
	
	fptr = fopen(filename, "r+");
	if (!fptr) { perror("fopen"); }

	printf("[");
	for (i=0; i<256; i++)
	{
		fseek(fptr, 0x100+(i*256), SEEK_SET);
		
		if (!used2[i]) {
			printf(".");
			fwrite(nul_arr, 256, 1, fptr);
		} else {
			printf("o");
		}
		fflush(stdout);
	}
	printf("]\n");
	
	fclose(fptr);
}

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
//	printf("Callback %d  audio_buf_bytes: %d\n", len, audio_buf_bytes);
	
	if (g_cfg_update_in_callback)
	{
		while (len>audio_buf_bytes)
		{
			SPC_update(&audiobuf[audio_buf_bytes]);
	//		printf("."); fflush(stdout);
			audio_buf_bytes+=spc_buf_size;
		}
		memcpy(stream, audiobuf, len);
		memmove(audiobuf, &audiobuf[len], audio_buf_bytes - len);
		audio_buf_bytes -= len;
	//		printf("."); fflush(stdout);
	}
	else
	{
		//SDL_LockAudio();

		if (audio_buf_bytes<len) {
			printf("Underrun\n");
			memset(stream, 0, len);
		//	SDL_UnlockAudio();
			return;
		}

		memcpy(stream, audiobuf, len);
		memmove(audiobuf, &audiobuf[len], audio_buf_bytes - len);
		audio_buf_bytes -= len;
		
	}
	audio_samples_written += len/4; // 16bit stereo
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

void write_mask(unsigned char packed_mask[32])
{
	FILE *msk_file;
	char *sep;
	char filename[1024];
	unsigned char tmp;
	int i;
	strncpy(filename, g_cfg_playlist[g_cur_entry], 1024);
#ifdef WIN32
	sep = strrchr(filename, '\\');
#else
	sep = strrchr(filename, '/');
#endif
	// keep only the path
	if (sep) { sep++; *sep = 0; } 
	else { 
		filename[0] = 0; 
	}

	// add the filename
	strncat(filename, g_real_filename, 1024);

	// but remove the extension if any
	sep = strrchr(filename, '.');
	if (sep) { *sep = 0; }

	// and use the .msk extension
	strncat(filename, ".msk", 1024);

	msk_file = fopen(filename, "wb");
	if (!msk_file) {
		perror("fopen");
	}
	else
	{
		fwrite(packed_mask, 32, 1, msk_file);
	}

	printf("Writing mask to '%s'\n", filename);

	// the first 32 bytes are for the 256BytesBlock mask
	printf("256 Bytes-wide block mask:\n");
	for (i=0; i<32; i++) {
		printf("%02X",packed_mask[i]);
	}
	printf("\n");

	printf("Byte level mask..."); fflush(stdout);
	memset(packed_mask, 0, 32);
	for (i=0; i<65536; i+=8)
	{
		tmp = 0;
		if (used[i]) tmp |= 128;
		if (used[i+1]) tmp |= 64;
		if (used[i+2]) tmp |= 32;
		if (used[i+3]) tmp |= 16;
		if (used[i+4]) tmp |= 8;
		if (used[i+5]) tmp |= 4;
		if (used[i+6]) tmp |= 2;
		if (used[i+7]) tmp |= 1;
		fwrite(&tmp, 1, 1, msk_file);
	}
	printf("Done.\n");
	fclose(msk_file);
}

static char now_playing[1024];
static char *marquees[3] = { CREDITS, now_playing, NULL };
static char *cur_marquee = NULL;
static int cur_marquee_id = 0;

void do_scroller(int elaps_milli)
{
	int i;
	char c[2] = { 0, 0 };	
	static int cs;
	static int p = 0;
	static int cur_len;
	static int cur_min;
	SDL_Rect tmprect;
	static float start_angle = 0.0;
	float angle;
	int off;
	int steps;
	static int keep=0;

	keep += elaps_milli;	
//	printf("%d %d\n", keep, elaps_milli);
//	return;
	
	steps = keep*60/1000;
	if (!steps) { return; }

	elaps_milli = keep;
	keep=0;

	if (cur_marquee == NULL) { 
		cur_marquee = marquees[cur_marquee_id]; 
		p = screen->w;
	}
	
	cur_len = strlen(cur_marquee);
	cur_min = -cur_len*8;

	angle = start_angle;
				
	cs = audio_samples_written/44100;
	cs %= 12;

	// clear area	
	tmprect.x = 0;
	tmprect.y = 0;
	tmprect.w = screen->w;
	tmprect.h = 28;
	SDL_FillRect(screen, &tmprect, color_screen_black);
	
	
	for (i=0; i<cur_len; i++)
	{
		off = sin(angle)*8.0;
		c[0] = cur_marquee[i];
		if (	(tmprect.x + i*8 + p > 0) && (tmprect.x + i*8 + p < screen->w) )
		{
			sdlfont_drawString(screen, tmprect.x + i*8 + p, 12 + off, c, colorscale[cs]);
		}
		angle-=0.1;
	}
	start_angle += steps * 0.02;
	p-=steps;

	if (p<cur_min) {
		if (marquees[cur_marquee_id+1]!=NULL) {
			cur_marquee = marquees[++cur_marquee_id];
		}
		else {
			p = screen->w;
		}
	}
}

int init_sdl()
{
	SDL_AudioSpec desired;
	Uint32 flags=0;
	
	/* SDL initialisation */
	// can keep these variables i guess
	if (!g_cfg_novideo) { flags |= SDL_INIT_VIDEO; }
	if (!g_cfg_nosound) { flags |= SDL_INIT_AUDIO; }

	SDL_Init(flags);	

	if (!g_cfg_novideo) {
		// video
		screen = SDL_SetVideoMode(800, 600, 0, SDL_SWSURFACE);
		if (screen == NULL) {
			printf("Failed to set video mode\n");
			return 0;
		}

		SDL_WM_SetCaption(PROG_NAME_VERSION_STRING, NULL);
		
		// precompute some colors
		color_screen_black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		color_screen_white = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
		color_screen_yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
		color_screen_cyan = SDL_MapRGB(screen->format, 0x00, 0xff, 0xff);
		color_screen_magenta = SDL_MapRGB(screen->format, 0xff, 0x00, 0xff);
		color_screen_gray = SDL_MapRGB(screen->format, 0x7f, 0x7f, 0x7f);
		color_screen_red = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
		color_screen_green = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);

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
	
	if (!g_cfg_nosound) {
		// audio
		desired.freq = 44100;
		desired.format = AUDIO_S16SYS;
		desired.channels = 2;
		desired.samples = 1024;
		//desired.samples = 4096;
		desired.callback = my_audio_callback;
		desired.userdata = NULL;
		if ( SDL_OpenAudio(&desired, NULL) < 0 ){
			fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
			return -1;
		}

		printf("sdl audio frag size: %d\n", desired.samples *4);
	}

	dblclick::init();
	//SDL_AddTimer(800, dblclicktimer, 0);
	return 0;	
}

void pack_mask(unsigned char packed_mask[32])
{
	int i;
	
	memset(packed_mask, 0, 32);
	for (i=0; i<256; i++)
	{
		if (used2[i])
		packed_mask[i/8] |=	128 >> (i%8);
	}
}

int hexdump_address=0x0000, hexdump_locked=0;

int main(int argc, char **argv)
{
  void *buf=NULL;
	int tmp, i;
	int updates;
	int cur_mouse_address=0x0000;
	SDL_Rect tmprect;
	SDL_Rect memrect;
	char tmpbuf[100];
	id666_tag tag;
	int song_time, cur_time; // in seconds
	Uint32 current_ticks, song_started_ticks;
	unsigned char packed_mask[32];
	Uint32 time_last=0, time_cur=0;
	
	
	//memset(used, 0, 65536);

	printf("%s\n", PROG_NAME_VERSION_STRING);
	
	parse_args(argc, argv);

	if (g_cfg_num_files < 1) {
		printf("No files specified\n");
		return 1;
	}

	
	
	/* REPLACE WITH BLARGG APU INIT */
  spc_buf_size = SPC_init(&spc_config);
	printf("spc buffer size: %d\n", spc_buf_size);
	buf = malloc(spc_buf_size*2);
	/* */
	
	init_sdl();
	time_cur = time_last = SDL_GetTicks();

	memsurface_data = (unsigned char *)malloc(512*512*4);
	memset(memsurface_data, 0, 512*512*4);

reload:
#ifdef WIN32
	g_real_filename = strrchr(g_cfg_playlist[g_cur_entry], '\\');
#else
	g_real_filename = strrchr(g_cfg_playlist[g_cur_entry], '/');
#endif
	if (!g_real_filename) {
		g_real_filename = g_cfg_playlist[g_cur_entry];
	}
	else {
		// skip path sep
		g_real_filename++;
	}	
	
	{
		FILE *fptr;
		fptr = fopen(g_cfg_playlist[g_cur_entry], "rb");
		if (fptr==NULL) {
			printf("Failed to open %s\n", g_cfg_playlist[g_cur_entry]);
				if (g_cur_entry == g_cfg_num_files-1) {
				g_cfg_num_files--;
			}
			else
			{
				memmove(&g_cfg_playlist[g_cur_entry], &g_cfg_playlist[g_cur_entry+1], g_cfg_num_files-g_cur_entry);
				g_cfg_num_files--;
				g_cur_entry++;
			}
			if (g_cfg_num_files<=0) { printf ("penis1\n"); goto clean; }
			if (g_cur_entry >= g_cfg_num_files) { g_cur_entry = 0; }
			goto reload;
		}
		
		read_id666(fptr, &tag); 

		/* decide how much time the song will play */
		if (!g_cfg_ignoretagtime) {
			song_time = atoi((const char *)tag.seconds_til_fadeout);
			if (song_time <= 0) {
				song_time = g_cfg_defaultsongtime;
			}
		}
		else {
			song_time = g_cfg_defaultsongtime;
		}

		song_time += g_cfg_extratime;

		now_playing[0] = 0;
		if (tag.title)
		{
			if (strlen((const char *)tag.title)) {
				sprintf(now_playing, "Now playing: %s (%s), dumped by %s\n", tag.title, tag.game_title, tag.name_of_dumper);
			}		
		}
		if (strlen(now_playing)==0) {
			sprintf(now_playing, "Now playing: %s\n", g_cfg_playlist[g_cur_entry]);
		}
		
		fclose(fptr);
	}
	
    if (!SPC_load(g_cfg_playlist[g_cur_entry])) 
	{
		printf("Failure\n");
		if (g_cur_entry == g_cfg_num_files-1) {
			g_cfg_num_files--;
		}
		else
		{
			memmove(&g_cfg_playlist[g_cur_entry], &g_cfg_playlist[g_cur_entry+1], g_cfg_num_files-g_cur_entry);
			g_cfg_num_files--;
			g_cur_entry++;
		}
		if (g_cfg_num_files<=0) { printf ("penis2\n");goto clean; }
		if (g_cur_entry >= g_cfg_num_files) { g_cur_entry = 0; }
	}
	memset(memsurface_data, 0, 512*512*4);
	memset(used, 0, sizeof(used));
	memset(used2, 0, sizeof(used2));
	cur_mouse_address =0;
	cur_time = 0;
	audio_samples_written = 0;
	last_pc = -1;

	// draw one-time stuff
	if (!g_cfg_novideo)
	{
		SDL_FillRect(screen, NULL, 0);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		memsurface = SDL_CreateRGBSurfaceFrom(memsurface_data,512,512,32,2048,0xFF000000,0x00FF0000,0x0000FF00,0x0);
#else
		memsurface = SDL_CreateRGBSurfaceFrom(memsurface_data,512,512,32,2048,0xFF,0xFF00,0xFF0000,0x0);		
#endif
	
		tmprect.x = MEMORY_VIEW_X-1;
		tmprect.y = MEMORY_VIEW_Y-1;
		tmprect.w = 512+2;
		tmprect.h = 512+2;
		SDL_FillRect(screen, &tmprect, color_screen_white);	
		
		sdlfont_drawString(screen, MEMORY_VIEW_X, MEMORY_VIEW_Y-10, "spc memory:", color_screen_white);

		sprintf(tmpbuf, " QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK");
		sdlfont_drawString(screen, 0, screen->h-9, tmpbuf, color_screen_yellow);

		/* information */
		sdlfont_drawString(screen, INFO_X, INFO_Y, "      - Info -", color_screen_white);
		sprintf(tmpbuf, "Filename: %s", g_real_filename);
		sdlfont_drawString(screen, INFO_X, INFO_Y+8, tmpbuf, color_screen_white);
		sprintf(tmpbuf, "Title...: %s", tag.title);
		sdlfont_drawString(screen, INFO_X, INFO_Y+16, tmpbuf, color_screen_white);
		sprintf(tmpbuf, "Game....: %s", tag.game_title);
		sdlfont_drawString(screen, INFO_X, INFO_Y+24, tmpbuf, color_screen_white);
		sprintf(tmpbuf, "Dumper..: %s", tag.name_of_dumper);
		sdlfont_drawString(screen, INFO_X, INFO_Y+32, tmpbuf, color_screen_white);
		sprintf(tmpbuf, "Comment.: %s", tag.comments);
		sdlfont_drawString(screen, INFO_X, INFO_Y+40, tmpbuf, color_screen_white);
		
		sprintf(tmpbuf, "Echo....: %s", spc_config.is_echo ? "On" : "Off");	
		sdlfont_drawString(screen, INFO_X, INFO_Y+56, tmpbuf, color_screen_white);

		sprintf(tmpbuf, "Interp. : %s", spc_config.is_interpolation ? "On" : "Off");	
		sdlfont_drawString(screen, INFO_X, INFO_Y+64, tmpbuf, color_screen_white);
	
		sprintf(tmpbuf, "Autowrite mask.: %s", g_cfg_autowritemask ? "Yes" : "No");
		sdlfont_drawString(screen, INFO_X, INFO_Y+72, tmpbuf, color_screen_white);

		sprintf(tmpbuf, "Ignore tag time: %s", g_cfg_ignoretagtime ? "Yes" : "No");
		sdlfont_drawString(screen, INFO_X, INFO_Y+80, tmpbuf, color_screen_white);

		sprintf(tmpbuf, "Default time...: %d", g_cfg_defaultsongtime);
		sdlfont_drawString(screen, INFO_X, INFO_Y+88, tmpbuf, color_screen_white);

		
		sdlfont_drawString(screen, PORTTOOL_X, PORTTOOL_Y, "     - Port tool -", color_screen_white);
	}


	song_started_ticks = 0;

	if (!g_cfg_nosound) {
		SDL_PauseAudio(0);
	}
	g_paused = 0;
  for (;;) 
	{
		SDL_Event ev;

		pack_mask(packed_mask);
		
		if (g_cfg_statusline) {
			printf("%s  %d / %d (%d %%)        \r", 
					g_cfg_playlist[g_cur_entry],
					audio_samples_written/44100,
					song_time,
					(audio_samples_written/44100)/(song_time));
			fflush(stdout);
		}
		
		/* Check if it is time to change tune.
		 */		
		if (audio_samples_written/44100 >= song_time) 
		{
			goto skip;
			if (g_cfg_autowritemask) {
				write_mask(packed_mask);
				if (g_cfg_apply_block) {
					printf("Applying mask on file %s using $%02X as filler\n", g_cfg_playlist[g_cur_entry], g_cfg_filler);
					applyBlockMask(g_cfg_playlist[g_cur_entry]);
				}
			}
			if (!g_cfg_nosound) {
				SDL_PauseAudio(1);
			}
			g_cur_entry++;
			if (g_cur_entry>=g_cfg_num_files) { printf ("penis3\n"); } //goto clean; }
			goto reload;
		}
		skip:
		
		if (!g_cfg_novideo)
		{
			//if (0))
			while (SDL_PollEvent(&ev))
			{
				dblclick::check_event(&ev);
				switch (ev.type)
				{
					case SDL_QUIT:
						if (!g_cfg_nosound) {
							SDL_PauseAudio(1);
						}
						printf ("penis4\n");
						goto clean;
						break;
					case SDL_MOUSEMOTION:
						{
							if (mode == MODE_NAV)
							{
								if (	ev.motion.x >= MEMORY_VIEW_X && 
										ev.motion.x < MEMORY_VIEW_X + 512 &&
										ev.motion.y >= MEMORY_VIEW_Y &&
										ev.motion.y < MEMORY_VIEW_Y + 512)
								{
									int x, y;
									x = ev.motion.x - MEMORY_VIEW_X;
									y = ev.motion.y - MEMORY_VIEW_Y;
									x /= 2;
									y /= 2;
									cur_mouse_address = y*256+x;
									if (!hexdump_locked) {
										hexdump_address = cur_mouse_address;
									}
		//							printf("%d,%d: $%04X\n", x, y, y*256+x);
								}
								else
								{
									//cur_mouse_address = -1;
								}
							}
						
						}
						break;
					case SDL_KEYDOWN:
					{
						if (mode == MODE_NAV)
						{
							if (ev.key.keysym.sym == SDLK_ESCAPE)
							{
								if (hexdump_locked)
								{
									hexdump_locked = 0;
								}
								else
								{
									if (!g_cfg_nosound) {
										SDL_PauseAudio(1);
									}
									//printf ("penis4\n");
									goto clean;
								}
							}
						}
						else if (mode == MODE_EDIT)
						{
							int scancode = ev.key.keysym.sym;
							if ( (scancode >= '0') && (scancode <= '9') || ((scancode >= 'A') && (scancode <= 'F')) || 
								((scancode >= 'a') && (scancode <= 'f')))
							{
								uint i;

								if ((scancode >= '0') && (scancode <= '9'))
									i = scancode - '0';
								else if ((scancode >= 'A') && (scancode <= 'F'))
									i = (scancode - 'A') + 0x0a;
								else if ((scancode >= 'a') && (scancode <= 'f'))
									i = (scancode - 'a') + 0x0a;  

								if (mouse_hexdump::highnibble)
								{
									i <<= 4;
									i &= 0xf0;
									//IAPU.RAM[hexdump_address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] &= i;
									IAPU.RAM[hexdump_address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] &= 0x0f;
								}
								else
								{
									i &= 0x0f;
									IAPU.RAM[hexdump_address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] &= 0xf0;
								}

								IAPU.RAM[hexdump_address+(mouse_hexdump::res_y*8)+mouse_hexdump::res_x] |= i;
								
								
								mouse_hexdump::inc_cursor_pos();
	    					
							}
					    else if ((scancode == SDLK_SPACE))
					    {

					    }
					    else if ((scancode == SDLK_TAB))
					    {

					    }
					    else if ((scancode == SDLK_BACKSPACE))
					    {

					    }
					    else if ((scancode == SDLK_LEFT))
					    {
					    	mouse_hexdump::dec_cursor_pos();
					    }
					    else if ((scancode == SDLK_RIGHT))
					    {
					    	mouse_hexdump::inc_cursor_pos();
					    }

							if (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_RETURN)
							{
								mode = MODE_NAV;
								mouse_hexdump::stop_timer();
								//hexdump_locked=0;
							}
							
							


						}
						
						
					}
						break;

					
					case SDL_USEREVENT:
					{
						//fprintf(stderr, "DERP");
						

						if (ev.user.code == UserEvents::mouse_react)
    				{
							//fprintf (stderr, ",2,");
				      
				      //top-left of memory write-region: p_x = 584, tmp_y = 229
				      //bottom-right of memory write-region: p_x = 724, tmp_y = 364

				      //fprintf(stderr, "x = %d, y = %d\n", ev->user.data1, ev->user.data2);
				      //ev.motion.x = (long)(ev.user.data1);  // possible pointer size issues here
				      //ev.motion.y = (long)(ev.user.data2);
				      SDL_Event *te = (SDL_Event *)ev.user.data1;
				      if (te->motion.x >= (MOUSE_HEXDUMP_START_X - 2) && te->motion.x <= (MOUSE_HEXDUMP_END_X + 2) &&
				        te->motion.y >= MOUSE_HEXDUMP_START_Y && te->motion.y <= MOUSE_HEXDUMP_END_Y)
				      {
				        // editor stuffz
				        mode = MODE_EDIT;
				        hexdump_locked = 1;

				        const int entry_width = MOUSE_HEXDUMP_ENTRY_X_INCREMENT;
				        const int entry_height = MOUSE_HEXDUMP_ENTRY_Y_INCREMENT;

				        mouse_hexdump::rel_x = te->motion.x - MOUSE_HEXDUMP_START_X;
				        mouse_hexdump::rel_x+=2;
				        mouse_hexdump::rel_y = te->motion.y - MOUSE_HEXDUMP_START_Y;

				        mouse_hexdump::res_x = mouse_hexdump::rel_x / entry_width;
				        mouse_hexdump::res_y = mouse_hexdump::rel_y / entry_height;
				        int res_half = mouse_hexdump::rel_x % entry_width;
				        int tmp = entry_width / 2;
				        
				        if (res_half < tmp) mouse_hexdump::highnibble = 1;
				        else mouse_hexdump::highnibble = 0;

				        if (mouse_hexdump::res_y == 16) mouse_hexdump::res_y = 15;

				        fprintf (stderr, "(%d,%d, %d), ", mouse_hexdump::res_x, mouse_hexdump::res_y, mouse_hexdump::highnibble);

				        // mouse_hexdump::cursor_toggle = 1; done in start_timer
				        mouse_hexdump::stop_timer();
				        mouse_hexdump::start_timer();

				      }
				    }
					} break;
					case SDL_MOUSEBUTTONDOWN:						
						{
							if (mode == MODE_NAV)
							{
								// click in memory view. Toggle lock
								if (	ev.motion.x >= MEMORY_VIEW_X && 
										ev.motion.x < MEMORY_VIEW_X + 512 &&
										ev.motion.y >= MEMORY_VIEW_Y &&
										ev.motion.y < MEMORY_VIEW_Y + 512 )
								{
									hexdump_locked = (!hexdump_locked);
								}
							}

							/* porttool */
							if (	(ev.button.x >= PORTTOOL_X + (8*5)) &&
									ev.button.y >= PORTTOOL_Y)
							{
								int x, y;
								x = ev.button.x - (PORTTOOL_X + (8*5));
								x /= 8;
								y = ev.button.y - PORTTOOL_Y;
								y /= 8;
								if (y==1) 
//								printf("%d\n", x);
								if (ev.button.button == SDL_BUTTON_LEFT)
								{
									switch (x)
									{
										case 1: IAPU.RAM[0xf4]++; break;
										case 4: IAPU.RAM[0xf4]--; break;
										case 6: IAPU.RAM[0xf5]++; break;
										case 9: IAPU.RAM[0xf5]--; break;
										case 11: IAPU.RAM[0xf6]++; break;
										case 14: IAPU.RAM[0xf6]--; break;
										case 16: IAPU.RAM[0xf7]++; break;
										case 19: IAPU.RAM[0xf7]--; break;
									}
								}
								if (ev.button.button == SDL_BUTTON_WHEELUP ||
									ev.button.button == SDL_BUTTON_WHEELDOWN)
								{
									if (ev.button.button == SDL_BUTTON_WHEELUP) { i=1; } else { i = -1; }
									if (x>1 && x<4) { IAPU.RAM[0xf4] += i; }
									if (x>6 && x<9) { IAPU.RAM[0xf5] += i; }
									if (x>11 && x<14) { IAPU.RAM[0xf6] += i; }
									if (x>16 && x<19) { IAPU.RAM[0xf7] += i; }
								}
							}	

							/* menu bar */
							if (
								((ev.button.y >screen->h-12) && (ev.button.y<screen->h)))
							{
								int x = ev.button.x / 8;
								if (x>=1 && x<=4) { printf ("penis5\n");goto clean; } // exit
								if (x>=8 && x<=12) { 
									if (g_paused) { 
										g_paused = 0; SDL_PauseAudio(0); 
									} else {
										g_paused = 1; SDL_PauseAudio(1);
									}
								} // pause

								if (x>=16 && x<=22) {  // restart
									SDL_PauseAudio(1);
									goto reload;
								}

								if (x>=26 && x<=29) {  // prev
									SDL_PauseAudio(1);
									g_cur_entry--;
									if (g_cur_entry<0) { g_cur_entry = g_cfg_num_files-1; }
									goto reload;

								}

								if (x>=33 && x<=36) { // next
									SDL_PauseAudio(1);
									g_cur_entry++;
									if (g_cur_entry>=g_cfg_num_files) { g_cur_entry = 0; }
									goto reload;
								}

								if (x>=41 && x<=50) { // write mask
									write_mask(packed_mask);
								}
							}
						}
						break;
						default:
						//fprintf(stderr, "type = %d, SDL_USEREVENT = %d\n", ev.type, SDL_USEREVENT);
						break;
				}
			} // while (pollevent)
			
		} // !g_cfg_novideo

		if (g_cfg_nosound) {			
			SPC_update(&audiobuf[audio_buf_bytes]);			
			audio_samples_written += spc_buf_size/4; // 16bit stereo
			SPC_update(&audiobuf[audio_buf_bytes]);			
			audio_samples_written += spc_buf_size/4; // 16bit stereo
			SPC_update(&audiobuf[audio_buf_bytes]);			
			audio_samples_written += spc_buf_size/4; // 16bit stereo
			SPC_update(&audiobuf[audio_buf_bytes]);			
			audio_samples_written += spc_buf_size/4; // 16bit stereo
		}
		else
		{	
			if (!g_cfg_update_in_callback && !g_paused)
			{
				// fill the buffer when possible
				updates = 0;
			
				while (BUFFER_SIZE - audio_buf_bytes >= spc_buf_size )
				{
					if (!g_cfg_novideo) {
						if (SDL_MUSTLOCK(memsurface)) {
							SDL_LockSurface(memsurface);
						}
					}
					while (BUFFER_SIZE - audio_buf_bytes >= spc_buf_size) {						
						SDL_LockAudio();						
						SPC_update(&audiobuf[audio_buf_bytes]);						
						SDL_UnlockAudio();
						audio_buf_bytes += spc_buf_size;
					}
					
					if (!g_cfg_novideo) {
						if (SDL_MUSTLOCK(memsurface)) {
							SDL_UnlockSurface(memsurface);
						}
					}					
				}
			}

		}

//return 0; // not reached		

		if (!g_cfg_novideo)
		{
			time_cur = SDL_GetTicks();
			do_scroller(time_cur - time_last);
			
			fade_arrays();			
			memrect.x = MEMORY_VIEW_X; memrect.y = MEMORY_VIEW_Y;
			
			// draw the memory read/write display area
			SDL_BlitSurface(memsurface, NULL, screen, &memrect);	

			// draw the 256 bytes block usage bar
			tmprect.x = MEMORY_VIEW_X-3;
			tmprect.w = 2; tmprect.h = 2;
			tmp=0;
			for (i=0; i<256; i++)
			{
				tmprect.y = MEMORY_VIEW_Y + i * 2;
				if (used2[i])
				{
					SDL_FillRect(screen, &tmprect, color_screen_white);	
					tmp++;
				}
			}
			
			sprintf(tmpbuf, "Blocks used: %3d/256 (%.1f%%)  ", tmp, (float)tmp*100.0/256.0);
			sdlfont_drawString(screen, MEMORY_VIEW_X, MEMORY_VIEW_Y + memsurface->h + 2, tmpbuf, color_screen_white);

			if (1)
			{
				sprintf(tmpbuf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
						packed_mask[0], packed_mask[1], packed_mask[2], packed_mask[3],
						packed_mask[4], packed_mask[5], packed_mask[6], packed_mask[7],
						packed_mask[8], packed_mask[9], packed_mask[10], packed_mask[11],
						packed_mask[12], packed_mask[13], packed_mask[14], packed_mask[15],
						packed_mask[16], packed_mask[17], packed_mask[18], packed_mask[19],
						packed_mask[20], packed_mask[21], packed_mask[22], packed_mask[23],
						packed_mask[24], packed_mask[25], packed_mask[26], packed_mask[27],
						packed_mask[28], packed_mask[29], packed_mask[30], packed_mask[31]);

				sdlfont_drawString(screen, MEMORY_VIEW_X, MEMORY_VIEW_Y + memsurface->h + 2 + 9, tmpbuf, color_screen_white);
			}
			i = 32;

			// write the address under mouse cursor
			if (cur_mouse_address >=0)
			{
				sprintf(tmpbuf, "Addr mouse: $%04X", cur_mouse_address);
				sdlfont_drawString(screen, MEMORY_VIEW_X+8*(23), MEMORY_VIEW_Y-10, tmpbuf, color_screen_white);
			}

			// write the program counter
			last_pc = (int)(IAPU.PC - IAPU.RAM);
			sprintf(tmpbuf, "PC: $%04x  ", last_pc);
			sdlfont_drawString(screen, MEMORY_VIEW_X+8*12, MEMORY_VIEW_Y-10, tmpbuf, color_screen_white);

			tmp = i+10; // y 
			
			sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp, "Voices pitchs:", color_screen_white);
			tmp += 9;
			
			tmprect.x=MEMORY_VIEW_X+520;
			tmprect.y=tmp;
			tmprect.w=screen->w-tmprect.x;
			tmprect.h=8*8;
			SDL_FillRect(screen, &tmprect, color_screen_black);
			tmprect.w=5;
			tmprect.h = 5;
			for (i=0; i<8; i++)
			{
				unsigned short pitch = APU.DSP[2+(i*0x10)] | (APU.DSP[3+(i*0x10)]<<8);
				Uint32 cur_color;
			
				if (APU.DSP[0x5c]&(1<<i)) {
					cur_color = color_screen_white;
				} else {
					cur_color = color_screen_gray;
				}
				
				sprintf(tmpbuf,"%d:",i);
				sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp + (i*8), tmpbuf, color_screen_white);
				
				tmprect.y= tmp+(i*8)+2;
				tmprect.x = MEMORY_VIEW_X+520+18;
				tmprect.x += pitch*(screen->w-tmprect.x-20)/((0x10000)>>2);
				SDL_FillRect(screen, &tmprect, color_screen_white);
				
			}
			tmp += 9*8;
			
			sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp, "Voices volumes:", color_screen_white);
			sdlfont_drawString(screen, MEMORY_VIEW_X+520+(16*8), tmp, "Left", color_screen_yellow);
			sdlfont_drawString(screen, MEMORY_VIEW_X+520+(20*8)+4, tmp, "Right", color_screen_cyan);
			sdlfont_drawString(screen, MEMORY_VIEW_X+520+(26*8), tmp, "Gain", color_screen_magenta);
			tmp += 9;

			tmprect.x=MEMORY_VIEW_X+520;
			tmprect.y=tmp;
			tmprect.w=screen->w-tmprect.x;
			tmprect.h=8*10;
			SDL_FillRect(screen, &tmprect, color_screen_black);		
			tmprect.w=2;
			tmprect.h=2;
			for (i=0; i<8; i++)
			{
				unsigned char left_vol = APU.DSP[0+(i*0x10)];
				unsigned char right_vol = APU.DSP[1+(i*0x10)];
				unsigned char gain = APU.DSP[7+(i*0x10)];

				sprintf(tmpbuf,"%d:", i);
				sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp + (i*10), tmpbuf, color_screen_white);
				tmprect.x = MEMORY_VIEW_X+520+18;
				tmprect.y = tmp+(i*10);
				tmprect.w = left_vol*(screen->w-tmprect.x-20)/255;

				SDL_FillRect(screen, &tmprect, color_screen_yellow);

				tmprect.x = MEMORY_VIEW_X+520+18;
				tmprect.w = right_vol*(screen->w-tmprect.x-20)/255;
				tmprect.y = tmp+(i*10)+3;
				
				SDL_FillRect(screen, &tmprect, color_screen_cyan);

				tmprect.x = MEMORY_VIEW_X+520+18;
				tmprect.w = gain * (screen->w-tmprect.x-20)/255;
				tmprect.y = tmp+(i*10)+6;
				SDL_FillRect(screen, &tmprect, color_screen_magenta);
			}
			tmp += 8*10 + 8;

			sdlfont_drawString(screen, MEMORY_VIEW_X+520, tmp, "  - Mouseover Hexdump -", color_screen_white);
			if (hexdump_locked) {
				sdlfont_drawString(screen, MEMORY_VIEW_X+520+24*8, tmp, "locked", color_screen_red);
			} else {
				sdlfont_drawString(screen, MEMORY_VIEW_X+520+24*8, tmp, "      ", color_screen_red);
			}
			
			tmp+=9;
			if (cur_mouse_address>=0)
			{
				
				for (i=0; i<128; i+=8)
				{

					unsigned char *st = &IAPU.RAM[hexdump_address+i];
					// *st = ;
					int p = MEMORY_VIEW_X+520, j;
					//printf ("top_left of rect p = ")
					if ((hexdump_address+i) > 0xffff)
						sprintf(tmpbuf, "    : ");
					else
						sprintf(tmpbuf, "%04X: ", hexdump_address+i);
					sdlfont_drawString(screen, p, tmp, tmpbuf, color_screen_white);
					p += 6*8;
					//if (i==0)
						//printf ("top-left of memory write-region: p_x = %d, tmp_y = %d\n", p, tmp); // top-left of memory write-region
					for (j=0; j<8; j++) {
						int idx = ((hexdump_address+i+j)&0xff00)<<4;	
						Uint32 color;

						idx += ((hexdump_address+i+j) % 256)<<3;
						color = SDL_MapRGB(screen->format, 
								0x7f + (memsurface_data[idx]>>1), 
								0x7f + (memsurface_data[idx+1]>>1), 
								0x7f + (memsurface_data[idx+2]>>1)
								);
								
						if ((hexdump_address+i+j) > 0xffff)
							sprintf(tmpbuf, "   ");
						else sprintf(tmpbuf, "%02X ", *st);
						st++;

						sdlfont_drawString(screen, p, tmp, tmpbuf, color);
						//if ( j == 7 && i == 120 )
							//printf ("bottom-right of memory write-region: p_x = %d, tmp_y = %d\n", p, tmp);
						p+= 2*8 + 4;
					}
					
					tmp += 9;
				}
				if (mouse_hexdump::cursor_toggle)
				{
					if (mouse_hexdump::highnibble)
	        {
	          sdlfont_drawString(screen, MOUSE_HEXDUMP_START_X + (mouse_hexdump::res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT), MOUSE_HEXDUMP_START_Y + (mouse_hexdump::res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), "\x5B", color_screen_green);
	        }
	        else
	        {
	          sdlfont_drawString(screen, MOUSE_HEXDUMP_START_X + (mouse_hexdump::res_x * MOUSE_HEXDUMP_ENTRY_X_INCREMENT + (7)), MOUSE_HEXDUMP_START_Y + (mouse_hexdump::res_y * MOUSE_HEXDUMP_ENTRY_Y_INCREMENT), "\x5B", color_screen_green);
	        }
	      }
			}


			sdlfont_drawString(screen, PORTTOOL_X, PORTTOOL_Y+8, " APU:", color_screen_white);
			sdlfont_drawString(screen, PORTTOOL_X, PORTTOOL_Y+16, "SNES:", color_screen_white);

			sprintf(tmpbuf, " +%02X- +%02X- +%02X- +%02X-", IAPU.RAM[0xf4], IAPU.RAM[0xf5], IAPU.RAM[0xf6], IAPU.RAM[0xf7]);		
			sdlfont_drawString(screen, PORTTOOL_X + (8*5), PORTTOOL_Y+8, tmpbuf, color_screen_white);
			
			sprintf(tmpbuf, "  %02X   %02X   %02X   %02X", APU.OutPorts[0], APU.OutPorts[1], APU.OutPorts[2], APU.OutPorts[3]);		
			sdlfont_drawString(screen, PORTTOOL_X + (8*5), PORTTOOL_Y+16, tmpbuf, color_screen_white);

			current_ticks = audio_samples_written/44100;
			sprintf(tmpbuf, "Time....: %0d:%02d / %0d:%02d", 
					(current_ticks-song_started_ticks)/60,
					((current_ticks-song_started_ticks))%60,
					song_time/60, song_time%60);
			sdlfont_drawString(screen, INFO_X, INFO_Y+48, tmpbuf, color_screen_white);

			
			
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			time_last = time_cur;
			if (g_cfg_nice) {  SDL_Delay(100); }
		} // if !g_cfg_novideo
	}
clean:
	SDL_PauseAudio(1);
	SDL_Quit();
    SPC_close();

    return 0;
}


