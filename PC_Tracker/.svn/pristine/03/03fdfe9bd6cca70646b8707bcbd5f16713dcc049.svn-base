
#include "Globals.h"


void CollideMove( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type )
{
	Collision->iCollisionType = 0; // no Collision
	Collision->iCollisionNumber = 0;
	Collision->collide = -1;
	Collision->direction = -1;

	// Jan 15 2013.. Took out Velx from sprite->Width calculation here....................vvvvv
	// initital tests show it's fine.
	PositionCheck( (int)( Sprite->posx + velx ) , (int)( Sprite->posy + vely ), (int)(velx + Sprite->width), (int)Sprite->height, Collision, type );
	
	if( Collision->collide == -1 )
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
		bool movex = 0;
		bool movey = 0;

		if( vely != 0 )
		{
			movey = 1;	// TRUE
		}

		if( velx != 0 )
		{
			movex = 1;	// TRUE
		}

		while( movey || movex ) // While there is movement 
		{
			if( movex )
			{
				PositionCheck( (int)( Sprite->posx + ( (fvelx > 0) ? (1) : (-1) ) ), (int)Sprite->posy, (int)Sprite->width, (int)Sprite->height, Collision, type  );
				
				if( Collision->collide == -1 )	// if no collisions
				{
					Sprite->posx += ((fvelx > 0) ? (1) : (-1)); // Add the one

					// But if the actual velocity is < 1, add that lesser value instead of 1
					if((Sprite->posx > posx_old + fvelx && fvelx > 0) || (Sprite->posx < posx_old + fvelx && fvelx < 0) )
					{
						Sprite->posx = posx_old + fvelx;
					}
					movex = 0;
				}
				else	// there was a collision
				{

					/** The direction of all Collisions\n
	 * -1 = No Collision detected		<br>
	 * 1 = Collision Up/Down/Left/Right	<br>
	 * 2 = Collision in Left/Right		<br>
	 * 3 = Collision Up/Down			<br>
	 */


					if( Collision->direction == ALL_COLLISIONS_NONE ) 
					{
						Collision->direction = ALL_COLLISIONS_LR;	// Collision Left/Right
					}
					else if( Collision->direction == ALL_COLLISIONS_UD ) 
					{
						Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
					}

					movex = 0;
				}
			}

			if( movey )
			{
				PositionCheck( (int)Sprite->posx, (int)( Sprite->posy + ( (fvely > 0) ? (1) : (-1) ) ), (int)Sprite->width, (int)Sprite->height, Collision, type );
				
				if( Collision->collide == -1 )
				{
					Sprite->posy += ((fvely > 0) ? (1) : (-1));
					
					if((Sprite->posy > posy_old + fvely && fvely > 0) || (Sprite->posy < posy_old + fvely && fvely < 0)) 
					{
						Sprite->posy = posy_old + fvely;
					}
					movey = 0;
				}
				else
				{
					if( Collision->direction == ALL_COLLISIONS_NONE ) 
					{
						Collision->direction = ALL_COLLISIONS_UD;	// Collision Up/Down
					}
					else if( Collision->direction == ALL_COLLISIONS_LR ) 
					{
						Collision->direction = ALL_COLLISIONS_UDLR;	// Collision Up/Down/Left/Right
					}

					movey = 0;
				}
			}
		}
	}
}

