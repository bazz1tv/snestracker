
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Globals.h"

/// Enemy Types
#define ENEMY_UNDEFINED 0
#define ENEMY_AF373 1

/// Enemy States
#define ENEMY_AI_STATE_UNKNOWN		0
#define ENEMY_AI_STATE_WAITING		1
#define ENEMY_AI_STATE_SEARCHING	2
#define ENEMY_AI_STATE_FOLLOWING	3
#define ENEMY_AI_STATE_TARGETTING	4
#define ENEMY_AI_STATE_FIRING		5

class cEnemy : public cMVelSprite
{
public:
	cEnemy( double nposx, double nposy );
	~cEnemy( void );

	virtual void init( void );
	
	virtual void Update( void );
	virtual void Draw( SDL_Renderer *renderer );

	virtual void Jump( int power = 5 );

	virtual void Die( void );
	
	/**
	 * 0 = facing left
	 * 1 = facing right
	 * 2 = facing up
	 * 3 = facing down
	 */
	int direction;

	/// The Enemy Type
	unsigned int Enemy_type;
	
	/// The Enemy State
	unsigned int Enemy_state;

	/// The Health	
	unsigned int health;

	/// Maximum Speed
	double max_speed;

	/// The Acceleration
	double Acceleration;

};

extern unsigned int EnemyCount;
extern cEnemy **Enemies;

/// Adds an Enemy with the given type
void AddEnemy( double nposx, double nposy, unsigned int etype );

/// Updates all Enemies
void UpdateEnemies( void );

/// Gets the first Collision if the rect overlaps
int GetCollidingEnemyNum( SDL_Rect *Crect );
cEnemy *GetCollidingEnemy( SDL_Rect *Crect );

/// Draws all Enemies
void DrawEnemies( SDL_Renderer *renderer );

/// Seletes an given Enemy
void DeletEnemy( unsigned int number );

/// Deletes every Enemy and frees the array
void DeleteAllEnemies( void );

#endif