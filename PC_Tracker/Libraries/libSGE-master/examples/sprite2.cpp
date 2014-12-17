#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "sge.h"

#define NSPRITES 100

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
	SDL_WM_SetCaption("SGE testsprite", "testsprite");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(640, 480, 0, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Setup the screen class */
	sge_screen screenc(screen);
		
	//Load the smiley
	SDL_Surface *temp,*bitmap;
	temp = SDL_LoadBMP("smiley.bmp");
	if(!temp){
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		exit(1);
	}	
	bitmap = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	SDL_SetColorKey(bitmap, SDL_SRCCOLORKEY|SDL_RLEACCEL, sge_GetPixel(bitmap,0,0));
	
	sge_Randomize();
	
	/* Setup the sprites */	
	sge_ssprite *sprite[NSPRITES];
	
	int i;
	for(i=0; i<NSPRITES; i++){
		sprite[i]=new sge_ssprite(screen,bitmap);

		//Add this sprite to the list
		screenc.add_shape_p(sprite[i]);
		
		//Random starting speed
		while ( ! sprite[i]->get_xvel() && ! sprite[i]->get_yvel() ) {
			sprite[i]->set_xvel( sge_Random(-1,1) );
			sprite[i]->set_yvel( sge_Random(-1,1) );
		}
		
		//Random starting pos
		sprite[i]->move_to(sge_Random(0, screen->w-bitmap->w), sge_Random(0, screen->h-bitmap->h));
	}
	
	
	sge_Update_OFF();
	
	/* Main loop */
	SDL_Event event;
	int quit=0;
	int loops=0;
	Uint32 start=SDL_GetTicks();
	
	do{		
		for(i=0; i<NSPRITES; i++){
		  sprite[i]->clear(0);   //Remove sprite from screen
			sprite[i]->update(); //Move the sprite
		}
		
		//Let the screen class redraw and update rects for all sprites
		screenc.update();	
	
		loops++;
		
		/* Check events */
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){quit=1;}
			if(event.type==SDL_QUIT){quit=1;}
		}
	}while(!quit);
	
	//Show FPS
	printf("%.2f FPS.\n",((double)loops*1000)/(SDL_GetTicks()-start));
	
	//Clean up
	for(i=0; i<NSPRITES; i++)
		delete sprite[i];

	SDL_FreeSurface(bitmap);
	
	return 0;
}
