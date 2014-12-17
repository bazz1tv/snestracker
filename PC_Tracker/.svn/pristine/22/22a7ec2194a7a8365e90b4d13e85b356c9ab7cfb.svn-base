

#include "Globals.h"

cAF373 :: cAF373( double nposx, double nposy )
: cEnemy( nposx, nposy )
{
	for( int i = 0;i < 2; i++ )
	{
		images[i] = NULL;
	}

	pWeapon = NULL;

	onGround = 0;

	state = STATE_FALLING;

	AI_direction = DOWN;

	AI_movingtime = -1;
}

cAF373 :: ~cAF373( void )
{
	if( pWeapon )
	{
		delete pWeapon;
	}
}

void cAF373 :: init( void )
{
	direction = RIGHT;

	Enemy_type = ENEMY_AF373;

	SetVelocity( 0, 0 );

	// Right Standing
	IMan->Add( LoadImage( PIXMAPS_DIR "game/enemies/Bots/AF-373/right_1.png", colorkey ), "AF373_right" );
	// Left Satnding
	IMan->Add( LoadImage( PIXMAPS_DIR "game/enemies/Bots/AF-373/left_1.png", colorkey ), "AF373_left" );

	// Get the Pointer
	images[LEFT] = IMan->GetPointer( "AF373_left" );
	images[RIGHT] = IMan->GetPointer( "AF373_right" );

	SetImage( Renderer,images[direction] );

	if( pWeapon )
	{
		delete pWeapon;
	}
	
	pWeapon = new cWeapon_Laser_X1( SPRITE_TYPE_ENEMY, 1, -1 );
}

void cAF373 :: Update( void )
{
	SetImage( Renderer,images[direction] );
	
	cEnemy :: Update();

	pWeapon->Update();
	
	PositionCheck( (int)posx, (int)posy + 1, (int)width, (int)height, Collision, type );

	if( Collision->collide == DOWN ) 
	{
		if( Collision->iCollisionType == 1 ) 
		{
			onGround = 1; // an Massive ground
			state = STATE_STAYING;
			SetVelocity( velx, 0 );
		}
		else if( Collision->iCollisionType == 2 ) 
		{
			onGround = 2; // an Halfmassive ground
			state = STATE_STAYING;
			SetVelocity( velx, 0 );
		}
		else
		{
			onGround = 0; // no ground
		}
	}
	else
	{
			onGround = 0; // no ground
	}
	
	// Movement
	if( AI_direction == LEFT )
	{
		if( onGround ) 
		{
			state = STATE_WALKING;
		}
		else if( vely > 0)
		{
			state = STATE_FALLING;
		}

		AddVelocity( -0.2, 0, -5 ,0 );

		direction = LEFT;
	}
	else if( AI_direction == RIGHT ) 
	{
		if( onGround ) 
		{
			state = STATE_WALKING;
		}
		else if( vely > 0)
		{
			state = STATE_FALLING;
		}
		
		AddVelocity( 0.2, 0, 5 ,0 );

		direction = RIGHT;
	}
	else // slow down
	{
		if( velx > 0 )
		{
			AddVelocity( -0.05, 0 );

			if ( velx < 0 )
			{
				SetVelocity( 0, vely );
			}
		}
		else if( velx < 0 )
		{
			AddVelocity( 0.05, 0 );

			if ( velx > 0 )
			{
				SetVelocity( 0, vely );
			}
		}
	}

	if( onGround == SDL_FALSE )
	{
		AddVelocity( 0, ( 0.05 ) * pFramerate->speedfactor, 0, 6 );
	}

	int srange = AI_PlayerSearch( RIGHT );

	if( srange > 0 ) 
	{
		if( direction == LEFT ) 
		{
			if( srange < 100 )
			{
				AI_direction = RIGHT;
			}
			else if( srange > 200 )
			{
				AI_direction = LEFT;
			}
		}
		else if( direction == RIGHT ) 
		{
			if( srange < 50 )
			{
				AI_direction = LEFT;
			}
			else if( srange > 200 )
			{
				AI_direction = RIGHT;
			}
		}
		
		AI_movingtime = 1;

		Fire( RIGHT );
		direction = RIGHT;
	}

	if( srange == 0 ) 
	{
		srange = AI_PlayerSearch( LEFT );
	}


	if( srange < 0 ) 
	{
		if( direction == LEFT ) 
		{
			if( srange > -100 )
			{
				AI_direction = RIGHT;
			}
			else if( srange < -200 )
			{
				AI_direction = LEFT;
			}
		}
		else if( direction == RIGHT ) 
		{
			if( srange > -50 )
			{
				AI_direction = LEFT;
			}
			else if( srange < -200 )
			{
				AI_direction = RIGHT;
			}
		}

		AI_movingtime = 1;

		Fire( LEFT );
		direction = LEFT;
	}

	// Position Correction
	if( posx < 0 ) 
	{
		SetPos( 0, posy );
	}
	else if( (double)posx + width > (double)window_width + pCamera->x ) 
	{
		posx = (double)window_width - width;
	}

	if( posy < 0 ) // Up
	{
		SetPos( posx, 0 );
		SetVelocity( velx, 0 );
		state = STATE_FALLING;
	}
	else if( (double)posy + height > (double)window_height + pCamera->y ) // Bottom
	{
		SetPos( posx, (double)window_height - height );
		
		state = STATE_STAYING;
	}
	

	AI_movingtime -= pFramerate->speedfactor;

	if( AI_movingtime <= 0 ) 
	{
		AI_direction = -1;
		AI_movingtime = 0;	
	}

	if( AI_direction == -1 ) 
	{
		AI_Idle();
	}
}

