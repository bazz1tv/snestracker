/// @addtogroup Sprites
// @{


#ifndef __SPRITE_H__
#define __SPRITE_H__

#ifdef _WIN32
#include <SDL_ep.h>
#elif __APPLE__
#include <MeatBallEngine/SDL_ep.h>
#endif

class Collisiondata;
extern SDL_Renderer *Renderer;

/// @addtogroup SPRITE_TYPES
// @{
// SPRITE_TYPES
enum SPRITE_TYPES {
SPRITE_TYPE_NO_TYPE,
SPRITE_TYPE_PLAYER,
SPRITE_TYPE_MASSIVE,
SPRITE_TYPE_PASSIVE,
SPRITE_TYPE_HALFMASSIVE,
SPRITE_TYPE_BULLET,
SPRITE_TYPE_PARTICLE,
SPRITE_TYPE_ENEMY,
};
// @}

class cMVelSprite : public cVelocitySprite
{
public:
	cMVelSprite( SDL_Surface *new_image, double x, double y, double nvelx = 0, double nvely = 0 );
	~cMVelSprite( void );

	void Update( void );

	void Draw( SDL_Renderer *renderer );

	SDL_Rect GetRect( SDL_bool with_camerapos = SDL_FALSE );

	/** The Type
	 * 0 = No Type			<br>
	 * 1 = Player			<br>
	 * 2 = Sprite Massive	<br>
	 * 3 = Sprite Passive	<br>
	 * 4 = Sprite Halfmassive	<br>
	 * 5 = Sprite Bullet		<br>
	 * 6 = ?				<br>
	 * 7 = ?				<br>
	 *
	 */
	int type;
	
	/// Start Position
	double Startposy, Startposx;

	Collisiondata *Collision;
};

class cMAngleSprite : public cAngleSprite
{
public:
	cMAngleSprite( SDL_Surface *new_image, double x, double y, double nangle = 0, double nspeed = 0 );
	~cMAngleSprite( void );

	void Update( void );

	void Draw( SDL_Renderer *renderer );

	SDL_Rect GetRect( SDL_bool with_camerapos = SDL_FALSE );

	/** The Type				<br>
	 * 0 = No Type				<br>
	 * 1 = Player				\n
	 * 2 = Sprite Massive		\n
	 * 3 = Sprite Passive		\n
	 * 4 = Sprite Halfmassive	\n
	 * 5 = Sprite Bullet		\n
	 * 6 = ?					\n
	 * 7 = ?					\n
	 *
	 */
	int type;
	
	/// Start Position
	double Startposy, Startposx;

	Collisiondata *Collision;
};

#endif
// @}