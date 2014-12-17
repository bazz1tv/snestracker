
#include "SDL_ep.h"

SDL_Color cFont :: coldefText = SetColor( 255, 255, 255 );
SDL_Color cFont :: coldefBackground = SetColor( 0, 0, 100 );

cFont :: cFont( void )
{
	coldefBackground = SetColor( 0, 0, 100 );	// Default Background Color : Dark Blue
	coldefText = SetColor( 255, 255, 255 );	// Default Text Color : White

	if( !TTF_WasInit() ) 
	{
		TTF_Init();
	}
}

cFont :: ~cFont( void )
{
	if( TTF_WasInit() ) 
	{
		TTF_Quit();
	}
}

/*void cFont::RenderText(TextObject *o)
{
	CreateText(o->text.c_str(), o->font);
}*/

SDL_Surface *cFont :: CreateText( const char *text, TTF_Font *font, unsigned int type,
		SDL_Color Text_Color, SDL_Color Background_Color)
{
	if( !font ) 
	{
		printf( "CreateText Error : Font is not allocated\n" );
		return NULL;
	}

	if( type == 1 )
	{
		return TTF_RenderText_Blended( font, text, Text_Color );
	}
	else if ( type == 2 ) 
	{
		return TTF_RenderText_Shaded( font, text, Text_Color, Background_Color );
	}
	else if ( type == 3 ) 
	{
		return TTF_RenderText_Solid( font, text, Text_Color );
	}
	else
	{
		printf( "CreateText Error : unknown Font type : %d\n", type );
		return NULL;
	}
}

TTF_Font *cFont :: CreateFont( const char *Font_File, unsigned int ptsize, unsigned int style /* = 0  */ )
{
	TTF_Font *font = TTF_OpenFont( Font_File, ptsize );
	
	if( !font ) 
	{
		printf( "CreateFont Error : Could not create font : %s\n", Font_File );
		return NULL;
	}

	if( style ) 
	{
		SetFontStyle( font, style );
	}
	

	return font;
}

void cFont :: SetFontStyle( TTF_Font *font, unsigned int style /* = 0  */ )
{
	if( !font )
	{
		printf( "SetFontStyle Error : Font is not allocated\n" );
		return;
	}
	
	if( style < 5 && font && ( (unsigned int)TTF_GetFontStyle( font ) != style) )
	{
		TTF_SetFontStyle( font, style );
	}
}