void cAF373 :: Draw( SDL_Renderer *renderer )
{
	if( visible == SDL_FALSE || !renderer || !image )
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

void cAF373 :: Die( void )
{
	DEBUGLOG( "Died\n" );
}

void cAF373 :: Fire( int fdirection )
{
	if( !pWeapon->can_Fire() ) 
	{
		return;
	}
	
	double Bullet_posx = 0;
	double Particle_posx = posx;
	double Particle_direction = 0;

	if( fdirection == RIGHT )
	{
		Particle_direction = 0;
	}
	else if( fdirection == LEFT )
	{
		Particle_direction = 180;
	}

	if( fdirection == RIGHT )
	{
		Particle_posx += width;
		Bullet_posx += width;
	}

	pWeapon->SetPos( posx + Bullet_posx, posy + height/2 );
	
	pWeapon->direction = fdirection;
	
	pWeapon->Fire();

	AddParticleEmitter( Particle_posx, posy + height/2, Random( 1.0, 2.5 ), 250, 90, 50, 3, 3, 3, Particle_direction ); // red
}

int cAF373 :: AI_PlayerSearch( int sdirection )
{
	int range = 0;

	Collision->collide = -1;

	while( Collision->collide == -1 )
	{
		if( sdirection == RIGHT ) 
		{
			range += (int)width;
		}
		else if( sdirection == LEFT ) 
		{
			range -= (int)width;
		}
		else
		{
			DEBUGLOG( "Warning : AF373 AI_PlayerSearch Wrong direction used : %d\n", sdirection );
			return 0;
		}


		if( range > 400 || range < -400 ) 
		{
			return 0;
		}
		
		PositionCheck( (int)posx + range, (int)posy, (int)width, (int)height, Collision, type );

		if( Collision->iCollisionType == SPRITE_TYPE_PLAYER ) 
		{
			return range;
		}
	}

	return 0;
}

void cAF373 :: AI_Idle( void )
{
	if( Random( 0.0, 100 * pFramerate->speedfactor ) < 1 * pFramerate->speedfactor ) 
	{
		if( Random( 0, 1 ) == 1 ) 
		{
			AI_direction = LEFT;
		}
		else
		{
			AI_direction = RIGHT;
		}
		
		AI_movingtime = Random( 1.0, 5.0 );
	}
}
