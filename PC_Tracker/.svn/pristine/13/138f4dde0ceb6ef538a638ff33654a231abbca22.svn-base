
#include "sprite.h"
#include "Collision.h"

cMVelSprite :: cMVelSprite( SDL_Surface *new_image, double x, double y, double nvelx /* = 0 */, double nvely /* = 0  */ )
: cVelocitySprite( Renderer, new_image, x, y, nvelx /* = 0 */, nvely /* = 0  */ )
{
	Startposx = posx;
	Startposy = posy;

	type = SPRITE_TYPE_NO_TYPE;

	Collision = new Collisiondata();
}

cMVelSprite :: ~cMVelSprite( void )
{
	if( Collision ) 
	{
		delete Collision;
		Collision = NULL;
	}
}

void cMVelSprite :: Update( void )
{
	cVelocitySprite::Update(Renderer, SDL_FALSE );
}

void cMVelSprite :: Draw( SDL_Renderer *renderer )
{
	if ( !visible || !image || width <= 0 || height <= 0 )
	{
		return;
	}

	rect.x = (int)(posx - pCamera->x);
	rect.y = (int)(posy - pCamera->y);

	//SDL_BlitSurface( image, NULL, target, &rect );
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	
	rect.x = (int)posx;
	rect.y = (int)posy;
}

/// Use SDL_TRUE to get the RECT with camera position coordinates calculated also.
SDL_Rect cMVelSprite :: GetRect( SDL_bool with_camerapos /* = 0  */)
{
	if( !with_camerapos ) 
	{
		return rect;
	}

	return SetRect( rect.x - (int)pCamera->x, rect.y - (int)pCamera->y, rect.w, rect.h );
}



/*################################ MAngle Sprite #########################################*/



cMAngleSprite :: cMAngleSprite( SDL_Surface *new_image, double x, double y, double nangle /* = 0  */, double nspeed /* = 0  */ )
: cAngleSprite( Renderer, new_image, x, y, nangle /* = 0 */, nspeed /* = 0  */ )
{
	Startposx = posx;
	Startposy = posy;

	type = SPRITE_TYPE_NO_TYPE;

	Collision = new Collisiondata;
}



cMAngleSprite :: ~cMAngleSprite( void )
{
	if( Collision ) 
	{
		delete Collision;
		Collision = NULL;
	}
}

void cMAngleSprite :: Update( void )
{
	cAngleSprite::Update(Renderer, SDL_FALSE );
}

void cMAngleSprite :: Draw( SDL_Renderer *renderer )
{
	if ( !visible || !image || !renderer || width <= 0 || height <= 0 )
	{
		return;
	}

	rect.x = (int)(posx - pCamera->x);
	rect.y = (int)(posy - pCamera->y);

	SDL_RenderCopy(renderer, texture, NULL, &rect);
	//SDL_BlitSurface( image, NULL, target, &rect );

	rect.x = (int)posx;
	rect.y = (int)posy;
}

SDL_Rect cMAngleSprite :: GetRect( SDL_bool with_camerapos /* = 0  */)
{
	if( !with_camerapos ) 
	{
		return rect;
	}

	return SetRect( rect.x - (int)pCamera->x, rect.y - (int)pCamera->y, rect.w, rect.h );
}