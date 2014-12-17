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

///framerate independent Framerate class ;)

/** The Speedfactor is the heart of this class. When it is set by SetSpeedFactor, 
it becomes a number with that you multiply all your motions. For instance, 
if the targetfps is 100, and the actual fps is 85, the speedfactor will be set to 100/85, 
or about 1.2. You then multiply all your motion is the game, at its lowest level, by this number. */
class cFramerate
{
public:
	cFramerate( double tfps )
	{
		speedfactor = 1;
		fps = 0;
		Init ( tfps );
	}
	
	~cFramerate ( void )
	{
		
	}
	
	void Init( double tfps )
	{
		targetfps = tfps;
		maxspeedfactor = tfps/5;
		framedelay = SDL_GetTicks();
	}

	void SetSpeedFactor( void )
	{
		currentticks = SDL_GetTicks();
		speedfactor = (double)( currentticks - framedelay )/( (double)1000/targetfps );

		fps = targetfps/speedfactor;
		
		if (speedfactor <= 0)
		{
			speedfactor = 1;
		}
		else if(speedfactor > maxspeedfactor) 
		{
			speedfactor = maxspeedfactor;
		}
		
		framedelay = currentticks;
	}

	void Reset( void )
	{
		framedelay = SDL_GetTicks();
		speedfactor = 0;
	}
	
	void SetMaxSpeedFactor( double maxsf )
	{
		maxspeedfactor = maxsf;
	}

	double         targetfps;
	double         fps;

	Uint32 currentticks;
	Uint32 framedelay;

	double speedfactor;
	double maxspeedfactor;
};

/** 
	Fixed framerate method
	
	V.1.0
*/
inline void CorrectFrameTime( unsigned int fps = 32 )
{
	static Uint32 stime = 0;
	
	while( SDL_GetTicks () - stime < 1000 / fps )
	{
		SDL_Delay(1);
	}

	stime = SDL_GetTicks ();
}

#endif
// @}