#include <stdlib.h>
#include <stdio.h>
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
	SDL_WM_SetCaption("Bouncing ball!", "bounce");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}

	/* Open TT font file */
	if(sge_TTF_Init()!=0){fprintf(stderr,"TT error: %s\n", SDL_GetError());exit(1);}
	sge_TTFont *font;
	font=sge_TTF_OpenFont("font.ttf", 50);
	if(font==NULL){fprintf(stderr,"TT error: %s\n", SDL_GetError());exit(1);}
	
	/* Make a nice background */
 	sge_tt_textout(screen,font,"Super Fun", 160,180, 0,255,0, 0,0,0, SDL_ALPHA_OPAQUE);
 	sge_tt_textout(screen,font,"Bouncing Ball!!", 100,260, 0,255,0,0,0,0, SDL_ALPHA_OPAQUE); 	
	sge_TTF_CloseFont(font);

	/* Allocate buffers */
	SDL_Surface *buffer,*circle;
	circle=SDL_AllocSurface(SDL_SWSURFACE,100,100,16, 0,0,0,0);
	buffer=SDL_AllocSurface(SDL_SWSURFACE,640,480,16, 0,0,0,0);

 	/* Draws a filled circle on the circle buffer */
	sge_ClearSurface(circle,0);
	sge_AAFilledCircle(circle,50,50,49, 255,0,0);
	
	/* Set the transparity on the circle */
	SDL_SetColorKey(circle, SDL_SRCCOLORKEY, 0); //Set the transparent color
	SDL_SetAlpha(circle, SDL_SRCALPHA|SDL_RLEACCEL, 128);  //Set the alpha blending value

	/* Turns off automatic update */
	sge_Update_OFF();
	
	/* Buffers the background */
	sge_Blit(screen,buffer,0,0, 0,0 ,640,480);

	Sint32 x=250,y=200,oldx=250,oldy=200;
	bool left=false, down=false;
	int loops=0;

	/* Sets start time */
	Uint32 start = SDL_GetTicks();

	/* Main loop */
	SDL_Event event;
	do{
		SDL_Delay(10);
		
		/* Bounce at screen borders */
		if(x>540)
			left = true;
		else if(x<0)
			left = false;
		if(y>380)
			down = false;
		else if(y<0)
			down = true;  	

		/* Updates coords */
		if(left)
			x-=2;
		else
			x+=2;
		if(down)
			y+=2;
		else
			y-=2;

		/* Updates the last area */
		sge_Blit(buffer,screen,oldx-5,oldy-5, oldx-5,oldy-5, 120,120); oldx=x;oldy=y;	

		/* Draws the circel */
   		sge_Blit(circle,screen,0,0, x,y, 100,100);

		/* Updates the screen */
		sge_Update_ON();
		sge_UpdateRect(screen,x-5,y-5,120,120);
		sge_Update_OFF();

		/* Calculate fps */
    	loops++;
		
		/* Check events */
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
			if(event.type==SDL_QUIT){break;}
		}
	}while(true);

	
	/* Print FPS */
	printf("%.2f FPS (target: 100).\n",((double)loops*1000)/(SDL_GetTicks()-start));

	/* Clean up */
	SDL_FreeSurface(buffer);
	SDL_FreeSurface(circle);

	return 0;
}

