/// @addtogroup Engine_Font
// @{
/** 

	@file font.h  Font Class

	V 1.3

	LGPL (c) F. Richter

*/


#ifndef __EP_FONT_H__
#define __EP_FONT_H__


#define FONT_TYPE_BLENDED 1
#define FONT_TYPE_SHADED 2
#define FONT_TYPE_SOLID 3

#ifdef _WIN32
#include <SDL_ttf.h>
#else
#include <SDL2_ttf/SDL_ttf.h>
#endif

class cFont
{
public:
	DLLINTERFACE cFont( void );
	DLLINTERFACE ~cFont( void );

	//DLLINTERFACE void RenderText(TextObject *o);

	/**	Creates the text in an Surface 
	 *	types : 
	 *	1 = Blended
	 *	2 = Shaded
	 *	3 = Solid
	 */
	static DLLINTERFACE SDL_Surface *CreateText( const char *text, TTF_Font *font, unsigned int type,
		SDL_Color Text_Color, SDL_Color Background_Color );

	DLLINTERFACE SDL_Surface *CreateText( const char *text, TTF_Font *font, unsigned int type = FONT_TYPE_BLENDED )
	{
		return CreateText( text, font, type, coldefText, coldefBackground );
	}

	/**	Creates an Font from an .ttf file
	 *	default pointsize is 16
	 *	default style is TTF_STYLE_NORMAL
	 */
	DLLINTERFACE TTF_Font *CreateFont( const char *Font_File, unsigned int ptsize = 16, unsigned int style = TTF_STYLE_NORMAL );

	/**	Sets the Style of an Font
	 *	styles :
	 *	TTF_STYLE_NORMAL
	 *	TTF_STYLE_BOLD
	 *	TTF_STYLE_ITALIC
	 *	TTF_STYLE_UNDERLINE
	 */
	DLLINTERFACE void SetFontStyle( TTF_Font *font, unsigned int style = TTF_STYLE_NORMAL );
	
	SDL_Color coldefText, coldefBackground;
};

#endif
// @}