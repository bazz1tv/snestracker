#include "Misc.h"



int UniversalEventHandler(SDL_Event *event)
{
	switch(event->type)
	{
		case SDL_QUIT:
		{
			return MODE_QUIT;	// Shouldn't return.. should actually QUIT
			break;
		}
		case SDL_WINDOWEVENT:
			{
				switch (event->window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						{
							break;
						}
				}
				break;
			}
		case SDL_KEYDOWN:
		{
			if( event->key.keysym.sym == SDLK_BACKSLASH && (SDL_GetModState() & KMOD_ALT) )
			{
				SDL_ToggleFS(window.sdlw);
			}
			break;
		}

	}
	return 0;
}




string StringPadding(string original, size_t charCount )
{
    original.resize( charCount, ' ' );
    return original;
}

string ParseParm( string &str )
{
	string empty("");
	string temp;
	temp = str;
	size_t found = temp.find_first_of( ' ' );
	
	if (found == string::npos)
		return str;
	
	
	
	string::iterator beginning = temp.begin()+ found+1;
	string::iterator ending = temp.end() ;
	temp.erase( beginning, ending );
	
	str = str.substr(found+1,str.length());
	return temp;
}


/// Check for Collision with the Mouse
/// @returnsSDL_TRUE for collision
SDL_bool MouseCollidesWith(SDL_Rect *Crect)
{
	return pMouse->CollisonCheck( Crect );
}