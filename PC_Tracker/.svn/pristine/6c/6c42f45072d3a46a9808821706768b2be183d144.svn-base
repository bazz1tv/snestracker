/*
*	SDL Graphics Extension
*	Johan E. Thelin's BLib (header)
*
*	Started 000428
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)2000-2003 Anders Lindström & Johan E. Thelin
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#ifndef sge_blib_H
#define sge_blib_H

#include "SDL.h"
#include "sge_internal.h"


#ifdef _SGE_C
extern "C" {
#endif
DECLSPEC void sge_FadedLine(SDL_Surface *dest,Sint16 x1,Sint16 x2,Sint16 y,Uint8 r1,Uint8 g1,Uint8 b1,Uint8 r2,Uint8 g2,Uint8 b2);
DECLSPEC void sge_TexturedLine(SDL_Surface *dest,Sint16 x1,Sint16 x2,Sint16 y,SDL_Surface *source,Sint16 sx1,Sint16 sy1,Sint16 sx2,Sint16 sy2);

DECLSPEC void sge_Trigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint32 color);
DECLSPEC void sge_TrigonAlpha(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint32 color, Uint8 alpha);
DECLSPEC void sge_AATrigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint32 color);
DECLSPEC void sge_AATrigonAlpha(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint32 color, Uint8 alpha);

DECLSPEC void sge_FilledTrigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint32 color);
DECLSPEC void sge_FilledTrigonAlpha(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint32 color, Uint8 alpha);

DECLSPEC void sge_FadedTrigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint32 c1,Uint32 c2,Uint32 c3);
DECLSPEC void sge_TexturedTrigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,SDL_Surface *source,Sint16 sx1,Sint16 sy1,Sint16 sx2,Sint16 sy2,Sint16 sx3,Sint16 sy3);

DECLSPEC void sge_TexturedRect(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Sint16 x4,Sint16 y4,SDL_Surface *source,Sint16 sx1,Sint16 sy1,Sint16 sx2,Sint16 sy2,Sint16 sx3,Sint16 sy3,Sint16 sx4,Sint16 sy4);

DECLSPEC int sge_FilledPolygon(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint32 color);
DECLSPEC int sge_FilledPolygonAlpha(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint32 color, Uint8 alpha);
DECLSPEC int sge_AAFilledPolygon(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint32 color);

DECLSPEC int sge_FadedPolygon(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint8 *R, Uint8 *G, Uint8 *B);
DECLSPEC int sge_FadedPolygonAlpha(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint8 *R, Uint8 *G, Uint8 *B, Uint8 alpha);
DECLSPEC int sge_AAFadedPolygon(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint8 *R, Uint8 *G, Uint8 *B);
#ifdef _SGE_C
}
#endif

#ifndef sge_C_ONLY
DECLSPEC void sge_Trigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_TrigonAlpha(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_AATrigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_AATrigonAlpha(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_FilledTrigon(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_FilledTrigonAlpha(SDL_Surface *dest,Sint16 x1,Sint16 y1,Sint16 x2,Sint16 y2,Sint16 x3,Sint16 y3,Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC int sge_FilledPolygon(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint8 r, Uint8 g, Uint8 b);
DECLSPEC int sge_FilledPolygonAlpha(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha);
DECLSPEC int sge_AAFilledPolygon(SDL_Surface *dest, Uint16 n, Sint16 *x, Sint16 *y, Uint8 r, Uint8 g, Uint8 b);
#endif /* sge_C_ONLY */


#endif /* sge_blib_H */
