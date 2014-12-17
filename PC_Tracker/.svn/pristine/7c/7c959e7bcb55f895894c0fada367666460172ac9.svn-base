/*
*	SDL Graphics Extension
*	Text/Bitmap font functions
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

/*
*  Some of this is taken from SDL_DrawText by Garrett Banuk (mongoose@wpi.edu)
*  http://www.wpi.edu/~mongoose/SDL_Console
*  Thanks to Karl Bartel for the SFont format!
*/

#include "SDL.h"
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <new>
#include "sge_surface.h"
#include "sge_bm_text.h"
#include "sge_tt_text.h"
#include "sge_textpp.h"

#ifdef _SGE_HAVE_IMG
#include <SDL_image.h>
#endif

using namespace std;

/* Globals used for sge_Update/sge_Lock (defined in sge_surface) */
extern Uint8 _sge_update;
extern Uint8 _sge_lock;

//==================================================================================
// Creates a new font from a surface
//==================================================================================
sge_bmpFont* sge_BF_CreateFont(SDL_Surface *surface, Uint8 flags)
{
	sge_bmpFont	*font;

	font = new(nothrow) sge_bmpFont; if(font==NULL){SDL_SetError("SGE - Out of memory");return NULL;}
	
	if(!(flags&SGE_BFNOCONVERT) && !(flags&SGE_BFSFONT)){    /* Get a converted copy */
		font->FontSurface = SDL_DisplayFormat(surface);
		if(font->FontSurface==NULL){SDL_SetError("SGE - Out of memory");return NULL;}
		
		if(flags&SGE_BFPALETTE){  //We want an 8bit surface
			SDL_Surface *tmp;
			tmp = SDL_AllocSurface(SDL_SWSURFACE, surface->w, surface->h, 8, 0, 0, 0, 0);
			if(tmp==NULL){SDL_SetError("SGE - Out of memory");SDL_FreeSurface(font->FontSurface);return NULL;}
			
			//Set the palette
			SDL_Color c[2];
			c[0].r=0; c[1].r=255;
			c[0].g=0; c[1].g=255;
			c[0].b=0; c[1].b=255;
			SDL_SetColors(tmp, c, 0, 2);
			
			if (SDL_MUSTLOCK(font->FontSurface) && _sge_lock)
				if (SDL_LockSurface(font->FontSurface) < 0){
					SDL_SetError("SGE - Locking error");
					SDL_FreeSurface(font->FontSurface);
					return NULL;
				}

			//Copy the font to the 8bit surface
			Sint16 x,y;
			Uint32 bc=sge_GetPixel(font->FontSurface,0,surface->h-1);
			for(y=0; y<font->FontSurface->h; y++){
   				for(x=0; x<font->FontSurface->w; x++){
   					if(sge_GetPixel(font->FontSurface,x,y)==bc)
						*((Uint8 *)tmp->pixels + y * tmp->pitch + x)=0;
					else
						*((Uint8 *)tmp->pixels + y * tmp->pitch + x)=1;
   				}
   			}
			
			if (SDL_MUSTLOCK(font->FontSurface) && _sge_lock) {
				SDL_UnlockSurface(font->FontSurface);
			}

			//sge_Blit(surface, tmp, 0,0,0,0,surface->w, surface->h);
			SDL_FreeSurface(font->FontSurface);
			font->FontSurface=tmp;	
		}
			
		
		if((flags&SGE_FLAG8))
			SDL_FreeSurface(surface);
	}
	else if(flags&SGE_FLAG8) /* Use the source */
		font->FontSurface = surface;
	else                     /* Get a copy */
		font->FontSurface = sge_copy_surface(surface);
		
	if(font->FontSurface==NULL){SDL_SetError("SGE - Out of memory");return NULL;}
		
	SDL_Surface *fnt = font->FontSurface; //Shorthand
	font->Chars=0;
			
	if(!(flags&SGE_BFSFONT)){  /* Fixed width font */
		font->CharWidth = font->FontSurface->w/256;
		font->CharHeight = font->FontSurface->h;
		font->CharPos = NULL;
		font->yoffs = 0;
		font->Chars=256;
	}
	else{                    /* Karl Bartel's sfont */
		Sint16 x=0;
		int i=0;
		
		font->CharPos = new(nothrow) Sint16[256];
		if(!font->CharPos){SDL_SetError("SGE - Out of memory");sge_BF_CloseFont(font);return NULL;}
		
		Uint32 color = sge_GetPixel(fnt,0,0);  //get data color

		while ( x < fnt->w && font->Chars < 256 ){
			if (sge_GetPixel(fnt,x,0)==color) {
				font->CharPos[i++]=x;
				
				while ( x < fnt->w-1 && sge_GetPixel(fnt,x,0) == color )
					x++;
			
				font->CharPos[i++]=x;
				font->Chars++;
			}
			x++;
		}
		
		font->CharHeight = font->FontSurface->h-1;
		font->CharWidth = 0;
		font->yoffs = 1;
	}

	/* Set font as transparent if the flag is set */
	if (SDL_MUSTLOCK(font->FontSurface) && _sge_lock)
		if (SDL_LockSurface(font->FontSurface) < 0){
			return font;
		}
	font->bcolor=sge_GetPixel(font->FontSurface,0,font->FontSurface->h-1);
	if (SDL_MUSTLOCK(font->FontSurface) && _sge_lock) {
		SDL_UnlockSurface(font->FontSurface);
	}
	if(flags&SGE_BFTRANSP || flags&SGE_BFSFONT)
		#if SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL) >= \
    		SDL_VERSIONNUM(1, 1, 4)
		SDL_SetColorKey(font->FontSurface,SDL_SRCCOLORKEY, font->bcolor);  //Some versions of SDL have a bug with SDL_RLEACCEL
		#else
		SDL_SetColorKey(font->FontSurface,SDL_SRCCOLORKEY|SDL_RLEACCEL, font->bcolor);
		#endif

	return font;
}


