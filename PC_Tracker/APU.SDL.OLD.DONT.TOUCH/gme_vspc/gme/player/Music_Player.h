// Simple game music file player

// Game_Music_Emu 0.5.2
#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "gme/Music_Emu.h"
#include "gme/Spc_Emu.h"
#include <vector>
#include <iostream>
#include "gme/Spc_Filter.h"



// should be in the player
void prev_track();
void next_track();
void toggle_pause();
void restart_track();

//

class Music_Player {
public:
	bool filter_is_active=false;
	// Initialize player and set sample rate
	blargg_err_t init( long sample_rate = 44100 );
	
	// Load game music file. NULL on success, otherwise error string.
	blargg_err_t load_file( const char* path );
	
	// (Re)start playing track. Tracks are numbered from 0 to track_count() - 1.
	blargg_err_t start_track( int track );
	
	// Stop playing current file
	void stop();
	
// Optional functions

	void ignore_silence()
	{
		gme_ignore_silence(emu_, 1);
	}
	// Number of tracks in current file, or 0 if no file loaded.
	int track_count() const;
	
	// Info for current track
	track_info_t const& track_info() const { return track_info_; }
	
	// Pause/resume playing current track.
	void pause( int );
	void toggle_pause();
	bool is_paused();
	
	// True if track ended
	bool track_ended() const;
	
	
	// Pointer to emulator
	Music_Emu* emu() const { return emu_; }
	Spc_Emu* spc_emu() const { return spc_emu_; }

	void spc_write(int addr, int data)
	{
		if (addr == 0xf3)
			spc_write_dsp(spc_emu_->ram()[0xf2], data);
		else spc_emu_->write(addr, data, 1);
	}
	uint8_t spc_read(int addr)
	{
		return spc_emu_->read(addr, 1);
	}
	uint8_t spc_read_dsp(int dspaddr)
	{
		//spc_emu_->write(0xf2, dspaddr, 1);
		//return spc_emu_->read(0xf3);
		return spc_emu_->read_dsp(dspaddr);
	}
	void spc_write_dsp(int dspaddr, int val)
	{
		//spc_emu_->write(0xf2, dspaddr, 1);
		//return spc_emu_->read(0xf3);
		spc_emu_->write_dsp(dspaddr, val);
	}

	void inc_ram(int addr, int i=1)
	{
	  spc_write(addr, (spc_read(addr))+i);
	}
	void dec_ram(int addr, int i=1)
	{
	  spc_write(addr, (spc_read(addr))-i);
	}
	// Set stereo depth, where 0.0 = none and 1.0 = maximum
	void set_stereo_depth( double );
	
	// Set tempo, where 0.5 = half speed, 1.0 = normal, 2.0 = double speed
	void set_tempo( double );
	
	// Set voice muting bitmask
	void mute_voices( int );
	
	// Set buffer to copy samples from each buffer into, or NULL to disable
	typedef short sample_t;
	void set_scope_buffer( sample_t* buf, int size ) { scope_buf = buf; scope_buf_size = size; }

	void inc_curtrack();
	void dec_curtrack();
	void play_prev();
	void play_next();
	void restart_track();
	int get_curtrack();



	
public:
	Music_Player();
	~Music_Player();
	double gain;
	/*void set_path(char *str)
	{
		strcpy(path,str);
	}*/
private:
	Spc_Filter* spc_filter;
	Music_Emu* emu_;
	Spc_Emu* spc_emu_;
	sample_t* scope_buf;

	long sample_rate;
	int scope_buf_size;
	bool paused;
	track_info_t track_info_;
	// byte alignment seems to matter, I've had app crash based on it..
	// so always add variables at the END of the class to not disturb
	int curtrack, filetrack;
	std::vector<std::string> files;	// this is kind of unnecessary..
	std::string path;
	bool track_started;
	
	
	void suspend();
	void resume();
	static void fill_buffer( void*, sample_t*, int );
  void apply_gain(sample_t* out, int count );
};

#endif
