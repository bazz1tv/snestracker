#include "sdlfont.h"
#include <string.h>
#include "SDL.h"
#include "font.h"
#include "utility.h"


// unused // static void put4pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void put4pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	putpixel(surface, x, y, pixel);
	putpixel(surface, x + 1, y, pixel);
	putpixel(surface, x, y + 1, pixel);
	putpixel(surface, x + 1, y + 1, pixel);
}

void put44pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	for (int yy=y; yy < y+8; yy++)
	{
		putpixel(surface, x, yy, pixel);
		putpixel(surface, x+1, yy, pixel);
		putpixel(surface, x+2, yy, pixel);
		putpixel(surface, x+3, yy, pixel);
		putpixel(surface, x+4, yy, pixel);
		putpixel(surface, x+5, yy, pixel);
		putpixel(surface, x+6, yy, pixel);
		putpixel(surface, x+7, yy, pixel);
	}
	
}

static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	pixel |= 0xff000000;

	switch (bpp)
	{
		case 1:
			*p = pixel;
			break;
		case 2:
			*(Uint16 *)p = pixel;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) * 0xff;
				p[2] = (pixel & 0xff);
			} else {
				p[0] = (pixel & 0xff);
				p[1] = (pixel >> 8) * 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

static void _sdlfont_drawChar(SDL_Surface *dst, int X, int Y, const char ch, Uint32 color, 
	bool flipV=false, bool flipH=false)
{
	int x, y;
	unsigned char *c;
	
	c = font_getChar(ch);
	
	if (flipV) c += 6;

	for (y=0; y<7; y++)
	{
		/*if (flipH)
		{
			for (x=7; x>=0; x--)
			{
				if ((*c)&(128>>x)) {
					putpixel(dst, x+X, y+Y, color);	
				}
			}
		}
		else*/
		{
			for (x=0; x<8; x++)
			{
				if ( *c & (0x80 >> (flipH ? 7-x:x) ) ) {
					putpixel(dst, x+X, y+Y, color);	
				}
			}
		}
		if (flipV)
			c--;
		else c++;
	}
}

static void _sdlfont_drawChar2c(SDL_Surface *dst, int X, int Y, const char ch, Uint32 color1, Uint32 color2)
{
	int x, y;
	unsigned char *c;
	
	c = font_getChar2c(ch);
	for (y=0; y<7; y++)
	{
		for (x=0; x<8; x++)
		{
			//char res = (*c)&(128>>x);
			if ((*c)&(128>>x)) {
				putpixel(dst, x+X, y+Y, color1);	
			}
			else if ( *(c+7) & (128>>x)  )
			{
				putpixel(dst, x+X, y+Y, color2);	
			}
		}
		c++;
	}
}

void sdlfont_drawString2(SDL_Surface *dst, int x, int y, const char *string, Colors::Interface::Type bg_color_type,Colors::Interface::Type fg_color_type/*=Colors::Interface::Type::text_fg*/, 
bool prefill/*=true*/, bool flipV/*=false*/, bool flipH/*=false*/)
{
	int n, len;
	SDL_Rect under;
 
	len = strlen(string);

	under.x = x; under.y = y;
	under.h = 8; under.w = 8*len;
	if (prefill)
		SDL_FillRect(dst, &under, Colors::Interface::color[bg_color_type]);

	if (SDL_MUSTLOCK(dst)) {
		SDL_LockSurface(dst);
	}

	for (n=0; n<len; n++)
	{
		if ((n*8)+x+8 >= dst->w) {
		} else {
			_sdlfont_drawChar(dst, x + (n*8), y, *string, Colors::Interface::color[fg_color_type], flipV, flipH);
		}
		string++;
	}
	
	if (SDL_MUSTLOCK(dst)) {
		SDL_UnlockSurface(dst);
	}
}

void sdlfont_drawString(SDL_Surface *dst, int x, int y, const char *string, Uint32 fg_color/*=Colors::Interface::text_fg*/, 
Uint32 bg_color/*=Colors::Interface::color[Colors::Interface::text_bg]*/, bool prefill/*=true*/, bool flipV/*=false*/, bool flipH/*=false*/)
{
	int n, len;
	SDL_Rect under;
 
	len = strlen(string);

	under.x = x; under.y = y;
	under.h = 8; under.w = 8*len;
	if (prefill)
		SDL_FillRect(dst, &under, bg_color);

	if (SDL_MUSTLOCK(dst)) {
		SDL_LockSurface(dst);
	}

	for (n=0; n<len; n++)
	{
		if ((n*8)+x+8 >= dst->w) {
		} else {
			_sdlfont_drawChar(dst, x + (n*8), y, *string, fg_color, flipV, flipH);
		}
		string++;
	}
	
	if (SDL_MUSTLOCK(dst)) {
		SDL_UnlockSurface(dst);
	}
}

void sdlfont_drawString2c(SDL_Surface *dst, int x, int y, const char *string, Uint32 color1, Uint32 color2)
{
	int n, len;
	SDL_Rect under;
 
	len = strlen(string);

	under.x = x; under.y = y;
	under.h = 8; under.w = 8*len;
	SDL_FillRect(dst, &under, 0);

	if (SDL_MUSTLOCK(dst)) {
		SDL_LockSurface(dst);
	}

	for (n=0; n<len; n++)
	{
		if ((n*8)+x+8 >= dst->w) {
		} else {
			_sdlfont_drawChar2c(dst, x + (n*8), y, *string, color1, color2);
		}
		string++;
	}
	
	if (SDL_MUSTLOCK(dst)) {
		SDL_UnlockSurface(dst);
	}
}



