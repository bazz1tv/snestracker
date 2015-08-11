// Simple game music file player

// Game_Music_Emu 0.5.2
#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "gme/Music_Emu.h"
#include "gme/Spc_Emu.h"
#include <vector>
#include <iostream>
#include "gme/Spc_Filter.h"
#include "types.h"
#include "Audio_Context.h"
#include "App_Settings_Context.h"

typedef double gain_t;

void sound_start();

void sound_stop();

void sound_cleanup();


// should be in the player
void prev_track();
void next_track();
void toggle_pause();
void restart_track();

//

class Music_Player : public Audio_Context, public App_Settings_Context {
public:
	bool filter_is_active=false;
	bool has_no_song = true;
	double tempo=1.0;
	bool needs_to_fade_out=false;
	bool needs_to_fade_in=false;

	void set_gain_db(gain_t new_gain_db, bool immediate=false);
	

	void inc_tempo() {tempo+=0.1; emu_->set_tempo(tempo); }
	void dec_tempo() {tempo-=0.1; emu_->set_tempo(tempo); }
	// Initialize player and set sample rate
	blargg_err_t init( long sample_rate = 44100, const char *audio_out_dev=NULL );
	
	// Load game music file. NULL on success, otherwise error string.
	blargg_err_t load_file( const char* path );
	
	// (Re)start playing track. Tracks are numbered from 0 to track_count() - 1.
	blargg_err_t start_track( int track, bool test=false );
	
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
	void pause( int, bool with_fade=true, bool is_fade_threaded=true );
	void toggle_pause();
	bool is_paused();
	
	// True if track ended
	bool track_ended() const;
	
	
	// Pointer to emulator
	Music_Emu* emu() const { return emu_; }
	Spc_Emu* spc_emu() const { return spc_emu_; }

	void spc_write(int addr, int data);
	uint8_t spc_read(int addr);
	uint8_t spc_read_dsp(int dspaddr);
	void spc_write_dsp(int dspaddr, int val);

	void inc_ram(int addr, int i=1);
	void dec_ram(int addr, int i=1);
	// Set stereo depth, where 0.0 = none and 1.0 = maximum
	void set_stereo_depth( double );
	
	// Set tempo, where 0.5 = half speed, 1.0 = normal, 2.0 = double speed
	void set_tempo( double );
	
	// Set voice muting bitmask
	void mute_voices( int );
	
	// Set buffer to copy samples from each buffer into, or NULL to disable
	//typedef int sample_t;
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
	gain_t gain_db=0.0, linear_gain=1.0; 
	static double min_gain_db, max_gain_db;
	bool gain_has_changed=false;
	bool is_using_zero_crossover=false;
	gain_t new_gain_db = 0.0;
	gain_t fade_gain=1.0;
	gain_t target_gain = 1.0;
	bool track_started;
	long sample_rate;
	// *(
	/*void set_path(char *str)
	{
		strcpy(path,str);
	}*/
	void fade_out(bool threaded=false); // public function
	double stereo_bufs_per_sec;

	static void fill_buffer( void*, sample_t*, int );
	static bool exporting;
private:
	Spc_Filter* spc_filter;
	Music_Emu* emu_;
	bool paused;
	Spc_Emu* spc_emu_;
	sample_t* scope_buf;
	
	int scope_buf_size;
	
	track_info_t track_info_;
	// byte alignment seems to matter, I've had app crash based on it..
	// so always add variables at the END of the class to not disturb
	int curtrack, filetrack;
	std::vector<std::string> files;	// this is kind of unnecessary..
	std::string path;
	

	SDL_Thread *thread;
	// thread function
	static int fade_out(void *ptr);
	// only to be called by thread
	void low_level_fade_out(bool threaded);
	
	
	void suspend();
	void resume();
  void apply_gain(sample_t* out, int count );
};

#endif