void CollideMove_Meatball( cBasicSprite *Sprite, double velx, double vely, Collisiondata *Collision, unsigned int type )
{
	Collision->iCollisionType = 0; // no Collision
	Collision->iCollisionNumber = 0;
	Collision->collide = -1;
	Collision->direction = -1;

	Collisiondata lastCollision;


	PositionCheck( (int)( Sprite->posx+12 + velx ) , (int)( Sprite->posy + vely ), (int)(velx + 10), (int)Sprite->height, Collision, type );
	lastCollision = *Collision;

	if (Collision->collide == 5)
	{
		//printf("\nstrange unexplained Collision on Regular generic check.. Setting to Down Collision to get the movements checked\n");
		Collision->collide = DOWN;
	}
	if( Collision->collide == -1 )
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
		bool movex = 0;
		bool movey = 0;

		if( vely != 0 )
		{
			movey = 1;
		}

		if( velx != 0 )
		{
			movex = 1;
		}

		while( movey || movex ) 
		{
			if( movex )
			{
				PositionCheck( (int)( Sprite->posx+12 + ( (fvelx > 0) ? (1) : (-1) ) ), (int)Sprite->posy, 10, (int)Sprite->height, Collision, type  );
				
				printf("MoveX: %d-%d | ", lastCollision.collide, Collision->collide);
				/*if (Collision->collide == 3 || Collision->collide == 2)
				{
					printf("(X_collision_Check..Down Collision?? - Setting to No Collision(-1) (to allow X movement)\n");
					//Collision->collide = -1;
					//Collision->direction = 3;
				}*/
				if( Collision->collide == -1)
				{
					Sprite->posx += ((fvelx > 0) ? (1) : (-1));

					if((Sprite->posx > posx_old + fvelx && fvelx > 0) || (Sprite->posx < posx_old + fvelx && fvelx < 0) )
					{
						Sprite->posx = posx_old + fvelx;

						movex = 0;
					}
				}
				//else
				//{
				if (Collision->collide != -1)
				{
					if( Collision->direction == -1 ) 
					{
						if (Collision->collide == 3 || Collision->collide == 2)
							;//Collision->direction = 3;
						else
							Collision->direction = 2;	// Collision Left/Right
					}
					else if( Collision->direction == 3 ) 
					{
						Collision->direction = 1;	// Collision Up/Down/Left/Right
					}

					movex = 0;
				}
				//}

				/*if (Collision->collide == 2 || Collision->collide == 3)
				{
					Collision->direction = 1;
				}
				else if (Collision->collide == 5)
					Collision->direction = 1;*/
			}

			if( movey )
			{
				PositionCheck( (int)Sprite->posx+12, (int)( Sprite->posy + ( (fvely > 0) ? (1) : (-1) ) ), 10, (int)Sprite->height, Collision, type );
				//printf("MoveY: %d-%d | ", lastCollision.collide, Collision->collide);
				/*if (Collision->collide == 5)
				{
					printf("\nstrange unexplained Collision on Vertical Check.. Setting to Down Collision\n");
					//Collision->collide = -1;
				}

				if (Collision->collide == 1 || Collision->collide == 0) // the unexplainable
				{
					printf("\n Right Collision on a Vertical check??? Setting to Down Collision\n");
					//Collision->collide = -1;
					//Collision->direction = 2;
				}*/
				if (lastCollision.collide == 3 && Collision->collide == 1)
				{
					Sprite->posx += ((fvelx > 0) ? (1) : (-1));

					if((Sprite->posx > posx_old + fvelx && fvelx > 0) || (Sprite->posx < posx_old + fvelx && fvelx < 0) )
					{
						Sprite->posx = posx_old + fvelx;

						movex = 0;
					}
				}
				if( Collision->collide == -1 )
				{
					
					Sprite->posy += ((fvely > 0) ? (1) : (-1));
					
					if((Sprite->posy > posy_old + fvely && fvely > 0) || (Sprite->posy < posy_old + fvely && fvely < 0)) 
					{
						Sprite->posy = posy_old + fvely;
						
						movey = 0;
					}
				}
				if (Collision->collide != -1)
				{
					if( Collision->direction == -1 ) 
					{
						if (Collision->collide == 1 || Collision->collide == 0)
							Collision->direction = 2;
						else Collision->direction = 3;	// Collision Up/Down
					}
					else if( Collision->direction == 2 ) 
					{
						Collision->direction = 1;	// Collision Up/Down/Left/Right
					}

					movey = 0;
				}

				/*if (Collision->collide == 1 || Collision->collide == 0)
				{
					Collision->direction = 1;
				}
				if (Collision->collide == 5)
					Collision->direction =1;*/
			}
		}
	}

	
}


