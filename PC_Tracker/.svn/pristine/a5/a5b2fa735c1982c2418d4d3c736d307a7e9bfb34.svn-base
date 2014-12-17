

#include "SDL_ep.h"


double *Spritespeedfactor = NULL; // The Sprite Speed factor

void SetSpriteSpeedfactor( double *sf )
{
	if( Spritespeedfactor ) 
	{
		//delete Spritespeedfactor;
		Spritespeedfactor = NULL;
	}
	
	Spritespeedfactor = sf;
}

cBasicSprite :: cBasicSprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y )
{
	rect = SetRect( 0, 0, 0, 0 );
	srcrect = SetRect( 0, 0, 0, 0 );
	startrect = SetRect( 0, 0, 0, 0 );

	startrect.x = (int) x;
	startrect.y = (int) y;

	startimage = NULL;
	srcimage = NULL;
	image = NULL;
	texture = NULL;

	alpha = 255;

	visible = SDL_TRUE;

	SetImage( renderer, new_image );

	SetPos( x, y );
	
	rect.x = (int) posx;
	rect.y = (int) posy;

	width = (double)srcrect.w;
	height = (double)srcrect.h;
	
	spawned = SDL_FALSE;

	Spritetype = SPRITE_BASIC;
	ID = 0;
}

cBasicSprite :: ~cBasicSprite( void )
{
	if( image )
	{
		SDL_FreeSurface( image );
	}
	
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}

	image = NULL;
	srcimage = NULL;
	startimage = NULL;
}

void cBasicSprite :: UpdateTexture( SDL_Renderer *renderer )
{
	DestroyTexture();
	
	texture = SDL_CreateTextureFromSurface(renderer, image);
}

void cBasicSprite :: DestroyTexture ( void )
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}

void cBasicSprite :: SetImage( SDL_Renderer *renderer, SDL_Surface *new_image, SDL_bool OverrideSize /* = 1  */ )
{
	srcimage = new_image;
	
	DestroyTexture();
	texture = SDL_CreateTextureFromSurface(renderer, new_image);

	drawimg = SDL_TRUE;

	if( !new_image )
	{
		width = 0;
		height = 0;
		
		this->srcrect.w = 0;
		this->srcrect.h = 0;
		
		//SetAlpha( 255 );
		return;
	}
	else
	{
		
		srcrect.w = new_image->w;
		srcrect.h = new_image->h;

		Uint8 tAlpha;
		if (SDL_GetSurfaceAlphaMod(srcimage, &tAlpha) < 0)
		{
			printf("SDL_GetSurfaceAlphaMod failed!");
			return;
		}
		
		SetAlpha( tAlpha );

		if( tAlpha < 255 )
		{
			SDL_SetSurfaceAlphaMod( srcimage, 255 );
		}
		
	}
	
	if ( !startimage )
	{
		startimage = new_image;

		startrect.w = new_image->w;
		startrect.h = new_image->h;
	}

	if( OverrideSize == SDL_TRUE)
	{
		width = (double)new_image->w;
		height = (double)new_image->h;
	}

	return;
}

void cBasicSprite :: SetSize( double nwidth, double nheight )
{
	if( nwidth < 0 || nheight < 0 ) 
	{
		return;
	}

	width = nwidth;
	height = nheight;

	if( image && (image->w != width) && (image->h != height) )
	{
		drawimg = SDL_TRUE;
	}
	else if( srcimage && (width > 0 && height > 0) ) 
	{
		drawimg = SDL_TRUE;
	}
}

void cBasicSprite :: AddSize( double naddwidth, double naddheight )
{
	if( naddwidth == 0 && naddheight == 0 ) 
	{
		return;
	}

	width += naddwidth;
	height += naddheight;

	if( width < 0 )
	{
		width = 0;
	}

	if( height < 0 )
	{
		height = 0;
	}

	if( srcimage ) 
	{
		drawimg = SDL_TRUE;
	}

}
void cBasicSprite :: SetPos( double x, double y )
{
	posx = x;
	posy = y;

	if ( !startrect.x && !startrect.y )
	{
		startrect.x = (int) x;
		startrect.y = (int) y;
	}
}

void cBasicSprite :: SetAlpha( Uint8 Alpha )
{
	this->alpha = Alpha;

	drawimg = SDL_TRUE;
}

