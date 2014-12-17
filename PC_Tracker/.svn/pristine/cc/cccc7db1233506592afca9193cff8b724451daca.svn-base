/// @addtogroup Engine_Misc
// @{
/** 

	@file misc.h  Misc things

	V 1.2

	 Bazz

*/


#ifndef __EP_MISC_H__
#define __EP_MISC_H__

#include <string.h>


/// Random float between min and max
DLLINTERFACE float Random( float min, float max );

/// Random integer between min and max
DLLINTERFACE int Random( int min, int max );

/// Random integer between 0 and max
DLLINTERFACE int Random( int max );

/// Random float between 0 and max
DLLINTERFACE float Random( float max );

/// Random double between 0 and max
DLLINTERFACE double Random( double max );
/// Random double between min and max
DLLINTERFACE double Random( double min, double max );

/// Returns an SDL_rect
DLLINTERFACE SDL_Rect SetRect( int x, int y, unsigned int w, unsigned int h );

/// return an SDL_Color
DLLINTERFACE SDL_Color SetColor( Uint8 Red, Uint8 Green, Uint8 Blue );

 /// returns 1 if 1 or more points intersect
DLLINTERFACE SDL_bool RectIntersect( const SDL_Rect *r1, const SDL_Rect *r2 );

/// returns 1 if the complete rect1 intersects with rect2
DLLINTERFACE SDL_bool FullRectIntersect(const SDL_Rect *r1, const SDL_Rect *r2 );

DLLINTERFACE std::string Get_Curr_Date();

#endif

// @}