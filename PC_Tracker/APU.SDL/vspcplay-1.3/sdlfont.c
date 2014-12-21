#include <string.h>
#include "SDL.h"
#include "font.h"

static void put4pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

static void put4pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	putpixel(surface, x, y, pixel);
	putpixel(surface, x + 1, y, pixel);
	putpixel(surface, x, y + 1, pixel);
	putpixel(surface, x + 1, y + 1, pixel);
}

static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

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

static void _sdlfont_drawChar(SDL_Surface *dst, int X, int Y, const char ch, Uint32 color)
{
	int x, y;
	char *c;
	
	c = font_getChar(ch);
	for (y=0; y<7; y++)
	{
		for (x=0; x<8; x++)
		{
			if ((*c)&(128>>x)) {
				putpixel(dst, x+X, y+Y, color);	
			}
		}
		c++;
	}
}

void sdlfont_drawString(SDL_Surface *dst, int x, int y, const char *string, Uint32 color)
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
			_sdlfont_drawChar(dst, x + (n*8), y, *string, color);
		}
		string++;
	}
	
	if (SDL_MUSTLOCK(dst)) {
		SDL_UnlockSurface(dst);
	}
}



