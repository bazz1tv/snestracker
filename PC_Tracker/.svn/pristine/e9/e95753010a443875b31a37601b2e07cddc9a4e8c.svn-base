/// @addtogroup Engine_Video
// @{
/** 

	@file video.h  Video Initialization and other

	V 1.1

	(c)2012 Bazz

*/


#ifndef __EP_VIDEO_H__
#define __EP_VIDEO_H__

#include "SDL_ep.h"

DLLINTERFACE int InitSDL( Uint32 flags = SDL_INIT_EVERYTHING );

DLLINTERFACE int InitWindow( SDL_Window *win, SDL_Renderer *Screen, const char *title, int width, int height, int bpp, Uint32 wflags, Uint32 rflags, SDL_bool fullscreen = SDL_FALSE);

DLLINTERFACE SDL_Window *GetWindow(const char *title, int width, int height, int bpp, Uint32 wflags);

DLLINTERFACE SDL_Renderer *GetRenderer(SDL_Window *win, Uint32 rflags);


DLLINTERFACE void SetWindowCaption( SDL_Window *win, const char *Caption );


DLLINTERFACE void SetWindowIcon( SDL_Window *win, SDL_Surface *Icon );

/// Quits SDL
DLLINTERFACE void QuitSDL( void );

#endif
// @}