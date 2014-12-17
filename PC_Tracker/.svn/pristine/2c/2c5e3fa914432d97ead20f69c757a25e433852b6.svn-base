
#include "enemy.h"
#include "AF-373.h"
#include "Collision.h"

//unsigned int EnemyCount = 0;
//cEnemy **Enemies = NULL;
ObjectManager<cEnemy> Enemies(OM_OBLITERATE_OBJS_AT_DESTROY, OM_DELETE_OBJS);


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

void cEnemy :: Draw( SDL_Renderer *renderer )
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
	for( register unsigned int i = 0; i < Enemies.objcount; i++ )
	{
		if ( !Enemies.objects[i]->visible && Enemies.objects[i]->Enemy_type == etype )
		{
			Enemies.objects[i]->SetPos( nposx, nposy );
			
			Enemies.objects[i]->init();
			
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
		DEBUGLOG( "Warning : Unknow Enemy Type : %d\n", etype );
		return;
	}

	new_Enemy->init();	

	Enemies.add(new_Enemy);
	//Enemies = (cEnemy**) SDL_realloc( Enemies, ++EnemyCount * sizeof(cEnemy*) );
	//Enemies[EnemyCount - 1] = new_Enemy;
}

void UpdateEnemies( void )
{
	for( register unsigned int i = 0; i < Enemies.objcount; i++ )
	{
		if( !Enemies.objects[i] ) 
		{
			continue;
		}
		
		if( Enemies.objects[i]->visible ) 
		{
			CollideMove( (cBasicSprite*)Enemies.objects[i], Enemies.objects[i]->velx * pFramerate->speedfactor, Enemies.objects[i]->vely * pFramerate->speedfactor, Enemies.objects[i]->Collision, Enemies.objects[i]->type );
			Enemies.objects[i]->Update();
		}
	}
}

int GetCollidingEnemyNum( SDL_Rect *Crect )
{
	if( Enemies.objects && Crect )
	{
		for( register unsigned int i = 0; i < Enemies.objcount; i++ )
		{
			if( !Enemies.objects[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)Enemies.objects[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				return i;
			}
		}
	}

	return -1;
}

cEnemy *GetCollidingEnemy( SDL_Rect *Crect )
{
	if( Enemies.objects && Crect )
	{
		for( register unsigned int i = 0; i < Enemies.objcount; i++ )
		{
			if( !Enemies.objects[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)Enemies.objects[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				return Enemies.objects[i];
			}
		}
	}

	return NULL;
}

void DrawEnemies( SDL_Renderer *renderer )
{
	for( register unsigned int i = 0; i < Enemies.objcount; i++ )
	{
		if( !Enemies.objects[i] ) 
		{
			continue;
		}
		
		Enemies.objects[i]->Draw( renderer );
	}
}

void DeletEnemy( unsigned int number )
{
	if( Enemies.objects && number < Enemies.objcount && Enemies.objects[number] )
	{
		delete Enemies.objects[number];
		Enemies.objects[number] = NULL;
		
	}
}

void DeleteAllEnemies( void )
{	
	Enemies.~ObjectManager();
}