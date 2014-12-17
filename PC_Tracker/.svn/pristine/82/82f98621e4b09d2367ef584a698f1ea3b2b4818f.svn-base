
#include "weapon.h"
#include "bullet.h"

cWeapon :: cWeapon( unsigned int nOrigin, unsigned int nPower, int nLifetime ) :
cBasicSprite( Renderer,NULL, 0, 0 )
{
	Weapon_type = WEAPON_UNKNOWN;

	direction = RIGHT;

	Origin = nOrigin;
	Power = nPower;
	Lifetime = nLifetime;

	timer = 0;
	
	sound_Fire = NULL;
}

cWeapon :: ~cWeapon( void )
{
	//
}

void cWeapon :: Fire( void )
{
	// virtual 
}

SDL_bool cWeapon :: can_Fire( void )
{
	if( timer <= 0) 
	{
		return SDL_TRUE;
	}

	return SDL_FALSE;
}

void cWeapon :: Update( void )
{
	if( timer > 0 ) 
	{
		timer -= pFramerate->speedfactor;
	}
}

// ##################### The Pistol ###########################

cWeapon_Pistol :: cWeapon_Pistol( unsigned int nOrigin, unsigned int nPower, int nLifetime )
: cWeapon( nOrigin, nPower, nLifetime )
{
	Weapon_type = WEAPON_PISTOL;

	// Preload the Firing Sound
	SMan->Add( pAudio->LoadSound( DIR_SOUNDS_GUNS "pistol_1.ogg" ), "pistol" );
}

cWeapon_Pistol :: ~cWeapon_Pistol( void )
{

}

void cWeapon_Pistol :: Fire( void )
{
	pAudio->PlaySound( SMan->GetPointer( "pistol" ) );

	// Add an Pistol Bullet
	AddBullet( posx, posy, direction, BULLET_PISTOL, Origin );

	timer = 10;
}

// ##################### The Machinegun ###########################

cWeapon_Machinegun :: cWeapon_Machinegun( unsigned int nOrigin, unsigned int nPower, int nLifetime )
: cWeapon( nOrigin, nPower, nLifetime )
{
	Weapon_type = WEAPON_MACHINEGUN;

	// Preload the Firing Sound
	SMan->Add( pAudio->LoadSound( DIR_SOUNDS_GUNS "machinegun_1.ogg" ), "machinegun" );
}

cWeapon_Machinegun :: ~cWeapon_Machinegun( void )
{

}

void cWeapon_Machinegun :: Fire( void )
{
	pAudio->PlaySound( SMan->GetPointer( "machinegun" ) );

	// Add an Machinegun Bullet
	AddBullet( posx, posy, direction, BULLET_MACHINEGUN, Origin );

	timer = 5;
}

// ##################### The Laser X-1 ###########################

cWeapon_Laser_X1 :: cWeapon_Laser_X1( unsigned int nOrigin, unsigned int nPower, int nLifetime )
: cWeapon( nOrigin, nPower, nLifetime )
{
	Weapon_type = WEAPON_LASER_X1;

	// Preload the Firing Sound
	SMan->Add( pAudio->LoadSound( DIR_SOUNDS_GUNS "Laser_X1.ogg" ), "laser" );
}

cWeapon_Laser_X1 :: ~cWeapon_Laser_X1( void )
{

}

void cWeapon_Laser_X1 :: Fire( void )
{
	pAudio->PlaySound( SMan->GetPointer( "laser" ) );

	// Add an Machinegun Bullet
	AddBullet( posx, posy, direction, BULLET_LASER_X1, Origin );

	timer = 50;
}
