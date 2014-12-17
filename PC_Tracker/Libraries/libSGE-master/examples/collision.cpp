#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "SDL.h"
#include "sge.h"


int main(int argc, char** argv)
{	
	/* Init SDL */
	if ( SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't load SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* Clean up on exit */
	atexit(SDL_Quit);

	/* Set window title */
	SDL_WM_SetCaption("Collision", "collision");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}

	sge_ClearSurface(screen,0);
	
	//Allocate some buffers
	SDL_Surface *img1,*buffer;
	
	//The buffer for the ball
	img1=SDL_AllocSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,30,30,16, 0,0,0,0);
	sge_ClearSurface(img1,0);
	sge_AAFilledCircle(img1,15,15, 14, 150,200,50);
	SDL_SetColorKey(img1, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0); //Set transparent color
	
	//Draw a background (add more filled rectangles if you want) 
	sge_FilledRect(screen, 300,80, 340,400, 0,0,255);
	sge_FilledRect(screen, 0,360, 200,400, 0,0,255);
	sge_FilledRect(screen, 340,260, 550,300, 0,0,255);
	sge_FilledRect(screen, 100,80, 140,360, 0,0,255);
	sge_FilledRect(screen, 500,80, 550,130, 0,0,255);
	buffer=SDL_DisplayFormat(screen); //The buffer for the background
	

	//Make the collision maps
	sge_cdata *cimg1, *cbg;
	
	cimg1=sge_make_cmap(img1);

	SDL_SetColorKey(buffer, SDL_SRCCOLORKEY, 0); //Must set transparent color on background
	cbg=sge_make_cmap(buffer);
	SDL_SetColorKey(buffer, 0, 0); //No transparency
	
	
 	// If the delay between two PollEvent is greater than 100 and all events are added to the
	// event queue, the queue will grow *fast*. You're program will have no change to catch
	// up. You *must* try to avoid adding events unnecessarily - mousemotions for example.
	SDL_EventState(SDL_MOUSEMOTION,SDL_IGNORE);

	Sint16 x=50,y=190;
	Sint16 oldx=x,oldy=y;
	bool right=true, down=false;

	sge_Update_OFF();	

	// Sets start time
	Uint32 tstart=SDL_GetTicks();
	int loops=0;

	// Main loop
	SDL_Event event;
	do{
		SDL_Delay(10);
	
		//Check borders
		if( (x+img1->w)>=screen->w )
			right=false;
		else if(x<=0)
			right=true;
		if( (y+img1->h)>=screen->h )
			down=false;
		else if(y<=0)
			down=true;
			
		//Check for collision
		if( sge_cmcheck(cbg,0,0, cimg1,x,y) ){
			//SDL_Delay(200);
				
			//Bounce the ball
			if( abs(sge_get_cx()-(x+img1->w/2)) < abs(sge_get_cy()-(y+img1->h/2)) ){ 
				if( sge_get_cy()-(y+img1->h/2) < 0 )
					down=true;
				else
					down=false;
			}
			else{
				if( sge_get_cx()-(x+img1->w/2) < 0 )
					right=true;
				else
					right=false;
			}
					
		}
			
			
		//Update pos
		if(right)
			x++;
		else
			x--;	
		if(down)
			y++;
		else
			y--;
					
		//Update last pos from buffer
		sge_Blit(buffer,screen,oldx,oldy, oldx,oldy, img1->w,img1->h);
			
		//Draw the circle
		sge_Blit(img1,screen,0,0, x,y, img1->w,img1->h);
						
		oldx=x;oldy=y;
			 
		//Update screen
		sge_Update_ON();
		sge_UpdateRect(screen,x-1,y-1,img1->w+2,img1->h+2);
		sge_Update_OFF();			

		loops++;

		// Check events
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
			if(event.type==SDL_QUIT){break;}
		}

	}while(true);

	/* Print FPS */
	printf("%.2f FPS (target: 100).\n",((double)loops*1000)/(SDL_GetTicks()-tstart));

	//Clean upp
	SDL_FreeSurface(img1);
	SDL_FreeSurface(buffer);
	
	sge_destroy_cmap(cimg1);
	sge_destroy_cmap(cbg);
	
	return 0;	
}

