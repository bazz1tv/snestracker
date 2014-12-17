
#include "SDL_ep.h"

#include <time.h>

int InitEP( void )
{
	srand( (unsigned int)time(NULL) );
	
	static double Spritespeedfactor = 1.0; // hmmmm static (untested)
	
	SetSpriteSpeedfactor( &Spritespeedfactor );
	
	return 1;
}