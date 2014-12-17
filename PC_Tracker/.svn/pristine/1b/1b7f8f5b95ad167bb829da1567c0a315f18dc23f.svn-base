/*
*	SDL Graphics Extension
*	SGE internal header
*
*	Started 000627
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)2000-2003 Anders Lindström
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#ifndef sge_internal_H
#define sge_internal_H

/* This header is included in all sge_*.h files */

#include "sge_config.h"

/*
*  C compatibility
*  Thanks to Ohbayashi Ippei (ohai@kmc.gr.jp) for this clever hack!
*/
#ifdef _SGE_C_AND_CPP
	#ifdef __cplusplus
		#define _SGE_C           /* use extern "C" on base functions */
	#else
		#define sge_C_ONLY       /* remove overloaded functions */
		#define _SGE_NO_CLASSES  /* no C++ classes */
	#endif
#endif


/*
*  This is traditional
*/
#ifndef PI
	#define PI 3.1414926535
#endif


/*
*  Bit flags
*/
#define SGE_FLAG0 0x00
#define SGE_FLAG1 0x01
#define SGE_FLAG2 0x02
#define SGE_FLAG3 0x04
#define SGE_FLAG4 0x08
#define SGE_FLAG5 0x10
#define SGE_FLAG6 0x20
#define SGE_FLAG7 0x40
#define SGE_FLAG8 0x80


/*
*  Define the right alpha values 
*  (they were fliped in SDL 1.1.5+)
*/
#ifndef SDL_ALPHA_OPAQUE
	#define SDL_ALPHA_OPAQUE 0
#endif
#ifndef SDL_ALPHA_TRANSPARENT
	#define SDL_ALPHA_TRANSPARENT 255
#endif


/*
*  Older versions of SDL doesn't have SDL_VERSIONNUM
*/
#ifndef SDL_VERSIONNUM
	#define SDL_VERSIONNUM(X, Y, Z)      \
		(X)*1000 + (Y)*100 + (Z)
#endif


/*
*  Older versions of SDL doesn't have SDL_CreateRGBSurface
*/
#ifndef SDL_AllocSurface
	#define SDL_CreateRGBSurface  SDL_AllocSurface
#endif


/*
*  Macro to get clipping
*/
#if SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL) >= \
    SDL_VERSIONNUM(1, 1, 5)
	#define sge_clip_xmin(pnt) pnt->clip_rect.x
	#define sge_clip_xmax(pnt) pnt->clip_rect.x + pnt->clip_rect.w-1
	#define sge_clip_ymin(pnt) pnt->clip_rect.y
	#define sge_clip_ymax(pnt) pnt->clip_rect.y + pnt->clip_rect.h-1
#else
	#define sge_clip_xmin(pnt) pnt->clip_minx
	#define sge_clip_xmax(pnt) pnt->clip_maxx
	#define sge_clip_ymin(pnt) pnt->clip_miny
	#define sge_clip_ymax(pnt) pnt->clip_maxy
#endif


/*
*  Macro to get the smallest bounding box from two (SDL_Rect) rectangles
*/
#define sge_RectUnion(dst_rect, rect1, rect2)\
	dst_rect.x = (rect1.x < rect2.x)? rect1.x:rect2.x;\
	dst_rect.y = (rect1.y < rect2.y)? rect1.y:rect2.y;\
	dst_rect.w = (rect1.x + rect1.w > rect2.x + rect2.w)? rect1.x + rect1.w - dst_rect.x : rect2.x + rect2.w - dst_rect.x;\
	dst_rect.h = (rect1.y + rect1.h > rect2.y + rect2.h)? rect1.y + rect1.h - dst_rect.y : rect2.y + rect2.h - dst_rect.y;


/*
*  We need to use alpha sometimes but older versions of SDL doesn't have
*  alpha support.
*/
#if SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL) >= \
    SDL_VERSIONNUM(1, 1, 5)
	#define sge_MapRGBA SDL_MapRGBA
	#define sge_GetRGBA SDL_GetRGBA
#else
	#define sge_MapRGBA(fmt, r, g, b, a) SDL_MapRGB(fmt, r, g, b)
	#define sge_GetRGBA(pixel, fmt, r, g, b, a) SDL_GetRGBA(pixel, fmt, r, g, b)
#endif


/*
*  Some compilers use a special export keyword
*  Thanks to Seung Chan Lim (limsc@maya.com or slim@djslim.com) to pointing this out
*  (From SDL)
*/
#ifndef DECLSPEC
	#ifdef __BEOS__
		#if defined(__GNUC__)
			#define DECLSPEC __declspec(dllexport)
		#else
			#define DECLSPEC __declspec(export)
		#endif
	#else
		#ifdef WIN32
			#define DECLSPEC __declspec(dllexport)
		#else
			#define DECLSPEC
		#endif
	#endif
#endif

#endif /* sge_internal_H */
