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

#include "gme/blargg_source.h"

#include "debugger/globals.h"
#include "DEBUGLOG.h"
#include "sdl_userevents.h"

// Number of audio buffers per second. Adjust if you encounter audio skipping.
const int fill_rate = 64; //45;

bool Music_Player::exporting = false;
double Music_Player::min_gain_db=-96.0, Music_Player::max_gain_db = 20.0;

// Simple sound driver using SDL
typedef void (*sound_callback_t)( void* data, short* out, int count );
static const char* sound_init( long sample_rate, int buf_size, sound_callback_t, void* data, const char *audio_out_dev=NULL );
//static void sound_start();
//static void sound_stop();
//static void sound_cleanup();

void Music_Player::set_gain_db(gain_t new_gain_db, bool immediate/*=false*/)
{
	this->new_gain_db = new_gain_db;

	if (!immediate)
		gain_has_changed = true;
	else
		gain_db = new_gain_db;
}

void Music_Player::spc_write(int addr, int data)
{
	//sound_stop();
	if (addr == 0xf3)
		spc_write_dsp(spc_emu_->ram()[0xf2], data);
	else spc_emu_->write(addr, data, 1);
	//sound_start();
}
uint8_t Music_Player::spc_read(int addr)
{
	//sound_stop();
	uint8_t v = spc_emu_->read(addr, 1);
	//sound_start();
	return v;
}
uint8_t Music_Player::spc_read_dsp(int dspaddr)
{
	//spc_emu_->write(0xf2, dspaddr, 1);
	//return spc_emu_->read(0xf3);
	//sound_stop();
	uint8_t v = spc_emu_->read_dsp(dspaddr);
	//sound_start();
	return v;
}
void Music_Player::spc_write_dsp(int dspaddr, int val)
{
	//spc_emu_->write(0xf2, dspaddr, 1);
	//return spc_emu_->read(0xf3);
	//sound_stop();
	spc_emu_->write_dsp(dspaddr, val);
	//sound_start();
}

void Music_Player::inc_ram(int addr, int i/*=1*/)
{
  spc_write(addr, (spc_read(addr))+i);
}
void Music_Player::dec_ram(int addr, int i/*=1*/)
{
  spc_write(addr, (spc_read(addr))-i);
}



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
	//fade_out();
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

Music_Player::Music_Player() :
	gain_db(0.0),
	track_started(false),
	spc_filter(new Spc_Filter),
	emu_(0),
	paused(false),
	scope_buf(0),
	curtrack(0),
	filetrack(0)
{
	spc_filter->set_gain(Spc_Filter::gain_unit * 1);
	spc_filter->set_bass(Spc_Filter::bass_max);
}

blargg_err_t Music_Player::init( long rate, const char *audio_out_dev/*=NULL*/ )
{
	DEBUGLOG("Music_Player::init\n");
	bool change=false;
	if (Audio_Context::audio->devices.id)
	{
		//if (emu_)
			//stop();
		sound_cleanup();
		change = true;
	}
	sample_rate = rate;
	
	//int min_size = sample_rate * 2 / fill_rate;
	int sample_frame_size = 512 * pow(2,2);
	stereo_bufs_per_sec = (double)sample_rate / (double)sample_frame_size;
	//if (fill_rate % )

	DEBUGLOG("samples = %d\n", sample_frame_size);
	DEBUGLOG("audio buffs per sec = %0.2f\n", stereo_bufs_per_sec);


	blargg_err_t err= sound_init( sample_rate, sample_frame_size, fill_buffer, this, audio_out_dev );

	if (change)
	{
		pause(paused);
	}
	return err;
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
	fade_out(false);
	stop();

	// check if file is m3u 
	this->path = path;
	char* ext;
	char file1[500];
	ext = strrchr((char *)path,'.');
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

	has_no_song = false;

	
	return 0;
}

int Music_Player::track_count() const
{
	return emu_ ? emu_->track_count() : false;
}
// Very simple thread - counts 0 to 9 delaying 50ms between increments
int Music_Player::fade_out(void *ptr)
{
	Music_Player *p = (Music_Player*)ptr;
	p->low_level_fade_out(true);
	//if (paused) sound_stop();
	return 0;
}

