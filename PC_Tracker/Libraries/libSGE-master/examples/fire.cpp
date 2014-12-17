#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "sge.h"

#define NCOLORS 1200
#define XRES 300
#define YRES 100

void setup_palette(SDL_Surface *Surface, Uint32 *ctab, int start,int stop);
void fade(SDL_Surface *surface, int speed);

/* Globals */
int cb[YRES][XRES];   //The color buffer
Uint32 ctab[NCOLORS]; //The color table

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
	SDL_WM_SetCaption("Fire!", "fire");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(XRES, YRES, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}	
	
	/* Clear the color buffer */
	for(int y=0; y<YRES; y++){
		memset(cb[y],0,sizeof(int)*XRES);
	}	
	
	/* Make the nice fire pallete */
	setup_palette(screen,ctab,0,NCOLORS-1);
	
	sge_Randomize();
	
	sge_Update_OFF();
	
	/* Main loop */
	SDL_Event event;
	int loops=0,x,lc=0,speed=1;
	Uint32 start=SDL_GetTicks();
	
	do{
		/* Insert random line */
		for(x=0; x<XRES; x++){
			if(sge_Random(0,10)==0)
				cb[YRES-2][x]=ctab[NCOLORS-1]; //Er, should be =NCOLORS-1 but this looks better
		}	
		
		/* Raise the flame? */
		if(sge_Random(0,700)==0){
			speed=0;
			lc=loops;
		}
		else if(speed==0){
			if(loops-lc>60) // Lower the flame
				speed=1;	
		}
			
		fade(screen,speed); // Do the fading
   	
		/* Update screen */
		SDL_UpdateRect(screen,0,0,0,0);
	
		loops++;
		
		/* Check events */
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
			if(event.type==SDL_QUIT){break;}
		}
	}while(true);
	
	//Show FPS
	printf("%.2f FPS.\n",((double)loops*1000)/(SDL_GetTicks()-start));
	
	return 0;
}


//====================================================================
// This function creates a "flame" palette (using sge_Fader)
//====================================================================
void setup_palette(SDL_Surface *Surface, Uint32 *ctab, int start, int stop)
{
	int slice=(int)((stop-start)/5);

	sge_Fader(Surface, 0,0,0, 0,0,40, ctab, start,slice);       /* black - dark blue */
	sge_Fader(Surface, 0,0,40, 255,0,0, ctab, slice+1,2*slice); /* - red */
	sge_Fader(Surface, 255,0,0, 255,255,0, ctab, 2*slice+1, 3*slice); /* -yellow */
	sge_Fader(Surface, 255,255,0, 255,255,255, ctab, 3*slice+1,stop); /* -white */

}


//====================================================================
// Do the fading magic
//====================================================================
void fade(SDL_Surface *surface, int speed)
{
	int color,x,y;
	Uint16 block[XRES];

	if(SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);
		
	for(y=0; y<YRES; y++){
		for(x=0; x<XRES; x++){					
			color=cb[y][x];
			if(x>0){
				color+=cb[y][x-1];  //pixel to the left
			}
			if(x<XRES-1){
				color+=cb[y][x+1];  //pixel to the right
			}
			if(y<YRES-1){
				color+=cb[y+1][x];
			}
			
			color=(color>>2)-speed; //average (well, often) and fade
			
			if(color>NCOLORS-1)
				color=NCOLORS-1;
			if(color<0)
				color=0;
				
			block[x]=(Uint16)ctab[color];
			cb[y][x]=color;									
		}
		sge_write_block16(surface, block, y); //Copy the new block to screen
	}	
	
	if(SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
				
}
