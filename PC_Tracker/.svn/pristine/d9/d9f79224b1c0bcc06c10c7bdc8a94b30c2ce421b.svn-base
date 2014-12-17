#include "Globals.h"

#define PISTOL_VELOCITY	10
#define MACHINEGUN_VELOCITY	15
#define LASER_VELOCITY 15

//unsigned int BulletCount = 0;
//cBullet **Bullets = NULL;
ObjectManager<cBullet> Bullets(OM_OBLITERATE_OBJS_AT_DESTROY, OM_DELETE_OBJS);

cBullet :: cBullet( double nposx, double nposy )
: cMVelSprite( NULL, nposx, nposy )
{
	direction = RIGHT;

	Bullet_type = BULLET_UNKNOWN;	// no type
	type = SPRITE_TYPE_BULLET;
	
	Origin = 0; // Unknown

	visible =SDL_FALSE;

	sound_Explosion = NULL;
}

cBullet :: ~cBullet( void )
{
	// Nothing
}

void cBullet :: init( int ndirection, unsigned int nBullet_type, unsigned int nOrigin, double nextravel /** = 0.0  */ )
{
	if( !nBullet_type ) 
	{
		visible =SDL_FALSE;
		
		return;
	}

	double nVelocity = 0; // The Velocity

	Bullet_type = nBullet_type; // Set the Type
	direction = ndirection; // Set the Direction

	if( Bullet_type == BULLET_PISTOL ) // The Pistol Bullet
	{
		nVelocity = PISTOL_VELOCITY;

		
		SetImage( Renderer, IMan->GetPointer( PIXMAPS_DIR "game/weapons/pistol/default.png" ) );
	}
	else if( Bullet_type == BULLET_MACHINEGUN )  // The Machinegun Bullet
	{
		nVelocity = MACHINEGUN_VELOCITY;
		
		SetImage( Renderer, IMan->GetPointer( PIXMAPS_DIR "game/weapons/machinegun/default.png" ) );
	}
	else if( Bullet_type == BULLET_LASER_X1 )  // The Laser X-1
	{
		nVelocity = LASER_VELOCITY;
		
		if( direction == LEFT ) 
		{
			SetImage( Renderer, IMan->GetPointer( PIXMAPS_DIR "game/weapons/Laser_X1/left.png" ) );
		}
		else if( direction == RIGHT ) 
		{
			SetImage( Renderer, IMan->GetPointer( PIXMAPS_DIR "game/weapons/Laser_X1/right.png" ) );
		}
		else
		{
			DEBUGLOG( "Warning : Unsupported Laser X-1 direction used : %d\n", direction );

			visible =SDL_FALSE;
			return;
		}
	}
	else
	{
		DEBUGLOG( "Warning : Unknown Bullet type used : %d\n", Bullet_type );

		visible =SDL_FALSE;
		return;
	}

	if( direction == RIGHT ) 
	{
		SetVelocity( nVelocity + nextravel, 0 );
	}
	else if( direction == LEFT ) 
	{
		SetVelocity( -( nVelocity + nextravel ), 0 );

		posx -= width;
	}
	else if( direction == UP ) 
	{
		SetVelocity( 0, -( nVelocity + nextravel ) );
	}
	else if( direction == DOWN ) 
	{
		SetVelocity( 0, nVelocity + nextravel );	
	}
	else
	{
		DEBUGLOG( "Warning : Unknown Bullet direction used : %d\n", direction );

		visible =SDL_FALSE;
		return;
	}
	
	Origin = nOrigin;

	posy -= height/2;

	visible =SDL_TRUE;
}

void cBullet :: Update( void )
{
	if( visible == SDL_FALSE )
	{
		return;
	}

	cMVelSprite::Update();

	if( !velx && !vely ) // not Moving
	{
		visible =SDL_FALSE;
		return;
	}


	// If there's an Collision 
	if( Collision->direction != -1 || Collision->collide != -1 )
	{
		if( ( Collision->iCollisionType == SPRITE_TYPE_PLAYER && this->Origin == SPRITE_TYPE_ENEMY ) ||
			( Collision->iCollisionType == SPRITE_TYPE_ENEMY && this->Origin == SPRITE_TYPE_PLAYER ) ||
			( Collision->iCollisionType == SPRITE_TYPE_MASSIVE || Collision->iCollisionType == SPRITE_TYPE_HALFMASSIVE ) ) 
		{
			if( Bullet_type == BULLET_PISTOL )
			{
				DoDamage();
				Explode();
			}
			else if( Bullet_type == BULLET_MACHINEGUN ) 
			{
				DoDamage();
				Explode();
			}
			else if( Bullet_type == BULLET_LASER_X1 ) 
			{
				DoDamage();
				Explode();
			}
		}
	}	

	// Position Correction
	Windows_BoundsChecking();
}

