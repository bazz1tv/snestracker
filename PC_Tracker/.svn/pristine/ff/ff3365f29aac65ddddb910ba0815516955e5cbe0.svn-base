/// @addtogroup Engine_Image_Manager
// @{
/** 

	@file img_manager.h  Image Handler/Manager

	V 1.1

	LGPL (c) F. Richter

*/


#ifndef __EP_IMAGE_MANAGER_H__
#define __EP_IMAGE_MANAGER_H__

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif


#include <string>
#include <vector>

using std::string;
using std::vector;

class cImageManager
{
public:	
	DLLINTERFACE cImageManager( void );
	DLLINTERFACE ~cImageManager( void );

	DLLINTERFACE SDL_Surface *GetPointer( const std::string nIdentifier );
	DLLINTERFACE SDL_Surface *GetPointer( unsigned int nId );

	DLLINTERFACE SDL_Surface *Copy( string nIdentifier );

	DLLINTERFACE string GetIdentifier( SDL_Surface *nItem );

	/**	Returns the Current Size
	 *	/Count of the Items
	 */
	DLLINTERFACE unsigned int GetSize( void );

	/**	Adds an Image
	 *	Should always have an Identifier
	 */
	DLLINTERFACE void Add( SDL_Surface *nItem, const string nIdentifier = "Unknown" );

	DLLINTERFACE SDL_Surface *operator [] ( unsigned int nId )
	{
		return GetPointer( nId );
	}

	DLLINTERFACE SDL_Surface *operator [] ( const string nIdentifier )
	{
		return GetPointer( nIdentifier );
	}

	DLLINTERFACE string operator [] ( SDL_Surface *nItem )
	{
		return GetIdentifier( nItem );
	}

	DLLINTERFACE void DeleteAll( void );

private:
		
	struct ImageItem 
	{
		SDL_Surface *Item;
		string Identifier;
		unsigned int CountId;
	};

	unsigned int Count; // images loaded
	vector<ImageItem> ImageItems;
};

#endif
// @}