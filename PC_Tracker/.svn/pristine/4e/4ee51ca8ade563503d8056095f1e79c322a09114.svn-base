/*
*	SDL Graphics Extension
*	Text/TrueType functions (header)
*
*	Started 990815
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)1999-2003 Anders Lindström
*
*	Uses the excellent FreeType 2 library, available at:
*	http://www.freetype.org/
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#ifndef sge_tt_text_H
#define sge_tt_text_H

#include "SDL.h"
#include "sge_internal.h"

/* Text input flags */
#define SGE_IBG SGE_FLAG1
#define SGE_IDEL SGE_FLAG2
#define SGE_INOKR SGE_FLAG3

#ifndef _SGE_NOTTF

/* the truetype font structure */
typedef struct _sge_TTFont sge_TTFont;

/* Font style */
#define SGE_TTF_NORMAL SGE_FLAG0
#define SGE_TTF_BOLD SGE_FLAG1
#define SGE_TTF_ITALIC SGE_FLAG2
#define SGE_TTF_UNDERLINE SGE_FLAG3

/* ZERO WIDTH NO-BREAKSPACE (Unicode byte order mark) */
#define UNICODE_BOM_NATIVE	0xFEFF
#define UNICODE_BOM_SWAPPED	0xFFFE

#endif /* _SGE_NOTTF */



#ifdef _SGE_C
extern "C" {
#endif
#ifndef _SGE_NOTTF
DECLSPEC void sge_TTF_AAOff(void);
DECLSPEC void sge_TTF_AAOn(void);
DECLSPEC void sge_TTF_AA_Alpha(void);
DECLSPEC void sge_TTF_ByteSwappedUNICODE(int swapped);

DECLSPEC int sge_TTF_Init(void);
DECLSPEC sge_TTFont *sge_TTF_OpenFont(const char *file, int ptsize);
DECLSPEC sge_TTFont *sge_TTF_OpenFontRW( SDL_RWops *src, int freesrc, int ptsize, int xdpi, int ydpi);
DECLSPEC sge_TTFont *sge_TTF_OpenFontIndex(const char *file, int ptsize, long index, int xdpi, int ydpi);
DECLSPEC sge_TTFont *sge_TTF_OpenFontIndexRW(SDL_RWops *src, int freesrc, int ptsize, long index, int xdpi, int ydpi);
DECLSPEC int sge_TTF_SetFontSize(sge_TTFont *font, int ptsize);
DECLSPEC int sge_TTF_SetFontSizeDPI(sge_TTFont *font, int ptsize, int xdpi, int ydpi);

DECLSPEC int sge_TTF_FontHeight(sge_TTFont *font);
DECLSPEC int sge_TTF_FontAscent(sge_TTFont *font);
DECLSPEC int sge_TTF_FontDescent(sge_TTFont *font);
DECLSPEC int sge_TTF_FontLineSkip(sge_TTFont *font);
DECLSPEC long sge_TTF_FontFaces(sge_TTFont *font);
DECLSPEC int sge_TTF_FontFaceIsFixedWidth(sge_TTFont *font);
DECLSPEC char *sge_TTF_FontFaceFamilyName(sge_TTFont *font);
DECLSPEC char *sge_TTF_FontFaceStyleName(sge_TTFont *font);

DECLSPEC void sge_TTF_SetFontStyle(sge_TTFont *font, Uint8 style);
DECLSPEC Uint8 sge_TTF_GetFontStyle(sge_TTFont *font);

DECLSPEC void sge_TTF_CloseFont(sge_TTFont *font);

DECLSPEC SDL_Rect sge_TTF_TextSizeUNI(sge_TTFont *font, const Uint16 *text);
DECLSPEC SDL_Rect sge_TTF_TextSize(sge_TTFont *Font, char *Text);

DECLSPEC SDL_Rect sge_tt_textout(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint32 fcolor, Uint32 bcolor, int Alpha);
DECLSPEC SDL_Rect sge_tt_textout_UTF8(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint32 fcolor, Uint32 bcolor, int Alpha);
DECLSPEC SDL_Rect sge_tt_textout_UNI(SDL_Surface *Surface, sge_TTFont *font, const Uint16 *uni, Sint16 x, Sint16 y, Uint32 fcolor, Uint32 bcolor, int Alpha);

DECLSPEC SDL_Rect sge_tt_textoutf(SDL_Surface *Surface, sge_TTFont *font, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha ,char *format,...);

DECLSPEC int sge_tt_input_UNI(SDL_Surface *screen,sge_TTFont *font,Uint16 *string,Uint8 flags, int pos,int len,Sint16 x,Sint16 y, Uint32 fcol, Uint32 bcol, int Alpha);
DECLSPEC int sge_tt_input(SDL_Surface *screen,sge_TTFont *font,char *string,Uint8 flags, int pos,int len,Sint16 x,Sint16 y, Uint32 fcol, Uint32 bcol, int Alpha);

DECLSPEC SDL_Surface *sge_TTF_Render(sge_TTFont *font,const Uint16 *text, SDL_Color fg, SDL_Color bg, int alpha_level);
DECLSPEC SDL_Surface *sge_TTF_RenderUNICODE(sge_TTFont *font,const Uint16 *text, SDL_Color fg, SDL_Color bg);
#endif /* _SGE_NOTTF */

DECLSPEC Uint16 *sge_Latin1_Uni(const char *text);
#ifdef _SGE_C
}
#endif


#ifndef sge_C_ONLY
#ifndef _SGE_NOTTF
DECLSPEC SDL_Rect sge_tt_textout(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha);
DECLSPEC SDL_Rect sge_tt_textout_UTF8(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha);
DECLSPEC SDL_Rect sge_tt_textout_UNI(SDL_Surface *Surface, sge_TTFont *font, const Uint16 *uni, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha);
DECLSPEC int sge_tt_input_UNI(SDL_Surface *screen,sge_TTFont *font,Uint16 *string,Uint8 flags, int pos,int len,Sint16 x,Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR,Uint8 bG,Uint8 bB, int Alpha);
DECLSPEC int sge_tt_input(SDL_Surface *screen,sge_TTFont *font,char *string,Uint8 flags, int pos,int len,Sint16 x,Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR,Uint8 bG,Uint8 bB, int Alpha);
#endif /* _SGE_NOTTF */
#endif /* sge_C_ONLY */

#endif /* sge_tt_text_H */
