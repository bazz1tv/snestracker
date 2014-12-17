#include "effects.h"
#include "Camera.h"
#include "Collision.h"


cMParticleEmitter **ParticleEmitter = NULL;
unsigned int ParticleEmitterCount = 0;

cMParticle :: cMParticle( double x, double y, double nangle /** = 0 */, double nspeed /** = 0  */ ) :
cMAngleSprite( NULL, x, y, nangle, nspeed )
{
	rect.w = 1;
	rect.h = 1;

	width = 1;
	height = 1;

	fadeoutspeed = 0;

	Randomness = 0;
	
	red = 0;
	green = 0;
	blue = 0;
	alpha = 255;

	active =SDL_TRUE;

	type = SPRITE_TYPE_PARTICLE;
}

cMParticle :: ~cMParticle( void )
{
	//
}

void cMParticle :: Update( void )
{
	if( visible == SDL_FALSE )
	{
		return;
	}

	/** Angle Directions :
	 * 0 = Right
	 * 90 = Down
	 * 180 = Left
	 * 270 = Up
	 */
	
	cMAngleSprite::Update();

	if( active == SDL_TRUE ) // If the Particle is moving
	{
		if( Collision->collide != -1) 
		{
			HandleCollision( Collision->collide );
		}
		else if( Collision->direction == 2 ) 
		{
			if( angle < 270 && angle > 90 ) // left 
			{
				HandleCollision( LEFT );
			}
			else // right
			{
				HandleCollision( RIGHT );
			}
		}
		else if( Collision->direction == 3 ) 
		{
			if( angle > 180 && angle < 90 ) // Up 
			{
				HandleCollision( UP );
			}
			else // down
			{
				HandleCollision( DOWN );
			}
		}
		else if( Collision->direction == 1 )
		{
			DEBUGLOG("Unhandled Particle Collision from 2 directions\n");
			// never happened 0.o
		}

		// Gravity
		if( Collision->collide == -1 && Collision->direction == -1 )  // No Collision
		{
			if( ( angle > 150 || angle < 30 ) && speed > 0 ) // flying upwards
			{
				AddSpeed( -0.03 * pFramerate->speedfactor );

				if( speed < 0.2 ) // if it has no speed
				{
					SetDirection( 90.0 ); // reverse
				}
			}
			else if( ( angle < 150 && angle > 30 ) && speed < 10) // falling down
			{
				AddSpeed( 0.03 * pFramerate->speedfactor );
			}

		}
		
		if( Collision->collide == -1 && Collision->direction == -1 ) // No Collision
		{
			AddDirection( Random( -Randomness, Randomness ) * pFramerate->speedfactor );
		}
	}


	alpha -= fadeoutspeed * pFramerate->speedfactor;
	
	if( alpha < 10 ) 
	{
		alpha = 0;
		visible =SDL_FALSE;
		active =SDL_FALSE;
	}
}



void cMParticle :: Draw( SDL_Renderer *renderer )
{
	if( visible == SDL_FALSE )
	{
		return;
	}
	
	DrawPixel(renderer, (int)( posx - pCamera->x ), (int)( posy - pCamera->y ), red,green,blue,alpha);
	
}

void cMParticle :: HandleCollision( int direction )
{
	if( direction == DOWN ) 
	{
		AddSpeed( -(speed*0.65) ); // big slow down

		AddDirection( (180 - ( angle - 90 ))  );

		if( speed < 0.1 ) // if too slow
		{
			active = SDL_FALSE; // deactivate
		}
	}
	else if( direction == UP ) 
	{
		AddSpeed( -(speed*0.2) ); // little slow down
		
		AddDirection( (180 - ( angle - 270 ))  );
	}
	else if( direction == LEFT ) 
	{
		AddSpeed( -(speed*0.3)); // slow down
		
		AddDirection( (180 + ( angle - 180 )) );
	}
	else if( direction == RIGHT ) 
	{
		AddSpeed( -(speed*0.3)); // slow down

		AddDirection( (180 + angle) );
	}
}
/**################################ MParticle Emitter #########################################*/

cMParticleEmitter :: cMParticleEmitter( double x, double y )
{	
	Particles = NULL;
	ParticleCount = 0;

	posx = x;
	posy = y;

	angle = -1;
	speed = 3.0;

	fadeoutspeed = 2.0;
	Randomness = 20;

	red =  255;
	green = 255;
	blue = 255;
	alpha = 255;
}

cMParticleEmitter :: ~cMParticleEmitter( void )
{
	if( Particles ) 
	{
		for( unsigned int i = 0; i < ParticleCount; i++ )
		{
			if( !Particles[i] ) 
			{
				continue;
			}

			delete Particles[i];
			Particles[i] = NULL;
		}

		SDL_free(Particles);
		//delete []Particles;
		Particles = NULL;
	}
}

