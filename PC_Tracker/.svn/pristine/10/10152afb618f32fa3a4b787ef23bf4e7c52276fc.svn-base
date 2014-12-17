/*
*	SDL Graphics Extension
*	Text/TrueType font functions
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

/*
*  Most of this code is taken from the SDL ttf lib by Sam Lantinga
*  <slouken@devolution.com>
*/	


#include "SDL.h"
#include "SDL_endian.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "sge_surface.h"
#include "sge_primitives.h"
#include "sge_tt_text.h"
#include "sge_textpp.h"

#ifndef _SGE_NOTTF
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include <freetype/ttnameid.h>
#include <freetype/internal/ftobjs.h>

/* The structure used to hold glyph information (cached) */
struct glyph {
	int stored;
	FT_UInt index;
	//FT_Bitmap bitmap;
	FT_Bitmap pixmap;
	int minx;
	int maxx;
	int miny;
	int maxy;
	int yoffset;
	int advance;
	Uint16 cached;
};

/* the truetype font structure */
struct _sge_TTFont{
	FT_Face face;

	/* Font metrics */
	int height;
	int ascent;
	int descent;
	int lineskip;

	/* The font style */
	Uint8 style;

	/* Extra width in glyph bounds for text styles */
	int glyph_overhang;
	float glyph_italics;

	/* Information in the font for underlining */
	int underline_offset;
	int underline_height;

	/* For now, support Latin-1 character set caching */
	glyph *current;
	glyph cache[256];
	glyph scratch;
	
	/* We are responsible for closing the font stream */
	SDL_RWops *src;
	int freesrc;
	FT_Open_Args args;

	/* For non-scalable formats, we must remember which font index size */
	int font_size_family;
};


/* FIXME: Right now we assume the gray-scale renderer Freetype is using
   supports 256 shades of gray, but we should instead key off of num_grays
   in the result FT_Bitmap after the FT_Render_Glyph() call. */
#define NUM_GRAYS       256

/* Handy routines for converting from fixed point */
#define FT_FLOOR(X)	((X & -64) / 64)
#define FT_CEIL(X)	(((X + 63) & -64) / 64)

#define CACHED_METRICS	0x10
#define CACHED_BITMAP	0x01
#define CACHED_PIXMAP	0x02

/* Fix a problem with older version of Freetype */
#ifndef FT_OPEN_STREAM
	#define FT_OPEN_STREAM ft_open_stream
#endif

/* The FreeType font engine/library */
static FT_Library _sge_library;
static int _sge_TTF_initialized = 0;
static int _sge_TTF_byteswapped = 0;

Uint8 _sge_TTF_AA=1;     //Rendering mode: 0-OFF, 1-AA, 2-Alpha


/**********************************************************************************/
/**                          Open/misc font functions                            **/
/**********************************************************************************/

//==================================================================================
// Turns TTF AntiAliasing On/Off or alpha (nice but slow) (Default: On)
//==================================================================================
void sge_TTF_AAOff(void)
{
	_sge_TTF_AA=0;
}
void sge_TTF_AAOn(void)
{
	_sge_TTF_AA=1;
}
void sge_TTF_AA_Alpha(void)
{
	_sge_TTF_AA=2;
}


//==================================================================================
// Control UNICODE byteswapping (default: no => swapped=0)
//==================================================================================
/* This function tells the library whether UNICODE text is generally
   byteswapped.  A UNICODE BOM character at the beginning of a string
   will override this setting for that string.
 */
void sge_TTF_ByteSwappedUNICODE(int swapped)
{
	_sge_TTF_byteswapped = swapped;
}


//==================================================================================
// Closes the ttf engine, done by exit
//==================================================================================
void sge_TTF_Quit(void)
{
	if ( _sge_TTF_initialized ) {
		FT_Done_FreeType( _sge_library );
	}
	_sge_TTF_initialized = 0;
}


//==================================================================================
// Starts the ttf engine, must be called first
//==================================================================================
int sge_TTF_Init(void)
{
	FT_Error error;

	if( _sge_TTF_initialized )
		return 0;

	error = FT_Init_FreeType( &_sge_library );
	if ( error ) {
		SDL_SetError("SGE - Couldn't init FreeType engine");
		return(-1);
	} else {
		_sge_TTF_initialized = 1;
	}
	atexit(sge_TTF_Quit); //dont't trust the user...
	return 0;
}


//==================================================================================
// Some helper functions
//==================================================================================
void Flush_Glyph(glyph *glyph)
{
	glyph->stored = 0;
	glyph->index = 0;
	//if( glyph->bitmap.buffer ) {
	//	free( glyph->bitmap.buffer );
	//	glyph->bitmap.buffer = 0;
	//}
	if( glyph->pixmap.buffer ) {
		free( glyph->pixmap.buffer );
		glyph->pixmap.buffer = 0;
	}
	glyph->cached = 0;
}
		
void Flush_Cache(sge_TTFont *font)
{
	int i;
	int size = sizeof( font->cache ) / sizeof( font->cache[0] );

	for( i = 0; i < size; ++i ) {
		if( font->cache[i].cached ) {
			Flush_Glyph( &font->cache[i] );
		}

	}
	if( font->scratch.cached ) {
		Flush_Glyph( &font->scratch );
	}
}


//==================================================================================
// Remove font from memory
//==================================================================================
void sge_TTF_CloseFont(sge_TTFont *font)
{
	Flush_Cache( font );
	
	if ( font->face )
		FT_Done_Face( font->face );
	if ( font->args.stream )
		free( font->args.stream );	
	if ( font->freesrc )
		SDL_RWclose( font->src );
	
	free( font );
}


//==================================================================================
// Seek and read stream (internal)
//==================================================================================
unsigned long RWread(FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count)
{
	SDL_RWops *src;

	src = (SDL_RWops *)stream->descriptor.pointer;
	SDL_RWseek( src, (int)offset, SEEK_SET );
	return SDL_RWread( src, buffer, 1, (int)count );
}