//==================================================================================
// Loads the font into a new struct
//==================================================================================
sge_bmpFont* sge_BF_OpenFont(char *file, Uint8 flags)
{
	sge_bmpFont *font;
	SDL_Surface	*Temp;
	
	/* load the font bitmap */
	#ifdef _SGE_HAVE_IMG
	if(NULL ==  (Temp = IMG_Load(file)))  //We have SDL_Img lib!
	#else
	if(NULL ==  (Temp = SDL_LoadBMP(file))) //We can only load bmp files...
	#endif
	{
		sge_SetError("SGE - Couldn't load font file: %s",file);
		return NULL;
	}

	font = sge_BF_CreateFont(Temp,flags|SGE_FLAG8); //SGE_FLAG8 - no need to make a copy of the surface
	
	return font;
}


//==================================================================================
// Draws string to surface with the selected font
// Returns pos. and size of the drawn text
//==================================================================================
SDL_Rect sge_BF_textout(SDL_Surface *surface, sge_bmpFont *font, char *string, Sint16 x, Sint16 y)
{
	SDL_Rect ret;  ret.x=0;ret.y=0;ret.w=0;ret.h=0;

	if(font==NULL){return ret;}

	int characters;
	Sint16 xsrc,xdest,ofs,adv=font->CharWidth;
	float diff=0;

	/* Valid coords ? */
	if(surface)
		if(x>surface->w || y>surface->h) 
			return ret;

	characters = strlen(string);

	xdest=x;

	/* Now draw it */
	for(int i=0; i<characters; i++)
	{
		if(!font->CharPos) /* Fixed width */
			xsrc = string[i] * font->CharWidth;
		else{              /* Variable width */
			if(string[i]==' ' || (string[i]-33)>font->Chars || string[i]<33){
				xdest += font->CharPos[2]-font->CharPos[1];
				continue;
			}	
			ofs = (string[i]-33)*2+1;
			xsrc = (font->CharPos[ofs]+font->CharPos[ofs-1])/2;
			//font->CharWidth = (font->CharPos[ofs+2]+font->CharPos[ofs+1])/2-(font->CharPos[ofs]+font->CharPos[ofs-1])/2-1;
			font->CharWidth = (font->CharPos[ofs+2]+font->CharPos[ofs+1])/2-(font->CharPos[ofs]+font->CharPos[ofs-1])/2;
			adv = font->CharPos[ofs+1]-font->CharPos[ofs];
			diff =float( (font->CharPos[ofs]-font->CharPos[ofs-1])/2.0 );
		}
		
		if(surface)
			sge_Blit(font->FontSurface, surface, xsrc,font->yoffs, int(xdest-diff),y, font->CharWidth,font->CharHeight);	
		
		xdest += adv;
	}

	ret.x=x; ret.y=y; ret.w=xdest-x+font->CharWidth; ret.h=font->CharHeight;
	
	if(surface)
		sge_UpdateRect(surface, x, y, ret.w, ret.h);
	
	return ret;
}


//==================================================================================
// Returns the size (w and h) of the string (if rendered with font)
//==================================================================================
SDL_Rect sge_BF_TextSize(sge_bmpFont *font, char *string)
{
	return sge_BF_textout(NULL, font, string, 0,0);
}


//==================================================================================
// Draws formated text to surface with the selected font
// Returns pos. and size of the drawn text
// * just like printf(char *format, ...) *
//==================================================================================
SDL_Rect sge_BF_textoutf(SDL_Surface *surface, sge_bmpFont *font, Sint16 x, Sint16 y , char *format, ...)
{
	char buf[256];

	va_list ap;
	
	#ifdef __WIN32__
	va_start((va_list*)ap, format); //Stupid w32 crosscompiler
	#else
	va_start(ap, format);
	#endif
	
	vsprintf(buf, format, ap);
	va_end(ap);

	return sge_BF_textout(surface, font, buf, x, y);
}


//==================================================================================
// Returns the height of the font
// Returns 0 if the struct was invalid
//==================================================================================
Sint16 sge_BF_GetHeight(sge_bmpFont *font)
{
	if(font)
		return font->CharHeight;
	else
		return 0;
}


