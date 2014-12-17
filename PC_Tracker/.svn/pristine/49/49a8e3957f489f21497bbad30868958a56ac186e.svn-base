

#include "SDL_ep.h"


// http://nebuladevice.sourceforge.net/doc/doxydoc/nebulacore/html/ntime__main_8cc-source.html
// http://nebuladevice.sourceforge.net/doc2/doxydoc/nebula2/html/ntime__main_8cc-source.html
#if defined(__LINUX__) || defined(__MACOSX__)

#include <sys/time.h>
#define N_MICROSEC_INT    (1000000)
#define tv2micro(x) ((double)x.tv_sec * (double)N_MICROSEC_INT + (double)x.tv_usec);

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
	last_time = tv2micro( tv );
	time_diff = 0.0;
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
	time_diff = time_stop;
	time_stop = tv2micro(tv);
	time_diff = time_stop - time_diff;
	speedfactor = time_diff;
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
}

std::string cFramerate :: Getfps( void )
{
    std::string s = "FPS : " + (unsigned int)fps;
	return s;
}


void cFramerate :: Reset( void )
{
#ifdef _WIN32
	QueryPerformanceCounter( &framedelay );
#elif defined(__LINUX__) || defined(__MACOSX__)
	struct timeval tv;
	gettimeofday( &tv, NULL );
	time_stop = tv2micro( tv );
	time_diff = 0.0;
#endif
	speedfactor = 0;
}

void cFramerate :: SetMaxSpeedFactor( double maxsf )
{
	maxspeedfactor = maxsf;
}


void CorrectFrameTime( unsigned int fps /* = 60  */)
{
	static Uint32 stime = 0;
	
	while( SDL_GetTicks () - stime < 1000 / fps )
	{
		SDL_Delay(1);
	}

	stime = SDL_GetTicks ();
}