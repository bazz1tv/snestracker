// Game_Music_Emu 0.5.2. http://www.slack.net/~ant/

#include "Music_Player.h"

#include "gme/Music_Emu.h"

#include <string.h>
#include <ctype.h>

#include <stdio.h>

/* Copyright (C) 2005-2006 by Shay Green. Permission is hereby granted, free of
charge, to any person obtaining a copy of this software module and associated
documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the
following conditions: The above copyright notice and this permission notice
shall be included in all copies or substantial portions of the Software. THE
SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "blargg_source.h"

// Number of audio buffers per second. Adjust if you encounter audio skipping.
const int fill_rate = 45;

// Simple sound driver using SDL
typedef void (*sound_callback_t)( void* data, short* out, int count );
static const char* sound_init( long sample_rate, int buf_size, sound_callback_t, void* data );
static void sound_start();
static void sound_stop();
static void sound_cleanup();



void Music_Player::restart_track()
{
	start_track(curtrack);
}

void Music_Player::play_prev()
{
	if (files.empty())
		return;
	if (filetrack == 0)
		filetrack = files.size() - 1;
	else filetrack--;

	const char *str = files.at(filetrack).c_str();
	load_file(str);
	start_track(0);
}

void Music_Player::play_next()
{
	if (files.empty())
		return;
	if (filetrack == (files.size()-1))
		filetrack = 0;
	else filetrack++;

	const char *str = files.at(filetrack).c_str();
	load_file(str);
	start_track(0);
}

int Music_Player::get_curtrack() { return curtrack; }
void Music_Player::inc_curtrack() { curtrack++; }
void Music_Player::dec_curtrack() { curtrack--; }

Music_Player::Music_Player()
{
	emu_          = 0;
	scope_buf     = 0;
	paused        = false;
	curtrack 	    = 0;
	filetrack	    = 0;
	track_started = false;
	spc_filter = new Spc_Filter;
	spc_filter->set_gain(Spc_Filter::gain_unit * 1);
	spc_filter->set_bass(Spc_Filter::bass_norm);
}

blargg_err_t Music_Player::init( long rate )
{
	sample_rate = rate;
	
	int min_size = sample_rate * 2 / fill_rate;
	int buf_size = 512;
	while ( buf_size < min_size )
		buf_size *= 2;
	
	return sound_init( sample_rate, buf_size, fill_buffer, this );
}

void Music_Player::stop()
{
	sound_stop();
	delete emu_;
	emu_ = 0;
}

Music_Player::~Music_Player()
{
	stop();
	gme_delete(emu_);
	sound_cleanup();
	delete spc_filter;
}

blargg_err_t Music_Player::load_file( const char* path )
{
	stop();

	// check if file is m3u 
	this->path = path;
	char* ext;
	char file1[500];
	ext = strrchr(path,'.');
	if (ext == NULL)
		return "no . in filename";
	ext++;

	if (!strcmp(ext, "m3u"))
	{
		FILE *fp = fopen(path, "r");
		if (fp == NULL)
			return "cant open file";
		while (fgets(file1, 500, fp) != NULL)
		{	
			// strip off new line
			int i = strlen(file1);

			if (file1[i-1] == 0x0a) file1[i-1] = 0;
			std::string s (file1);
			files.push_back(s);
		}
		//handle_error(player->load_file(files.at(0).c_str()));
		path = files.at(0).c_str();
	}
	//else handle_error( player->load_file( path ) ); 
	
	RETURN_ERR( gme_open_file( path, &emu_, sample_rate ) );

	spc_emu_ = (Spc_Emu*)emu_;

	/*for (int i=i; i < track_count(); i++)
	{
		track_info_t info;
		handle_error( gme_track_info( emu_, &info, i ) );
		fprintf(stderr, )
	}*/

	
	/*char m3u_path [256 + 5];
	strncpy( m3u_path, path, 256 );
	m3u_path [256] = 0;
	char* p = strrchr( m3u_path, '.' );
	if ( !p )
		p = m3u_path + strlen( m3u_path );
	strcpy( p, ".m3u" );
	if ( emu_->load_m3u( m3u_path ) ) { } // ignore error*/
	
	return 0;
}

int Music_Player::track_count() const
{
	return emu_ ? emu_->track_count() : false;
}

