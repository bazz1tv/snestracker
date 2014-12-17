/// @addtogroup Engine_Sprite
// @{
/** 

	@file sprite.h Sprite Class

	V 2.3.2

	LGPL (c) F. Richter

*/


#ifndef __EP_SPRITE_H__
#define __EP_SPRITE_H__

#include "gfx.h"

#include <math.h>

#include "framerate.h"
#include "misc.h"

/// @addtogroup SPRITE_TYPES
// @{
// Sprite Types
#define SPRITE_BASIC 1
#define SPRITE_VELOCITY 2 
#define SPRITE_ANGLE 3
#define SPRITE_ACCELEREATION 4
#define SPRITE_MOUSECURSOR 100
// @}

/**	Sets the Sprite Speedfactor
 *	this affects the movement
 */
DLLINTERFACE void SetSpriteSpeedfactor( double *sf );

class cBasicSprite
{
public:

	DLLINTERFACE cBasicSprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y );
	DLLINTERFACE ~cBasicSprite( void );

	DLLINTERFACE void SetImage( SDL_Renderer *renderer, SDL_Surface *new_image, SDL_bool OverrideSize = SDL_TRUE );
	DLLINTERFACE void SetSize( double nwidth, double nheight );
	
	DLLINTERFACE void AddSize( double naddwidth, double naddheight );

	DLLINTERFACE void SetPos( double x, double y );
	DLLINTERFACE void SetAlpha( Uint8 Alpha );
	DLLINTERFACE void SetColorKey( Uint32 Colorkey );

	DLLINTERFACE Uint32 GetAlpha( void );
	DLLINTERFACE Uint32 GetColorkey( void );

	DLLINTERFACE double GetSizeWidth( void );
	DLLINTERFACE double GetSizeHeight( void );

	DLLINTERFACE void Move( double move_x, double move_y );

	DLLINTERFACE void Draw( SDL_Renderer *renderer );

	DLLINTERFACE void UpdateTexture( SDL_Renderer *renderer );
	DLLINTERFACE void DestroyTexture( void );
	
	//DLLINTERFACE virtual void Update( void );
	DLLINTERFACE virtual void Update( SDL_Renderer *renderer );
	
	// Tagged for SDL2 Additions
	SDL_Texture *texture;

	/**	The final stretched/zoomed image to draw
	 *	if visible == 0 the image won't be drawn
	 */	
	SDL_Surface *image;

	/**	Alpha 0 (completely not visible) - 255 (completely visible)
	 *	Read Only
	 */ 
	Uint8 alpha;
	
	double posx, posy; // Position.
	double width, height; ///< Size for stretching/zooming.

	SDL_bool visible; // if 1 the image will not be drawn.
	SDL_bool spawned; // Got this sprite get spawned ingame.

	unsigned int ID; // todo unique ID.
	
	/**	The first image which was set
	 *	Read only
	 */
	SDL_Surface *startimage;
	/**	The image which is currently set
	*	Read only
	*/
	SDL_Surface *srcimage;

	/**	The rects for all images
	 *	Read only
	 */
	SDL_Rect rect, srcrect, startrect;

	/**	If this is 1 the image needs to be redrawn to maintain the width and height
	 *	Read only
	 */
	SDL_bool drawimg;

	/**	What sprite type is this
	 *	1 = Basic, 2 = Velocity, 3 = Angle, 4 = Acceleration
	 */
	unsigned int Spritetype;
};

class cVelocitySprite : public cBasicSprite
{
public:
	DLLINTERFACE cVelocitySprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y, double nvelx = 0, double nvely = 0 );

	DLLINTERFACE ~cVelocitySprite( void );
	
	DLLINTERFACE virtual void SetVelocity( double nvelx, double nvely );
	DLLINTERFACE virtual void SetVelocityX( double nvelx );
	DLLINTERFACE virtual void SetVelocityY( double nvely );

	DLLINTERFACE void AddVelocity( double addvelx, double addvely, double maxvelx = 0, double maxvely = 0 );
	
	DLLINTERFACE void Update( SDL_Renderer *renderer, SDL_bool nMove = SDL_TRUE );

	double velx, vely; // Velocity.
};

