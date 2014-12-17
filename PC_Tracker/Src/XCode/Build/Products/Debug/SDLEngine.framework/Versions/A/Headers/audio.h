/// @addtogroup Engine_Audio
// @{
/** 
	@file audio.h  Audio Engine Header

	Version 2.5

	 Bazz

*/

#ifndef __EP_AUDIO_H__
#define __EP_AUDIO_H__

#ifdef _WIN32
#include <SDL_mixer.h>
#else
#include <SDL2_mixer/SDL_mixer.h>
#endif

#include <iostream>

class cAudio
{
public:
	/**	
	 *	
	 */
	DLLINTERFACE cAudio( SDL_bool playMusic = SDL_TRUE, SDL_bool playSounds = SDL_TRUE, Uint8 MusicVol = 80, Uint8 SoundVol = 110 );
	/**
	 */
	DLLINTERFACE ~cAudio( void );

	/**	Initializes the Audio
	 *	with the specified settings
	 */
	DLLINTERFACE void InitAudio( void );
	
	/// Reserves Channels for default mixing.
	DLLINTERFACE void ReserveChannels( int num );

	/// Channels for Mixing.
	DLLINTERFACE void AllocateChannels( int num ); ///< Do not use !
	
	/** Loads an sound
	 *	if volume is -1 the default sound volume will be used
	 *	if volume is -2 no volume will be set
	 */
	DLLINTERFACE Mix_Chunk *LoadSound( const char *filename, int sVolume = -2 );
	/**	Plays an Sound chunk
	 *	if channel is -1 the sound will be played on the next free channel
	 *	if volume is -1 the default volume will be used
	 *	if volume is -2 no volume will be set
	 */
	
	DLLINTERFACE int PlaySound( Mix_Chunk *sound, int channel = -1, int sVolume = -1 );
	
	/**	if no forcing the file will be played after the current Music file 
	*	if loops is -1 music will be looped infinity
	*	Can play WAVE, MOD, MIDI, OGG (with ogg vorbis), mp3 ( with smpeg)
	*/
	DLLINTERFACE void PlayMusik( const char *filename, int loops = -1, SDL_bool force = SDL_TRUE );

	DLLINTERFACE void LoadMusic( const char *filename );
	
	/// Stops playing Music.
	DLLINTERFACE void StopMusic( void );

	/// Fades the music out for the given miliseconds.
	DLLINTERFACE void FadeOutMusic( unsigned int ms );

	/// Pauses playing Music.
	DLLINTERFACE void PauseMusic( void );

	/// Resumes Paused Music.
	DLLINTERFACE void ResumeMusic( void );
	
	/// Halt playback on specified channels or all if -1 set.
	DLLINTERFACE void HaltSounds( int channel = -1 );

	/// Stops specified Sounds/Channels or all if -1 set.
	DLLINTERFACE void PauseSounds( int channel = -1 );

	/// Resume Playing specified sounds if got stopped or all if -1 set.
	DLLINTERFACE void ResumeSounds( int channel = -1 );

	/// Deletes The Temporary Sound Buffer .
	DLLINTERFACE void DeleteSounds( void );

	/** Fades Sounds out
	 *	if channel is -1 fades all sounds out
	 */
	DLLINTERFACE void FadeOutSounds( int milliseconds = 2000, int channel = -1 );

	/// Set a specific sound's volume with it's chunk ptr.
	DLLINTERFACE int SetSoundVolume ( Mix_Chunk *chunk, int iVolume );
	
	DLLINTERFACE int GetSoundVolume(Mix_Chunk *chunk);
	


	/**	Sets All the Sounds default Volume
	 * Returns avg_volume
	 */
	DLLINTERFACE int SetAllSoundsVolume( Uint8 iVolume );

	/**	Sets the Music default Volume
	 *
	 */
	DLLINTERFACE int SetMusicVolume( Uint8 iVolume );

	/**	Checks if no music is played
	 *
	 */
	DLLINTERFACE void Update( void );

	DLLINTERFACE SDL_bool MusicPlaying();
	
	Mix_Music *Music;
	
	int iHz,iBuffer,iChannels;
	int Sounds_Played,Music_Played;

	Uint8 Sound_Volume,Music_Volume;

	SDL_bool bSounds,bMusic,bDebug;
	SDL_bool bInitialised;
};


#endif
// @}