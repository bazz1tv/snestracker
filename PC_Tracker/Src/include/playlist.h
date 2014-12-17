#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include "boost.h"
#include <string.h>

/// Conjured to do smooth transitioning of Music while I develop..
/// May not be a permanent thing.. who knows!
namespace Playlist
{
	static std::string musicfiles[100];
	extern int numsongs;
	static fs::path full_path;

	void ls_music(); ///< This gets all music files from the music directory.
						/// Puts them into musicfiles array
	void musicover();	///< Hooked into the Mixer Song over Hook. It will randomly
						///< pick another song to play
}

#endif