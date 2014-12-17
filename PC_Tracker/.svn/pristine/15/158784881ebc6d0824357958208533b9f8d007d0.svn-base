

#include "SDL_ep.h"

#if defined(__LINUX__) || defined(__MACOSX__)

#include <sys/time.h>
#define N_MICROSEC_INT    (1000000)
#define N_MICROSEC_FLOAT   (1000000.0)
#define tv2micro(x) (x.tv_sec * N_MICROSEC_INT + x.tv_usec);

#endif

cFramerate::cFramerate( double tfps /* = 60  */ )
{
	speedfactor = 1;
	fps = 0;
	Init ( tfps );
}

cFramerate :: ~cFramerate ( void )
{
	// nothing
}


void cFramerate :: Init( double tfps )
{
	targetfps = tfps;
	maxspeedfactor = tfps/5;
#ifdef _WIN32	
	QueryPerformanceCounter( &framedelay );
	QueryPerformanceFrequency( &tickspersecond );
#elif defined(__LINUX__) || defined(__MACOSX__)	
	struct timeval tv;
	gettimeofday( &tv, NULL );
	start_time = tv2micro( tv );
	time_diff = start_time;
#endif	
}




void cFramerate :: SetSpeedFactor( void )
{
#ifdef _WIN32
	//	This frame's length out of desired length
	QueryPerformanceCounter( &currentticks );
	speedfactor = (double)(currentticks.QuadPart-framedelay.QuadPart)/((double)tickspersecond.QuadPart/targetfps);
#elif defined(__LINUX__) || defined(__MACOSX__)
	struct timeval tv;
	gettimeofday( &tv, NULL );
	stop_time = tv2micro(tv);
	time_diff = stop_time - time_diff;
	speedfactor = time_diff/(N_MICROSEC_FLOAT/targetfps);
    time_diff = stop_time;
#endif
	

	fps = targetfps/speedfactor;

	if( speedfactor <= 0 )
	{
		speedfactor = 1;
	}
	else if( speedfactor > maxspeedfactor ) 
	{
		speedfactor = maxspeedfactor;
	}

	framedelay = currentticks;
    
    SetSpriteSpeedfactor( &speedfactor );
}


void cFramerate :: Reset( void )
{
#ifdef _WIN32
	QueryPerformanceCounter( &framedelay );
#elif defined(__LINUX__) || defined(__MACOSX__)
	struct timeval tv;
	gettimeofday( &tv, NULL );
	start_time = tv2micro( tv );
	time_diff = start_time;
#endif
	speedfactor = 0;
}

void cFramerate :: SetMaxSpeedFactor( double maxsf )
{
	maxspeedfactor = maxsf;
}

