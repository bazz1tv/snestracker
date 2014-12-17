#ifndef __MISC_H__
#define __MISC_H__

/* Misc.h

*** Put miscellaneous Routines that may be used by several different files HERE

*/
#include "Globals.h"
#include <sstream>

extern cMouseCursor *pMouse;


/// Toggle fullScreen
int SDL_ToggleFS(SDL_Window*);

/// More english version of 
SDL_bool MouseCollidesWith(SDL_Rect *Crect);

/// The Universal Event Handler handles some events that should be handled in ANY event handler,
/// like Window Resize events for instance
int UniversalEventHandler(SDL_Event*);

template< typename T >
int strtoval(const std::string& str, T &val)
{
	std::istringstream iss(str);
	std::istringstream ess(str);
	//T result;
	
	if( !(iss >> val) )
	{
		string wrong;
		ess >> wrong;
		error << "Error in syntax: " << wrong;
		return 0;
	}//throw "Dude, you need error handling!";
	
	return 1;
}
//
template< typename T >
int strtovals(const std::string& str, T* vals, int n)
{
	std::istringstream iss(str);
	std::istringstream ess(str);
	string wrong;
	
	//T *result = (T*) SDL_malloc (sizeof(T)*n);
	for (int i=0; i < n; i++)
	{
		ess >> wrong;
		if( !(iss >> vals[i]) )
		{
			
			
			error << "Error in syntax: Parameter#" << i+1 << " : " <<wrong ;
			return 0;
		}//throw "Dude, you need error handling!";
	}
	
	return 1;
}
#endif