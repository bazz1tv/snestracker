
#include "SDL_ep.h"

cImageManager :: cImageManager( void )
{
	Count = 0;
}

cImageManager :: ~cImageManager( void )
{
	DeleteAll();
}


SDL_Surface *cImageManager :: GetPointer( std::string nIdentifier )
{
	for(unsigned int i = 0; i < ImageItems.size(); i++)
	{
		if( ImageItems[i].Identifier.compare( nIdentifier ) == 0 )
		{
			return ImageItems[i].Item;	// the first found
		}
	}

	return NULL; // not found
}

SDL_Surface *cImageManager :: GetPointer( unsigned int nId )
{
	if( Count >= nId ) 
	{
		for(unsigned int i = 0; i < ImageItems.size(); i++)
		{
			if( ImageItems[i].CountId == nId ) 
			{
				return ImageItems[i].Item;
			}
		}
	}

	return NULL;
}

SDL_Surface *cImageManager :: Copy( string nIdentifier )
{
	for(unsigned int i = 0; i < ImageItems.size(); i++)
	{
		if( ImageItems[i].Identifier.compare( nIdentifier.c_str() ) == 0 )	// the first found
		{
			SDL_Surface *ImageCopy = CreateSurface( ImageItems[i].Item->w, ImageItems[i].Item->w, 
				ImageItems[i].Item->format->BytesPerPixel, ImageItems[i].Item->flags );

			SDL_BlitSurface( ImageItems[i].Item, NULL, ImageCopy, NULL );

			return ImageCopy;
		}
	}

	return NULL; // not found
}

string cImageManager :: GetIdentifier( SDL_Surface *nItem )
{
	for(unsigned int i = 0; i < ImageItems.size(); i++)
	{
		if( ImageItems[i].Item == nItem )
		{
			return ImageItems[i].Identifier; // the first found
		}
	}

	return NULL; // not found
}


unsigned int cImageManager :: GetSize( void )
{
	return ImageItems.size();
}


void cImageManager :: Add( SDL_Surface *nItem, string nIdentifier /* = "Unknown"  */ )
{
	// If the identifier already exists
	if( GetPointer( nIdentifier ) ) 
	{
		SDL_FreeSurface( nItem );
		return;
	}
	
	Count++;
	
	ImageItem tItem;
	tItem.Item = nItem;
	tItem.Identifier = nIdentifier;
	tItem.CountId = Count;

	ImageItems.push_back( tItem );
}

void cImageManager :: DeleteAll( void )
{
	if( Count <= 0 || ImageItems.empty() ) 
	{
		return;
	}

	for( unsigned int i = 0; i < ImageItems.size(); i++ )
	{
		if( ImageItems[i].Item ) 
		{
			SDL_FreeSurface( ImageItems[i].Item );
			ImageItems[i].Item = NULL;
		}
	}

	ImageItems.clear(); // seems to work

	Count = 0;
}
