/// @addtogroup Engine
// @{
/** 
	@file SDL_ep.h
	SDL_ep - Engine primitives for SDL

	Uses : 
	SDL
	SDL_mixer
	SDL_image
	SDL_ttf
	SDL_net
	SGE

	LGPL (c) F. Richter
*/

#ifndef _SDL_EnginePrimitives_h
#define _SDL_EnginePrimitives_h


#ifdef _WIN32
	#ifdef _DEBUG
		#include <vld.h>
	#endif

	#include <SDL.h>

#elif __APPLE__
	#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#undef main
#endif


#ifdef _WIN32
#pragma comment( lib, "SDL.lib" )
#pragma comment( lib, "SDL_mixer.lib" )
#pragma comment( lib, "SDL_image.lib" )
#pragma comment( lib, "SDL_ttf.lib" )
#pragma comment( lib, "SDL_net.lib" )
//#pragma comment( lib, "Sge.lib" )
#endif

/**  WIN32 DLL interface */

#ifdef _WIN32
#define DLLINTERFACE __declspec(dllexport)
#else
#define DLLINTERFACE
#endif

#include "framerate.h"
#include "image.h"
#include "audio.h"
#include "video.h"
#include "sprite.h"
#include "misc.h"
#include "font.h"
#include "img_manager.h"
#include "snd_manager.h"

#include "net.h"

#define SDL_ENGINEPRIMITIVES_MAJOR	0
#define SDL_ENGINEPRIMITIVES_MINOR	9
#define SDL_ENGINEPRIMITIVES_MICRO	8

/// Inits SDL_ep. It seeds the random number generator and sets the SpriteSpeed Factor to 1
int DLLINTERFACE InitEP( void );

#endif				/** _SDL_ep_h */

/**
	@Todo :

	Audio :
	{
		Sample sound creation .
		Working handling of Channels .
	}
	
	Sprite :
	{
		Properly rotating (new variables rot_x,rot_y)

		Physicsprite min/max speed
	}

	sprite manager

	AI class ?!?
	Particle class.
	
	The Anglesprite Setdirection default argument -1 should be removed !

	Sort of Effect functions which creates images with SGE !
*/
// @}