#include "effects.h"
#include "Camera.h"
#include "Collision.h"


//cMParticleEmitter **ParticleEmitter = NULL;
//unsigned int ParticleEmitterCount = 0;

ObjectManager<cMParticleEmitter> ParticleEmitter(OM_OBLITERATE_OBJS_AT_DESTROY, OM_DELETE_OBJS);

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
		if( Collision->collision)
		{
			HandleCollision( Collision->collide );
		}
		else if( Collision->direction == 2 ) 
		{
			if( angle < 270 && angle > 90 ) // left 
			{
				HandleCollision( RIGHT );
			}
			else // right
			{
				HandleCollision( LEFT );
			}
		}
		else if( Collision->direction == 3 ) 
		{
			if( angle > 180 && angle < 90 ) // Up 
			{
				HandleCollision( DOWN );
			}
			else // down
			{
				HandleCollision( UP );
			}
		}
		else if( Collision->direction == 1 )
		{
			DEBUGLOG("Unhandled Particle Collision from 2 directions\n");
			// never happened 0.o
		}

		// Gravity
		if( !Collision->collision && Collision->direction == -1 )  // No Collision
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
		
		if( !Collision->collision && Collision->direction == -1 ) // No Collision
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

cMParticleEmitter :: cMParticleEmitter( double x, double y ) :
Particles(OM_OBLITERATE_OBJS_AT_DESTROY, OM_DELETE_OBJS)
{	
	//Particles = NULL;
	//ParticleCount = 0;

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
	/*if( Particles ) 
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
	}*/
	Particles.~ObjectManager();
}

void cMParticleEmitter :: InitParticles( unsigned int numParticles )
{
	/*if( Particles )
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
	}*/
	//Particles.~ObjectManager();
	Particles.DeleteAllObjectContents();

	//ParticleCount = 0;

	for( register unsigned int i = 0; i < numParticles; i++ )
	{
		//SDL_bool found = SDL_FALSE;
		double nangle = angle;

		if( angle < 0 ) 
		{
			// Random Direction
			nangle = Random( 360.0 ); 
		}

		/*for( unsigned int i = 0; i < Particles.objcount; i++ )
		{
			if ( !Particles.objects[i]->visible )
			{
				cMParticle *new_Particle = Particles.objects[i];
				new_Particle->posx = posx;
				new_Particle->posy = posy;
				new_Particle->angle = nangle;
				new_Particle->speed = speed;
				new_Particle->active = SDL_TRUE;
				new_Particle->red = this->red;
				new_Particle->green = this->green;
				new_Particle->blue = this->blue;
				new_Particle->alpha = this->alpha;

				new_Particle->fadeoutspeed = this->fadeoutspeed;
				new_Particle->Randomness = Randomness;

				new_Particle->visible = SDL_TRUE;
				found = SDL_TRUE;
				break;
			}
		}

		if (found)
			continue;*/

		
		
		cMParticle *new_Particle = new cMParticle( posx, posy, nangle, speed );
		
		new_Particle->red = this->red;
		new_Particle->green = this->green;
		new_Particle->blue = this->blue;
		new_Particle->alpha = this->alpha;

		new_Particle->fadeoutspeed = this->fadeoutspeed;
		new_Particle->Randomness = Randomness;

		new_Particle->visible = SDL_TRUE;

		Particles.add(new_Particle);
		//Particles = (cMParticle**) SDL_realloc( Particles, ++ParticleCount * sizeof(cMParticle*) );
		//Particles[ParticleCount - 1] = new_Particle;
	}
}

void cMParticleEmitter :: Update( void )
{
	if( visible == SDL_FALSE )
	{
		return;
	}
	
	int PvisibleCount = Particles.objcount;

	for( register unsigned int i = 0; i < Particles.objcount; i++ )
	{
		cMParticle *ptr = Particles.objects[i];
		if( ptr->active == SDL_TRUE && ptr->visible == SDL_TRUE )
		{
			CollideMove( (cBasicSprite*)ptr, ptr->dirx * pFramerate->speedfactor, ptr->diry * pFramerate->speedfactor, ptr->Collision, ptr->type );
		}
		
		ptr->Update();

		if( ptr->visible == SDL_FALSE )
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
	
	
	
	for( register unsigned int i = 0; i < Particles.objcount; i++ )
	{
		Particles.objects[i]->Draw( renderer );
	}
	
	
}

void AddParticleEmitter( double x, double y, double nSpeed,Uint8 nred,Uint8 ngreen,Uint8 nblue,
						double nfadeoutspeed, unsigned int power, double nRandomness, double fixedDirection /** = -1  */)
{
	for( register unsigned int i = 0; i < ParticleEmitter.objcount; i++ )
	{
		if( !ParticleEmitter.objects[i] ) 
		{
			continue;
		}

		if( ParticleEmitter.objects[i]->visible == SDL_FALSE )
		{
			ParticleEmitter.objects[i]->posx = x;
			ParticleEmitter.objects[i]->posy = y;
			ParticleEmitter.objects[i]->angle = fixedDirection;
			ParticleEmitter.objects[i]->speed = nSpeed;
			ParticleEmitter.objects[i]->red = nred;
			ParticleEmitter.objects[i]->green = ngreen;
			ParticleEmitter.objects[i]->blue = nblue;
			ParticleEmitter.objects[i]->alpha = 255;
			ParticleEmitter.objects[i]->fadeoutspeed = nfadeoutspeed;
			ParticleEmitter.objects[i]->Randomness = nRandomness;	
			ParticleEmitter.objects[i]->visible = SDL_TRUE;

			ParticleEmitter.objects[i]->InitParticles( power );
			
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

	//ParticleEmitter = (cMParticleEmitter**) SDL_realloc( ParticleEmitter, ++ParticleEmitterCount * sizeof(cMParticleEmitter*) );
	//ParticleEmitter[ParticleEmitterCount - 1] = new_ParticleEmitter;
	ParticleEmitter.add(new_ParticleEmitter);
}

void UpdateParticleEmitter( void )
{
	for( register unsigned int i = 0; i < ParticleEmitter.objcount; i++ )
	{
		if( !ParticleEmitter.objects[i] ) 
		{
			continue;
		}

		ParticleEmitter.objects[i]->Update();
	}
}

void DrawParticleEmitter( SDL_Renderer *renderer )
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	for( register unsigned int i = 0; i < ParticleEmitter.objcount; i++ )
	{
		if( !ParticleEmitter.objects[i] ) 
		{
			continue;
		}

		ParticleEmitter.objects[i]->Draw( renderer );
	}
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void DeleteAllParticleEmitter( void )
{
	ParticleEmitter.~ObjectManager();
}