//==================================================================================
// Open the TT font sream and returns the font
//==================================================================================
sge_TTFont *sge_TTF_OpenFontIndexRW(SDL_RWops *src, int freesrc, int ptsize, long index, int xdpi, int ydpi)
{
	sge_TTFont *font;
	FT_Error error;
	FT_Face face;
	FT_Fixed scale;
	FT_Stream stream;
	int position;

	if( !_sge_TTF_initialized){
		SDL_SetError("SGE - Freetype not initialized");
		return NULL;
	}

	/* Check to make sure we can seek in this stream */
	position = SDL_RWtell(src);
	if ( position < 0 ) {
		SDL_SetError("SGE - Can't seek in font stream");
		return NULL;
	}

	font = (sge_TTFont *)malloc(sizeof(*font));
	if ( font == NULL ) {
		SDL_SetError("SGE - Out of memory");
		return(NULL);
	}
	memset(font, 0, sizeof(*font));
	
	font->src = src;
	font->freesrc = freesrc;
	
	stream = (FT_Stream)malloc(sizeof(*stream));
	if ( stream == NULL ) {
		SDL_SetError("SGE - Out of memory");
		sge_TTF_CloseFont( font );
		return NULL;
	}
	memset(stream, 0, sizeof(*stream));

	stream->memory = _sge_library->memory;
	stream->read = RWread;
	stream->descriptor.pointer = src;
	stream->pos = (unsigned long)position;
	SDL_RWseek(src, 0, SEEK_END);
	stream->size = (unsigned long)(SDL_RWtell(src) - position);
	SDL_RWseek(src, position, SEEK_SET);

	font->args.flags = FT_OPEN_STREAM;
	font->args.stream = stream;

	/* Open the font stream and create ancillary data */
	error = FT_Open_Face( _sge_library, &font->args, index, &font->face );
	if ( error ) {
		SDL_SetError("SGE - Couldn't open font face");
		sge_TTF_CloseFont( font );
		return NULL;
	}
	face = font->face;
	
	/* Handle scalable font face (global metrics) */
	if ( FT_IS_SCALABLE(face) ) {
		
		/* Set the character size and use DPI in arguments */
		error = FT_Set_Char_Size( font->face, 0, ptsize * 64, xdpi, ydpi );  
		if( error ) {
			SDL_SetError("SGE - Couldn't set font size");
			sge_TTF_CloseFont( font );
			return NULL;
		}
	
		/* Get the scalable font metrics for this font */
		scale = face->size->metrics.y_scale;
		font->ascent  = FT_CEIL(FT_MulFix(face->bbox.yMax, scale));
		font->descent = FT_CEIL(FT_MulFix(face->bbox.yMin, scale));
		font->height  = font->ascent - font->descent + /* baseline */ 1;
		font->lineskip = FT_CEIL(FT_MulFix(face->height, scale));
		font->underline_offset = FT_FLOOR(FT_MulFix(face->underline_position, scale));
		font->underline_height = FT_FLOOR(FT_MulFix(face->underline_thickness, scale));
	} else {
		/* Non-scalable font face.  ptsize determines which family
		 * or series of fonts to grab from the non-scalable format.
		 * It is not the point size of the font.
		 * */
		if ( ptsize >= font->face->num_fixed_sizes )
			ptsize = font->face->num_fixed_sizes - 1;
		
		font->font_size_family = ptsize;
		error = FT_Set_Pixel_Sizes( face, face->available_sizes[ptsize].height, face->available_sizes[ptsize].width );
	  	
		/* With non-scalale fonts, Freetype2 likes to fill many of the
		 * font metrics with the value of 0.  The size of the
		 * non-scalable fonts must be determined differently
		 * or sometimes cannot be determined.
		 * */
	  	font->ascent = face->available_sizes[ptsize].height;
	  	font->descent = 0;
	  	font->height = face->available_sizes[ptsize].height;
	  	font->lineskip = FT_CEIL(font->ascent);
	  	font->underline_offset = FT_FLOOR(face->underline_position);
	  	font->underline_height = FT_FLOOR(face->underline_thickness);
	}
	
	if ( font->underline_height < 1 )
		font->underline_height = 1;
		
	/* Set the default font style */
	font->style = SGE_TTF_NORMAL;
	font->glyph_overhang = face->size->metrics.y_ppem / 10;
	/* x offset = cos(((90.0-12)/360)*2*M_PI), or 12 degree angle */
	font->glyph_italics = 0.207f;
	font->glyph_italics *= font->height;
	
	return font;
}

sge_TTFont *sge_TTF_OpenFontRW( SDL_RWops *src, int freesrc, int ptsize, int xdpi, int ydpi)
{
	return sge_TTF_OpenFontIndexRW(src, freesrc, ptsize, 0, xdpi, ydpi);
}
 
sge_TTFont *sge_TTF_OpenFontIndex( const char *file, int ptsize, long index, int xdpi, int ydpi)
{
	return sge_TTF_OpenFontIndexRW(SDL_RWFromFile(file, "rb"), 1, ptsize, index, xdpi, ydpi);
}

sge_TTFont *sge_TTF_OpenFont(const char *file, int ptsize)
{
	return sge_TTF_OpenFontIndex(file, ptsize, 0, 96, 96);
}


