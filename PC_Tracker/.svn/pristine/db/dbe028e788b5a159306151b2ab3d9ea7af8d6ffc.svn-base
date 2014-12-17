
#include "SDL_ep.h"

cSoundManager :: cSoundManager( void )
{
	Count = 0;
}

cSoundManager :: ~cSoundManager( void )
{
	DeleteAll();
}


Mix_Chunk *cSoundManager :: GetPointer( std::string nIdentifier )
{
	for(unsigned int i = 0; i < SoundItems.size(); i++)
	{
		if( SoundItems[i].Identifier.compare( nIdentifier ) == 0 )
		{
			return SoundItems[i].Item;	// the first found
		}
	}

	return NULL; // not found
}

Mix_Chunk *cSoundManager :: GetPointer( unsigned int nId )
{
	if( Count >= nId ) 
	{
		for(unsigned int i = 0; i < SoundItems.size(); i++)
		{
			if( SoundItems[i].CountId == nId ) 
			{
				return SoundItems[i].Item;
			}
		}
	}

	return NULL;
}

string cSoundManager :: GetIdentifier( Mix_Chunk *nItem )
{
	for(unsigned int i = 0; i < SoundItems.size(); i++)
	{
		if( SoundItems[i].Item == nItem )
		{
			return SoundItems[i].Identifier; // the first found
		}
	}

	return NULL; // not found
}


unsigned int cSoundManager :: GetSize( void )
{
	return SoundItems.size();
}


void cSoundManager :: Add( Mix_Chunk *nItem, string nIdentifier /* = "Unknown"  */ )
{
	Count++;

	// If the identifier already exists
	if( GetPointer( nIdentifier ) ) 
	{
		Mix_FreeChunk( nItem );
		return;
	}

	SoundItem tItem;
	tItem.Item = nItem;
	tItem.Identifier = nIdentifier;
	tItem.CountId = Count;

	SoundItems.push_back( tItem );
}

void cSoundManager :: DeleteAll( void )
{
	if( Count <= 0 || SoundItems.empty() ) 
	{
		return;
	}

	for( unsigned int i = 0; i < SoundItems.size(); i++ )
	{
		if( SoundItems[i].Item ) 
		{
			Mix_FreeChunk( SoundItems[i].Item );
			SoundItems[i].Item = NULL;
		}
	}

	SoundItems.clear(); // seems to work

	Count = 0;
}