void Music_Player::fade_out(bool threaded/*=false*/)
{
	DEBUGLOG("fadeout()\n");
	SDL_AudioStatus audio_status = SDL_GetAudioDeviceStatus(audio->devices.id);
	if (audio_status == SDL_AUDIO_PAUSED || audio_status == SDL_AUDIO_STOPPED)
		return;
	//else if (emu_->tell() >= (track_info_.length))
		//return;
	//if (paused)
		//return;
	DEBUGLOG("\tprocessing\n");

	if (threaded)
	{
		thread = SDL_CreateThread(&Music_Player::fade_out, "FadeOutThread", this);
		SDL_DetachThread(thread); /* will go away on its own upon completion. */
	}
	else 
	{
		low_level_fade_out(false);
	}
}
void Music_Player::low_level_fade_out(bool threaded)
{
	gain_t linear_gain=this->linear_gain; 
	needs_to_fade_out=true;
	while (needs_to_fade_out);
	this->linear_gain = linear_gain;
	
	if (paused)
	{
		if (threaded)
		{
			SDL_Event event2;
		  event2.type = SDL_USEREVENT;
		  event2.user.code = UserEvents::sound_stop;
		  event2.user.data1 = 0;
		  event2.user.data2 = 0;
		  SDL_PushEvent(&event2);
		}
		else
		{
			sound_stop();
		}
		
	}
	//sound_stop();
}

