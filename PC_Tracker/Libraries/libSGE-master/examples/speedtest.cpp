#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "SDL.h"
#include "sge.h"

#define NUM_RANDOM	1000

using namespace std;

/* Bitmap fonts */
sge_bmpFont *font;
sge_bmpFont *cfont;

/* The texture */
SDL_Surface *texture;

/* Coordinates */
Sint16 xl1[NUM_RANDOM], xl2[NUM_RANDOM], y1[NUM_RANDOM], y2[NUM_RANDOM];
Sint16 xl3[NUM_RANDOM], xl4[NUM_RANDOM], y3[NUM_RANDOM], y4[NUM_RANDOM];

Sint16 xr1[NUM_RANDOM], xr2[NUM_RANDOM]; //xl - left, xr -right coord
Sint16 xr3[NUM_RANDOM], xr4[NUM_RANDOM];

/* Radii and offsets */
Sint16 r1[NUM_RANDOM], r2[NUM_RANDOM];

/* Colors and alpha */
Uint32 col1[NUM_RANDOM], col2[NUM_RANDOM], col3[NUM_RANDOM];
Uint8 r[NUM_RANDOM], g[NUM_RANDOM], b[NUM_RANDOM];
Uint8 a[NUM_RANDOM];


/* Clear and redraw left */
void init_screen_left(SDL_Surface *screen)
{
	SDL_Rect r;
	r.x=0; r.y=0; r.w=screen->w; r.h=screen->h;
	SDL_SetClipRect(screen, &r);

	sge_ClearSurface(screen,SDL_MapRGB(screen->format,0,0,0));

	sge_BF_textoutf(screen, font, 5, 2, "%s","SGE drawing primitives performance test - LMB for next test, RMB for previous.");

	sge_HLine(screen,0, screen->w, 16, 255,255,255);
	sge_HLine(screen,0, screen->w, 35, 255,255,255);
	sge_VLine(screen, screen->w/2, 16,screen->h, 255,255,255);


	if (SDL_MUSTLOCK(screen))
		if (SDL_LockSurface(screen) < 0)
			return;
	
	/* Draw the colored area */
	Sint16 x,y;
	float stepx, stepy, fx, fy, fxy;
	stepx=1.0/(screen->w/2-screen->w/8);
	stepy=1.0/(screen->h-36 - (screen->h-36)/4);
	fx=0.0;
	for (x=0; x<screen->w/2; x++) {
		fy=0.0;
		for (y=36; y<screen->w; y++) {
			fxy=1.0-fx*fy;
			_PutPixel(screen,x,y,(int)(128.0*fx*fx),(int)(128.0*fxy*fxy),(int)(128.0*fy*fy));
			fy += stepy;
		}
		fx += stepx;
	}
	
	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}

	SDL_UpdateRect(screen,0,0,0,0);
	r.x=0; r.y=37; r.w=screen->w/2; r.h=screen->h-37;
	SDL_SetClipRect(screen, &r);
}


/* Clear and redraw right */
void init_screen_right(SDL_Surface *screen)
{
	SDL_Rect r;
	r.x=0; r.y=0; r.w=screen->w; r.h=screen->h;
	SDL_SetClipRect(screen, &r);

	if (SDL_MUSTLOCK(screen))
		if (SDL_LockSurface(screen) < 0)
			return;
	
	/* Draw the colored area */
	Sint16 x,y;
	float stepx, stepy, fx, fy, fxy;
	stepx=1.0/(screen->w/2-screen->w/8);
	stepy=1.0/(screen->h-36 - (screen->h-36)/4);
	fx=0.0;
	for (x=screen->w/2+1; x<screen->w; x++) {
		fy=0.0;
		for (y=36; y<screen->w; y++) {
			fxy=1.0-fx*fy;
			_PutPixel(screen,x,y,(int)(128.0*fx*fx),(int)(128.0*fxy*fxy),(int)(128.0*fy*fy));
			fy += stepy;
		}
		fx += stepx;
	}
	
	
	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	
	SDL_UpdateRect(screen,0,0,0,0);
	r.x=screen->w/2+1; r.y=37; r.w=screen->w/2; r.h=screen->h-37;
	SDL_SetClipRect(screen, &r);
}


