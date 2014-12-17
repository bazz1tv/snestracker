
#include "Camera.h"
#include "player.h"

cCamera :: cCamera( void )
{
	x = 0.0;
	y = 0.0;

	// default limits
	Lleft = 0;
	Lright = 100000000;
}

cCamera :: ~cCamera( void )
{

}

void cCamera :: Update( void )
{
	if( mode == MODE_GAME ) 
	{
		
		// The actual algorithm here is a mystery to me. It was made by FLuxy 
		if( (pPlayer->posx - window_width) > (x - 300) ) // Right
		{
			x += ((pPlayer->posx - x + 300 - window_width)/10)*pFramerate->speedfactor; // nice and smooth ;)
		}
		else if( pPlayer->posx < (x + 300) ) // Left
		{
			x += ((pPlayer->posx - x - 300)/10)*pFramerate->speedfactor;	// nice and smooth ;)
		}

		// Added Vertical Scrolling. Jan 6 2012
		// Note: Play with (y-200) and (y+200) to adjust when scrolling activates
		if( (pPlayer->posy - window_height) > (y - 200) ) // DOWN
		{
			y += ((pPlayer->posy - y + 200 - window_height)/10)*pFramerate->speedfactor; // nice and smooth ;)
		}
		else if( pPlayer->posy < (y + 140) ) // UP
		{
			y += ((pPlayer->posy - y - 140)/10)*pFramerate->speedfactor;	// nice and smooth ;)
		}

		Limits_Check();
	}
}

void cCamera :: SetPos( double nx, double ny )
{
	x = nx;
	y = ny;

	if( mode == MODE_GAME ) 
	{
		Limits_Check();
	}
}

void cCamera :: Move( double ax, double ay )
{
	x += ax;
	y += ay;

	if( mode == MODE_GAME ) 
	{
		Limits_Check();
	}
}


void cCamera ::Limits_Check( void )
{
	if( x > (double)Lright ) 
	{
		x = (double)Lright;
	}
	else if( x < (double)Lleft ) 
	{
		x = (double)Lleft;
	}

	if( y > 0 ) // Will not let the camera Scroll Down
	{
		//y = 0;
	}
}