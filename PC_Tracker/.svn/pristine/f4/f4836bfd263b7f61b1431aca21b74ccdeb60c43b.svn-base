/// @addtogroup Engine_Snd_Manager
// @{
/** 

	@file snd_manager.h  Sound Handler/Manager

	V 1.1

	LGPL (c) F. Richter

*/


#ifndef __EP_SOUND_MANAGER_H__
#define __EP_SOUND_MANAGER_H__

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif


#include <string>
#include <vector>

using std::string;
using std::vector;

class cSoundManager
{
public:	
	DLLINTERFACE cSoundManager( void );
	DLLINTERFACE ~cSoundManager( void );

	DLLINTERFACE Mix_Chunk *GetPointer( const std::string nIdentifier );
	DLLINTERFACE Mix_Chunk *GetPointer( unsigned int nId );

	DLLINTERFACE Mix_Chunk *Copy( string nIdentifier );

	DLLINTERFACE string GetIdentifier( Mix_Chunk *nItem );

	/**	Returns the Current Size
	 *	/Count of the Items
	 */
	DLLINTERFACE unsigned int GetSize( void );

	/**	Adds an Sound
	 *	Should always have an Identifier
	 */
	DLLINTERFACE void Add( Mix_Chunk *nItem, const string nIdentifier = "Unknown" );

	DLLINTERFACE Mix_Chunk *operator [] ( unsigned int nId )
	{
		return GetPointer( nId );
	}

	DLLINTERFACE Mix_Chunk *operator [] ( const string nIdentifier )
	{
		return GetPointer( nIdentifier );
	}

	DLLINTERFACE string operator [] ( Mix_Chunk *nItem )
	{
		return GetIdentifier( nItem );
	}

	DLLINTERFACE void DeleteAll( void );

private:
		
	struct SoundItem 
	{
		Mix_Chunk *Item;
		string Identifier;
		unsigned int CountId;
	};

	unsigned int Count; // Sounds loaded
	vector<SoundItem> SoundItems;
};

#endif
// @}