/* Macro to test 'function' in the left part of the screen */
#define RUN_TEST_LEFT(screen,title,testloops,showloops,lock,function)\
{\
	SDL_Rect rect;\
	rect.x=0; rect.y=37; rect.w=screen->w/2; rect.h=screen->h-37;\
	SDL_SetClipRect(screen, &rect);\
	\
	if (SDL_MUSTLOCK(screen) && lock){\
		sge_Lock_OFF();\
		if (SDL_LockSurface(screen) < 0)\
			exit(-1);\
	}else\
		sge_Lock_ON();\
	\
	int i=0;\
	Uint32 start = SDL_GetTicks();\
	for (int j=0; j<testloops; j++) {\
		i=j%(NUM_RANDOM-1);\
  		function;\
	}\
	Uint32 stop = SDL_GetTicks();\
	\
	if (SDL_MUSTLOCK(screen) && lock) {\
		SDL_UnlockSurface(screen);\
	}\
	sge_Lock_ON();\
	\
	init_screen_left(screen);\
	\
	if (SDL_MUSTLOCK(screen) && lock){\
		sge_Lock_OFF();\
		if (SDL_LockSurface(screen) < 0)\
			exit(-1);\
	}else\
		sge_Lock_ON();\
	\
	for (int j=0; j<showloops; j++) {\
		i=j%(NUM_RANDOM-1);\
  		function;\
	}\
	\
	if (SDL_MUSTLOCK(screen) && lock) {\
		SDL_UnlockSurface(screen);\
	}\
	sge_Lock_ON();\
	\
	\
	rect.x=0; rect.y=0; rect.w=screen->w; rect.h=screen->h;\
	SDL_SetClipRect(screen, &rect);\
	sge_BF_textoutf(screen, font, 5, 20, "%s - %.0f/s.",title,((double)testloops*1000)/(stop-start));\
	printf("%s - %.0f/s.\n",title,((double)testloops*1000)/(stop-start));\
	SDL_UpdateRect(screen,0,0,0,0);\
}


/* Macro to test 'function' in the right part of the screen */
#define RUN_TEST_RIGHT(screen,title,testloops,showloops,lock,function)\
{\
	SDL_Rect rect;\
	rect.x=screen->w/2+1; rect.y=37; rect.w=screen->w/2; rect.h=screen->h-37;\
	SDL_SetClipRect(screen, &rect);\
	\
	if (SDL_MUSTLOCK(screen) && lock){\
		sge_Lock_OFF();\
		if (SDL_LockSurface(screen) < 0)\
			exit(-1);\
	}else\
		sge_Lock_ON();\
	\
	int i=0;\
	Uint32 start = SDL_GetTicks();\
	for (int j=0; j<testloops; j++) {\
		i=j%(NUM_RANDOM-1);\
  		function;\
	}\
	Uint32 stop = SDL_GetTicks();\
	\
	if (SDL_MUSTLOCK(screen) && lock) {\
		SDL_UnlockSurface(screen);\
	}\
	sge_Lock_ON();\
	\
	init_screen_right(screen);\
	\
	if (SDL_MUSTLOCK(screen) && lock){\
		sge_Lock_OFF();\
		if (SDL_LockSurface(screen) < 0)\
			exit(-1);\
	}else\
		sge_Lock_ON();\
	\
	for (int j=0; j<showloops; j++) {\
		i=j%(NUM_RANDOM-1);\
  		function;\
	}\
	\
	if (SDL_MUSTLOCK(screen) && lock) {\
		SDL_UnlockSurface(screen);\
	}\
	sge_Lock_ON();\
	\
	\
	rect.x=0; rect.y=0; rect.w=screen->w; rect.h=screen->h;\
	SDL_SetClipRect(screen, &rect);\
	sge_BF_textoutf(screen, font, screen->w/2+6, 20, "%s - %.0f/s.",title,((double)testloops*1000)/(stop-start));\
	printf("%s - %.0f/s.\n",title,((double)testloops*1000)/(stop-start));\
	SDL_UpdateRect(screen,0,0,0,0);\
}


