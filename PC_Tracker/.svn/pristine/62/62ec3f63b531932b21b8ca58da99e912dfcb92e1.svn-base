/*
*	SDL Graphics Extension
*	Collision routines (header)
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

#ifndef sge_collision_H
#define sge_collision_H

#include "SDL.h"
#include "sge_internal.h"

/* The collision struct */
typedef struct
{
	Uint8 *map;
	Uint16 w,h;
} sge_cdata;

#ifdef _SGE_C
extern "C" {
#endif
DECLSPEC sge_cdata *sge_make_cmap(SDL_Surface *img);
DECLSPEC int sge_bbcheck(sge_cdata *cd1,Sint16 x1,Sint16 y1, sge_cdata *cd2,Sint16 x2,Sint16 y2);
DECLSPEC int _sge_bbcheck(Sint16 x1,Sint16 y1,Sint16 w1,Sint16 h1, Sint16 x2,Sint16 y2,Sint16 w2,Sint16 h2);
DECLSPEC int _sge_cmcheck(sge_cdata *cd1,Sint16 x1,Sint16 y1, sge_cdata *cd2,Sint16 x2,Sint16 y2);
DECLSPEC int sge_cmcheck(sge_cdata *cd1,Sint16 x1,Sint16 y1, sge_cdata *cd2,Sint16 x2,Sint16 y2);
DECLSPEC Sint16 sge_get_cx(void);
DECLSPEC Sint16 sge_get_cy(void);
DECLSPEC void sge_destroy_cmap(sge_cdata *cd);
DECLSPEC void sge_unset_cdata(sge_cdata *cd, Sint16 x, Sint16 y, Sint16 w, Sint16 h);
DECLSPEC void sge_set_cdata(sge_cdata *cd, Sint16 x, Sint16 y, Sint16 w, Sint16 h);
#ifdef _SGE_C
}
#endif

#ifndef _SGE_NO_CLASSES
class DECLSPEC sge_shape;
DECLSPEC int sge_bbcheck_shape(sge_shape *shape1, sge_shape *shape2);
#endif /* _SGE_NO_CLASSES */


#endif /* sge_collision_H */
