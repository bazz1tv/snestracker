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
	SDL_WM_SetCaption("Sprite", "sprite");

	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(640, 400, 16, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}

	//Allocate some buffers for the sprites
	SDL_Surface *img1,*img2, *img3, *img4;
	img1=SDL_AllocSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,80,80,16, 0,0,0,0);
 	img2=SDL_AllocSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,80,80,16, 0,0,0,0);
	img3=SDL_AllocSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,80,80,16, 0,0,0,0);
	img4=SDL_AllocSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,80,80,16, 0,0,0,0);	
	
	//Sprite 1 - frame 1
	sge_ClearSurface(img1,0);
	sge_FilledCircle(img1,40,40,39, 150,200,50);
	SDL_SetColorKey(img1,SDL_SRCCOLORKEY|SDL_RLEACCEL,0);

	//Sprite 2 - frame 1
	sge_ClearSurface(img2,0);
	sge_FilledCircle(img2,40,40,39, 50,90,210);
	SDL_SetColorKey(img2,SDL_SRCCOLORKEY|SDL_RLEACCEL,0);

	//Sprite 1 - frame 2
	sge_ClearSurface(img3,0);
	sge_FilledRect(img3,0,0,80,80, 150,200,50);	
	
	//Sprite 1 - frame 3
	sge_ClearSurface(img4,0);
	sge_FilledRect(img4,0,0,80,80, 50,90,210);

	//Init sprite 1
	sge_sprite sprite1(screen,img1,100,100);
	sprite1.add_frame(img3);  //Add frame 2
	sprite1.add_frame(img4);  //Add frame 3
	//sprite1.set_seq(1,3);     //Set the playing sequence
	sprite1.set_pps(-140, 0); //Set speed
	sprite1.set_fps(2);       //Set fps

	//Init sprite 2
	sge_sprite sprite2(screen,img2,100,200);
	sprite2.set_pps(350,350);
	

 	// If the delay between two PollEvent is greater than 100 and all events are added to the
	// event queue, the queue will grow *fast*. You're program will have no change to catch
	// up. You *must* try to avoid adding events unnecessarily - mousemotions for example.
	SDL_EventState(SDL_MOUSEMOTION,SDL_IGNORE);

	sge_Update_OFF();
	
	/* Sets start time */
	Uint32 start=SDL_GetTicks();
	Uint32 time=start;
	
	int lps=0;

	/* Main loop */
	SDL_Event event;
	int quit=0;
	bool upd1,upd2;
	do{
		//SDL_Delay(1);	//Uncomment to see how the sprite class handles delays
		time=SDL_GetTicks();
		
		//Calculate sprite data
		upd1=sprite1.update(time);
		upd2=sprite2.update(time);
		
		if(upd1 || upd2){   //Redraw needed?
			sprite1.clear(0); //Clear the prev. drawed
			sprite2.clear(0);
			
			sprite1.draw();   //Draw the new
			sprite2.draw();
			
			sge_Update_ON();
			sprite1.UpdateRects(); //Update the correct area on screen
			sprite2.UpdateRects();
			sge_Update_OFF();
		}
	
		lps++;
		
		/* Check events */
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){quit=1;}
			if(event.type==SDL_QUIT){quit=1;}
		}

	}while(quit==0);

	printf("%d LPS.\n",int((lps*1000.0)/(SDL_GetTicks()-start)));

	//Clean upp
	SDL_FreeSurface(img1);
	SDL_FreeSurface(img2);
	SDL_FreeSurface(img3);
	SDL_FreeSurface(img4);

	return 0;
}

