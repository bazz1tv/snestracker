#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
	SDL_WM_SetCaption("Blib test", "blib");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(700, 450, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}	
	
	/* Load the texture */
	SDL_Surface *temp,*texture;
	temp = SDL_LoadBMP("tux.bmp");
	if ( temp == NULL ) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		exit(1);
	}
	texture = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	sge_Update_OFF();
	
	sge_ClearSurface(screen,100,100,100);
	
	/* Draw some triangles*/
	Uint32 colour;
	int i;
	for ( i = 0; i < 11; i++ ) {
		colour = SDL_MapRGB(screen->format,0,0,25 * i);
		sge_Trigon(screen,        30+60*i,  50, (Sint16)(30+60*i + 40*cos(3.14/180*i*36)), (Sint16)( 50-sin(3.14/180*i*36)*40), (Sint16)(30+60*i+sin(3.14/180*i*36)*40), (Sint16)( 50 + 40*cos(3.14/180*i*36)), colour);
		sge_FilledTrigon(screen,  30+60*i, 150, (Sint16)(30+60*i + 40*cos(3.14/180*i*36)), (Sint16)(150-sin(3.14/180*i*36)*40), (Sint16)(30+60*i+sin(3.14/180*i*36)*40), (Sint16)(150 + 40*cos(3.14/180*i*36)), colour);
		sge_FadedTrigon(screen,   30+60*i, 250, (Sint16)(30+60*i + 40*cos(3.14/180*i*36)), (Sint16)(250-sin(3.14/180*i*36)*40), (Sint16)(30+60*i+sin(3.14/180*i*36)*40), (Sint16)(250 + 40*cos(3.14/180*i*36)), SDL_MapRGB(screen->format,255,0,0),SDL_MapRGB(screen->format,0,255,0),SDL_MapRGB(screen->format,0,0,255));
		sge_TexturedTrigon(screen,30+60*i, 350, (Sint16)(30+60*i + 40*cos(3.14/180*i*36)), (Sint16)(350-sin(3.14/180*i*36)*40), (Sint16)(30+60*i+sin(3.14/180*i*36)*40), (Sint16)(350 + 40*cos(3.14/180*i*36)), texture,0,0,0,texture->h-1,texture->w-1,0);
	}
	
	/* And some lines */
	for ( i = 0; i < 10; i++)
		sge_FadedLine(screen,300,400,400+i,255,0,255,0,255,127);
	for ( i = 0; i < 10; i++)
		sge_TexturedLine(screen,100,200,400+i,texture,0,i,texture->w-1,texture->h/2+i);
	
	SDL_UpdateRect(screen,0,0,0,0);

	/* Main loop */
	SDL_Event event;
	do{
		/* Check events */
		SDL_WaitEvent(&event);
		if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
		if(event.type==SDL_QUIT){break;}
	}while(true);
	
	//Clean up
	SDL_FreeSurface(texture);
	
	return 0;
}
