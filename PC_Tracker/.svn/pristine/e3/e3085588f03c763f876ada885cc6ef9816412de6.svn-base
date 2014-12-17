/// @addtogroup Engine_Framerate
// @{
/** 
	@file framerate.h  framerate independent Framerate class ;)
	
	The Speedfactor is the heart of this class. When it is set by SetSpeedFactor, 
	it becomes a number with that you multiply all your motions. For instance, if the targetfps is 100,
	and the actual fps is 85, the speedfactor will be set to 100/85, or about 1.2.
	You then multiply all your motion is the game, at its lowest level, by this number.
	
	
	(C) 2012 Bazz
	
	under LGPL License

	V 2.0
*/

#ifndef __EP_FRAMERATE_H__
#define __EP_FRAMERATE_H__

#include <iostream>

#if defined(__LINUX__) || defined(__MACOSX__)

#include <sys/time.h>
#define N_MICROSEC_INT    (1000000)
#define N_MICROSEC_FLOAT   (1000000.0)
#define tv2micro(x) (x.tv_sec * N_MICROSEC_INT + x.tv_usec);

#endif

///framerate independent Framerate class ;)

/** The Speedfactor is the heart of this class. When it is set by SetSpeedFactor, 
it becomes a number with that you multiply all your motions. For instance, 
if the targetfps is 100, and the actual fps is 85, the speedfactor will be set to 100/85, 
or about 1.2. You then multiply all your motion is the game, at its lowest level, by this number. */
class cFramerate
{
public:
	cFramerate( double tfps );
	
	~cFramerate ( void );
	
	void Init( double tfps );

	void SetSpeedFactor( void );

	void Reset( void );
    
	void SetMaxSpeedFactor( double maxsf );
    std::string Getfps( void );

	double         targetfps;
	double         fps;
    long long int  start_time,time_diff,stop_time;

	Uint32 currentticks;
	Uint32 framedelay;
    
    timeval before,after;

	double speedfactor;
	double maxspeedfactor;
};

/** 
	Fixed framerate method
	
	V.1.0
*/
inline void CorrectFrameTime( unsigned int fps = 60 ); ///maybe change to 60

#endif
// @}