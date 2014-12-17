
#include "Globals.h"

#include "defines.h"

#define WALK_VELOCITY 0.9


cPlayer :: cPlayer( double nposx, double nposy ) : cMVelSprite( NULL, nposx, nposy )
{
	direction = RIGHT;	// RIGHT
	onGround = 0;	// falling
	
	state = STATE_STAYING;

	type = SPRITE_TYPE_PLAYER;
	
	jump_timer = 0;
	jump_timer_power = 0;

	actWeapon = WEAPON_PISTOL; // default is Pistol

	pWeapon = NULL;

	Health = 100; // 100 HP

	updown_pressed = NONE;
	jumpvel = 0;
}

cPlayer :: ~cPlayer( void )
{
	if( pWeapon ) 
	{
		delete pWeapon;
		pWeapon = NULL;
	}

	// SubVersion Rulez !!
}

void cPlayer :: init( void )
{
	// Right Standing
	IMan->Add( LoadImage( PIXMAPS_DIR "game/meatball/right_3.png", colorkey ), "Meatball_right_1" );
	// Left Standing
	IMan->Add( LoadImage( PIXMAPS_DIR "game/meatball/left_3.png", colorkey ), "Meatball_left_1" );

	// Get the Pointer
	images[LEFT] = IMan->GetPointer( "Meatball_left_1" );
	images[RIGHT] = IMan->GetPointer( "Meatball_right_1" );
	images[UP] = NULL;		// Todo
	images[DOWN] = NULL;	// Todo

	if( pWeapon ) 
	{
		delete pWeapon;
	}

	// Pistol
	pWeapon = new cWeapon_Pistol( SPRITE_TYPE_PLAYER, 5, -1 );
}