//==================================================================================
// Load a glyph
//==================================================================================
FT_Error Load_Glyph(sge_TTFont *font, Uint16 ch, glyph *cached, int want )
{
	FT_Face face;
	FT_Error error;
	FT_GlyphSlot glyph;
	FT_Glyph_Metrics* metrics;
	FT_Outline* outline;
	
	if ( !font || !font->face ) {
		return FT_Err_Invalid_Handle;
	}

	face = font->face;

	/* Load the glyph */
	if ( ! cached->index ) {
		cached->index = FT_Get_Char_Index( face, ch );
	}
	error = FT_Load_Glyph( face, cached->index, FT_LOAD_DEFAULT );
	if( error ) {
		return error;
	}
	
	/* Get our glyph shortcuts */
	glyph = face->glyph;
	metrics = &glyph->metrics;
	outline = &glyph->outline;
	
	/* Get the glyph metrics if desired */
	if ( (want & CACHED_METRICS) && !(cached->stored & CACHED_METRICS) ) {
		if ( FT_IS_SCALABLE( face ) ) {
			/* Get the bounding box */
			cached->minx = FT_FLOOR(metrics->horiBearingX);
			cached->maxx = cached->minx + FT_CEIL(metrics->width);
			cached->maxy = FT_FLOOR(metrics->horiBearingY);
			cached->miny = cached->maxy - FT_CEIL(metrics->height);
			cached->yoffset = font->ascent - cached->maxy;
			cached->advance = FT_CEIL(metrics->horiAdvance);
		} else {
			/* Get the bounding box for non-scalable format.
			 * Again, freetype2 fills in many of the font metrics
			 * with the value of 0, so some of the values we
			 * need must be calculated differently with certain
			 * assumptions about non-scalable formats.
			 * */
			cached->minx = FT_FLOOR(metrics->horiBearingX);
			cached->maxx = cached->minx + FT_CEIL(metrics->horiAdvance);
			cached->maxy = FT_FLOOR(metrics->horiBearingY);
			cached->miny = cached->maxy - FT_CEIL(face->available_sizes[font->font_size_family].height);
			cached->yoffset = 0;
			cached->advance = FT_CEIL(metrics->horiAdvance);
		}

		/* Adjust for bold and italic text */
		if ( font->style & SGE_TTF_BOLD ) {
			cached->maxx += font->glyph_overhang;
		}
		if ( font->style & SGE_TTF_ITALIC ) {
			cached->maxx += (int)ceil(font->glyph_italics);
		}
		cached->stored |= CACHED_METRICS;
	}

	if ( ((want & CACHED_BITMAP) && !(cached->stored & CACHED_BITMAP)) ||
	     ((want & CACHED_PIXMAP) && !(cached->stored & CACHED_PIXMAP)) ) {
		//int mono = (want & CACHED_BITMAP);
		int i;
		FT_Bitmap* src;
		FT_Bitmap* dst;

		/* Handle the italic style */
		if( font->style & SGE_TTF_ITALIC ) {
			FT_Matrix shear;

			shear.xx = 1 << 16;
			shear.xy = (int) ( font->glyph_italics * ( 1 << 16 ) ) / font->height;
			shear.yx = 0;
			shear.yy = 1 << 16;

			FT_Outline_Transform( outline, &shear );
		}

		/* Render the glyph */
		//if ( mono ) {
		//	error = FT_Render_Glyph( glyph, ft_render_mode_mono );
		//} else {
			error = FT_Render_Glyph( glyph, ft_render_mode_normal );
		//}
		if( error ) {
			return error;
		}

		/* Copy over information to cache */
		src = &glyph->bitmap;
		//if ( mono ) {
		//	dst = &cached->bitmap;
		//} else {
			dst = &cached->pixmap;
		//}
		memcpy( dst, src, sizeof( *dst ) );
		
		/* FT_Render_Glyph() and .fon fonts always generate a
		 * two-color (black and white) glyphslot surface, even
		 * when rendered in ft_render_mode_normal.  This is probably
		 * a freetype2 bug because it is inconsistent with the
		 * freetype2 documentation under FT_Render_Mode section.
		 * */
		if ( !FT_IS_SCALABLE(face) ) {
			dst->pitch *= 8;
		}

		/* Adjust for bold and italic text */
		if( font->style & SGE_TTF_BOLD ) {
			int bump = font->glyph_overhang;
			dst->pitch += bump;
			dst->width += bump;
		}
		if( font->style & SGE_TTF_ITALIC ) {
			int bump = (int)ceil(font->glyph_italics);
			dst->pitch += bump;
			dst->width += bump;
		}

		if( dst->rows != 0 ) {
			dst->buffer = (unsigned char *)malloc( dst->pitch * dst->rows );
			if( !dst->buffer ) {
				return FT_Err_Out_Of_Memory;
			}
			memset( dst->buffer, 0, dst->pitch * dst->rows );

			for( i = 0; i < src->rows; i++ ) {
				int soffset = i * src->pitch;
				int doffset = i * dst->pitch;
				/*if ( mono ) {
					unsigned char *srcp = src->buffer + soffset;
					unsigned char *dstp = dst->buffer + doffset;
					int j;
					for ( j = 0; j < src->width; j += 8 ) {
						unsigned char ch = *srcp++;
						*dstp++ = (ch&0x80) >> 7;
						ch <<= 1;
						*dstp++ = (ch&0x80) >> 7;
						ch <<= 1;
						*dstp++ = (ch&0x80) >> 7;
						ch <<= 1;
						*dstp++ = (ch&0x80) >> 7;
						ch <<= 1;
						*dstp++ = (ch&0x80) >> 7;
						ch <<= 1;
						*dstp++ = (ch&0x80) >> 7;
						ch <<= 1;
						*dstp++ = (ch&0x80) >> 7;
						ch <<= 1;
						*dstp++ = (ch&0x80) >> 7;
					}
				} else */if ( !FT_IS_SCALABLE(face) ) {
					/* This special case wouldn't
					 * be here if the FT_Render_Glyph()
					 * function wasn't buggy when it tried
					 * to render a .fon font with 256
					 * shades of gray.  Instead, it
					 * returns a black and white surface
					 * and we have to translate it back
					 * to a 256 gray shaded surface. 
					 * */
					unsigned char *srcp = src->buffer + soffset;
					unsigned char *dstp = dst->buffer + doffset;
					unsigned char ch;
					int j, k;
					for ( j = 0; j < src->width; j += 8) {
						ch = *srcp++;
						for (k = 0; k < 8; ++k) {
							if ((ch&0x80) >> 7) {
								*dstp++ = NUM_GRAYS - 1;
							} else {
								*dstp++ = 0x00;
							}
							ch <<= 1;
						}
					}
				} else {
					memcpy(dst->buffer+doffset,
				    	   src->buffer+soffset, src->pitch);
				}
			}
		}

		/* Handle the bold style */
		if ( font->style & SGE_TTF_BOLD ) {
			int row;
			int col;
			int offset;
			int pixel;
			Uint8* pixmap;

			/* The pixmap is a little hard, we have to add and clamp */
			for( row = dst->rows - 1; row >= 0; --row ) {
				pixmap = (Uint8*) dst->buffer + row * dst->pitch;
				for( offset=1; offset <= font->glyph_overhang; ++offset ) {
					for( col = dst->width - 1; col > 0; --col ) {
						pixel = (pixmap[col] + pixmap[col-1]);
						if( pixel > NUM_GRAYS - 1 ) {
							pixel = NUM_GRAYS - 1;
						}
						pixmap[col] = (Uint8) pixel;
					}
				}
			}
		}

		/* Mark that we rendered this format */
		//if ( mono ) {
		//	cached->stored |= CACHED_BITMAP;
		//} else {
			cached->stored |= CACHED_PIXMAP;
		//}
	}

	/* We're done, mark this glyph cached */
	cached->cached = ch;

	return 0;
}