void cBasicSprite :: SetColorKey( Uint32 Colorkey )
{
	if( srcimage )
	{
		SDL_SetColorKey( srcimage, SDL_TRUE, Colorkey );
	}

	drawimg = SDL_TRUE;
}

Uint32 cBasicSprite :: GetAlpha( void )
{
	return alpha;
}

Uint32 cBasicSprite :: GetColorkey( void )
{
	if( !srcimage ) 
	{
		return 0;
	}
	Uint32 key;
	if (SDL_GetColorKey(srcimage, &key) < 0)
	{
		printf ("SDL_GetColorKey failed: %s", SDL_GetError());
		return NULL;
	}
	return key;
}

double cBasicSprite :: GetSizeWidth( void )
{
	return width;
}

double cBasicSprite :: GetSizeHeight( void )
{
	return height;
}


void cBasicSprite :: Move( double move_x, double move_y )
{
	posx += move_x;
	posy += move_y;
}

void cBasicSprite :: Draw( SDL_Renderer *renderer )
{
	if ( visible == SDL_FALSE || !image || !texture || width <= 0 || height <= 0)
	{
		return;
	}

	rect.x = (int) posx;
	rect.y = (int) posy;

	
	//SDL_BlitSurface( image, NULL, target, &rect );
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

/*void cBasicSprite:: Update(void)
{
	// Empty for now (migrating to SDL2)
}*/

void cBasicSprite :: Update( SDL_Renderer *renderer )
{
	// Image drawing

	if( drawimg == SDL_TRUE && !( this->Spritetype == SPRITE_ANGLE || this->Spritetype == SPRITE_ACCELEREATION ) && srcimage )
	{
		if( image )
		{
			SDL_FreeSurface( image );
			image = NULL;
		}
		
		DestroyTexture();
		
		Uint32 colkey;
		SDL_GetColorKey(srcimage, &colkey);

		if( (int)width == srcimage->w && (int)height == srcimage->h )
		{
			image = CreateSurface( srcimage->w, srcimage->h, srcimage->format->BitsPerPixel );

			
			SDL_FillRect( image, NULL, colkey );
			SDL_BlitSurface( srcimage, NULL, image, NULL );
			
			SDL_SetColorKey( image, SDL_TRUE, colkey );
			
			SDL_SetSurfaceAlphaMod(image, alpha);
			//SDL_SetAlpha( image, SDL_SRCCOLORKEY | SDL_RLEACCEL | SDL_SRCALPHA, alpha );
			
			rect.w = srcimage->w*ScreenScale.x;
			rect.h = srcimage->h*ScreenScale.y;
			
			texture = SDL_CreateTextureFromSurface(renderer, image);
		}
		else if( width > 0 && height > 0 ) 
		{
			// image = zoomSurface( srcimage, width/(double)srcimage->w, height/(double)srcimage->h, 0 );
			image = SDL_ConvertSurface(srcimage, srcimage->format, srcimage->flags);
			// image = sge_transform_surface( srcimage, srcimage->format->colorkey, 0,
			//	(float)(width/(double)srcimage->w), (float)(height/(double)srcimage->h), srcimage->flags ); // untested
			
			rect.w = (int)width;
			rect.h = (int)height;
			
			SDL_SetColorKey( image, SDL_TRUE, colkey );
			SDL_SetSurfaceAlphaMod(image, alpha);
			texture = SDL_CreateTextureFromSurface(renderer, image);
		}
		else
		{
			rect.w = 0;
			rect.h = 0;
		}

		drawimg = SDL_FALSE;

	}
	else if( !( this->Spritetype == SPRITE_ANGLE || this->Spritetype == SPRITE_ACCELEREATION ) )
	{
		drawimg = SDL_FALSE;
	}
	
	// CollisionCheck
}


cVelocitySprite :: cVelocitySprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y,
								   double nvelx /* = 0 */, double nvely /* = 0  */ )
	: cBasicSprite( renderer, new_image, x, y )
{

	velx = nvelx;
	vely = nvely;

	Spritetype = SPRITE_VELOCITY;
}

cVelocitySprite :: ~cVelocitySprite( void )
{
	// Nothing
}

void cVelocitySprite :: SetVelocity( double nvelx, double nvely )
{
	velx = nvelx;
	vely = nvely;
}