/* Setup random data */
void rand_data(SDL_Surface *screen)
{
	Uint8 R,G,B;
	for (int i=0; i<NUM_RANDOM; i++) {
		xl1[i]=sge_Random(0,screen->w/2-1);
		xl2[i]=sge_Random(0,screen->w/2-1);
		xl3[i]=sge_Random(0,screen->w/2-1);
		xl4[i]=sge_Random(0,screen->w/2-1);
		y1[i]=sge_Random(37,screen->h);
		y2[i]=sge_Random(37,screen->h);
		y3[i]=sge_Random(37,screen->h);
		y4[i]=sge_Random(37,screen->h);
		
		xr1[i]=screen->w/2 + 1 + xl1[i];
		xr2[i]=screen->w/2 + 1 + xl2[i];
		xr3[i]=screen->w/2 + 1 + xl3[i];
		xr4[i]=screen->w/2 + 1 + xl4[i];
		
		r1[i]=sge_Random(5,60);
		r2[i]=sge_Random(5,60);
		R=sge_Random(0,255);
		G=sge_Random(0,255);
		B=sge_Random(0,255);
		col1[i]=SDL_MapRGB(screen->format,R,G,B);
		R=sge_Random(0,255);
		G=sge_Random(0,255);
		B=sge_Random(0,255);
		col2[i]=SDL_MapRGB(screen->format,R,G,B);
		R=sge_Random(0,255);
		G=sge_Random(0,255);
		B=sge_Random(0,255);
		col3[i]=SDL_MapRGB(screen->format,R,G,B);
		r[i]=R;
		g[i]=G;
		b[i]=B;
		a[i]=sge_Random(40,240);
 	} 
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
	SDL_WM_SetCaption("SpeedTest", "speedtest");

	/*  Handle arguments */
	int video_bpp = 0;
	Uint32 videoflags = SDL_SWSURFACE;
	while ( argc > 1 ) {
		--argc;
		if ( string(argv[argc-1]) == "-bpp" ) {
			video_bpp = atoi(argv[argc]);
			--argc;
		} else
		if ( string(argv[argc]) == "-hw" )
			videoflags |= SDL_HWSURFACE;
		else
		if ( string(argv[argc]) == "-fullscreen" )
			videoflags |= SDL_FULLSCREEN;
		else {
			fprintf(stderr,"Usage: %s [-bpp N] [-hw] [-fullscreen]\n",argv[0]);
			exit(1);
		}
	}
	
	/* Initialize the display */
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(640, 480, video_bpp, videoflags);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Open bitmap fonts, optimized for blits or color change */
	font=sge_BF_OpenFont("font.bmp",SGE_BFTRANSP);
	if(font==NULL){fprintf(stderr,"Error: %s\n", SDL_GetError());exit(1);}
	cfont=sge_BF_OpenFont("font.bmp",SGE_BFTRANSP|SGE_BFPALETTE);
	if(cfont==NULL){fprintf(stderr,"Error: %s\n", SDL_GetError());exit(1);}
	
	/* Load the texture */
	SDL_Surface *temp;
	temp = SDL_LoadBMP("tux.bmp");
	if ( temp == NULL ) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		exit(1);
	}
	texture = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	sge_Update_OFF();
	sge_Lock_OFF();

	sge_Randomize();
	rand_data(screen);

	SDL_Delay(100);  /* Time for the system to calm down */
	
	/* Main loop */
	SDL_Event event;
	int op = 1;
	int next_op = 1;
	
	do{
		switch ( op ) {
			case 1:
				RUN_TEST_LEFT(screen,"Pixels",500000,1000,true,_PutPixel(screen,xl1[i],y1[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Pixels (alpha)",300000,1000,true,_PutPixelAlpha(screen,xr1[i],y1[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 2:
				RUN_TEST_LEFT(screen,"Horizontal lines",150000,200,false,sge_HLine(screen,xl1[i],xl2[i],y1[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Horizontal lines (alpha)",15000,200,true,sge_HLineAlpha(screen,xr1[i],xr2[i],y1[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 3:
				RUN_TEST_LEFT(screen,"Vertical lines",7000,200,false,sge_VLine(screen,xl1[i],y1[i],y2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Vertical lines (alpha)",6000,200,true,sge_VLineAlpha(screen,xr1[i],y1[i],y2[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 4:
				RUN_TEST_LEFT(screen,"Normal lines",7000,200,true,sge_Line(screen,xl1[i],y1[i],xl2[i],y2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Normal lines (alpha)",4000,200,true,sge_LineAlpha(screen,xr1[i],y1[i],xr2[i],y2[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 5:
				RUN_TEST_LEFT(screen,"Anti-aliased lines",1000,200,true,sge_AALine(screen,xl1[i],y1[i],xl2[i],y2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Anti-aliased lines (alpha)",1000,200,true,sge_AALineAlpha(screen,xr1[i],y1[i],xr2[i],y2[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 6:
				RUN_TEST_LEFT(screen,"Rectangles",6000,100,false,sge_Rect(screen,xl1[i],y1[i],xl2[i],y2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Rectangles (alpha)",2000,100,true,sge_RectAlpha(screen,xr1[i],y1[i],xr2[i],y2[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 7:
				RUN_TEST_LEFT(screen,"Filled rectangles",2000,100,false,sge_FilledRect(screen,xl1[i],y1[i],xl2[i],y2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Filled rectangles (alpha)",100,100,true,sge_FilledRectAlpha(screen,xr1[i],y1[i],xr2[i],y2[i],col1[i],a[i]));
				op = 0;
				break;
			
			case 8:
				RUN_TEST_LEFT(screen,"Circles",7000,100,true,sge_Circle(screen,xl1[i],y1[i],r1[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Circles (alpha)",4000,100,true,sge_CircleAlpha(screen,xr1[i],y1[i],r1[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 9:
				RUN_TEST_LEFT(screen,"AA circles",3000,100,true,sge_AACircle(screen,xl1[i],y1[i],r1[i],col1[i]));
				RUN_TEST_RIGHT(screen,"AA circles (alpha)",1000,100,true,sge_AACircleAlpha(screen,xr1[i],y1[i],r1[i],col1[i],a[i]));
				op = 0;
				break;	
			
			case 10:
				RUN_TEST_LEFT(screen,"Filled circles",1000,100,false,sge_FilledCircle(screen,xl1[i],y1[i],r1[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Filled circles (alpha)",500,100,true,sge_FilledCircleAlpha(screen,xr1[i],y1[i],r1[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 11:
				RUN_TEST_LEFT(screen,"Ellipses",4000,100,true,sge_Ellipse(screen,xl1[i],y1[i],r1[i],r2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Ellipses (alpha)",3000,100,true,sge_EllipseAlpha(screen,xr1[i],y1[i],r1[i],r2[i],col1[i],a[i]));
				op = 0;
				break;
			
			case 12:
				RUN_TEST_LEFT(screen,"AA ellipses",1000,100,true,sge_AAEllipse(screen,xl1[i],y1[i],r1[i],r2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"AA ellipses (alpha)",1000,100,true,sge_AAEllipseAlpha(screen,xr1[i],y1[i],r1[i],r2[i],col1[i],a[i]));
				op = 0;
				break;	
				
			case 13:
				RUN_TEST_LEFT(screen,"Filled ellipses",1000,100,false,sge_FilledEllipse(screen,xl1[i],y1[i],r1[i],r2[i],col1[i]));
				RUN_TEST_RIGHT(screen,"Filled ellipses (alpha)",500,100,true,sge_FilledEllipseAlpha(screen,xr1[i],y1[i],r1[i],r2[i],col1[i],a[i]));
				op = 0;
				break;
				
			case 14:
				RUN_TEST_LEFT(screen,"AA filled circles",500,100,false,sge_AAFilledCircle(screen,xl1[i],y1[i],r1[i],col1[i]));
				RUN_TEST_RIGHT(screen,"AA filled ellipses",500,100,false,sge_AAFilledEllipse(screen,xr1[i],y1[i],r1[i],r2[i],col1[i]));
				op = 0;
				break;
				
			case 15:
				RUN_TEST_LEFT(screen,"Bezier curves",1500,50,true,sge_Bezier(screen, xl1[i],y1[i],xl2[i],y2[i],xl3[i],y3[i],xl4[i],y4[i], 6, col1[i]));
				RUN_TEST_RIGHT(screen,"Bezier curves (alpha)",800,50,true,sge_BezierAlpha(screen, xr1[i],y1[i],xr2[i],y2[i],xr3[i],y3[i],xr4[i],y4[i], 6, col1[i], a[i]));
				op = 0;
				break;
				
			case 16:
				RUN_TEST_LEFT(screen,"AA bezier curves",800,50,true,sge_AABezier(screen, xl1[i],y1[i],xl2[i],y2[i],xl3[i],y3[i],xl4[i],y4[i], 6, col1[i]));
				RUN_TEST_RIGHT(screen,"AA bezier curves (alpha)",800,50,true,sge_AABezierAlpha(screen, xr1[i],y1[i],xr2[i],y2[i],xr3[i],y3[i],xr4[i],y4[i], 6, col1[i], a[i]));
				op = 0;
				break;
				
			case 17:
				RUN_TEST_LEFT(screen,"Triangles",1500,30,true,sge_Trigon(screen, xl1[i],y1[i],xl2[i],y2[i],xl3[i],y3[i], col1[i]));
				RUN_TEST_RIGHT(screen,"Triangles (alpha)",1000,30,true,sge_TrigonAlpha(screen, xr1[i],y1[i],xr2[i],y2[i],xr3[i],y3[i], col1[i], a[i]));
				op = 0;
				break;
			
			case 18:
				RUN_TEST_LEFT(screen,"AA triangles",500,30,true,sge_AATrigon(screen, xl1[i],y1[i],xl2[i],y2[i],xl3[i],y3[i], col1[i]));
				RUN_TEST_RIGHT(screen,"AA triangles (alpha)",500,30,true,sge_AATrigonAlpha(screen, xr1[i],y1[i],xr2[i],y2[i],xr3[i],y3[i], col1[i], a[i]));
				op = 0;
				break;
				
			case 19:
				RUN_TEST_LEFT(screen,"Filled triangles",800,30,false,sge_FilledTrigon(screen, xl1[i],y1[i],xl2[i],y2[i],xl3[i],y3[i], col1[i]));
				RUN_TEST_RIGHT(screen,"Filled triangles (alpha)",100,30,true,sge_FilledTrigonAlpha(screen, xr1[i],y1[i],xr2[i],y2[i],xr3[i],y3[i], col1[i], a[i]));
				op = 0;
				break;
				
			case 20:
				RUN_TEST_LEFT(screen,"Gourand shaded triangles",400,20,true,sge_FadedTrigon(screen, xl1[i],y1[i],xl2[i],y2[i],xl3[i],y3[i], col1[i],col2[i],col3[i]));
				
				if (SDL_MUSTLOCK(texture))
					if (SDL_LockSurface(texture) < 0)
						break;
				RUN_TEST_RIGHT(screen,"Texture mapped triangles",500,20,true,sge_TexturedTrigon(screen, xr1[i],y1[i],xr2[i],y2[i],xr3[i],y3[i], texture,0,0,0,texture->h-1,texture->w-1,0));
				if (SDL_MUSTLOCK(texture))
					SDL_UnlockSurface(texture);
				
				op = 0;
				break;
				
			case 21:
				RUN_TEST_LEFT(screen,"Bitmap text",4000,100,false,sge_BF_textout(screen, font, "Hello World!", xl1[i], y1[i]));
				RUN_TEST_RIGHT(screen,"Colored bitmap text",4000,100,false,sge_BF_SetColor(cfont,r[i],g[i],b[i]);sge_BF_textout(screen, cfont, "Hello World!", xr1[i], y1[i]));
				op = 0;
				break;
		}
		
		/* Wait for user input */
		SDL_WaitEvent(&event);
		if(event.type==SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
			/* Goto next test */
			op = ++next_op;
			if(op > 21){
				op=1;
				next_op=1;
				rand_data(screen);
			}
		}
		if(event.type==SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
			/* Goto previous test */
			op = --next_op;
			if(op < 1){
				op=21;
				next_op=21;
				rand_data(screen);
			}
		}
		if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
		if(event.type==SDL_QUIT){break;}
	}while(true);

	/* Clean up */
	sge_BF_CloseFont(font);
	sge_BF_CloseFont(cfont);
	SDL_FreeSurface(texture);
	
	return 0;
}