blargg_err_t Music_Player::start_track( int track )
{
	// I do this to clean up the sound buffer when "scrollin" to the next track..
	// after a previous track was paused.. you would sometimes hear some of that old song
	// data play before the new track.. temporary fix is to reallocate the sound device..
	// permanent fix would be to add "fadeout after pause" or "fade in after play"
	if (paused)
	{
		sound_cleanup();
		init(sample_rate);
	}
	spc_filter->clear();

	curtrack = track;
	if ( emu_ )
	{
		// Sound must not be running when operating on emulator
		sound_stop();
		if (paused)
		{
			track_started = false;
			//
			Music_Emu *tmp;
			gme_open_file(path.c_str(), &tmp, gme_info_only);
			tmp->start_track(track);
			
			// Calculate track length
			if ( !tmp->track_info( &track_info_ ) )
			{
				if ( track_info_.length <= 0 )
					track_info_.length = track_info_.intro_length +
							track_info_.loop_length * 2;
			}
			// = derp;
			//fprintf(stderr, "game = %s", derp.game);
			if ( track_info_.length <= 0 )
				track_info_.length = (long) (2.5 * 60 * 1000);
			//tmp->set_fade( track_info_.length );
			//paused = false;
			gme_delete(tmp);
		}
		else	
		{
			track_started = true;
			RETURN_ERR( emu_->start_track( track ) );
			//track_started = true;
			// Calculate track length
			if ( !emu_->track_info( &track_info_ ) )
			{
				if ( track_info_.length <= 0 )
					track_info_.length = track_info_.intro_length +
							track_info_.loop_length * 2;
			}
			if ( track_info_.length <= 0 )
				track_info_.length = (long) (2.5 * 60 * 1000);
			emu_->set_fade( track_info_.length );
			sound_start();
		}
		
		//fprintf(stderr, "game = %s", track_info_.game);
		
		//paused = false;
		//pause(pause);

		
		//pause(paused);
	}
	return 0;
}

void Music_Player::toggle_pause()
{
	pause(!paused);
	
}

bool Music_Player::is_paused()
{
	return paused;
}

void Music_Player::pause( int b )
{
	paused = b;
	if ( b )
		sound_stop();
	else
	{
		if (!track_started)
		{
			start_track(curtrack);
		}
		sound_start();
	}
}

void Music_Player::suspend()
{
	if ( !paused )
		sound_stop();
}

void Music_Player::resume()
{
	if ( !paused )
		sound_start();
}

bool Music_Player::track_ended() const
{
	return emu_ ? emu_->track_ended() : false;
}

void Music_Player::set_stereo_depth( double tempo )
{
	suspend();
	gme_set_stereo_depth( emu_, tempo );
	resume();
}

void Music_Player::set_tempo( double tempo )
{
	suspend();
	emu_->set_tempo( tempo );
	fprintf(stderr,"%f,", tempo);
	resume();
}

void Music_Player::mute_voices( int mask )
{
	suspend();
	emu_->mute_voices( mask );
	emu_->ignore_silence( mask != 0 );
	resume();
}

void Music_Player::fill_buffer( void* data, sample_t* out, int count )
{
	Music_Player* self = (Music_Player*) data;
	if ( self->emu_ )
	{
		if ( self->emu_->play( count, out ) ) { } // ignore error
		self->spc_filter->run(out, count);
		
		if ( self->scope_buf )
			memcpy( self->scope_buf, out, self->scope_buf_size * sizeof *self->scope_buf );
	}
}

// Sound output driver using SDL

#include "SDL.h"

static sound_callback_t sound_callback;
static void* sound_callback_data;

static void sdl_callback( void* data, Uint8* out, int count )
{
	if ( sound_callback )
		sound_callback( sound_callback_data, (short*) out, count / 2 );
}

static const char* sound_init( long sample_rate, int buf_size,
		sound_callback_t cb, void* data )
{
	sound_callback = cb;
	sound_callback_data = data;
	
	static SDL_AudioSpec as; // making static clears all fields to 0
	as.freq     = sample_rate;
	as.format   = AUDIO_S16SYS;
	as.channels = 2;
	as.callback = sdl_callback;
	as.samples  = buf_size;
	if ( SDL_OpenAudio( &as, 0 ) < 0 )
	{
		const char* err = SDL_GetError();
		if ( !err )
			err = "Couldn't open SDL audio";
		return err;
	}
	
	return 0;
}

static void sound_start()
{
	//SDL_UnlockAudio();
	SDL_PauseAudio( false );
}

static void sound_stop()
{
	SDL_PauseAudio( true );
	
	// be sure audio thread is not active
	SDL_LockAudio();
	SDL_UnlockAudio();
}

static void sound_cleanup()
{
	sound_stop();
	SDL_CloseAudio();
}