//==================================================================================
// Find glyph
//==================================================================================
FT_Error Find_Glyph(sge_TTFont *font, Uint16 ch, int want)
{
	int retval = 0;

	if( ch < 256 ) {
		font->current = &font->cache[ch];
	} else {
		if ( font->scratch.cached != ch ) {
			Flush_Glyph( &font->scratch );
		}
		font->current = &font->scratch;
	}
	if ( (font->current->stored & want) != want ) {
		retval = Load_Glyph( font, ch, font->current, want );
	}
	return retval;
}


//==================================================================================
// Change the size of font
//==================================================================================
int sge_TTF_SetFontSizeDPI(sge_TTFont *font, int ptsize, int xdpi, int ydpi)
{
	FT_Error error;
	FT_Fixed scale;
	FT_Face face;
	
	face = font->face;
	Flush_Cache(font);
	
	if ( FT_IS_SCALABLE(face) ) {
		
		/* Set the character size */
		error = FT_Set_Char_Size( font->face, 0, ptsize * 64, xdpi, ydpi );
		if( error ) {
			sge_SetError("SGE - Couldn't set font size");
			sge_TTF_CloseFont( font );
			return -1;
		}
	
		/* Get the scalable font metrics for this font */
		scale = face->size->metrics.y_scale;
		font->ascent  = FT_CEIL(FT_MulFix(face->bbox.yMax, scale));
		font->descent = FT_CEIL(FT_MulFix(face->bbox.yMin, scale));
		font->height  = font->ascent - font->descent + /* baseline */ 1;
		font->lineskip = FT_CEIL(FT_MulFix(face->height, scale));
		font->underline_offset = FT_FLOOR(FT_MulFix(face->underline_position, scale));
		font->underline_height = FT_FLOOR(FT_MulFix(face->underline_thickness, scale));
	
	} else {
		/* Non-scalable font case.  ptsize determines which family
		 * or series of fonts to grab from the non-scalable format.
		 * It is not the point size of the font.
		 * */
		if ( ptsize >= font->face->num_fixed_sizes )
			ptsize = font->face->num_fixed_sizes - 1;
		
		font->font_size_family = ptsize;
		error = FT_Set_Pixel_Sizes( face, face->available_sizes[ptsize].height, face->available_sizes[ptsize].width );
	  	
		/* With non-scalale fonts, Freetype2 likes to fill many of the
		 * font metrics with the value of 0.  The size of the
		 * non-scalable fonts must be determined differently
		 * or sometimes cannot be determined.
		 * */
	  	font->ascent = face->available_sizes[ptsize].height;
	  	font->descent = 0;
	  	font->height = face->available_sizes[ptsize].height;
	  	font->lineskip = FT_CEIL(font->ascent);
	  	font->underline_offset = FT_FLOOR(face->underline_position);
		font->underline_height = FT_FLOOR(face->underline_thickness);
	}
	
	if ( font->underline_height < 1 ) {
		font->underline_height = 1;
	}
	
	/* Set the default font style */
	//font->style = SGE_TTF_NORMAL;
	font->glyph_overhang = face->size->metrics.y_ppem / 10;
	/* x offset = cos(((90.0-12)/360)*2*M_PI), or 12 degree angle */
	font->glyph_italics = 0.207f;
	font->glyph_italics *= font->height;

	return 0;
}

int sge_TTF_SetFontSize(sge_TTFont *font, int ptsize)
{
	return sge_TTF_SetFontSizeDPI(font, ptsize, 96 ,96);
}


//==================================================================================
// Get font geometrics
//==================================================================================
int sge_TTF_FontHeight(sge_TTFont *font)
{
	return(font->height);
}
int sge_TTF_FontAscent(sge_TTFont *font)
{
	return(font->ascent);
}
int sge_TTF_FontDescent(sge_TTFont *font)
{
	return(font->descent);
}
int sge_TTF_FontLineSkip(sge_TTFont *font)
{
	return(font->lineskip);
}
long sge_TTF_FontFaces(sge_TTFont *font)
{
	return(font->face->num_faces);
}
int sge_TTF_FontFaceIsFixedWidth(sge_TTFont *font)
{
	return(FT_IS_FIXED_WIDTH(font->face));
}
char *sge_TTF_FontFaceFamilyName(sge_TTFont *font)
{
	return(font->face->family_name);
}
char *sge_TTF_FontFaceStyleName(sge_TTFont *font)
{
	return(font->face->style_name);
}
int sge_TTF_GlyphMetrics(sge_TTFont *font, Uint16 ch, int* minx, int* maxx, int* miny, int* maxy, int* advance)
{
	FT_Error error;

	error = Find_Glyph(font, ch, CACHED_METRICS);

	if ( error ) {
		return -1;
	}

	if ( minx ) {
		*minx = font->current->minx;
	}
	if ( maxx ) {
		*maxx = font->current->maxx;
	}
	if ( miny ) {
		*miny = font->current->miny;
	}
	if ( maxy ) {
		*maxy = font->current->maxy;
	}
	if ( advance ) {
		*advance = font->current->advance;
	}
	return 0;
}


