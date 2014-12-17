

#ifndef __ENEMY_AF373_H__
#define __ENEMY_AF373_H__

#include "Globals.h"

class cAF373 : public cEnemy
{
public:
	cAF373( double nposx, double nposy );
	~cAF373( void );
	
	void init( void );
	
	void Update( void );

	void Draw( SDL_Renderer *renderer );

	void Die( void );
	
	void Fire( int fdirection );

	int AI_PlayerSearch( int sdirection );
	void AI_Idle( void );

	/**	The images
	 *	0 = Left
	 *	1 = Right
	 */
	SDL_Surface *images[2];
	

	cWeapon *pWeapon;

	/** 0 : falling
	 * 1 : onground massive
	 * 2 : onground halfmassive
	 */
	int onGround;

	/** The Direction the AI wants to move
	 * uses the defined directions
	 */
	int AI_direction;
	
	/** The time the Ai should move
	 * if <= 0 AI_direction is set to idle ( -1 )
	 */
	double AI_movingtime;

	/** 0 = Staying ( not doing anything )
	 * 1 = Moving,Walking
	 * 2 = Jumping
	 * 3 = Falling
	 * 4 = Flying
	 */	
	int state;
};

#endif