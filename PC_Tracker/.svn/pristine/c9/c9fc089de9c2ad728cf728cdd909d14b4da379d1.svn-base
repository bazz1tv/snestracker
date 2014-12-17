/*
*	SDL Graphics Extension
*	Text/Bitmap font functions (header)
*
*	Started 990815
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

#ifndef sge_bm_text_H
#define sge_bm_text_H

#include "SDL.h"
#include "sge_internal.h"

/* BF open flags */
#define SGE_BFTRANSP SGE_FLAG1
#define SGE_BFSFONT SGE_FLAG2
#define SGE_BFNOCONVERT SGE_FLAG3
#define SGE_BFPALETTE SGE_FLAG4

/* Text input flags */
#define SGE_IBG SGE_FLAG1
#define SGE_IDEL SGE_FLAG2
#define SGE_INOKR SGE_FLAG3

/* the bitmap font structure */
typedef struct{
	SDL_Surface *FontSurface;
	Sint16      CharWidth;
	Sint16      CharHeight;
	Sint16      *CharPos;
	Sint16      yoffs;
	Uint32      bcolor;
	Sint16      Chars;					
} sge_bmpFont;


#ifdef _SGE_C
extern "C" {
#endif
DECLSPEC sge_bmpFont* sge_BF_CreateFont(SDL_Surface *surface, Uint8 flags);
DECLSPEC sge_bmpFont* sge_BF_OpenFont(char *file, Uint8 flags);
DECLSPEC void sge_BF_CloseFont(sge_bmpFont *font);
DECLSPEC void sge_BF_SetColor(sge_bmpFont *font, Uint8 R, Uint8 G, Uint8 B);
DECLSPEC void sge_BF_SetAlpha(sge_bmpFont *font, Uint8 alpha);
DECLSPEC Sint16 sge_BF_GetHeight(sge_bmpFont *font);
DECLSPEC Sint16 sge_BF_GetWidth(sge_bmpFont *font);
DECLSPEC SDL_Rect sge_BF_TextSize(sge_bmpFont *font, char *string);

DECLSPEC SDL_Rect sge_BF_textout(SDL_Surface *surface, sge_bmpFont *font, char *string, Sint16 x, Sint16 y);
DECLSPEC SDL_Rect sge_BF_textoutf(SDL_Surface *surface, sge_bmpFont *font, Sint16 x, Sint16 y , char *format, ...);

DECLSPEC int sge_BF_input(SDL_Surface *screen,sge_bmpFont *font,char *string, Uint8 flags,int pos,int len,Sint16 x,Sint16 y);
DECLSPEC int sge_BF_inputAlpha(SDL_Surface *screen, sge_bmpFont *font, char *string, Uint8 flags, int pos,int len, Sint16 x, Sint16 y, int Alpha);
DECLSPEC int sge_BF_input_UNI(SDL_Surface *screen, sge_bmpFont *font, Uint16 *string, Uint8 flags, int pos,int len, Sint16 x,Sint16 y);
DECLSPEC int sge_BF_inputAlpha_UNI(SDL_Surface *screen, sge_bmpFont *font, Uint16 *string, Uint8 flags, int pos,int len, Sint16 x,Sint16 y, int Alpha);
#ifdef _SGE_C
}
#endif


#endif /* sge_bm_text_H */