//==================================================================================
// Set font style
//==================================================================================
void sge_TTF_SetFontStyle(sge_TTFont *font, Uint8 style)
{
	font->style = style;
	Flush_Cache(font);
}


//==================================================================================
// Get font style
//==================================================================================
Uint8 sge_TTF_GetFontStyle(sge_TTFont *font)
{
	return(font->style);
}
#endif /* _SGE_NOTTF */


//==================================================================================
// Convert the Latin-1 text to UNICODE
//==================================================================================
Uint16 *ASCII_to_UNICODE(Uint16 *unicode, const char *text, int len)
{
	int i;

	for ( i=0; i < len; ++i ) {
		unicode[i] = ((const unsigned char *)text)[i];
	}
	unicode[i] = 0;

	return unicode;
}

Uint16 *sge_Latin1_Uni(const char *text)
{
	Uint16 *unicode_text;
	int i, unicode_len;

	/* Copy the Latin-1 text to a UNICODE text buffer */
	unicode_len = strlen(text);
	unicode_text = (Uint16 *)malloc((unicode_len+1)*(sizeof *unicode_text));
	if ( unicode_text == NULL ) {
		SDL_SetError("SGE - Out of memory");
		return(NULL);
	}
	for ( i=0; i < unicode_len; ++i ) {
		unicode_text[i] = ((const unsigned char *)text)[i];
	}
	unicode_text[i] = 0;

	return(unicode_text);
}

//==================================================================================
// Convert the UTF-8 text to UNICODE
//==================================================================================
Uint16 *UTF8_to_UNICODE(Uint16 *unicode, const char *utf8, int len)
{
	int i, j;
	Uint16 ch;

	for ( i=0, j=0; i < len; ++i, ++j ) {
		ch = ((const unsigned char *)utf8)[i];
		if ( ch >= 0xF0 ) {
			ch  =  (Uint16)(utf8[i]&0x07) << 18;
			ch |=  (Uint16)(utf8[++i]&0x3F) << 12;
			ch |=  (Uint16)(utf8[++i]&0x3F) << 6;
			ch |=  (Uint16)(utf8[++i]&0x3F);
		} else
		if ( ch >= 0xE0 ) {
			ch  =  (Uint16)(utf8[i]&0x3F) << 12;
			ch |=  (Uint16)(utf8[++i]&0x3F) << 6;
			ch |=  (Uint16)(utf8[++i]&0x3F);
		} else
		if ( ch >= 0xC0 ) {
			ch  =  (Uint16)(utf8[i]&0x3F) << 6;
			ch |=  (Uint16)(utf8[++i]&0x3F);
		}
		unicode[j] = ch;
	}
	unicode[j] = 0;

	return unicode;
}

Uint16 *sge_UTF8_Uni(const char *text)
{
	Uint16 *unicode_text;
	int unicode_len;

	/* Copy the UTF-8 text to a UNICODE text buffer */
	unicode_len = strlen(text);
	unicode_text = (Uint16 *)malloc((unicode_len+1)*(sizeof *unicode_text));
	if ( unicode_text == NULL ) {
		SDL_SetError("SGE - Out of memory");
		return(NULL);
	}
	
	return UTF8_to_UNICODE(unicode_text, text, unicode_len);
}

#ifndef _SGE_NOTTF
//==================================================================================
// Get the width of the text with the given font
//==================================================================================
SDL_Rect sge_TTF_TextSizeUNI(sge_TTFont *font, const Uint16 *text)
{
	SDL_Rect ret; ret.x=0; ret.y=0, ret.w=0, ret.h=0;
	const Uint16 *ch;
	int swapped;
	int x, z;
	int minx, maxx;
	int miny, maxy;
	glyph *glyph;
	FT_Error error;

	/* Initialize everything to 0 */
	if ( ! _sge_TTF_initialized ) {
		return ret;
	}

	minx = miny = 0;
	maxx = maxy = 0;
	swapped = _sge_TTF_byteswapped;

	/* Load each character and sum it's bounding box */
	x= 0;
	for ( ch=text; *ch; ++ch ) {
		Uint16 c = *ch;
		if ( c == UNICODE_BOM_NATIVE ) {
			swapped = 0;
			if ( text == ch ) {
				++text;
			}
			continue;
		}
		if ( c == UNICODE_BOM_SWAPPED ) {
			swapped = 1;
			if ( text == ch ) {
				++text;
			}
			continue;
		}
		if ( swapped ) {
			c = SDL_Swap16(c);
		}
		
		error = Find_Glyph(font, c, CACHED_METRICS);
		if ( error ) {
			return ret;
		}
		glyph = font->current;
		
		if ( (ch == text) && (glyph->minx < 0) ) {
			/* Fixes the texture wrapping bug when the first letter
			 * has a negative minx value or horibearing value.  The entire
			 * bounding box must be adjusted to be bigger so the entire
			 * letter can fit without any texture corruption or wrapping.
			 *
			 * Effects: First enlarges bounding box.
			 * Second, xstart has to start ahead of its normal spot in the
			 * negative direction of the negative minx value.
			 * (pushes everything to the right).
			 *
			 * This will make the memory copy of the glyph bitmap data
			 * work out correctly.
			 * */
			z -= glyph->minx;
		}
		
		z = x + glyph->minx;
		if ( minx > z ) {
			minx = z;
		}
		if ( font->style & SGE_TTF_BOLD ) {
			x += font->glyph_overhang;
		}
		if ( glyph->advance > glyph->maxx ) {
			z = x + glyph->advance;
		} else {
			z = x + glyph->maxx;
		}
		if ( maxx < z ) {
			maxx = z;
		}
		x += glyph->advance;

		if ( glyph->miny < miny ) {
			miny = glyph->miny;
		}
		if ( glyph->maxy > maxy ) {
			maxy = glyph->maxy;
		}
	}

	/* Fill the bounds rectangle */
	ret.w = (maxx - minx);
	//ret.h = (maxy - miny); /* This is correct, but breaks many applications */
	ret.h = font->height; 

	return ret;
}

