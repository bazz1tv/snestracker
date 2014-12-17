#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "sge.h"

int main(int argc, char** argv)
{
	SDL_Surface *screen;
	SDL_Event event;
	sge_TTFont *font;
	
	/* Init SDL */
	if ( SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't load SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* Clean up on exit */
	atexit(SDL_Quit);

	/* Set window title */
	SDL_WM_SetCaption("SGE Basics", "basics");

	/* Initialize the display */
	screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}


	/* Draws some primitives on screen */
	sge_HLine(screen,0,640,100, SDL_MapRGB(screen->format,255,0,0));
	sge_VLine(screen,100,0,480, SDL_MapRGB(screen->format,255,255,0));
	sge_Line(screen,0,0,640,480, SDL_MapRGB(screen->format,0,0,255));
	sge_Rect(screen,150,150,250,250, SDL_MapRGB(screen->format,255,255,255));
	sge_Rect(screen,260,260,360,360, SDL_MapRGB(screen->format,255,255,255));
	sge_FilledRect(screen,260,150,360,250, SDL_MapRGB(screen->format,255,0,255));
	sge_AACircle(screen,200,200,150, SDL_MapRGB(screen->format,0,255,0));
	sge_AAFilledCircle(screen,500,150,50, SDL_MapRGB(screen->format,255,255,0));
	sge_AAFilledEllipse(screen,500,280, 80,50, SDL_MapRGB(screen->format, 100,150,200)); 

	/* Draws TT text on screen */
	if(sge_TTF_Init()!=0){fprintf(stderr,"TT error: %s\n",SDL_GetError());exit(1);}
	font=sge_TTF_OpenFont("font.ttf", 50);
	if(font==NULL){fprintf(stderr,"TT error: %s\n",SDL_GetError());exit(1);} 
	
	/* Draws TT text on screen */
	sge_tt_textout(screen,font,"SGE", 30,100,  SDL_MapRGB(screen->format,0,0,255),  SDL_MapRGB(screen->format,0,0,0), 120); 
	
	/* Change font style and size */
	sge_TTF_SetFontSize(font, 30);
	sge_TTF_SetFontStyle(font, SGE_TTF_ITALIC|SGE_TTF_UNDERLINE);
	
	sge_tt_textout(screen,font,"Hello World!",400,470, SDL_MapRGB(screen->format,155,200,100),  SDL_MapRGB(screen->format,0,0,0), 255); 	 
	sge_TTF_CloseFont(font);

	/* Main loop */
	do{
		/* Wait for user input */
		SDL_WaitEvent(&event);
		if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
		if(event.type==SDL_QUIT){break;}
	}while(1);

	return 0;
}
