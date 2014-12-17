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
	screen = SDL_SetVideoMode(640, 200, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}

  	/* Init TTF */
	if(sge_TTF_Init()!=0){fprintf(stderr,"TT error: %s\n", SDL_GetError());exit(1);}
	sge_TTFont *font;
	font=sge_TTF_OpenFont("font.ttf", 25);
	if(font==NULL){fprintf(stderr,"TT error: %s\n", SDL_GetError());exit(1);}

	//sge_TTF_SetFontStyle(font, SGE_TTF_ITALIC);

	/* Make a nice background */
	int max = int(1.5*sge_TTF_FontHeight(font)+5);
	Uint32 *col=new Uint32[max];
	sge_Fader(screen, 100,255,100, 0,0,0, col,0, (int)(1.5*sge_TTF_FontHeight(font)+4));
	sge_HLine(screen,0, 640, 100-sge_TTF_FontHeight(font), 255,0,0);
	sge_HLine(screen,0, 640, 100+(int)(sge_TTF_FontHeight(font)/2), 255,0,0);	
	int i=0;
	for(int y=101-sge_TTF_FontHeight(font); y<100+(int)(sge_TTF_FontHeight(font)/2); y++){
		if(i<max)
			sge_HLine(screen,0, 640, y, col[i++]);
	}
	delete[] col;
	sge_HLine(screen,0, 640, 100, 255,0,0);

	int quit=0;
	char string[52];
	strcpy(string,"Edit Me!"); //The default text

	//Keyrepeat
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL+50);
	
	/* Ok, let the user edit the text */
 	sge_TTF_AA_Alpha();
	if(sge_tt_input(screen,font,string,SGE_IBG|SGE_INOKR, 1,50, 10,100, 0,0,255, 0,0,0, 160)<0){ quit=1; };
	
	if(quit==0){
		printf("%s\n",string); //print the text
	}	

	/* Clean up */
	sge_TTF_CloseFont(font);	

	/* Main loop */
	SDL_Event event;
	while(quit==0){
		/* Check events */
		SDL_WaitEvent(&event);
		if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){quit=1;}
		if(event.type==SDL_QUIT){quit=1;}
	}

	return 0;
}


