
#include "Globals.h"


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

	active = true;

	type = SPRITE_TYPE_PARTICLE;
}

cMParticle :: ~cMParticle( void )
{
	//
}

void cMParticle :: Update( void )
{
	if( !visible ) 
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

	if( active ) // If the Particle is moving
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
			printf("Unhandled Particle Collision from 2 directions\n");
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
		visible = false;
		active = false;
	}
}

/*void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	//if (y * surface->pitch +

	
    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
#define sge_clip_xmin(pnt) pnt->clip_rect.x
#define sge_clip_xmax(pnt) pnt->clip_rect.x + pnt->clip_rect.w-1
#define sge_clip_ymin(pnt) pnt->clip_rect.y
#define sge_clip_ymax(pnt) pnt->clip_rect.y + pnt->clip_rect.h-1
//==================================================================================
// Put pixel with alpha blending
//==================================================================================
void _PutPixelAlpha(SDL_Surface *surface, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha)
{
	if(x>=sge_clip_xmin(surface) && x<=sge_clip_xmax(surface) && y>=sge_clip_ymin(surface) && y<=sge_clip_ymax(surface)){
		Uint32 Rmask = surface->format->Rmask, Gmask = surface->format->Gmask, Bmask = surface->format->Bmask, Amask = surface->format->Amask;
		Uint32 R,G,B,A=0;
	
		switch (surface->format->BytesPerPixel) {
			case 1: { // Assuming 8-bpp 
				if( alpha == 255 ){
					*((Uint8 *)surface->pixels + y*surface->pitch + x) = color;
				}else{
					Uint8 *pixel = (Uint8 *)surface->pixels + y*surface->pitch + x;
					
					Uint8 dR = surface->format->palette->colors[*pixel].r;
					Uint8 dG = surface->format->palette->colors[*pixel].g;
					Uint8 dB = surface->format->palette->colors[*pixel].b;
					Uint8 sR = surface->format->palette->colors[color].r;
					Uint8 sG = surface->format->palette->colors[color].g;
					Uint8 sB = surface->format->palette->colors[color].b;
					
					dR = dR + ((sR-dR)*alpha >> 8);
					dG = dG + ((sG-dG)*alpha >> 8);
					dB = dB + ((sB-dB)*alpha >> 8);
				
					*pixel = SDL_MapRGB(surface->format, dR, dG, dB);
				}
			}
			break;

			case 2: { // Probably 15-bpp or 16-bpp 		
				if( alpha == 255 ){
					*((Uint16 *)surface->pixels + y*surface->pitch/2 + x) = color;
				}else{
					Uint16 *pixel = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
					Uint32 dc = *pixel;
				
					R = ((dc & Rmask) + (( (color & Rmask) - (dc & Rmask) ) * alpha >> 8)) & Rmask;
					G = ((dc & Gmask) + (( (color & Gmask) - (dc & Gmask) ) * alpha >> 8)) & Gmask;
					B = ((dc & Bmask) + (( (color & Bmask) - (dc & Bmask) ) * alpha >> 8)) & Bmask;
					if( Amask )
						A = ((dc & Amask) + (( (color & Amask) - (dc & Amask) ) * alpha >> 8)) & Amask;

					*pixel= R | G | B | A;
				}
			}
			break;

			case 3: { // Slow 24-bpp mode, usually not used 
				Uint8 *pix = (Uint8 *)surface->pixels + y * surface->pitch + x*3;
				Uint8 rshift8=surface->format->Rshift/8;
				Uint8 gshift8=surface->format->Gshift/8;
				Uint8 bshift8=surface->format->Bshift/8;
				Uint8 ashift8=surface->format->Ashift/8;
				
				
				if( alpha == 255 ){
  					*(pix+rshift8) = color>>surface->format->Rshift;
  					*(pix+gshift8) = color>>surface->format->Gshift;
  					*(pix+bshift8) = color>>surface->format->Bshift;
					*(pix+ashift8) = color>>surface->format->Ashift;
				}else{
					Uint8 dR, dG, dB, dA=0;
					Uint8 sR, sG, sB, sA=0;
					
					pix = (Uint8 *)surface->pixels + y * surface->pitch + x*3;
					
					dR = *((pix)+rshift8); 
            		dG = *((pix)+gshift8);
            		dB = *((pix)+bshift8);
					dA = *((pix)+ashift8);
					
					sR = (color>>surface->format->Rshift)&0xff;
					sG = (color>>surface->format->Gshift)&0xff;
					sB = (color>>surface->format->Bshift)&0xff;
					sA = (color>>surface->format->Ashift)&0xff;
					
					dR = dR + ((sR-dR)*alpha >> 8);
					dG = dG + ((sG-dG)*alpha >> 8);
					dB = dB + ((sB-dB)*alpha >> 8);
					dA = dA + ((sA-dA)*alpha >> 8);

					*((pix)+rshift8) = dR; 
            		*((pix)+gshift8) = dG;
            		*((pix)+bshift8) = dB;
					*((pix)+ashift8) = dA;
				}
			}
			break;

			case 4: { // Probably 32-bpp 
				if( alpha == 255 ){
					*((Uint32 *)surface->pixels + y*surface->pitch/4 + x) = color;
				}else{
					Uint32 *pixel = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
					Uint32 dc = *pixel;
			
					R = ((dc & Rmask) + (( (color & Rmask) - (dc & Rmask) ) * alpha >> 8)) & Rmask;
					G = ((dc & Gmask) + (( (color & Gmask) - (dc & Gmask) ) * alpha >> 8)) & Gmask;
					B = ((dc & Bmask) + (( (color & Bmask) - (dc & Bmask) ) * alpha >> 8)) & Bmask;
					if( Amask )
						A = ((dc & Amask) + (( (color & Amask) - (dc & Amask) ) * alpha >> 8)) & Amask;
					
					*pixel = R | G | B | A;
				}
			}
			break;
		}
	}
}



void sge_PutPixelAlpha(SDL_Surface *surface, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha)
{
	if ( SDL_MUSTLOCK(surface) )
		if ( SDL_LockSurface(surface) != 0 )
			return;

	_PutPixelAlpha(surface,x,y,color,alpha);
	
	// unlock the display 
	if (SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
	
	//if(_sge_update!=1){return;}
	//sge_UpdateRect(surface, x, y, 1, 1);
}


void _PutPixelAlpha(SDL_Surface *surface, Sint16 x, Sint16 y, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha)
{
  _PutPixelAlpha(surface,x,y, SDL_MapRGB(surface->format, R, G, B),alpha);
}

void sge_PutPixelAlpha(SDL_Surface *surface, Sint16 x, Sint16 y, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha)
{
  sge_PutPixelAlpha(surface,x,y, SDL_MapRGB(surface->format, R, G, B), alpha);
}
*/
void cMParticle :: Draw( SDL_Surface *target )
{
	if( !visible ) 
	{
		return;
	}
	
	
	sge_PutPixelAlpha( target, (int)( posx - pCamera->x ), (int)( posy - pCamera->y ), (Uint8)red, (Uint8)green, (Uint8)blue, (Uint8)alpha );
}