SDL_Rect sge_TTF_TextSize(sge_TTFont *font, char *text)
{
	SDL_Rect ret; ret.x=ret.y=ret.w=ret.y=0;
	Uint16 *unicode_text;
	int unicode_len;

	/* Copy the Latin-1 text to a UNICODE text buffer */
	unicode_len = strlen(text);
	unicode_text = (Uint16 *)malloc((unicode_len+1)*(sizeof *unicode_text));
	if ( unicode_text == NULL ) {
		SDL_SetError("SGE - Out of memory");
		return ret;
	}
	ASCII_to_UNICODE(unicode_text, text, unicode_len);

	/* Render the new text */
	ret = sge_TTF_TextSizeUNI(font, unicode_text);

	/* Free the text buffer and return */
	free(unicode_text);
	
	return ret;
}



/**********************************************************************************/
/**                           TTF output functions                               **/
/**********************************************************************************/

//==================================================================================
// TT Render (unicode)
// Returns an 8bit or 32bit(8/8/8/8-alpha) surface with TT text
//==================================================================================
SDL_Surface *sge_TTF_Render(sge_TTFont *font,const Uint16 *text, SDL_Color fg, SDL_Color bg, int alpha_level)
{
	int xstart, width;
	int w, h;
	SDL_Surface *textbuf;
	SDL_Palette *palette;
	int index;
	int rdiff, gdiff, bdiff;
	int swapped;
	const Uint16 *ch;
	Uint8 *src, *dst;
	Uint32 *dst32;
	Uint32 alpha=0;
	Uint32 pixel=0;
	Uint32 Rmask=0, Gmask=0, Bmask=0, Amask=0;
	int row, col;
	FT_Error error;

	/* Get the dimensions of the text surface */
	SDL_Rect ret=sge_TTF_TextSizeUNI(font, text);
	w=ret.w; h=ret.h;
	if ( !w ) {
		SDL_SetError("SGE - Text has zero width");
		return(NULL);
	}

	/* Create the target surface */
	width = w;	
	if(_sge_TTF_AA!=2) /* Allocate an 8-bit pixmap */
		textbuf = SDL_AllocSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0);
	else{ /* Allocate an 32-bit alpha pixmap */
		if ( SDL_BYTEORDER == SDL_LIL_ENDIAN ) {
			Rmask = 0x000000FF;
			Gmask = 0x0000FF00;
			Bmask = 0x00FF0000;
			Amask = 0xFF000000;
		} else {
			Rmask = 0xFF000000;
			Gmask = 0x00FF0000;
			Bmask = 0x0000FF00;
			Amask = 0x000000FF;
		}
		textbuf = SDL_AllocSurface(SDL_SWSURFACE, w, h, 32, Rmask, Gmask, Bmask, Amask);	
	}
		
	if ( textbuf == NULL ) {
		SDL_SetError("SGE - Out of memory");
		return(NULL);
	}


	/* Setup our colors */
	switch(_sge_TTF_AA){
	
		case 0:{  /* No fancy antialiasing or alpha component */
			palette = textbuf->format->palette;
	
			palette->colors[0].r = bg.r;
			palette->colors[0].g = bg.g;
			palette->colors[0].b = bg.b;
			palette->colors[1].r = fg.r;
			palette->colors[1].g = fg.g;
			palette->colors[1].b = fg.b;	
		}
		break;
	
		case 1:{  /* Fill the palette with NUM_GRAYS levels of shading from bg to fg */
			palette = textbuf->format->palette;	
	
			rdiff = fg.r - bg.r;
			gdiff = fg.g - bg.g;
			bdiff = fg.b - bg.b;
			for ( index=0; index< NUM_GRAYS; ++index ) {
				palette->colors[index].r = bg.r + (index*rdiff)/(NUM_GRAYS-1);
				palette->colors[index].g = bg.g + (index*gdiff)/(NUM_GRAYS-1);
				palette->colors[index].b = bg.b + (index*bdiff)/(NUM_GRAYS-1);
			}
		}
		break;
	
		case 2:{  /* Alpha component magic */
			sge_ClearSurface(textbuf, SDL_MapRGBA(textbuf->format, bg.r,bg.g,bg.b,SDL_ALPHA_TRANSPARENT));
			//pixel = (fg.r<<16)|(fg.g<<8)|fg.b;
			pixel = (fg.b<<16)|(fg.g<<8)|fg.r;
		}
		break;
	}


	/* Load and render each character */
	xstart = 0;
	swapped = _sge_TTF_byteswapped;
	for ( ch=text; *ch; ++ch ) {
		Uint16 c = *ch;
		if ( c == UNICODE_BOM_NATIVE ) {
			swapped = 0;
			if ( text == ch ) {
				++text;
			}
			continue;
		}
		if ( c == UNICODE_BOM_SWAPPED ) {
			swapped = 1;
			if ( text == ch ) {
				++text;
			}
			continue;
		}
		if ( swapped ) {
			c = SDL_Swap16(c);
		}
	
		error = Find_Glyph(font, c, CACHED_METRICS|CACHED_PIXMAP);
		
		if ( ! error ) {
			/* Compensate for wrap around bug with negative minx's */
			if ( (ch == text) && (font->current->minx < 0) ) {
				xstart -= font->current->minx;
			}
			
			w = font->current->pixmap.width;
			for ( row = 0; row < font->current->pixmap.rows; ++row ) {
				/* Make sure we don't go over the limit */
				if ( row+font->current->yoffset >= textbuf->h )
					continue;
					
				dst = (Uint8 *)textbuf->pixels + (row + font->current->yoffset)* textbuf->pitch + xstart + font->current->minx;
				
				switch(_sge_TTF_AA){
				
					case 0:{  /* Normal */	
						src = font->current->pixmap.buffer + row * font->current->pixmap.pitch;
						for ( col=w; col>0; --col ) {
							*dst++ |= (*src++<NUM_GRAYS/2)? 0:1;
						}	
					}
					break;
					case 1:{  /* Antialiasing */
						src = font->current->pixmap.buffer + row * font->current->pixmap.pitch;
						for ( col=w; col>0; --col ) {
							*dst++ |= *src++;
						}	
					}
					break;
					
					case 2:{  /* Alpha */
						src = (Uint8*) (font->current->pixmap.buffer + font->current->pixmap.pitch * row);
						dst32 = (Uint32 *)textbuf->pixels + (row + font->current->yoffset)* textbuf->pitch/4 + xstart + font->current->minx;	
						if( alpha_level == SDL_ALPHA_OPAQUE ){
							for ( col=w; col>0; --col ) {
								alpha = *src++;
								*dst32++ |= pixel | (alpha << 24);
							}
						}else{
							//We need to scale the alpha value
							//Thanks mabi!
							for ( col=w; col>0; --col ) {
								alpha = (*src++) * alpha_level/255;
								*dst32++ |= pixel | (alpha << 24);
							}
						}
					}
					break;
				}
			}

			xstart += font->current->advance;
			if ( font->style & SGE_TTF_BOLD ) {
				xstart += font->glyph_overhang;
			}
		}
	}
	
	/* Handle the underline style */
	if ( font->style & SGE_TTF_UNDERLINE ) {
		int row_offset;

		row_offset = font->ascent - font->underline_offset - 1;
		if ( row_offset > textbuf->h ) {
			row_offset = (textbuf->h-1) - font->underline_height;
		}
		
		if(_sge_TTF_AA==0){
			dst = (Uint8 *)textbuf->pixels + row_offset * textbuf->pitch;
			for ( row=font->underline_height; row>0; --row ) {
				memset(dst, 1, textbuf->w );
				dst += textbuf->pitch;
			}
		}else if(_sge_TTF_AA==1){
			dst = (Uint8 *)textbuf->pixels + row_offset * textbuf->pitch;
			for ( row=font->underline_height; row>0; --row ) {
				memset(dst, NUM_GRAYS - 1, textbuf->w );
				dst += textbuf->pitch;
			}
		}else{
			//pixel |= Amask;
			pixel |= (alpha_level << 24);
			dst32 = (Uint32 *)textbuf->pixels+row_offset*textbuf->pitch/4;
			for ( row=font->underline_height; row>0; --row ) {
				for ( col=0; col < textbuf->w; ++col ) {
					dst32[col] = pixel;
				}
				dst32 += textbuf->pitch/4;
			}
		}	
	}
	return(textbuf);
}