void cBullet::Windows_BoundsChecking()
{
	if( posx - width > (double)window_width + pCamera->x )
	{
		visible =SDL_FALSE;
	}
	else if( posx < -width + pCamera->x )
	{
		visible =SDL_FALSE;
	}
	
	if( posy < -height + pCamera->y )
	{
		visible =SDL_FALSE;
	}
	else if( posy - height > (double)window_height + pCamera->y )
	{
		visible =SDL_FALSE;
	}
}

/// Finds the coordinates to place the explosion, adds the particle emmiter, and makes invisible the bullet
void cBullet::Explode()
{
	int explode_posx, explode_posy;
	/*DEBUGLOG ("\tCollision direction : %d\n", Collision->collide);
	DEBUGLOG ("\tCollisions directons: %d\n", Collision->direction);*/
	
	if (Collision->direction == ALL_COLLISIONS_LR)
	{
		if (velx < 0)
			explode_posx = Collision->cRect->x+Collision->cRect->w+1;
		else
			explode_posx = Collision->cRect->x-1;
		
		explode_posy = posy + (height/2);
	}
	else if (Collision->direction == ALL_COLLISIONS_UD)
	{
		//
		if (vely < 0)
			explode_posy = Collision->cRect->y+Collision->cRect->h+1;
		else
			explode_posy = Collision->cRect->y-1;
		
		explode_posx = posx + (width/2);
	}
	else if (Collision->direction == ALL_COLLISIONS_UDLR)
	{
		//
	}

	
	if (Bullet_type == BULLET_PISTOL)
		AddParticleEmitter( explode_posx, explode_posy, Random( ( velx + vely)/16, ( velx + vely)/8 ), 255, 250, 0, 10, 20, 20 ); // Yellow
	else if (Bullet_type == BULLET_MACHINEGUN)
		AddParticleEmitter( explode_posx, explode_posy, Random( ( velx + vely)/16, ( velx + vely)/8 ), 255, 150, 0, 11, 10, 30 ); // Red-Yellow
	else if (Bullet_type == BULLET_LASER_X1)
		AddParticleEmitter( explode_posx, explode_posy, Random( ( velx + vely)/8, ( velx + vely)/4 ), 0, 250, 0, 5, 10, 5 ); // Green
	
	visible = SDL_FALSE;
}

void cBullet::DoDamage()
{
	
}

void cBullet :: Draw( SDL_Renderer *renderer )
{
	if ( !visible || !renderer || !image )
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

void PreloadBulletimages( void )
{
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/pistol/default2.png", colorkey ),PIXMAPS_DIR "game/weapons/pistol/default.png" );
	// Machinegun
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/machinegun/default.png", colorkey ), PIXMAPS_DIR "game/weapons/machinegun/default.png" );
	// Laser X-1
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/Laser_X1/left.png", colorkey ), PIXMAPS_DIR "game/weapons/Laser_X1/left.png" );
	IMan->Add( LoadImage( PIXMAPS_DIR "game/weapons/Laser_X1/right.png", colorkey ), PIXMAPS_DIR "game/weapons/Laser_X1/right.png" );
}

void AddBullet( double nposx, double nposy, int ndirection, unsigned int nBullet_type, unsigned int nOrigin, double nextravel /** = 0.0  */ )
{
	for( unsigned int i = 0; i < Bullets.objcount; i++ )
	{
		if ( !Bullets.objects[i]->visible )
		{
			Bullets.objects[i]->SetPos( nposx, nposy );
			
			Bullets.objects[i]->init( ndirection, nBullet_type, nOrigin, nextravel );

			return;
		}
	}

	cBullet *new_Bullet = new cBullet( nposx, nposy );

	new_Bullet->init( ndirection, nBullet_type, nOrigin, nextravel );

	Bullets.add(new_Bullet);
}

void UpdateBullets( void )
{
	for( unsigned int i = 0; i < Bullets.objcount; i++ )
	{
		if( Bullets.objects[i]->visible == SDL_TRUE )
		{
			cBullet *bullet = Bullets.objects[i];
			CollideMove( (cBasicSprite*)bullet, bullet->velx * pFramerate->speedfactor, bullet->vely * pFramerate->speedfactor, bullet->Collision, bullet->type );
			bullet->Update();
		}
	}
}

void DrawBullets( SDL_Renderer *renderer )
{
	for( unsigned int i = 0; i < Bullets.objcount; i++ )
	{
		Bullets.objects[i]->Draw( renderer );
	}
}

void DeleteAllBullets( void )
{
	Bullets.~ObjectManager();
}