void cVelocitySprite :: SetVelocityX( double nvelx)
{
	velx = nvelx;
}
void cVelocitySprite :: SetVelocityY( double nvely )
{
	vely = nvely;
}
void cVelocitySprite :: AddVelocity( double addvelx, double addvely, 
									double maxvelx /* = 0 */, double maxvely /* = 0  */ )
{
	// If Maxvel is positive use <= else >=

	if( maxvelx != 0 )
	{
		if( maxvelx > 0 && velx + addvelx <= maxvelx ) 
		{
			velx += addvelx;
		}
		else if( maxvelx < 0 && velx + addvelx >= maxvelx ) 
		{
			velx += addvelx;
		}
		else
		{
			velx = maxvelx; // untested
		}
	}
	else
	{
		velx += addvelx;
	}

	if( maxvely != 0 )
	{
		if( maxvely > 0 && vely + addvely <= maxvely ) 
		{
			vely += addvely;
		}
		else if( maxvely < 0 && vely + addvely >= maxvely ) 
		{
			vely += addvely;
		}
		else
		{
			vely = maxvely; // untested
		}
	}
	else
	{
		vely += addvely;
	}
}

void cVelocitySprite :: Update( SDL_Renderer *renderer, SDL_bool nMove )
{
	cBasicSprite::Update(renderer);

	if( nMove == SDL_TRUE )
	{
		Move( velx * (*Spritespeedfactor) ,vely * (*Spritespeedfactor) );
	}

}


cAngleSprite :: cAngleSprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y, double nangle /* = 0 */,
			double nspeed /* = 0 */, SDL_bool nanglerotate /* = 1  */)
	: cBasicSprite( renderer, new_image, x, y )
{
	angle = 0;
	speed = 0;

	dirx = diry = 0;
	
	anglerotate = nanglerotate;

	SetDirection( nangle, nspeed );

	Spritetype = SPRITE_ANGLE;
}

cAngleSprite :: ~cAngleSprite( void )
{
	// Nothing
}

void cAngleSprite :: SetDirection( double nangle /* = -1 */, double nspeed /* = -1 */ )
{
	if( nangle == -1 ) 
	{
		nangle = angle;
	}

	if( nspeed == -1 )
	{
		nspeed = speed;
	}
	
	if( nangle > 360 || nangle < 0 )
	{
		return;
	}
	
	angle = nangle;
	
	dirx = (double) cos( angle * 0.017453292519943295 ) * nspeed;
	diry = (double) sin( angle * 0.017453292519943295 ) * nspeed;		

	speed = nspeed;

	drawimg = SDL_TRUE;
}

void cAngleSprite :: AddDirection( double nangle )
{
	angle += nangle;

	while (angle > 360) 
	{
		angle -= 360;
	}
	
	while (angle < 0) 
	{
		angle += 360;
	}

	SetDirection();
}

void cAngleSprite :: SetSpeed( double nspeed )
{
	speed = nspeed;

	SetDirection();
}

void cAngleSprite :: AddSpeed( double nspeed )
{
	speed += nspeed;

	SetDirection();
}

void cAngleSprite :: Update( SDL_Renderer *renderer, SDL_bool nMove )
{
	cBasicSprite::Update(renderer);

	if( anglerotate == SDL_TRUE && drawimg == SDL_TRUE && srcimage )
	{
		//printf("%f\n",width);
		if( image ) 
		{
			SDL_FreeSurface( image );
		}
		
		if (texture)
		{
			SDL_DestroyTexture(texture);
		}

		if( (int)width != srcimage->w && (int)height != srcimage->h )
		{
			//SDL_Surface *temp = zoomSurface( srcimage, width/(double)srcimage->w, height/(double)srcimage->h, 0 );
			//image = rotozoomSurface( temp, angle,srcimage->format->colorkey );
			// The comment below simply direct copies the surface
			//image = SDL_ConvertSurface(srcimage, srcimage->format, srcimage->flags);
			//image = sge_transform_surface( srcimage, srcimage->format->colorkey, (float)angle, (float)(width/(double)srcimage->w),
				//(float)(height/(double)srcimage->h), srcimage->flags ); // untested

			//SDL_FreeSurface( temp );
			
			
			rect.w = (int)width;
			rect.h = (int)height;
		}
		else
		{
			image = SDL_ConvertSurface(srcimage, srcimage->format, srcimage->flags);
			//image = sge_transform_surface( srcimage, srcimage->format->colorkey, (float)angle, 1,
				//1, srcimage->flags ); // untested
		}


		rect.w = (int)width;
		rect.h = (int)height;
		Uint32 colkey;
		SDL_GetColorKey(srcimage, &colkey);
		
		SDL_SetColorKey( image, SDL_TRUE, colkey );
			
		SDL_SetSurfaceAlphaMod(image, alpha);
		
		SDL_CreateTextureFromSurface(renderer, image);
		//SDL_SetAlpha( image, SDL_SRCCOLORKEY | SDL_RLEACCEL | SDL_SRCALPHA, alpha );
		
		drawimg = SDL_FALSE;
	}

	if( nMove == SDL_TRUE)
	{
		Move( dirx * *Spritespeedfactor, diry * *Spritespeedfactor );
	}

}


