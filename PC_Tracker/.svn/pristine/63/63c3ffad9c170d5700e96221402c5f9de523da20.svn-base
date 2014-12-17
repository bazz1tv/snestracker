/*
*	SDL Graphics Extension
*	Drawing primitives (header)
*
*	Started 990815 (split from sge_draw 010611)
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

#ifndef sge_primitives_H
#define sge_primitives_H

#include "SDL.h"
#include "sge_internal.h"


#ifdef _SGE_C
extern "C" {
#endif
DECLSPEC void sge_HLine(SDL_Surface *Surface, Sint16 x1, Sint16 x2, Sint16 y, Uint32 Color);
DECLSPEC void sge_HLineAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 x2, Sint16 y, Uint32 Color, Uint8 alpha);
DECLSPEC void sge_VLine(SDL_Surface *Surface, Sint16 x, Sint16 y1, Sint16 y2, Uint32 Color);
DECLSPEC void sge_VLineAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y1, Sint16 y2, Uint32 Color, Uint8 alpha);
DECLSPEC void sge_DoLine(SDL_Surface *Surface, Sint16 X1, Sint16 Y1, Sint16 X2, Sint16 Y2, Uint32 Color, void Callback(SDL_Surface *Surf, Sint16 X, Sint16 Y, Uint32 Color));
DECLSPEC void sge_Line(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 Color);
DECLSPEC void sge_LineAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 Color, Uint8 alpha);
DECLSPEC void sge_AALine(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
DECLSPEC void sge_AALineAlpha(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, Uint8 alpha);
DECLSPEC void sge_DomcLine(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, void Callback(SDL_Surface *Surf, Sint16 X, Sint16 Y, Uint32 Color));
DECLSPEC void sge_mcLine(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2);
DECLSPEC void sge_mcLineAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 alpha);
DECLSPEC void sge_AAmcLine(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2);
DECLSPEC void sge_AAmcLineAlpha(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 alpha);

DECLSPEC void sge_Rect(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
DECLSPEC void sge_RectAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, Uint8 alpha);
DECLSPEC void sge_FilledRect(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
DECLSPEC void sge_FilledRectAlpha(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, Uint8 alpha);

DECLSPEC void sge_DoEllipse(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color, void Callback(SDL_Surface *Surf, Sint16 X, Sint16 Y, Uint32 Color));
DECLSPEC void sge_Ellipse(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
DECLSPEC void sge_EllipseAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color, Uint8 Alpha);
DECLSPEC void sge_FilledEllipse(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
DECLSPEC void sge_FilledEllipseAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color, Uint8 alpha);
DECLSPEC void sge_AAEllipseAlpha(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color, Uint8 alpha);
DECLSPEC void sge_AAEllipse(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color);
DECLSPEC void sge_AAFilledEllipse(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color);

DECLSPEC void sge_DoCircle(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint32 color, void Callback(SDL_Surface *Surf, Sint16 X, Sint16 Y, Uint32 Color));
DECLSPEC void sge_Circle(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
DECLSPEC void sge_CircleAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint32 color, Uint8 alpha);
DECLSPEC void sge_FilledCircle(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
DECLSPEC void sge_FilledCircleAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint32 color, Uint8 alpha);
DECLSPEC void sge_AACircleAlpha(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 r, Uint32 color, Uint8 alpha);
DECLSPEC void sge_AACircle(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 r, Uint32 color);
DECLSPEC void sge_AAFilledCircle(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 r, Uint32 color);

DECLSPEC void sge_Bezier(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint32 color);
DECLSPEC void sge_BezierAlpha(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint32 color, Uint8 alpha);
DECLSPEC void sge_AABezier(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint32 color);
DECLSPEC void sge_AABezierAlpha(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint32 color, Uint8 alpha);
#ifdef _SGE_C
}
#endif

#ifndef sge_C_ONLY
DECLSPEC void sge_HLine(SDL_Surface *Surface, Sint16 x1, Sint16 x2, Sint16 y, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_HLineAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 x2, Sint16 y, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_VLine(SDL_Surface *Surface, Sint16 x, Sint16 y1, Sint16 y2, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_VLineAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y1, Sint16 y2, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_DoLine(SDL_Surface *Surface, Sint16 X1, Sint16 Y1, Sint16 X2, Sint16 Y2, Uint8 R, Uint8 G, Uint8 B, void Callback(SDL_Surface *Surf, Sint16 X, Sint16 Y, Uint32 Color));
DECLSPEC void sge_Line(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_LineAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_AALine(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b);
DECLSPEC void sge_AALineAlpha(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha);
DECLSPEC void sge_Rect(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_RectAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_FilledRect(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_FilledRectAlpha(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_DoEllipse(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B, void Callback(SDL_Surface *Surf, Sint16 X, Sint16 Y, Uint32 Color));
DECLSPEC void sge_Ellipse(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_EllipseAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_FilledEllipse(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_FilledEllipseAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_AAEllipseAlpha(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_AAEllipse(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_AAFilledEllipse(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_DoCircle(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint8 R, Uint8 G, Uint8 B, void Callback(SDL_Surface *Surf, Sint16 X, Sint16 Y, Uint32 Color));
DECLSPEC void sge_Circle(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_CircleAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_FilledCircle(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_FilledCircleAlpha(SDL_Surface *Surface, Sint16 x, Sint16 y, Sint16 r, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_AACircleAlpha(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 r, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_AACircle(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 r, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_AAFilledCircle(SDL_Surface *surface, Sint16 xc, Sint16 yc, Sint16 r, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_Bezier(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_BezierAlpha(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
DECLSPEC void sge_AABezier(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_AABezierAlpha(SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,Sint16 x3, Sint16 y3, Sint16 x4, Sint16 y4, int level, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha);
#endif /* sge_C_ONLY */


#endif /* sge_primitives_H */
