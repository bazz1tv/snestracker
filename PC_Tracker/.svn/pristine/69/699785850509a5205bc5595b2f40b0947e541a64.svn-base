/** @addtogroup Guns
 @{
 */
#ifndef __WEAPON_H__
#define __WEAPON_H__

//#include "Globals.h"
#include "sprite.h"
#include "bullet.h"
#include "defines.h"

extern cSoundManager *SMan;
extern cFramerate *pFramerate;
extern cAudio *pAudio;
extern SDL_Renderer *Renderer;

/// @defgroup WEAPON_TYPES WEAPON_TYPES 
//@{
// Weapon types
#define WEAPON_UNKNOWN 0
#define WEAPON_PISTOL 1
#define WEAPON_MACHINEGUN 2
#define WEAPON_LASER_X1 3
// @}

/// The Basic Weapon class
class cWeapon : public cBasicSprite
{
public:
	cWeapon( unsigned int nOrigin, unsigned int nPower, int nLifetime );
	~cWeapon( void );

	/// Fires an Bullet
	virtual void Fire( void );

	/// Updates the Weapon
	virtual void Update( void );

	/// Checks if the timer has run out
	virtual SDL_bool can_Fire( void );

	/** The Supported directions
	 * 0 = left
	 * 1 = right
	 */
	int direction;

	/// The Weapon Type
	unsigned int Weapon_type;

	/// Should be Player,Enemy or Static (todo)
	unsigned int Origin;

	/// The damage the one who gets hit takes
	unsigned int Power;

	/// The Lifetime ( -1 = infinite )
	int Lifetime;

	/** Time before you can shoot once again
	 * If you change the Weapon it's resetted
	 */
	double timer;

	/// The Weapon Fire Sound
	Mix_Chunk *sound_Fire;
};


//########## The Pistol #########

class cWeapon_Pistol : public cWeapon
{
public:
	cWeapon_Pistol( unsigned int Origin, unsigned int Power, int Lifetime );
	~cWeapon_Pistol( void );

	void Fire( void );
};

//########## The Machinegun #########

class cWeapon_Machinegun : public cWeapon
{
public:
	cWeapon_Machinegun( unsigned int Origin, unsigned int Power, int Lifetime );
	~cWeapon_Machinegun( void );

	void Fire( void );
};

//########## Laser X-1 #########

class cWeapon_Laser_X1 : public cWeapon
{
public:
	cWeapon_Laser_X1( unsigned int Origin, unsigned int Power, int Lifetime );
	~cWeapon_Laser_X1( void );

	void Fire( void );
};

#endif

// @}