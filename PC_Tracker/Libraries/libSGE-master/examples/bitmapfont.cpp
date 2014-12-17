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
	SDL_WM_SetCaption("Input", "input");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(300, 100, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}

	sge_bmpFont *font;
	font=sge_BF_OpenFont("font.bmp",SGE_BFTRANSP|SGE_BFPALETTE);
	if(font==NULL){fprintf(stderr,"Error: %s\n", SDL_GetError());exit(1);}

	/* Make a nice background */
	Uint32 color[301];
	sge_SetupRainbowPalette(screen,color,0, 0, 300);
	for(int i=0; i<300; i++){
		sge_VLine(screen, i, 0, 100, color[i]);
	}

	int a=21;
	float b=5.31;
	char c='q';
	char d[]="Hello World!";

	sge_BF_textoutf(screen, font, 10, 10, "Testing - Integer:%d, Float:%.3f",a,b);
	sge_BF_textoutf(screen, font, 10, 25, "Char:%c, String:%s",c,d);
	sge_BF_SetColor(font, 0,0,0); //Change the color of the font
	sge_BF_textout(screen, font, "Edit:", 10, 50);

	//Keyrepeat
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL+50);

	int quit=0;
	char string[40];
	
 	if(sge_BF_input(screen,font,string,SGE_INOKR, 0,39 ,10,65)<0){ quit=1; };
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


