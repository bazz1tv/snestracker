#include "SDL.h"
#include "sge.h"
#include <stdio.h>
#include <stdlib.h>

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
	SDL_WM_SetCaption("Input Deluxe", "input deluxe");
	
	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(600, 400, 0, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
	
	sge_Update_OFF();
	SDL_EnableUNICODE(1); //This is VERY important!!
	
	//Init our text sprites
	sge_TextSprite text(screen,"Edit Me!",20,90);
	text.show_cursor(true);
	sge_TextSprite text2(screen,"Hello World!",100,25);
	text.show_cursor(true);
	
	//Set and draw the border box
	SDL_Rect r; r.x=1; r.y=81; r.w=598; r.h=318; 
	text.set_border(r);
	sge_Rect(screen,0,80, 599,399, 255,0,0);   
	
	//Open a TT font
	if(sge_TTF_Init()!=0){fprintf(stderr,"TT error: %s\n",SDL_GetError());exit(1);} 	
	sge_TTFont *font;
	font=sge_TTF_OpenFont("font.ttf", 25);
	if(font==NULL){fprintf(stderr,"TT error: %s\n",SDL_GetError());exit(1);}

	text.set_ttFont(font,0,255,0);    //Use our font
	text2.set_ttFont(font,50,50,200);
	text.set_pps(60,60);              //set speed (pixels/second)
	text2.set_pps(-170,0);
	text.border_warp(true);           //WARP!
	text2.border_warp(true);
	
	
	SDL_UpdateRect(screen, 0,0,0,0); //Update whole screen
	
	//Keyrepeat
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL+50);
	
	SDL_Event event;
	do{
		//SDL_Delay(1);
	
		/* Check events */
		if(SDL_PollEvent(&event)==1){
			if(event.type==SDL_QUIT){break;}
			if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
			
			text.check(&event);  //Let our text sprite handle the event (editing the text)
		}
		
		if(text.update()){       //update() calculates the new pos and returns true if we need to redraw
			text.clear(0);       //clear text area
			text.draw();         //draw text at new pos
			sge_Update_ON();     
			text.UpdateRects();  //Update screen (old pos + new pos)
			sge_Update_OFF();
		}	
		if(text2.update()){      //Same thing for the other sprite
			text2.clear(0);
			text2.draw();
			sge_Update_ON();     
			text2.UpdateRects();
			sge_Update_OFF();
		}
		
	}while(true);
	
	
	sge_TTF_CloseFont(font);
	return 0;
}


