/*
*	SDL Graphics Extension
*	Misc functions
*
*	Started 990819
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)1999-2003 Anders Lindström
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "sge_misc.h"


Uint32 delay_res=10;

//==================================================================================
// Returns a random integer between min and max
//==================================================================================
int sge_Random(int min, int max)
{
	return min+(rand()%(max-min+1));
}


//==================================================================================
// Seed the random number generator with a number from the system clock.
// Should be called once before the first use of sge_Random.
//==================================================================================
void sge_Randomize(void)
{
	srand(time(NULL));
}


//==================================================================================
// Test the resolution of SDL_Delay()
//==================================================================================
Uint32 sge_CalibrateDelay(void)
{
  SDL_Delay(10);
  delay_res=SDL_GetTicks();
  SDL_Delay(1);
  delay_res=SDL_GetTicks()-delay_res;

  return delay_res;
}


//==================================================================================
// Get the resolution of SDL_Delay()
//==================================================================================
Uint32 sge_DelayRes(void)
{
	return delay_res;
}


//==================================================================================
// Delay 'ticks' ms.
// Tries to burn time in SDL_Delay() if possible
// Returns the exact delay time
//==================================================================================
Uint32 sge_Delay(Uint32 ticks)
{
	Uint32 start = SDL_GetTicks();
	Sint32 time_left = (Sint32)ticks;
	Uint32 tmp;
	
	if(ticks >= delay_res){
		tmp=ticks-(ticks%delay_res);
		SDL_Delay(tmp);
		time_left=(Sint32)(ticks-(SDL_GetTicks()-start));	//Possible error for large ticks... nah	
	}
	
	while(time_left>0){
		time_left=ticks-(SDL_GetTicks()-start);	
	}
	
	return SDL_GetTicks()-start;	
}
