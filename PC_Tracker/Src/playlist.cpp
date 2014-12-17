#include "playlist.h"

#ifdef _WIN32
#include <SDL_ep.h>
#elif __APPLE__
#include <MeatBallEngine/SDL_ep.h>
#endif

#include "Globals.h"

#define DIR_MUSIC "data/music/"
extern cAudio *pAudio;

namespace Playlist
{
	
int numsongs;
void ls_music()
{
	full_path = fs::system_complete(fs::path(fs::current_path().string() +"/data/music"));
	
	fs::directory_iterator end_iter;
		int i=0;
		for ( fs::directory_iterator dir_itr( full_path ); dir_itr != end_iter; ++dir_itr )
		{
			musicfiles[i++] = dir_itr->path().filename().string();
		}

		numsongs=i;	// the # of songs we found
}

void musicover()
{
	int index = Random(0,numsongs-1);
	std::string file = DIR_MUSIC+musicfiles[index];

		
	DEBUGLOG("Song ended. loading %s", musicfiles[index].c_str());
	pAudio->PlayMusik((char*)file.c_str(), SDL_TRUE, SDL_TRUE);
}

//End namespace
}