void cPlayer :: Update( void )
{
	pWeapon->Update();
	
	SetImage( Renderer, images[direction] );
	
	cBasicSprite::Update(Renderer);

	PositionCheck( (int)posx+12, (int)posy + 1, 10, (int)height, Collision, type );

	if( Collision->collide == DOWN && Collision->direction == ALL_COLLISIONS_UD) 
	{
		if( Collision->iCollisionType == SPRITE_TYPE_MASSIVE || Collision->iCollisionType == SPRITE_TYPE_ENEMY ) 
		{
			onGround = 1; // an Massive ground
			state = STATE_STAYING;
			SetVelocity( velx, 0 );
			jumpvel = 0;
		}
		else if( Collision->iCollisionType == SPRITE_TYPE_HALFMASSIVE ) 
		{
			onGround = 2; // an Halfmassive ground
			state = STATE_STAYING;
			jumpvel = 0;
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

	if( direction == LEFT && MOVELEFT_KEYS_HELD && mode != MODE_CONSOLE )
	{
		if( onGround ) 
		{
			state = STATE_WALKING;
		}
		else if( vely > 0)
		{
			state = STATE_FALLING;
		}


		if( state == STATE_WALKING || state == STATE_STAYING ) 
		{
			AddVelocity( -WALK_VELOCITY*pFramerate->speedfactor + (velx*0.03)*pFramerate->speedfactor, 0, -5 ,0 );
		}
		else
		{
			AddVelocity( (-WALK_VELOCITY+0.6)*pFramerate->speedfactor + (velx*0.01)*pFramerate->speedfactor, 0, -5 ,0 );
		}

		if( vely > 0 ) 
		{
			AddVelocity( (-WALK_VELOCITY+0.4)*pFramerate->speedfactor /*+ (velx*pFramerate->speedfactor)*/, 0 );				
		}
		if( vely < 0 ) 
		{
			
			AddVelocity( (-WALK_VELOCITY+0.4)*pFramerate->speedfactor, 0 );	
		}
	}
	else if( direction == RIGHT && MOVERIGHT_KEYS_HELD && mode != MODE_CONSOLE ) 
	{
		if( onGround ) 
		{
			state = STATE_WALKING;
		}
		else if( vely > 0)
		{
			state = STATE_FALLING;
		}
		else if ( vely < 0 )
		{
			state = STATE_JUMPING;
		}
		
		if( state == STATE_WALKING || state == STATE_STAYING ) 
		{
			AddVelocity( WALK_VELOCITY*pFramerate->speedfactor + (velx*0.03)*pFramerate->speedfactor, 0, 5 ,0 );
		}
		else
		{
			AddVelocity( (WALK_VELOCITY-0.6)*pFramerate->speedfactor + (velx*0.01)*pFramerate->speedfactor, 0, 5 ,0 );
		}



		if( vely < 0 ) 
		{
			
			AddVelocity( (WALK_VELOCITY-0.4)*pFramerate->speedfactor, 0 );	
		}
		if( vely > 0 ) 
		{
			AddVelocity( (WALK_VELOCITY-0.4)*pFramerate->speedfactor /*+ (velx*pFramerate->speedfactor)*/, 0 );				
		}
	}
	else // slow down
	{
		if( velx > 0 )
		{
			AddVelocity( (-WALK_VELOCITY+0.3)*pFramerate->speedfactor, 0 );

			if ( velx < 0 )
			{
				SetVelocity( 0, vely );
			}
		}
		else if( velx < 0 )
		{
			AddVelocity( (WALK_VELOCITY-0.3)*pFramerate->speedfactor, 0 );

			if ( velx > 0 )
			{
				SetVelocity( 0, vely );
			}
		}
	}

	
	//PositionCheck( (int)( posx + ( velx /** pFramerate->speedfactor*/ ) ), (int)( posy + ( vely /** pFramerate->speedfactor*/ ) ) , (int)width, (int)height, Collision, type );

	if( ( Collision->collide == UP || Collision->direction == 3 || Collision->direction == 1 ) && vely < 0 )
	{
		if( Collision->collide == DOWN ) 
		{
			if ( velx != 0 )
			{
				state = STATE_WALKING;
			}
			else
			{
				state = STATE_STAYING;
			}
		}
		else
		{
			state = STATE_FALLING;
		}
		
		SetVelocity(velx, -(vely*0.3) );
	}
	else if( Collision->direction == 2 || Collision->direction == 1 || Collision->collide == LEFT || Collision->collide == RIGHT )
	{
		SetVelocity( 0, vely );
	}


	if (state == STATE_JUMPING && !JUMP_KEYS_HELD && vely <= 0)
	{
		//jumpvel = 1;
		vely += 0.35;
	}
	// Gravity
	AddVelocity( 0, ( 0.30 + (vely*0.03) ) * pFramerate->speedfactor, 0, MAX_FALL_VELOCITY );

	if( jump_timer ) 
	{
		Jump( jump_timer_power );
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
		// just commented this out after putting some tiles at negative locations...
		// seems to be working fine 1/6/12
		/*SetPos( posx, 0 );
		SetVelocity( velx, 0 );
		state = STATE_FALLING;*/
	}
	else if( (double)posy + height > (double)window_height + pCamera->y ) // Bottom
	{
		SetPos( posx, (double)window_height - height );
		
		state = STATE_STAYING;
		jumpvel = 0;
	}
}

void cPlayer :: Draw( SDL_Renderer *renderer ) // Never use the internal Draw function
{
	if ( !visible || !renderer || !image )
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

void cPlayer :: Reset( void )
{
	// Starting Position
	posx = Startposx;
	posy = Startposy;

	// No Velocity
	velx = 0;
	vely = 0;

	direction = RIGHT;
	onGround = 0;

	ChangeActiveWeapon( WEAPON_PISTOL );
}
//



void cPlayer :: Jump( int power /* = 5  */)
{
	if( jump_timer > 0  ) 
	{
		if( !JUMP_KEYS_HELD ) 
		{
			jump_timer = 0;
			jump_timer_power = 0;
		}
		else if( state == STATE_STAYING || state == STATE_WALKING )
		{
			state = STATE_JUMPING;
			vely = (double)-power;
			jump_timer = 0;
			jump_timer_power = 0;
		}
		else
		{
			jump_timer -= pFramerate->speedfactor;

			if( jump_timer <  0) 
			{
				jump_timer = 0;
				jump_timer_power = 0;
			}
		}
	}
	else if( state == STATE_STAYING || state == STATE_WALKING )
	{
		state = STATE_JUMPING;
		vely = (double)-power;
	}
	else if( state == STATE_FALLING && !jump_timer )
	{
		jump_timer = 15 * pFramerate->speedfactor;
		jump_timer_power = power;
	}

}

void cPlayer :: ChangeActiveWeapon( unsigned int Weapon_type )
{
	if( actWeapon == Weapon_type ) 
	{
		return;
	}

	if( Weapon_type == WEAPON_PISTOL ) 
	{
		actWeapon = Weapon_type;

		delete pWeapon;
		pWeapon = new cWeapon_Pistol( SPRITE_TYPE_PLAYER, 3, -1 );
	}
	else if( Weapon_type == WEAPON_MACHINEGUN ) 
	{
		actWeapon = Weapon_type;

		delete pWeapon;
		pWeapon = new cWeapon_Machinegun( SPRITE_TYPE_PLAYER, 1, -1 );
	}
	else if( Weapon_type == WEAPON_LASER_X1 ) 
	{
		actWeapon = Weapon_type;

		delete pWeapon;
		pWeapon = new cWeapon_Laser_X1( SPRITE_TYPE_PLAYER, 10, -1 );
	}
}

void cPlayer :: Fire( void )
{
	if( !pWeapon->can_Fire() ) 
	{
		return;
	}

	double extravel = 0;
	double Particle_posx = posx;
	double Bullet_posx = posx;

	if( direction == LEFT ) 
	{
		extravel = -(velx/3);
	}
	else if( direction == RIGHT )
	{
		extravel = velx/3;
	}

	if( direction == RIGHT )
	{
		Bullet_posx += 1.0 + width;
		Particle_posx += - 2.0 + width;
	}
	else if( direction == LEFT )
	{
		Bullet_posx -= 1.0;
		Particle_posx += 2.0;
	}

	pWeapon->SetPos( Bullet_posx + velx, posy + height/2 + vely );
	
	pWeapon->direction = direction;

	pWeapon->Fire();

	// Todo : Ammo

	// The used ammo ;)
	AddParticleEmitter( Particle_posx, posy + 8, 2, 150, 150, 150, 0.1, 1, 2, 270 ); // grey
}

void cPlayer :: Get_Hit( unsigned int damage )
{
	if( Health - damage <= 0 ) 
	{
		Health = 0;
		Die();
	}
	else
	{
		Health -= damage;
	}
}

void cPlayer :: Die( void )
{
	// todo
}

void UpdatePlayer( void )
{
	CollideMove_Meatball( (cBasicSprite*)pPlayer, pPlayer->velx * pFramerate->speedfactor, pPlayer->vely * pFramerate->speedfactor, pPlayer->Collision, pPlayer->type );
	pPlayer->Update();
}