//==================================================================================
// Returns the width of the font (only fixed width fonts)
// Returns 0 if the struct was invalid
//==================================================================================
Sint16 sge_BF_GetWidth(sge_bmpFont *font)
{
	if(font)
		return font->CharWidth;
	else
		return 0;
}


//==================================================================================
// Removes font from memory
//==================================================================================
void sge_BF_CloseFont(sge_bmpFont *font)
{
	if(font){
		SDL_FreeSurface(font->FontSurface);
		
		if(font->CharPos)
			delete[] font->CharPos;
			
		delete font;
		font=NULL;
	}
}


//==================================================================================
// Change the font color
// Will not work on 'color' fonts!
// Doesn't like 24bpp
//==================================================================================
void sge_BF_SetColor(sge_bmpFont *font, Uint8 R, Uint8 G, Uint8 B)
{
	if(font==NULL){return;}

	if(!font->FontSurface->format->palette){  //Slow truecolor version
		Sint16 x,y;
		Sint16 ypnt;
		SDL_Surface *surface=font->FontSurface;
		Uint32 c_keep=font->bcolor;
	
		Uint32 color=SDL_MapRGB(font->FontSurface->format, R, G, B);

		switch(surface->format->BytesPerPixel){
  			case 1: { /* Assuming 8-bpp */
  				Uint8 *pnt;
   				for(y=0; y<surface->h; y++){
   					ypnt=y*surface->pitch;
   					for(x=0; x<surface->w; x++){
   						pnt=((Uint8 *)surface->pixels + x + ypnt);
   						if(*pnt!=c_keep){*pnt=(Uint8)color;}
   					}
   				}
   			}
   			break;

			case 2: { /* Probably 15-bpp or 16-bpp */
				Uint16 *pnt;
    			for(y=0; y<surface->h; y++){
   					ypnt=y*surface->pitch/2;
   					for(x=0; x<surface->w; x++){
   						pnt=((Uint16 *)surface->pixels + x + ypnt);
   						if(*pnt!=c_keep){*pnt=(Uint16)color;}
   					}
   				}
    		}
			break;

			case 3: { /* Slow 24-bpp mode, usually not used */
			}
    		break;

			case 4: { /* Probably 32-bpp */
				Uint32 *pnt;
    			for(y=0; y<surface->h; y++){
   					ypnt=y*surface->pitch/4;
   					for(x=0; x<surface->w; x++){
   						pnt=((Uint32 *)surface->pixels + x + ypnt);
   						if(*pnt!=c_keep){*pnt=(Uint32)color;}
   					}
   				}	
    		}
			break;
		}
	}else{  //Fast palette version
		SDL_Color c[2];
		c[0].r=0; c[1].r=R;
		c[0].g=0; c[1].g=G;
		c[0].b=0; c[1].b=B;
		SDL_SetColors(font->FontSurface, c, 0, 2);
	}
}


//==================================================================================
// Sets an alpha value for the font (don't work with sfonts with an alpha channel)
//==================================================================================
void sge_BF_SetAlpha(sge_bmpFont *font, Uint8 alpha)
{
	SDL_SetAlpha(font->FontSurface,SDL_SRCALPHA|SDL_RLEACCEL, alpha);
}


//==================================================================================
// BitmapText input
//==================================================================================
int sge_BF_inputAlpha(SDL_Surface *screen, sge_bmpFont *font, char *string, Uint8 flags, int pos,int len, Sint16 x, Sint16 y, int Alpha)
{
	if( pos==0 && len>0 )
		string[0] = '\0';

	sge_TextSurface text(screen, string, x, y);
	text.set_bmFont(font);
	text.show_cursor(true);
	text.set_alpha(Alpha);
	text.max_chars(len-1);
	
	int ret = sge_text_input(&text, flags);
	
	strncpy( string, text.get_string(false).c_str(), sizeof(char)*len );
	return ret;
}

int sge_BF_inputAlpha_UNI(SDL_Surface *screen, sge_bmpFont *font, Uint16 *string, Uint8 flags, int pos,int len, Sint16 x,Sint16 y, int Alpha)
{
	sge_TextSurface text(screen, "", x, y);
	
	if( pos!=0 )
		text.change_uctext(string);
	
	text.set_bmFont(font);
	text.show_cursor(true);
	text.set_alpha(Alpha);
	text.max_chars(len-1);
	
	int ret = sge_text_input(&text, flags);
	
	Uint16 *tmp = text.get_ucstring(false);
	
	strncpy( (char*)string, (char*)tmp, sizeof(Uint16)*len );
	
	delete[] tmp;
	
	return ret;
}

int sge_BF_input(SDL_Surface *screen,sge_bmpFont *font,char *string, Uint8 flags, int pos,int len,Sint16 x,Sint16 y)
{
	return sge_BF_inputAlpha(screen, font, string, flags, pos, len, x,y, SDL_ALPHA_OPAQUE);
}

int sge_BF_input_UNI(SDL_Surface *screen, sge_bmpFont *font, Uint16 *string, Uint8 flags, int pos,int len, Sint16 x,Sint16 y)
{
	return sge_BF_inputAlpha_UNI(screen, font, string, flags, pos, len, x,y, SDL_ALPHA_OPAQUE);
}
