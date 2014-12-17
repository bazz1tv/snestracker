/*
*	SDL Graphics Extension
*	Rotation routines (header)
*
*	Started 000625
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)1999-2003 Anders Lindström
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#ifndef sge_rotation_H
#define sge_rotation_H

#include "SDL.h"
#include "sge_internal.h"

/* Transformation flags */
#define SGE_TAA SGE_FLAG1
#define SGE_TSAFE SGE_FLAG2
#define SGE_TTMAP SGE_FLAG3

#ifdef _SGE_C
extern "C" {
#endif
DECLSPEC SDL_Rect sge_transform(SDL_Surface *src, SDL_Surface *dst, float angle, float xscale, float yscale ,Uint16 px, Uint16 py, Uint16 qx, Uint16 qy, Uint8 flags);
DECLSPEC SDL_Surface *sge_transform_surface(SDL_Surface *src, Uint32 bcol, float angle, float xscale, float yscale, Uint8 flags);


/* Old obolete functions - now uses sge_transform() anyway */
DECLSPEC SDL_Surface *sge_rotate_scaled_surface(SDL_Surface *src, int angle, double scale, Uint32 bcol);
DECLSPEC SDL_Surface *sge_rotate_surface(SDL_Surface *src, int angle, Uint32 bcol);
DECLSPEC SDL_Rect sge_rotate_xyscaled(SDL_Surface *dest, SDL_Surface *src, Sint16 x, Sint16 y, int angle, double xscale, double yscale);
DECLSPEC SDL_Rect sge_rotate_scaled(SDL_Surface *dest, SDL_Surface *src, Sint16 x, Sint16 y, int angle, double scale);
DECLSPEC SDL_Rect sge_rotate(SDL_Surface *dest, SDL_Surface *src, Sint16 x, Sint16 y, int angle);
#ifdef _SGE_C
}
#endif

#endif /* sge_rotation_H */
