#include <stdlib.h>
#include <stdio.h>
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
	SDL_WM_SetCaption("Sfont", "sfont");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(600, 200, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Open the SFont */
	sge_bmpFont *font;
	font=sge_BF_OpenFont("sfont.png",SGE_BFSFONT);
	if(font==NULL){fprintf(stderr,"Error: %s\n", SDL_GetError());exit(1);}

	/* Make a nice background */
	Uint32 color[201];
	sge_Fader(screen, 0,0,255, 255,0,0, color, 0, 200);
	for(int i=0; i<200; i++){
		sge_HLine(screen, 0,600,i, color[i]);
	}

	int a=21;
	float b=5.31;
	char c='q';
	char d[]="Hello World!";
	sge_BF_textoutf(screen, font, 10, 10, "Testing - Integer:%d, Float:%.3f",a,b);
	sge_BF_textoutf(screen, font, 10, 40, "Char:%c, String:%s",c,d);
	sge_BF_textout(screen, font, "Edit:", 10, 110);
	
	//Keyrepeat
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL+50);
	
	int quit=0;
	char string[21];
 	if(sge_BF_input(screen,font,string,SGE_INOKR, 0,20 ,10,150)<0){ quit=1; };

	if(quit==0){
		printf("%s\n",string); //print the text
	}	

	/* Clean up */
	sge_BF_CloseFont(font);	

	/* Main loop */
	SDL_Event event;
	do{
		SDL_Delay(10);
		

		/* Check events */
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){quit=1;}
			if(event.type==SDL_QUIT){quit=1;}
		}
	}while(quit==0);
	
	return 0;	
}