SDL_Surface *sge_TTF_RenderUNICODE(sge_TTFont *font,const Uint16 *text, SDL_Color fg, SDL_Color bg)
{
	return sge_TTF_Render(font, text, fg, bg, SDL_ALPHA_OPAQUE);
}

//==================================================================================
// Renders the Unicode string to TrueType on surface, with the color fcolor.
// bcolor is the target color for the antialiasing.
// Alpha sets the transparency of the text (255-solid, 0-max).
//==================================================================================
SDL_Rect sge_tt_textout_UNI(SDL_Surface *Surface, sge_TTFont *font, const Uint16 *uni, Sint16 x, Sint16 y, Uint32 fcolor, Uint32 bcolor, int Alpha)
{	
	SDL_Rect ret; ret.x=0; ret.y=0; ret.w=0; ret.h=0;	

	SDL_Color temp;
	SDL_Surface *text;

	text=sge_TTF_Render(font,uni,sge_GetRGB(Surface,fcolor),sge_GetRGB(Surface,bcolor), Alpha);
	if(text==NULL){return ret;}

 	/* Align the surface text to the baseline */
	Uint16 ascent=font->ascent;

	temp=sge_GetRGB(Surface,bcolor);
	sge_BlitTransparent(text,Surface,0,0,x,y-ascent,text->w,text->h,SDL_MapRGB(text->format,temp.r,temp.g,temp.b),Alpha);
	sge_UpdateRect(Surface,x,y-ascent,text->w,text->h);

	ret.x=x; ret.y=y-ascent; ret.w=text->w; ret.h=text->h;

	SDL_FreeSurface(text);
	return ret;	
}


//==================================================================================
// Renders the Unicode string to TrueType on surface, with the color fcolor.
// bcolor is the target color for the antialiasing.
// Alpha sets the transparency of the text (0-solid, 255-max). (RGB)
//==================================================================================
SDL_Rect sge_tt_textout_UNI(SDL_Surface *Surface, sge_TTFont *font, const Uint16 *uni, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha)
{
	SDL_Rect ret; ret.x=0; ret.y=0; ret.w=0; ret.h=0;
	SDL_Surface *text;

	text=sge_TTF_Render(font,uni,sge_FillPaletteEntry(fR,fG,fB),sge_FillPaletteEntry(bR,bG,bB), Alpha);
	if(text==NULL){return ret;}

	/* Align the surface text to the baseline */
	Uint16 ascent=font->ascent;

	sge_BlitTransparent(text,Surface,0,0,x,y-ascent,text->w,text->h,SDL_MapRGB(text->format,bR,bG,bB),Alpha);

	sge_UpdateRect(Surface,x,y-ascent,text->w,text->h);

	ret.x=x; ret.y=y-ascent; ret.w=text->w; ret.h=text->h;

	SDL_FreeSurface(text);
	return ret;	
}


//==================================================================================
// Renders the Latin-1 string to TrueType on surface, with the color fcolor.
// bcolor is the target color for the antialiasing.
// Alpha sets the transparency of the text (0-solid, 255-max).
//==================================================================================
SDL_Rect sge_tt_textout(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint32 fcolor, Uint32 bcolor, int Alpha)
{
	SDL_Rect ret;
	Uint16 *uni;

	uni=sge_Latin1_Uni(string);
	
	ret=sge_tt_textout_UNI(Surface,font,uni,x,y,fcolor,bcolor,Alpha);
	free(uni);
	
	return ret;	
}