blargg_err_t Music_Player::start_track( int track, bool test/*=false*/ )
{
	//sound_stop();
	// I do this to clean up the sound buffer when "scrollin" to the next track..
	// after a previous track was paused.. you would sometimes hear some of that old song
	// data play before the new track.. temporary fix is to reallocate the sound device..
	// permanent fix would be to add "fadeout after pause" or "fade in after play"
	//tempo = 1.0;
	emu_->set_tempo(tempo);
	//gain_db = 0.0;
	gain_has_changed=false;
	//new_gain_db = 0.0;
	needs_to_fade_out=false;
	//needs_to_fade_in=true;
	//target_linear_gain = gain;
	//linear_gain = 0.0;
	//fade_gain=1.0;
	/*if (paused)
	{
		sound_cleanup();
		init(sample_rate);
	}*/
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
			emu_->set_fade( track_info_.length + 1000 );
			/* I add 1000 above, because without, because I am calling fade_out from load_file,
			which is sometimes called when a song's elapsed time >= it's track length, there was an issue
			when the set_fade was set to the length, where the fade_out routine would take forever to finish
			due to a prolonged period inside the audio callback somehow related to the set_fade parameter..
			by increasing the parameter by 1 second, I avoid the prolonged callback AKA delay after certain
			songs end.. (ie Star Fox Track 1 intro track)
			
			*/
			//if (test)
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


void Music_Player::pause( int b, bool with_fade/*=true*/, bool fade_threaded/*=true*/ )
{
	if (has_no_song)
		return;
	
	if ( b )
	{
		paused = b;
		if (with_fade) fade_out(fade_threaded);
		else sound_stop();
	}
	else
	{
		paused = b;
		if (!track_started)
		{
			start_track(curtrack);
		}
		else needs_to_fade_in=true;
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
	//emu_->ignore_silence( mask != 0 );
	resume();
}

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

void Music_Player::apply_gain(sample_t* out, int count )
{
	//double gain; 
	double direction; 
	//static int fade_out_count=0, fade_in_count=0;
	static bool is_fade_in_init=false;
	static gain_t fade_in_gain_db;
	static bool is_fade_out_init=false;
	static gain_t fade_out_gain_db;
	static bool fade_out_finished=false;

	
	for (int i=0; i < count; i += 1)
	{
		double newsamp = out[i];
		
		if (needs_to_fade_in)
		{
			if (!is_fade_in_init)
			{
				is_fade_in_init=true;
				fade_in_gain_db = Music_Player::min_gain_db;
				//fade_in_count=0;
			}

			linear_gain = Audio::calculate_linear_gain_from_db(fade_in_gain_db, min_gain_db);
			
			/*if ( (newsamp == 0 && in[i+1] == 0) || 
				((newsamp <= 0 && in[i+1] > 0) || (newsamp >= 0 && in[i+1] < 0)))
			{*/
				fade_in_gain_db += (+0.01 * ((double)44100/(double)sample_rate));
				if (fade_in_gain_db >= gain_db)
				{
					/*for (i; i < count; i++)
					{
						in[i] = 0;
					}*/
					fade_in_gain_db = gain_db;
					//DEBUGLOG("test23, target = %f, gain=%f",target_gain, gain);
					//fade_in_count=1;
					//fade_in_finished=true;
					needs_to_fade_in=false;
					is_fade_in_init=false;
					//return;
				}		

				
			//}
			/*else
			{
				DEBUGLOG("1) %f 2) %f\n", newsamp, (double)out[i+1]);
			}*/
		}
		else if (needs_to_fade_out)
		{
			if (!is_fade_out_init)
			{
				is_fade_out_init=true;
				fade_out_gain_db = gain_db;
				//fade_out_count=0;
			}
			
			if (!is_using_zero_crossover || (is_using_zero_crossover && ((newsamp == 0 && out[i+1] == 0) || 
				((newsamp <= 0 && out[i+1] > 0) || (newsamp >= 0 && out[i+1] < 0)) ) ) )
			{
				linear_gain = Audio::calculate_linear_gain_from_db(fade_out_gain_db, min_gain_db);

				fade_out_gain_db -= (+0.01 * ((double)44100/(double)sample_rate));
				if (fade_out_gain_db <= Music_Player::min_gain_db)
				{
					for (; i < count; i++)
					{
						out[i] = 0;
					}
					fade_out_gain_db = min_gain_db;
					//DEBUGLOG("test23, target = %f, gain=%f",target_gain, gain);
					//fade_out_count=1;
					fade_out_finished=true;
					//SDL_PauseAudioDevice(Audio_Context::audio->devices.id, 1);  // start audio playing.

					needs_to_fade_out=false;
					is_fade_out_init=false;
					return;
				}		

				
			}
			/*else
			{
				DEBUGLOG("1) %f 2) %f\n", newsamp, (double)out[i+1]);
			}*/
		}
		else if (gain_has_changed)
		{
			direction = ((new_gain_db - gain_db > 0) ? (+0.02 * ((double)44100/(double)sample_rate)) : (-0.02 * ((double)44100/(double)sample_rate)) );
			//DEBUGLOG("direction = %f\n", direction);
			if (!is_using_zero_crossover || (is_using_zero_crossover && ((newsamp == 0 && out[i+1] == 0) || 
				((newsamp <= 0 && out[i+1] > 0) || (newsamp >= 0 && out[i+1] < 0)) ) ) )
			{
				gain_db += direction;
				if ((direction > 0 && gain_db >= new_gain_db) || (direction < 0 && gain_db <= new_gain_db))
				{
					gain_db = new_gain_db;
					gain_has_changed=false;
				}
				linear_gain = Audio::calculate_linear_gain_from_db(gain_db, min_gain_db);
				
			}
		}

		//DEBUGLOG("linear_gain = %f\n", gain);
		newsamp *= linear_gain;
		
		int io = round(newsamp);
		/*if (d > 32767)
		{
			d = 32767;
			//DEBUGLOG("+clip+!");
		}
		else if (d < -32768)
		{
			d = -32768;
			//DEBUGLOG("clip-!");
		}*/
		//CLAMP16(d);
		if ( (int16_t) io != io )
		{
		/*fprintf(stderr,"\t%d : ", io);*/
			io = (io >> 31) ^ 0x7FFF;
		/*fprintf(stderr,"\t%d\n", io);*/
		}
		out[i] = (sample_t)io;
	}
}
void Music_Player::fill_buffer( void* data, sample_t* out, int count )
{
	

	//memset(out, 0, count);
	//DEBUGLOG("out = %lx, count = %d\n", out, count);
	Music_Player* self = (Music_Player*) data;
	if ( self->emu_ )
	{
		if ( self->emu_->play( count, out ) ) { } // ignore error
		if (self->filter_is_active)
			self->spc_filter->run(out, count);
		
		self->apply_gain(out, count);
		
		
		
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
	{
		static int mycount=0;
		static int num = 0;
		static int time_start =0;
		static bool stop=false;

		if (num == 0)
		{
			time_start = SDL_GetTicks();
		}
		
		int time_end = SDL_GetTicks();

		if ((time_end - time_start) >= 1000 && !stop)
		{
			DEBUGLOG("%d buffs a sec, %lu stereo samples processed, count = %d\n", num, (mycount/sizeof(sample_t))/2, count);
			stop=true;
		}
		num++;
		mycount+=count;
		sound_callback( sound_callback_data, (sample_t*) out, count / 2 );
	}
}

static const char* sound_init( long sample_rate, int buf_size,
		sound_callback_t cb, void* data, const char *audio_out_dev/*=NULL*/ )
{
	sound_callback = cb;
	sound_callback_data = data;
	
	static SDL_AudioSpec as, have; // making static clears all fields to 0
	as.freq     = sample_rate;
	as.format   = AUDIO_S16LSB;
	as.channels = 2;
	as.callback = sdl_callback;
	as.samples  = buf_size;
	//DEBUGLOG("herpa %s\n",Audio_Context::audio->devices.device_strings[1]);
	//char *audio_out_dev = ;
	if (audio_out_dev)
	{
		bool match=false;
		for (int i=0; i < Audio::Devices::how_many; i++)
		{
			if (!strcmp(audio_out_dev, Audio_Context::audio->devices.device_strings[i]))
			{
				match = true;
				break;
			}
		}
		if (!match)
			audio_out_dev = Audio_Context::audio->devices.device_strings[0];
	}
	else audio_out_dev = Audio_Context::audio->devices.device_strings[0];


	DEBUGLOG("Opening Audio out Device: %s\n", audio_out_dev);
	Audio::Devices::selected_audio_out_dev = audio_out_dev;
	Audio_Context::audio->devices.id = SDL_OpenAudioDevice(audio_out_dev, Audio::Devices::playback, &as, &have, 0 );
	if (Audio_Context::audio->devices.id  == 0 )
	{
		const char* err = SDL_GetError();
		if ( !err )
			err = "Couldn't open SDL audio";
		return err;
	}
	else 
	{
    if (have.format != as.format)  // we let this one thing change.
      DEBUGLOG("\tas.format = %d, have.format = %d\n", as.format, have.format);
    if (have.freq != as.freq)  // we let this one thing change.
      DEBUGLOG("\tas.freq = %d, have.freq = %d\n", as.freq, have.freq);
    if (have.channels != as.channels)  // we let this one thing change.
      DEBUGLOG("\tas.channels = %d, have.channels = %d\n", as.channels, have.channels);
    if (have.samples != as.samples)  // we let this one thing change.
      DEBUGLOG("\tas.samples = %d, have.samples = %d\n", as.samples, have.samples);

    //SDL_PauseAudioDevice(Audio_Context::audio->devices.id, 0);  // start audio playing.
    //SDL_Delay(5000);  // let the audio callback play some sound for 5 seconds.
    //SDL_CloseAudioDevice(Audio_Context::audio->devices.id);
	}

	DEBUGLOG("aspec size = %d\n", have.size);
	
	return 0;
}

void sound_start()
{
	//SDL_UnlockAudio();
	if (!Music_Player::exporting)
		SDL_PauseAudioDevice(Audio_Context::audio->devices.id, 0);
}

void sound_stop()
{
	SDL_PauseAudioDevice(Audio_Context::audio->devices.id, 1);
	
	// be sure audio thread is not active
	//SDL_LockAudio();
	SDL_LockAudioDevice(Audio_Context::audio->devices.id);
	SDL_UnlockAudioDevice(Audio_Context::audio->devices.id);
}

void sound_cleanup()
{
	sound_stop();
	SDL_CloseAudioDevice(Audio_Context::audio->devices.id);
	Audio_Context::audio->devices.id=0;
}


