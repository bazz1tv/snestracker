/// @addtogroup Engine_Video
// @{
/** 

	@file video.h  Video Initialization and other

	V 1.1

	(c)2012 Bazz

*/


#ifndef __EP_VIDEO_H__
#define __EP_VIDEO_H__

DLLINTERFACE int InitSDL( Uint32 flags = SDL_INIT_EVERYTHING );

DLLINTERFACE SDL_Surface *InitScreen( int width, int height, int bpp, bool Fullscreen = 0, 
						Uint32 flags = SDL_HWSURFACE | SDL_HWACCEL | SDL_RLEACCEL | SDL_DOUBLEBUF );

DLLINTERFACE void SetWindowCaption( const char *Caption, const char *iconpath = NULL );

DLLINTERFACE void SetWindowIcon( SDL_Surface *Icon, Uint8 *Imask = NULL );

/// Quits SDL
DLLINTERFACE void QuitSDL( void );

#endif
// @}