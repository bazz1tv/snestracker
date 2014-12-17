

#include "SDL_ep.h"

SDL_Surface *LoadImage( const char *szFilename, Uint32 Colorkey /* = 0 */, Uint8 Alpha /* = 255  */ )
{
	FILE* fp = fopen( szFilename, "r" );

	if ( !fp )
	{
		printf( "Error loading file failed : %s\n", szFilename );
		return NULL;
	} 
	else
	{
		fclose( fp );
	}

	SDL_Surface *image, *temp;
	
	temp = IMG_Load( szFilename );

	if ( !temp )
	{
		printf("Error loading file : %s\n",szFilename);
		return NULL;
	}
    
    image = temp; //SDL_DisplayFormat( temp );
	
	SDL_SetColorKey( image, SDL_TRUE, Colorkey );
	//SDL_SetAlpha( image, SDL_TRUE, Alpha );
	if (SDL_SetSurfaceAlphaMod(image, Alpha) < 0)
	{
		printf("SDL_SetSurfaceAlphaMode failed!");
		return NULL;
	}
	

	//SDL_FreeSurface( temp );

	return image;
}

// SDL2 flags are obsolete and should be set to 0
SDL_Surface *CreateSurface( unsigned int width, unsigned int height, Uint8 bpp /* = 32 */, 
			Uint32 flags /* = 0 */)
{
	if( !width || !height ) 
	{
		printf("CreateRGBSurface Error : 0 dimension specified\n");
		return NULL;
	}

    Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	SDL_Surface *image, *temp;

	temp = SDL_CreateRGBSurface( flags, width, height, bpp, rmask, gmask, bmask, amask );

	image = temp; // SDL_DisplayFormat( temp );

	//SDL_FreeSurface( temp );
	
	if( !image ) 
	{
        printf( "CreateRGBSurface failed : %s\n", SDL_GetError() );
    }
	
	return image;
}