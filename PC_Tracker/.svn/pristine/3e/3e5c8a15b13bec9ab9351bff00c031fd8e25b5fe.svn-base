/// @addtogroup Engine_Image
// @{
/** 

	@file image.h  Image

	V 1.1.1

	LGPL (c) F. Richter

*/


#ifndef __EP_IMAGE_H__
#define __EP_IMAGE_H__

#ifdef _WIN32
#include <SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
/** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

/** 

	LoadImage - Loads an image and sets it to the best format !
	
	Colorkey : Every Pixel that matches identical with the specified Color 
	will be transparent .
	
	Alpha : The transparency of the whole image .
	
	
	Note : You should call it after the main screen was initialized 
*/
DLLINTERFACE SDL_Surface *LoadImage( const char *szFilename, Uint32 Colorkey = 0, Uint8 Alpha = 255 );	/// V 1.1.1

/** 

	CreateSurface - Creates an empty image and sets it to the best format
	
	width,height : The Dimensions

	bpp : the best way is to set here screen->format->bitsperpixel

	flags : for setting specified flags
	
	Note : You should call it after the main screen was initialized 
	
	

*/
DLLINTERFACE SDL_Surface *CreateSurface( unsigned int width, unsigned int height, Uint8 bpp = 32, 
						   Uint32 flags = 0 );	/// V 1.2.0

#endif
// @}