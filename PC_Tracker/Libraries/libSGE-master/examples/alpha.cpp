#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "sge.h"

//Main
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
	SDL_WM_SetCaption("Alpha magic", "alpha");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(600, 300, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
	
	
	sge_FilledRect(screen,0,0, screen->w,screen->h/2, 255,255,255);
	
	/* Open TT font file */
	if(sge_TTF_Init()!=0){fprintf(stderr,"TT error: %s\n",SDL_GetError());exit(1);} 	
	sge_TTFont *font;
	font=sge_TTF_OpenFont("font.ttf", 55);
	if(font==NULL){fprintf(stderr,"TT error: %s\n",SDL_GetError());exit(1);}
	
	/* Ugly font rendering */
	sge_TTF_AAOff();
	sge_tt_textout(screen,font,"Linux", 10,167, 0,0,255, 255,255,255, SDL_ALPHA_OPAQUE);	
	
	/* AA rendering */
	sge_TTF_AAOn();
	sge_tt_textout(screen,font,"Linux", 210,167, 0,0,255, 255,255,255, SDL_ALPHA_OPAQUE);	
	
	/* AA with alpha rendering */
	sge_TTF_AA_Alpha();
	sge_tt_textout(screen,font,"Linux", 410,167, 0,0,255, 1,2,3, SDL_ALPHA_OPAQUE);
	
	sge_TTF_SetFontSize(font, 25);
	sge_tt_textout(screen,font,"Normal", 50,200, 0,255,0, 0,0,0, SDL_ALPHA_OPAQUE);
	sge_tt_textout(screen,font,"Anti aliasing", 210,200, 0,255,0, 0,0,0, SDL_ALPHA_OPAQUE);
	sge_tt_textout(screen,font,"Alpha", 460,200, 0,255,0, 0,0,0, SDL_ALPHA_OPAQUE);
	
	sge_TTF_CloseFont(font);
	
	
	/* Some alpha component magic */
	SDL_Surface *img=sge_CreateAlphaSurface(SDL_SWSURFACE, 100, 100);  //Alocate buffer

	Uint32 ctab[101];
	sge_AlphaFader(255,0,255,0, 0,0,255,230, ctab,0,100);  //Make a nice alpha blended pallete
	
	/* Paint the buffer */
	for(int yy=0; yy<=100; yy++)
		sge_HLine(img,0,100,yy,ctab[yy]);
	
	
	SDL_UpdateRect(screen,0,0,0,0);
	
	
	/* The buffer for the background */
	SDL_Surface *buffer;
	buffer=SDL_DisplayFormat(screen);	
	
	
	// If the delay between two PollEvent is greater than 100 and all events are added to the
	// event queue, the queue will grow *fast*. You're program will have no change to catch
	// up. You *must* try to avoid adding events unnecessarily - mousemotions for example.
	SDL_EventState(SDL_MOUSEMOTION,SDL_IGNORE);
	
	
	Sint32 x=50,y=190;
	
	Sint32 oldx = 50, oldy = 190;
	
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
		if( (x+img->w) >= screen->w )
			right = false;
		else if( x <= 0 )
			right = true;
		if( (y+img->h) >= screen->h )
			down = false;
		else if( y <= 0)
			down = true;	
			
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
		sge_Blit(buffer,screen,oldx, oldy, oldx,oldy, img->w,img->h);
			
		//Draw the alpha surface
		sge_Blit(img,screen,0,0, x,y, img->w,img->h);
			
		//Update screen
		sge_Update_ON();
		sge_UpdateRect(screen, x-1, y-1, img->w+2, img->h+2);
		sge_Update_OFF();			

		oldx=x; oldy=y;

		loops++;
		
		// Check events
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
			if(event.type==SDL_QUIT){break;}
		}

	}while(true);

	
	/* Print FPS */
	printf("%.2f FPS (target: 100).\n",((double)loops*1000)/(SDL_GetTicks()-tstart));

	/* Clean up */
	SDL_FreeSurface(img);
	SDL_FreeSurface(buffer);
	
	return 0;	
}