void cMParticleEmitter :: InitParticles( unsigned int numParticles )
{
	if( Particles )
	{
		for( unsigned int i = 0; i < ParticleCount; i++ )
		{
			if( !Particles[i] ) 
			{
				continue;
			}

			delete Particles[i];
			Particles[i] = NULL;
		}

		//delete []Particles;
		SDL_free(Particles);
		Particles = NULL;
	}

	ParticleCount = 0;

	for( unsigned int i = 0; i < numParticles; i++ )
	{
		double nangle = angle;

		if( angle < 0 ) 
		{
			// Random Direction
			nangle = Random( 360.0 ); 
		}
		
		cMParticle *new_Particle = new cMParticle( posx, posy, nangle, speed );
		
		new_Particle->red = this->red;
		new_Particle->green = this->green;
		new_Particle->blue = this->blue;
		new_Particle->alpha = this->alpha;

		new_Particle->fadeoutspeed = this->fadeoutspeed;
		new_Particle->Randomness = Randomness;

		new_Particle->visible = SDL_TRUE;

		Particles = (cMParticle**) SDL_realloc( Particles, ++ParticleCount * sizeof(cMParticle*) );
		Particles[ParticleCount - 1] = new_Particle;
	}
}

void cMParticleEmitter :: Update( void )
{
	if( visible == SDL_FALSE )
	{
		return;
	}
	
	int PvisibleCount = ParticleCount;

	for( unsigned int i = 0; i < ParticleCount; i++ )
	{
		if( Particles[i]->active == SDL_TRUE && Particles[i]->visible == SDL_TRUE )
		{
			CollideMove( (cBasicSprite*)Particles[i], Particles[i]->dirx * pFramerate->speedfactor, Particles[i]->diry * pFramerate->speedfactor, Particles[i]->Collision, Particles[i]->type );
		}
		
		Particles[i]->Update();

		if( !Particles[i]->visible == SDL_TRUE )
		{
			PvisibleCount--;
		}
	}

	if( PvisibleCount < 1 ) 
	{
		this->visible =SDL_FALSE;
	}
}

void cMParticleEmitter :: Draw( SDL_Renderer *renderer )
{
	if( !visible ) 
	{
		return;
	}
	
	
	
	for( unsigned int i = 0; i < ParticleCount; i++ )
	{
		Particles[i]->Draw( renderer );
	}
	
	
}

void AddParticleEmitter( double x, double y, double nSpeed,Uint8 nred,Uint8 ngreen,Uint8 nblue,
						double nfadeoutspeed, unsigned int power, double nRandomness, double fixedDirection /** = -1  */)
{
	for( unsigned int i = 0; i < ParticleEmitterCount; i++ )
	{
		if( !ParticleEmitter[i] ) 
		{
			continue;
		}

		if( ParticleEmitter[i]->visible == SDL_FALSE )
		{
			ParticleEmitter[i]->posx = x;
			ParticleEmitter[i]->posy = y;
			ParticleEmitter[i]->angle = fixedDirection;
			ParticleEmitter[i]->speed = nSpeed;
			ParticleEmitter[i]->red = nred;
			ParticleEmitter[i]->green = ngreen;
			ParticleEmitter[i]->blue = nblue;
			ParticleEmitter[i]->alpha = 255;
			ParticleEmitter[i]->fadeoutspeed = nfadeoutspeed;
			ParticleEmitter[i]->Randomness = nRandomness;	
			ParticleEmitter[i]->visible = SDL_TRUE;

			ParticleEmitter[i]->InitParticles( power );
			
			return;
		}
	}

	cMParticleEmitter *new_ParticleEmitter = new cMParticleEmitter( x, y );

	new_ParticleEmitter->angle = fixedDirection;
	new_ParticleEmitter->speed = nSpeed;
	new_ParticleEmitter->red = nred;
	new_ParticleEmitter->green = ngreen;
	new_ParticleEmitter->blue = nblue;
	new_ParticleEmitter->fadeoutspeed = nfadeoutspeed;
	new_ParticleEmitter->Randomness = nRandomness;		
	new_ParticleEmitter->visible = SDL_TRUE;
	
	new_ParticleEmitter->InitParticles( power );

	ParticleEmitter = (cMParticleEmitter**) SDL_realloc( ParticleEmitter, ++ParticleEmitterCount * sizeof(cMParticleEmitter*) );
	ParticleEmitter[ParticleEmitterCount - 1] = new_ParticleEmitter;
}

void UpdateParticleEmitter( void )
{
	for( unsigned int i = 0; i < ParticleEmitterCount; i++ )
	{
		if( !ParticleEmitter[i] ) 
		{
			continue;
		}

		ParticleEmitter[i]->Update();
	}
}

void DrawParticleEmitter( SDL_Renderer *renderer )
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	for( unsigned int i = 0; i < ParticleEmitterCount; i++ )
	{
		if( !ParticleEmitter[i] ) 
		{
			continue;
		}

		ParticleEmitter[i]->Draw( renderer );
	}
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void DeleteAllParticleEmitter( void )
{
	if( ParticleEmitter ) 
	{
		for( unsigned int i = 0; i < ParticleEmitterCount; i++ )
		{
			if( !ParticleEmitter[i] ) 
			{
				continue;
			}

			delete ParticleEmitter[i];
			ParticleEmitter[i] = NULL;
		}

		delete []ParticleEmitter;
		ParticleEmitter = NULL;
	}

	ParticleEmitterCount = 0;
}