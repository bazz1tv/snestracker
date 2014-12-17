
#include "Globals.h"


unsigned int EnemyCount = 0;
cEnemy **Enemies = NULL;



cEnemy :: cEnemy( double nposx, double nposy )
: cMVelSprite( NULL, nposx, nposy )
{
	type = SPRITE_TYPE_ENEMY;

	Enemy_type = ENEMY_UNDEFINED;
	Enemy_state = ENEMY_AI_STATE_UNKNOWN;
	
	SetPos( nposx, nposy );
}

cEnemy :: ~cEnemy( void )
{
	// ...
}

void cEnemy :: init( void )
{
	// virtual 
}

void cEnemy :: Update( void )
{
	cMVelSprite :: Update();
}

void cEnemy :: Draw( SDL_Surface *target )
{
	// virtual 
}

void cEnemy :: Jump( int power /** = 5  */ )
{
	// virtual 
}

void cEnemy :: Die( void )
{
	// virtual 
}

void AddEnemy( double nposx, double nposy, unsigned int etype )
{
	for( unsigned int i = 0; i < EnemyCount; i++ )
	{
		if ( !Enemies[i]->visible && Enemies[i]->Enemy_type == etype )
		{
			Enemies[i]->SetPos( nposx, nposy );
			
			Enemies[i]->init();
			
			return;
		}
	}

	cEnemy *new_Enemy = NULL;

	if( etype == ENEMY_AF373 )// Enemy type 
	{
		new_Enemy = new cAF373( nposx, nposy );		
	}
	else
	{
		printf( "Warning : Unknow Enemy Type : %d\n", etype );
		return;
	}

	new_Enemy->init();	

	Enemies = (cEnemy**) realloc( Enemies, ++EnemyCount * sizeof(cEnemy*) );
	Enemies[EnemyCount - 1] = new_Enemy;
}

void UpdateEnemies( void )
{
	for( unsigned int i = 0; i < EnemyCount; i++ )
	{
		if( !Enemies[i] ) 
		{
			continue;
		}
		
		if( Enemies[i]->visible ) 
		{
			CollideMove( (cBasicSprite*)Enemies[i], Enemies[i]->velx * pFramerate->speedfactor, Enemies[i]->vely * pFramerate->speedfactor, Enemies[i]->Collision, Enemies[i]->type );
			Enemies[i]->Update();
		}
	}
}

int GetCollidingEnemyNum( SDL_Rect *Crect )
{
	if( Enemies && Crect ) 
	{
		for( unsigned int i = 0; i < EnemyCount; i++ )
		{
			if( !Enemies[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)Enemies[i]->GetRect( 1 ), Crect ) )
			{
				return i;
			}
		}
	}

	return -1;
}

cEnemy *GetCollidingEnemy( SDL_Rect *Crect )
{
	if( Enemies && Crect ) 
	{
		for( unsigned int i = 0; i < EnemyCount; i++ )
		{
			if( !Enemies[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)Enemies[i]->GetRect( 1 ), Crect ) ) 
			{
				return Enemies[i];
			}
		}
	}

	return NULL;
}

void DrawEnemies( void )
{
	for( unsigned int i = 0; i < EnemyCount; i++ )
	{
		if( !Enemies[i] ) 
		{
			continue;
		}
		
		Enemies[i]->Draw( Screen );
	}
}

void DeletEnemy( unsigned int number )
{
	if( Enemies && number < EnemyCount && Enemies[number] ) 
	{
		delete Enemies[number];
		Enemies[number] = NULL;
	}
}

void DeleteAllEnemies( void )
{
	if( Enemies ) 
	{
		for( unsigned int i = 0; i < EnemyCount; i++ )
		{
			if( !Enemies[i] ) 
			{
				continue;
			}

			delete Enemies[i];
			Enemies[i] = NULL;
		}

		delete []Enemies;
		Enemies = NULL;
	}

	EnemyCount = 0;
}