void cMParticle :: HandleCollision( int direction )
{
	if( direction == DOWN ) 
	{
		AddSpeed( -(speed*0.65) ); // big slow down

		AddDirection( 180 - ( angle - 90 ) );

		if( speed < 0.1 ) // if too slow
		{
			active = 0; // deactivate
		}
	}
	else if( direction == UP ) 
	{
		AddSpeed( -(speed*0.2) ); // little slow down
		
		AddDirection( 180 - ( angle - 270 )  );
	}
	else if( direction == LEFT ) 
	{
		AddSpeed( -(speed*0.3) ); // slow down
		
		AddDirection( 180 + ( angle - 180 ) );
	}
	else if( direction == RIGHT ) 
	{
		AddSpeed( -(speed*0.3) ); // slow down

		AddDirection( 180 + angle );
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

		delete []Particles;
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

		delete []Particles;
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

		new_Particle->visible = 1;

		Particles = (cMParticle**) realloc( Particles, ++ParticleCount * sizeof(cMParticle*) );
		Particles[ParticleCount - 1] = new_Particle;
	}
}

void cMParticleEmitter :: Update( void )
{
	if( !visible ) 
	{
		return;
	}
	
	int PvisibleCount = ParticleCount;

	for( unsigned int i = 0; i < ParticleCount; i++ )
	{
		if( Particles[i]->active && Particles[i]->visible ) 
		{
			CollideMove( (cBasicSprite*)Particles[i], Particles[i]->dirx * pFramerate->speedfactor, Particles[i]->diry * pFramerate->speedfactor, Particles[i]->Collision, Particles[i]->type );
		}
		
		Particles[i]->Update();

		if( !Particles[i]->visible ) 
		{
			PvisibleCount--;
		}
	}

	if( PvisibleCount < 1 ) 
	{
		this->visible = false;
	}
}

void cMParticleEmitter :: Draw( SDL_Surface *target )
{
	if( !visible ) 
	{
		return;
	}
	
	for( unsigned int i = 0; i < ParticleCount; i++ )
	{
		Particles[i]->Draw( target );
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

		if( !ParticleEmitter[i]->visible )
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
			ParticleEmitter[i]->visible = 1;

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
	new_ParticleEmitter->visible = 1;	
	
	new_ParticleEmitter->InitParticles( power );

	ParticleEmitter = (cMParticleEmitter**) realloc( ParticleEmitter, ++ParticleEmitterCount * sizeof(cMParticleEmitter*) );
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

void DrawParticleEmitter( void )
{
	for( unsigned int i = 0; i < ParticleEmitterCount; i++ )
	{
		if( !ParticleEmitter[i] ) 
		{
			continue;
		}

		ParticleEmitter[i]->Draw( Screen );
	}
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