class cAngleSprite : public cBasicSprite
{
public:
	DLLINTERFACE cAngleSprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y, double nangle = 0, double nspeed = 0,
		SDL_bool nanglerotate = SDL_TRUE );

	DLLINTERFACE ~cAngleSprite( void );
	
	DLLINTERFACE void SetDirection( double nangle = -1, double nspeed = -1);

	DLLINTERFACE void AddDirection( double nangle );
	
	DLLINTERFACE void SetSpeed( double nspeed );

	DLLINTERFACE void AddSpeed( double nspeed );

	DLLINTERFACE void Update( SDL_Renderer*, SDL_bool nMove = SDL_TRUE );

	double angle,speed; // Angle and Speed.

	/**	The Direction calculated by SetDirection for 360° image moving
	 *	Read Only
	 */
	double dirx,diry;

	/**	If SDL_TRUE the image will be rotated to the angle
	 *	Read only
	 */
	SDL_bool anglerotate;
};

//-----------------------------------------------------------------

class cAccelerationSprite : public cAngleSprite
{
public:
	DLLINTERFACE cAccelerationSprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y, double nangle = 0 , double nspeed = 0,
		double nacc = 0, double ndeacc = 0 );

	DLLINTERFACE ~cAccelerationSprite( void );

	DLLINTERFACE void SetAcceleration( double nacc );

	DLLINTERFACE void AddAcceleration( double nacc );

	DLLINTERFACE void SetDeAcceleration( double ndeacc );

	DLLINTERFACE void AddDeAcceleration( double ndeacc );
	
	DLLINTERFACE void Update( SDL_Renderer*, SDL_bool nMove = SDL_TRUE );

	double acc; // Acceleration.
	double deacc; // De Acceleration.

private:

};





//--------------------------------------------------------------------/

class cMouseCursor : public cBasicSprite
{
public:
	DLLINTERFACE cMouseCursor( SDL_Renderer *renderer, double x, double y, SDL_Surface *new_image );
	DLLINTERFACE virtual ~cMouseCursor( void );

	/**	Point Collisioncheck
	 */
	DLLINTERFACE SDL_bool CollisonCheck ( double x, double y );
	/**	Checks if the MouseCursor is in the rect
	 */
	DLLINTERFACE SDL_bool CollisonCheck ( SDL_Rect *Crect );

	DLLINTERFACE void Update( SDL_Renderer *renderer);
	DLLINTERFACE void hide();
	DLLINTERFACE void show();
};

#endif

// @}


/* Velocity Acceleration Sprite .......
 *	
 		if(velx < 0 && accx != 0)
		{
			velx -= accx * pFramerate->speedfactor;

			if(velx > 0)
			{
				velx = 0;
				accx = 0;
			}
		}
		else if(velx > 0 && accx != 0)
		{
			velx += accx * pFramerate->speedfactor;

			if(velx < 0)
			{
				velx = 0;
				accx = 0;
			}
		}

		if(vely < 0 && accy != 0)
		{
			vely -= accy * pFramerate->speedfactor;

			if(vely > 0)
			{
				vely = 0;
				accy = 0;
			}
		}
		else if(vely > 0 && accy != 0)
		{
			vely += accy * pFramerate->speedfactor;

			if(vely < 0)
			{
				vely = 0;
				accy = 0;
			}
		}


		if (accx < 0 && deaccx != 0)
		{
			accx -= deaccx * pFramerate->speedfactor;

			if(accx > 0)
			{
				accx = 0;
			}
		}
		else if(accx > 0 && deaccx != 0)
		{
			accx += deaccx * pFramerate->speedfactor;

			if(accx > 0)
			{
				accx = 0;
			}
		}

		if (accy < 0 && deaccy != 0)
		{
			accy -= deaccy * pFramerate->speedfactor;

			if(accy > 0)
			{
				accy = 0;
			}
		}
		else if(accy > 0 && deaccy != 0)
		{
			accy += deaccy * pFramerate->speedfactor;

			if(accy > 0)
			{
				accy = 0;
			}
		}

		if (velx < 0 && deaccx != 0)
		{
			velx += deaccx * pFramerate->speedfactor;

			if(velx > 0)
			{
				velx = 0;
				deaccx = 0;
			}
		}
		else if(velx > 0 && deaccx != 0)
		{
			accx -= deaccx * pFramerate->speedfactor;

			if(velx < 0)
			{
				velx = 0;
				deaccx = 0;
			}
		}

		if (vely < 0 && deaccy != 0)
		{
			vely += deaccy * pFramerate->speedfactor;

			if(vely > 0)
			{
				vely = 0;
				deaccy = 0;
			}
		}
		else if(vely > 0 && deaccy != 0)
		{
			accy -= deaccy * pFramerate->speedfactor;

			if(vely < 0)
			{
				vely = 0;
				deaccy = 0;
			}
		}
 */