//==================================================================================
// Renders the Latin-1 string to TrueType on surface, with the color fcolor.
// bcolor is the target color for the antialiasing.
// Alpha sets the transparency of the text (0-solid, 255-max). (RGB)
//==================================================================================
SDL_Rect sge_tt_textout(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha)
{
	SDL_Rect ret;
	Uint16 *uni;

	uni=sge_Latin1_Uni(string);
	
	ret=sge_tt_textout_UNI(Surface,font,uni,x,y, fR,fG,fB, bR,bG,bB, Alpha);
	free(uni);
	
	return ret;	
}


//==================================================================================
// Renders the UTF-8 string to TrueType on surface, with the color fcolor.
// bcolor is the target color for the antialiasing.
// Alpha sets the transparency of the text (0-solid, 255-max).
//==================================================================================
SDL_Rect sge_tt_textout_UTF8(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint32 fcolor, Uint32 bcolor, int Alpha)
{
	SDL_Rect ret;
	Uint16 *uni;

	uni=sge_UTF8_Uni(string);

	ret=sge_tt_textout_UNI(Surface,font,uni,x,y,fcolor,bcolor,Alpha);
	free(uni);
	
	return ret;	
}

//==================================================================================
// Renders the UTF-8 string to TrueType on surface, with the color fcolor.
// bcolor is the target color for the antialiasing.
// Alpha sets the transparency of the text (0-solid, 255-max). (RGB)
//==================================================================================
SDL_Rect sge_tt_textout_UTF8(SDL_Surface *Surface, sge_TTFont *font, const char *string, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha)
{
	SDL_Rect ret;
	Uint16 *uni;

	uni=sge_UTF8_Uni(string);

	ret=sge_tt_textout_UNI(Surface,font,uni,x,y, fR,fG,fB, bR,bG,bB, Alpha);
	free(uni);
	
	return ret;	
}


//==================================================================================
// Renders the formatet Latin-1 string to TrueType on surface, with the color fcolor.
// bcolor is the target color for the antialiasing.
// Alpha sets the transparency of the text (0-solid, 255-max). (RGB ONLY)
// * just like printf(char *format,...) *
//==================================================================================
SDL_Rect sge_tt_textoutf(SDL_Surface *Surface, sge_TTFont *font, Sint16 x, Sint16 y, Uint8 fR, Uint8 fG, Uint8 fB, Uint8 bR, Uint8 bG, Uint8 bB, int Alpha ,char *format,...)
{
	char buf[256];

	va_list ap;
	
	#ifdef __WIN32__
	va_start((va_list*)ap, format); //Stupid win32 crosscompiler
	#else
	va_start(ap, format);
	#endif
	
	vsprintf(buf, format, ap);
	va_end(ap);

	return sge_tt_textout(Surface, font, buf, x,y, fR,fG,fB, bR,bG,bB, Alpha);
}




/**********************************************************************************/
/**                          TTF 'input' functions                               **/
/**********************************************************************************/

// The old code that once lurked here has been replaced by shiny new code that uses
// the text classes.

int sge_tt_input(SDL_Surface *screen, sge_TTFont *font, char *string, Uint8 flags,int pos, int len, Sint16 x,Sint16 y, Uint8 fR,Uint8 fG,Uint8 fB, Uint8 bR,Uint8 bG,Uint8 bB, int Alpha)
{
	if( pos==0 && len>0 )
		string[0] = '\0';

	sge_TextSurface text(screen, string, x, y-sge_TTF_FontAscent(font));
	text.set_ttFont(font, fR,fG,fB, bR, bG, bB);
	text.show_cursor(true);
	text.set_alpha(Alpha);
	text.max_chars(len-1);
	
	int ret = sge_text_input(&text, flags);
	
	strncpy( string, text.get_string(false).c_str(), sizeof(char)*len );
	return ret;
}

int sge_tt_input_UNI(SDL_Surface *screen, sge_TTFont *font, Uint16 *string, Uint8 flags, int pos, int len, Sint16 x,Sint16 y, Uint8 fR,Uint8 fG,Uint8 fB, Uint8 bR,Uint8 bG,Uint8 bB, int Alpha)
{
	sge_TextSurface text(screen, "", x, y-sge_TTF_FontAscent(font));
	
	if( pos!=0 )
		text.change_uctext(string);
	
	text.set_ttFont(font, fR,fG,fB, bR, bG, bB);
	text.show_cursor(true);
	text.set_alpha(Alpha);
	text.max_chars(len-1);
	
	int ret = sge_text_input(&text, flags);
	
	Uint16 *tmp = text.get_ucstring(false);
	
	strncpy( (char*)string, (char*)tmp, sizeof(Uint16)*len );
	
	delete[] tmp;
	
	return ret;
}

int sge_tt_input_UNI(SDL_Surface *screen, sge_TTFont *font, Uint16 *string, Uint8 flags, int pos,int len, Sint16 x,Sint16 y, Uint32 fcol,Uint32 bcol, int Alpha)
{
	Uint8 fr,fg,fb, br,bg,bb;
	
	SDL_GetRGB(fcol, screen->format, &fr, &fg, &fb);
	SDL_GetRGB(bcol, screen->format, &br, &bg, &bb);
	 
	return sge_tt_input_UNI(screen, font, string, flags, pos,len, x,y, fr,fg,fb, br,bg,bb, Alpha);
}


int sge_tt_input(SDL_Surface *screen, sge_TTFont *font, char *string, Uint8 flags, int pos,int len, Sint16 x,Sint16 y, Uint32 fcol,Uint32 bcol, int Alpha)
{
	Uint8 fr,fg,fb, br,bg,bb;
	
	SDL_GetRGB(fcol, screen->format, &fr, &fg, &fb);
	SDL_GetRGB(bcol, screen->format, &br, &bg, &bb);
	 
	return sge_tt_input(screen, font, string, flags, pos,len, x,y, fr,fg,fb, br,bg,bb, Alpha);
}

#endif /* _SGE_NOTTF */
