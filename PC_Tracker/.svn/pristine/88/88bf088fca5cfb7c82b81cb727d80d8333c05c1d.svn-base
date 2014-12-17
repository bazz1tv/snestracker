
#include "player.h"
#include "weapon.h"
#include "Camera.h"
#include "preferences.h"
#include "effects.h"
#include "Collision.h"
#include "MasterBlaster.h"

extern cSettings *pGameSettings;
extern cPlayer *pPlayer;
extern MasterBlaster *MB;

cPlayer *pPlayer;

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
	CollideMove( (cBasicSprite*)this, velx * pFramerate->speedfactor, vely * pFramerate->speedfactor, Collision, type );
	
	pWeapon->Update();
	
	SetImage( Renderer, images[direction] );
	
	cBasicSprite::Update(Renderer);

	// posx+12 is actually base position/
	
	PositionCheck( (int)posx+12, (int)posy + 1, 10, (int)height, Collision, type );
	//CollideMove_Meatball( (cBasicSprite*)pPlayer, 0, 1, pPlayer->Collision, pPlayer->type );
	
	if( Collision->collision && Collision->direction == ALL_COLLISIONS_UD)
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

	if( direction == LEFT && MOVELEFT_KEYS_HELD && MB->mode != MODE_CONSOLE )
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
	else if( direction == RIGHT && MOVERIGHT_KEYS_HELD && MB->mode != MODE_CONSOLE )
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

	if( ( Collision->collision || Collision->direction == ALL_COLLISIONS_UD || Collision->direction == ALL_COLLISIONS_UDLR ) && vely < 0 )
	{
		if( Collision->collide == UP )
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
	else if( Collision->direction == ALL_COLLISIONS_LR || Collision->direction == ALL_COLLISIONS_UDLR || Collision->collide == LEFT || Collision->collide == RIGHT )
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
	else if( (double)posx + width > (double)window.w + pCamera->x )
	{
		//posx = (double)window.w - width;
	}

	if( posy < 0 ) // Up
	{
		// just commented this out after putting some tiles at negative locations...
		// seems to be working fine 1/6/12
		/*SetPos( posx, 0 );
		SetVelocity( velx, 0 );
		state = STATE_FALLING;*/
	}
	else if( (double)posy + height > (double)window.h + pCamera->y ) // Bottom
	{
		//SetPos( posx, (double)window.h - height );
		
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
	//AddParticleEmitter( Particle_posx, posy + 8, 2, 150, 150, 150, 1, 1, 2, 270 ); // grey
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
	pPlayer->Update();
}

void cPlayer :: CollideMove( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type )
{
	// Reset Collision Data
	Collision->Reset();
	
	Collisiondata lastCollision;
	
	
	PositionCheck( (int)( Sprite->posx+12 + velx ) , (int)( Sprite->posy + vely ), (int)(10), (int)Sprite->height, Collision, type );
	lastCollision = *Collision;
	
	if( !Collision->collision )
	{
		Sprite->posx += velx;
		Sprite->posy += vely;
	}
	else
	{
		double posx_old = Sprite->posx;
		double posy_old = Sprite->posy;
		double fvelx = velx;
		double fvely = vely;
		SDL_bool movex = SDL_FALSE;
		SDL_bool movey = SDL_FALSE;
		
		if( vely != 0 )
		{
			movey = SDL_TRUE;
		}
		
		if( velx != 0 )
		{
			movex = SDL_TRUE;
		}
		
		while( movey == SDL_TRUE || movex == SDL_TRUE )
		{
			if( movex == SDL_TRUE )
			{
				PositionCheck( (int)( Sprite->posx+12 + ( (fvelx > 0) ? (1) : (-1) ) ), (int)Sprite->posy, 10, (int)Sprite->height, Collision, type  );
				
				if( !Collision->collision)
				{
					Sprite->posx += ((fvelx > 0) ? (1) : (-1));
					
					if((Sprite->posx > posx_old + fvelx && fvelx > 0) || (Sprite->posx < posx_old + fvelx && fvelx < 0) )
					{
						Sprite->posx = posx_old + fvelx;
						
						movex = SDL_FALSE;
					}
				}
				else
				{
					if (velx > 0)
					{
						Collision->collide = RIGHT;
					}
					else
						Collision->collide = LEFT;
					
					
					if( Collision->direction == ALL_COLLISIONS_NONE )
					{
						Collision->direction = ALL_COLLISIONS_LR;	// Collision Left/Right
					}
					else if( Collision->direction == ALL_COLLISIONS_UD )
					{
						Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
					}
					
					movex = SDL_FALSE;
				}
				
			}
			
			if( movey == SDL_TRUE )
			{
				PositionCheck( (int)Sprite->posx+12, (int)( Sprite->posy + ( (fvely > 0) ? (1) : (-1) ) ), 10, (int)Sprite->height, Collision, type );
				
				if (lastCollision.collide == 3 && Collision->collision)
				{
					Sprite->posx += ((fvelx > 0) ? (1) : (-1));
					
					if((Sprite->posx > posx_old + fvelx && fvelx > 0) || (Sprite->posx < posx_old + fvelx && fvelx < 0) )
					{
						Sprite->posx = posx_old + fvelx;
						
						movex = SDL_FALSE;
					}
				}
				else if( !Collision->collision )
				{
					
					Sprite->posy += ((fvely > 0) ? (1) : (-1));
					
					if((Sprite->posy > posy_old + fvely && fvely > 0) || (Sprite->posy < posy_old + fvely && fvely < 0))
					{
						Sprite->posy = posy_old + fvely;
						
						movey = SDL_FALSE;
					}
				}
				if (Collision->collision)
				{
					if (vely > 0)
					{
						Collision->collide = DOWN;
					}
					else
						Collision->collide = UP;
					
					if( Collision->direction == ALL_COLLISIONS_NONE )
					{
						Collision->direction = ALL_COLLISIONS_UD;	// Collision Up/Down
					}
					else if( Collision->direction == ALL_COLLISIONS_LR )
					{
						Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
					}
					
					movey = SDL_FALSE;
				}
			}
		}
	}
}