/// @addtogroup Player
// @{
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "defines.h"
#include "sprite.h"



class cWeapon;
class cCamera;

extern cFramerate *pFramerate;
extern cCamera *pCamera;
extern cImageManager *IMan;


extern int window_width;
extern int window_height;





/// Keep track of the player.
/** Draw, update the player. Change his active weapon. Jump(), Fire() bullets,
	Die, track directions and whether he's on the ground, health,
*/
class cPlayer : public cMVelSprite
{
public:
	cPlayer( double nposx = 0 , double nposy = 0 );
	~cPlayer( void );
	
	/// Loads the Player images
	void init( void );
	
	/// Updates the Player
	void Update( void );

	/// Draws the Player
	void Draw( SDL_Renderer *renderer );

	/// Resets
	void Reset( void );

	/// Jump with the given Power
	void Jump( int power = 5 );

	/// Change the Active Weapon
	void ChangeActiveWeapon( unsigned int Weapon_type );

	/// Fires the specified Bullet
	void Fire( void );

	/// The Player gets hit by an Bullet or something else
	void Get_Hit( unsigned int damage );

	/// The Player dies
	void Die( void );

	/**	The images \n
	 *	0 = Standing Left \n
	 *	1 = Standing Right \n
	 *  2 = Standing Looking up \n
	 *	3 = Ducking \n
	 */
	SDL_Surface *images[5];

	/// The Player Weapon
	cWeapon *pWeapon;

	/**
	 * 0 = facing left\n
	 * 1 = facing right\n
	 * 2 = facing up\n
	 * 3 = facing down\n
	 *  we need to add angle shots too..\n 
	 *  we'll add when player possesses these abilities\n\n
	 */	
	int direction;

	/** 0 : falling\n
	 * 1 : onground massive\n
	 * 2 : onground halfmassive\n
	 */
	int onGround;

	/** 0 = Staying ( not doing anything :)\n
	 * 1 = Moving,Walking\n
	 * 2 = Jumping\n
	 * 3 = Falling\n
	 * 4 = Flying\n
	 */	
	int state;

	/** If you press the Jump Button before you
	 * touch the ground you have this time
	 * to touch the ground and then jump
	*/
	double jump_timer;

	/** Power of the Jump Timer given
	 * when pressed the Jump Button
	 */ 
	int jump_timer_power;

	double jumpvel;

	/** The Weapon which the Player has in his hands ( Active Weapon )\n
	 * see Weapon definitions
	 */ 
	unsigned int actWeapon;

	/** The Health\n
	 * if == 0 the player dies and the level restarts ;O
	 */
	unsigned int Health;

	unsigned int updown_pressed; ///< Up or Down -> Left or Right 
};

void UpdatePlayer( void );

#endif
// @}