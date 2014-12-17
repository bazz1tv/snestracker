
#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Globals.h"
class cBasicSprite;

/// @ingroup ALL_COLLISIONS_DIRECTIONS
// @{
#define ALL_COLLISIONS_NONE	-1
#define ALL_COLLISIONS_UDLR	1
#define ALL_COLLISIONS_LR	2
#define ALL_COLLISIONS_UD	3
// @}

class Collisiondata
{
public:
	/** The first direct Collision direction\n
	 * -1 : No Collision	\n
	 * 0  : Left			\n
	 * 1  : Right			\n
	 * 2  : Up				\n
	 * 3  : Down			\n
	 */
	int collide;

	/** Collisiontypes. Based on SPRITE_TYPES
	 * @see SPRITE_TYPES
	 */
	int iCollisionType;

	/// The Number of the Colliding Sprite
	int iCollisionNumber;

	/** The direction of all Collisions\n
	 * -1 = No Collision detected			<br>
	 *  1 = Collision Up/Down/Left/Right	<br>
	 *  2 = Collision in Left/Right			<br>
	 *  3 = Collision Up/Down				<br>
	 */	
	int direction;
	
	SDL_Rect *cRect;
};

/// Returns the Collision data if a Collision happened
void CollideMove( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type );

void CollideMove_Meatball( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type );
/**	Checks if the given position is valid
 *	if the Position is not valid it returns the direction
 */
void PositionCheck( int x, int y, int width, int height, Collisiondata *Collision, unsigned int type );

/// Returns the collision direction
int GetCollide( SDL_Rect *r1, SDL_Rect *r2 );

#endif