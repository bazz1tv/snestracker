#ifndef __MISC_H__
#define __MISC_H__

/* Misc.h

*** Put miscellaneous Routines that may be used by several different files HERE

*/
#include "Globals.h"
#include <sstream>

extern cMouseCursor *pMouse;


string StringPadding(string original, size_t charCount );

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
	char x;
	T temp;
	//T result;
	
	if( !(iss >> temp) )
	{
		string wrong;
		ess >> wrong;
		error << "Error in syntax: " << wrong;
		return 0;
	}//throw "Dude, you need error handling!";
	else {
		if  (!(iss >> x))
		{
			val = temp;
            return 1;
		}
        else
		{
            error << "Failed to convert string to int: junk in input: " << x;
			return 0;
		}
	}
	
	return 1;
}

// this is used by strtovals to properly check each parameter
string ParseParm( string &str );
//
using namespace std;
template< typename T >
int strtovals( std::string& str, T* vals, int n)
{
	
	string wrong;
	char x;
	T temp;
	string parm;
	
	for (int i=0; i < n; i++)
	{
		parm = ParseParm(str);
		
		std::istringstream iss(parm);
		std::istringstream ess(parm);
		
		
		ess >> wrong;
		if( !(iss >> temp) )
		{
			
			
			error << "Error in syntax: Parameter#" << i+1 << " : " <<wrong ;
			return 0;
		}
		else
		{
			if  (!(iss >> x))
			{
				vals[i] = temp;
				continue;
			}
			else
			{
				error << "Failed to convert string to int: junk in input: " << x;
				return 0;
			}
		}
	}
	
	return 1;
}
#endif