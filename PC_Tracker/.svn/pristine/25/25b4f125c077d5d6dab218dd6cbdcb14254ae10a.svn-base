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
    double          last_time,time_diff,time_stop;

	Uint32 currentticks;
	Uint32 framedelay;

	double speedfactor;
	double maxspeedfactor;
};

/** 
	Fixed framerate method
	
	V.1.0
*/
inline void CorrectFrameTime( unsigned int fps = 32 ); ///maybe change to 60

#endif
// @}