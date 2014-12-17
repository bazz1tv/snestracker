
#include "SDL_ep.h"
#include <sstream>
#include <ctime>


float Random( float min, float max )
{
	return min + ( rand()/(float)RAND_MAX ) * (max - min ); 
}

int Random( int min, int max )
{
	return min + ( rand() % (max - min + 1) ) ;
} 

int Random( int max )
{
	return Random( 0, max );
}

float Random( float max )
{
	return Random( 0.0f, max );
}

double Random( double max )
{
	return Random( 0.0f, (float)max );
}

double Random( double min, double max )
{
	return Random( (float)min, (float)max );
}

SDL_Rect SetRect( int x, int y, unsigned int w, unsigned int h )
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	return rect;
}

SDL_Color SetColor( Uint8 Red, Uint8 Green, Uint8 Blue )
{
	SDL_Color SColor;
	SColor.r = Red;
	SColor.g = Green;
	SColor.b = Blue;

	return SColor;
}

SDL_bool RectIntersect( const SDL_Rect *r1, const SDL_Rect *r2 )
{
	if( r1->x + r1->w <= r2->x )
	{
		return SDL_FALSE;
	}
	if( r1->x >= r2->x + r2->w )
	{
		return SDL_FALSE;
	}
	if( r1->y + r1->h <= r2->y )
	{
		return SDL_FALSE;
	}
	if( r1->y >= r2->y + r2->h )
	{
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

SDL_bool FullRectIntersect( const SDL_Rect *r1, const SDL_Rect *r2 )
{
	if( r1->x < r2->x )
	{
		return SDL_FALSE;
	}
	if( r1->x + r1->w > r2->x + r2->w )
	{
		return SDL_FALSE;
	}
	if( r1->y < r2->y )
	{
		return SDL_FALSE;
	}
	if( r1->y + r1->h > r2->y + r2->h )
	{
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

DLLINTERFACE std::string Get_Curr_Date()
{
	time_t t = time(0);   // get time now
	string str;
    struct tm *now = localtime( & t );
	
	std::stringstream var;
	
	var << (now->tm_year + 1900) << '-'<< (now->tm_mon + 1) << '-' <<  now->tm_mday << std::endl;
	//console_print(var.str().c_str());
	//sprintf(result,"%d-%d-%d", now->tm_year+1900, now->tm_mon+1, now->tm_mday);
	return var.str();
    //cout << ;
}

#include <fstream>
using namespace std;
SDL_bool FileValid( string filename )
{
	std::ifstream ifs;
	ifs.open( filename.c_str(), ios :: out );
	
	if( ifs )
	{
		ifs.close();
		return SDL_TRUE;
	}
	
	return SDL_FALSE;
}

SDL_bool is_valid_number( char *c )
{
	SDL_bool floatnumber = SDL_FALSE;
	if (*c == '-')
		c++;
	for(;*c;c++)
	{
		if (*c == '.')
		{
			if (!floatnumber)
			{
				floatnumber = SDL_TRUE;
				c++;
			}
			else { return SDL_FALSE; }
		}
		
		if ((*c < '0' || *c > '9') && *c != '\r' && *c != '\n')
		{
			return SDL_FALSE;
			
		}
	}
	return SDL_TRUE;
}