cAccelerationSprite :: cAccelerationSprite( SDL_Renderer *renderer, SDL_Surface *new_image, double x, double y, double nangle /* = 0  */,
					double nspeed /* = 0 */, double nacc /* = 0 */, double ndeacc /* = 0  */ )
	: cAngleSprite( renderer, new_image, x, y, nangle, nspeed )
{
	acc = nacc;

	deacc = ndeacc;

	Spritetype = SPRITE_ACCELEREATION;
}

cAccelerationSprite :: ~cAccelerationSprite( void )
{
	// nothing  
}

void cAccelerationSprite :: SetAcceleration( double nacc )
{
	acc = nacc;
}

void cAccelerationSprite :: AddAcceleration( double nacc )
{
	acc += nacc;
}

void cAccelerationSprite :: SetDeAcceleration( double ndeacc )
{
	deacc = ndeacc;
}

void cAccelerationSprite :: AddDeAcceleration( double ndeacc )
{
	deacc += ndeacc;
}

void cAccelerationSprite :: Update(SDL_Renderer *renderer, SDL_bool nMove )
{
	if(speed < 0 && acc != 0)
	{
		speed -= acc * *Spritespeedfactor;

		if(speed > 0)
		{
			speed = 0;
			acc = 0;
		}
	}
	else if(speed > 0 && acc != 0)
	{
		speed += acc * *Spritespeedfactor;

		if(speed < 0)
		{
			speed = 0;
			acc = 0;
		}
	}


	if( acc < 0 && deacc != 0 )
	{
		acc -= deacc * *Spritespeedfactor;

		if( acc > 0 )
		{
			acc = 0;
		}
	}
	else if( acc > 0 && deacc != 0 )
	{
		acc += deacc * *Spritespeedfactor;

		if(acc > 0)
		{
			acc = 0;
		}
	}

	if( speed < 0 && deacc != 0 )
	{
		speed += deacc * *Spritespeedfactor;

		if(speed > 0)
		{
			speed = 0;
			deacc = 0;
		}
	}
	else if( speed > 0 && deacc != 0 )
	{
		acc -= deacc * *Spritespeedfactor;

		if(speed < 0)
		{
			speed = 0;
			deacc = 0;
		}
	}

	cAngleSprite::Update( renderer, nMove );
}


cMouseCursor :: cMouseCursor( SDL_Renderer *renderer, double x, double y, SDL_Surface *new_image ) : cBasicSprite( renderer, new_image, x, y )
{
	Spritetype = SPRITE_MOUSECURSOR;
}

cMouseCursor :: ~cMouseCursor( void )
{
	//
}

SDL_bool cMouseCursor :: CollisonCheck( double x, double y )
{
	if( posx == x && posy == y ) 
	{
		return SDL_TRUE;
	}

	return SDL_FALSE;
}

SDL_bool cMouseCursor :: CollisonCheck( SDL_Rect *Crect )
{
	SDL_Rect Mouse_rect = SetRect( (int)posx, (int)posy, 1, 1 );

	if(  RectIntersect( Crect, &Mouse_rect ) == SDL_TRUE)
	{
		return SDL_TRUE;
	}

	return SDL_FALSE;
}


void cMouseCursor :: Update( SDL_Renderer *renderer )
{
	cBasicSprite::Update(renderer);
	
	int Mx, My;

	SDL_GetMouseState( &Mx, &My );

	this->SetPos( Mx, My );
}

void cMouseCursor::hide()
{
	visible = SDL_FALSE;
}

void cMouseCursor::show()
{
	visible = SDL_TRUE;
}