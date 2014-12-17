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
	SDL_WM_SetCaption("Rotating Tux. Press [Space]/[s]!", "tux");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(300, 300, 0, SDL_SWSURFACE);
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
	texture = SDL_DisplayFormat(temp);  //Convert it to the screen format for speed
	SDL_FreeSurface(temp);

	//SDL_Rect r;
	//r.x=40; r.y=40; r.w=40; r.h=40;
	//SDL_SetClipRect(texture, &r);

	Uint32 black = SDL_MapRGB(screen->format,0,0,0);
	
	sge_Update_OFF();
	
	SDL_Event event;
	int loops=0;
	Uint32 start=SDL_GetTicks();
	
	/* Starting values*/
	int angle=1;
	Uint8 flag=SGE_TTMAP;
	float xscale=3.0,yscale=3.0,xstep=-0.02, ystep=-0.01;
	SDL_Rect new_rect, old_rect, tmp_rect;
	old_rect.x=0; old_rect.y=0; old_rect.w=screen->w; old_rect.h=screen->h;
	bool delay=false;
	
	/* Main loop */
	do{
		if(delay)
			SDL_Delay(20);
	
		sge_FilledRect(screen, old_rect.x, old_rect.y, old_rect.x + old_rect.w-1, old_rect.y + old_rect.h-1, black);
		new_rect = sge_transform(texture, screen, angle, xscale, yscale, texture->w/2,texture->h/2, 150,150, flag);
		
		sge_RectUnion(tmp_rect, old_rect, new_rect);
		
		sge_Update_ON();
		sge_UpdateRect(screen,tmp_rect.x, tmp_rect.y, tmp_rect.w, tmp_rect.h);
		sge_Update_OFF();
		
		old_rect = new_rect;
		
		/* Change degree and scale for next loop */
		angle+=2;
		yscale+=ystep;
		xscale+=xstep;
		if(angle>=360)
			angle=1;
		if(yscale>3.5 || yscale<-3.5)
			ystep=-ystep;
		if(xscale>3.5 || xscale<0)
			xstep=-xstep;


		loops++;
		
		/* Check events */
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_SPACE){
				/* Change rendering method and report FPS for previous */
			
				if(flag&SGE_TTMAP){
					flag = 0;
					printf("Texture mapping: %.2f FPS.\n",((double)loops*1000)/(SDL_GetTicks()-start));
				}else if(flag==0){
					flag = SGE_TAA;
					printf("Normal rendering: %.2f FPS.\n",((double)loops*1000)/(SDL_GetTicks()-start));
				}else{
					flag=SGE_TTMAP;
					printf("AA rendering: %.2f FPS.\n",((double)loops*1000)/(SDL_GetTicks()-start));
				}

				loops=0;
				start=SDL_GetTicks();
			}
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_s){
				if(delay)
					delay=false;
				else
					delay=true;
					
				/* reset FPS */
				loops=0;
				start=SDL_GetTicks();
			}
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
			if(event.type==SDL_QUIT){break;}
		}
	}while(true);
	
	//Clean up
	SDL_FreeSurface(texture);
	
	//Show FPS
	printf("%.2f FPS.\n",((double)loops*1000)/(SDL_GetTicks()-start));
	
	return 0;
}