void PositionCheck( int x, int y, int width, int height, Collisiondata *Collision, unsigned int type )
{
	SDL_Rect rect1 = SetRect( x, y, width, height );

	unsigned int i;

	Collision->iCollisionNumber = 0;
	Collision->iCollisionType = 0;
	Collision->collide = -1;
	
	// Check for an Collision with the Player

	if( type != SPRITE_TYPE_PLAYER && type != SPRITE_TYPE_PARTICLE ) 
	{
		if( RectIntersect( &rect1, &pPlayer->rect ) )  // if Position is not valid
		{
			Collision->iCollisionNumber = 0;
			Collision->iCollisionType = SPRITE_TYPE_PLAYER;
			Collision->collide = GetCollide( &rect1, &pPlayer->rect );
			return;
		}
	}

	// Check for an Collision with the Level

	if( type == SPRITE_TYPE_PLAYER || type == SPRITE_TYPE_PARTICLE || type == SPRITE_TYPE_ENEMY || type == SPRITE_TYPE_BULLET ) 
	{
		// This seems to check all level sprites.. This should be optimized to only check for collision with on screen sprites
		// Check for an Collision with an LevelObject
		for( i = 0;i < pLevel->pLevelData->BasicSpriteCount;i++ )
		{
			if( !pLevel->pLevelData->BasicSprites[i] )
			{
				continue;
			}

			if( pLevel->pLevelData->BasicSprites[i]->type == SPRITE_TYPE_MASSIVE )
			{
				if( RectIntersect( &rect1, &pLevel->pLevelData->BasicSprites[i]->rect ) ) // if Position is not valid
				{
					Collision->iCollisionNumber = i;
					Collision->iCollisionType = SPRITE_TYPE_MASSIVE;	// Massive Object
					Collision->collide = GetCollide( &rect1, &pLevel->pLevelData->BasicSprites[i]->rect );
					return;
				}
			}

			if( pLevel->pLevelData->BasicSprites[i]->type == SPRITE_TYPE_HALFMASSIVE ) // if Position is not valid
			{
				if( RectIntersect( &rect1, &pLevel->pLevelData->BasicSprites[i]->rect ) )
				{
					Collision->iCollisionNumber = i;
					Collision->iCollisionType = SPRITE_TYPE_HALFMASSIVE;	// HalfMassive Object
					Collision->collide = GetCollide( &rect1, &pLevel->pLevelData->BasicSprites[i]->rect );
					return;
				}
			}
		}
	}

	// Check for an Collision with the enemies

	if( type == SPRITE_TYPE_PLAYER || type == SPRITE_TYPE_PARTICLE || type == SPRITE_TYPE_BULLET ) 
	{
		for( i = 0;i < EnemyCount;i++ )
		{
			if( !Enemies[i] )
			{
				continue;
			}

			if( RectIntersect( &rect1, &Enemies[i]->rect ) ) // if Position is not valid
			{
				Collision->iCollisionNumber = i;
				Collision->iCollisionType = SPRITE_TYPE_ENEMY;	// Enemy Object
				Collision->collide = GetCollide( &rect1, &Enemies[i]->rect );
				return;
			}
		}
	}

}

int GetCollide( SDL_Rect *r1, SDL_Rect *r2 )
{
	if( r1->y + 1 >= r2->y + r2->h )
	{
		return UP;
	}
	else if( r1->y + r1->h - 1 <= r2->y )
	{
		return DOWN;
	}
	else if( r1->x + 1 >= r2->x + r2->w )
	{
		return LEFT;
	}
	else if( r1->x + r1->w - 1 <= r2->x )
	{
		return RIGHT;
	}

	return 5; // should never happen ...
	// But it does.. On top of boxes usually....
}
