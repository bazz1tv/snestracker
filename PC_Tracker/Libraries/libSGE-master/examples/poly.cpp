#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "sge.h"

void do_poly(SDL_Surface *dest)
{
	Sint16 x[30];
	Sint16 y[30];
	Uint8 R,G,B;
	
	int points = sge_Random(3,30);  //Number of points in polygon
	int i;
	
	// Generate random coords
	for( i=0; i<points; i++ ){
		x[i] = sge_Random(0, dest->w-1);
		y[i] = sge_Random(0, dest->h-1);
	}
	
	// Pick a color
	R = sge_Random(0, 255);
	G = sge_Random(0, 255);
	B = sge_Random(0, 255);
	
	if( R<40 && G<40 && B<40 ){
		// Too dark
		R=G=B=255;
	}
	
	sge_AAFilledPolygon(dest, points, x, y, R,G,B);
}


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
	SDL_WM_SetCaption("Polygons (Press any key)", "poly");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(640, 480, 0, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}


	/* First draw a very nice gourand shaded pentagram */
	Sint16 x[]={111, 529, 190, 320, 450, 111, 320, 529, 450, 190};
	Sint16 y[]={193, 193, 439,  41, 439, 193,  41, 193, 439, 439};
	
	Uint8  R[]={ 95,  95, 255,   0, 255,  95,   0,  95, 255, 255};
	Uint8  G[]={  0,   0,   0,   0,   0,   0,   0,   0,   0,   0};
	Uint8  B[]={160, 160,   0, 255,   0, 160, 255, 160,   0,   0};
	
	sge_AAFadedPolygon(screen, 10, x, y, R, G, B);
	
	sge_Randomize();
	
	/* Main loop */
	SDL_Event event;
	do{
		/* Wait for user input */
		SDL_WaitEvent(&event);
		if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
		else if(event.type==SDL_KEYDOWN){
			sge_ClearSurface(screen, 0,0,0);
			do_poly(screen);
		}
		if(event.type==SDL_QUIT){break;}
	}while(true);

	return 0;
}
