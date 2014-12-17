/*
*	SDL Graphics Extension
*	SGE shape
*
*	Started 000430
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)2000-2003 Anders Lindström
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#include "SDL.h"
#include <stdio.h>
#include <math.h>
#include "sge_surface.h"
#include "sge_primitives.h"
#include "sge_shape.h"
#include "sge_misc.h"

#ifndef _SGE_NO_CLASSES

using namespace std;


sge_screen *the_screen=NULL;  //The pointer to the active screen class (or NULL)

//==================================================================================
// sge_screen
//==================================================================================
sge_screen::sge_screen(SDL_Surface *screen)
{
	sge_screen::screen=screen;  //Our screen pointer
	
	/* Test some flags */
	HW=(((screen->flags) & SDL_HWSURFACE) != 0);	
	DB=(((screen->flags) & SDL_DOUBLEBUF) != 0);
	FS=(((screen->flags) & SDL_FULLSCREEN) != 0);
	
	/* Test the resolution of SDL_Delay() */
	//sge_CalibrateDelay();

	/* Register us as the screen class */
	the_screen=this;

	#ifdef sge_debug
	if(HW)
		printf("Screen surface is in video memory (");
	else
		printf("Screen surface is in system memory (");	
	if(DB)
		printf("double-buffered ");
	else
		printf("single-buffered ");
	if(FS)
		printf("fullscreen mode).\n");
	else
		printf("window mode).\n");
	
	//printf("The resolution of SDL_Delay() is %d ms.\n",sge_DelayRes());
	#endif
}

void sge_screen::add_rect(SDL_Rect rect)
{
	if(!(HW||DB)){
		/* Corrects the coords */
		if(rect.x>=screen->w || rect.y>=screen->h){return;}
		Sint16 a=rect.w,b=rect.h;
		if(rect.x < 0){rect.x=0;}
		if(rect.y < 0){rect.y=0;}
		if(a+rect.x > screen->w)
			a=screen->w-rect.x;
		if(b+rect.y > screen->h)
			b=screen->h-rect.y;
	
		rect.w=a; rect.h=b;
		
		/* Put the rectangle last in the list */
		rects.push_back(rect);		
	}
}

void sge_screen::add_rect(Sint16 x, Sint16 y, Uint32 w, Uint32 h)
{
	if(!(HW||DB)){
		SDL_Rect rect; rect.x=x; rect.y=y; rect.w=w; rect.h=h;
		add_rect(rect);
	}
}

void sge_screen::add_shape(sge_shape *shape)
{
	shapes.push_back(shape);	
}

void sge_screen::add_shape_p(sge_shape *shape)
{
	shapes_p.push_back(shape);	
}

void sge_screen::remove_shape_p(sge_shape *shape)
{
	shapes_p.remove(shape);	
}

void sge_screen::clear_all(void)
{
	shapes.clear();
	shapes_p.clear();
	rects.clear();	
}

void sge_screen::update(void)
{
	SI i;

	//Draw shapes in list
	for(i=shapes.begin(); i != shapes.end(); i++){
		(*i)->draw();
		(*i)->UpdateRects(); //Adds rectangles with add_rect() automaticly
	}
	shapes.clear();
	
	//Draw permanent shapes in list
	for(i=shapes_p.begin(); i != shapes_p.end(); i++){
		(*i)->draw();
		(*i)->UpdateRects(); //Adds rectangles with add_rect() automaticly
	}

	//Updates the list of rectangles on screen
	if(!(HW||DB)){
		int j=0;

		SDL_Rect *r=new SDL_Rect[rects.size()];  //ANSI C++

		/* Copy every element in the linked list to the array */
		for(RI i=rects.begin(); i != rects.end(); i++){
			r[j++]=*i;	
		}
		
		SDL_UpdateRects(screen,rects.size(), r); //Let SDL update the rectangles
		
		delete[] r;

		rects.clear(); //Empty the list
	}
	else if(DB)   //double-buffered
		SDL_Flip(screen);
}


//==================================================================================
// sge_surface (derived from sge_shape)
// A class for moving/blitting surfaces
//==================================================================================
sge_surface::sge_surface(SDL_Surface *dest, SDL_Surface *src, Sint16 x, Sint16 y)
{
	surface=src;
	sge_surface::dest=dest;
	
	current_pos.x=x;current_pos.y=y;current_pos.w=src->w;current_pos.h=src->h;
	last_pos.x=last_pos.y=last_pos.w=last_pos.h=0;
	prev_pos=last_pos;
	
	border.x=border.y=0;border.w=dest->w;border.h=dest->h;
	warp_border=false;
}

sge_surface::~sge_surface(void)
{
	//Nothing for now...
}


bool sge_surface::check_warp(void)
{
	bool flag=false;
	
	if(warp_border){
		if(current_pos.x+current_pos.w<border.x){
			current_pos.x=border.x+border.w-current_pos.w;
			flag=true;
		}else if(current_pos.x>border.x+border.w){
			current_pos.x=border.x;
			flag=true;
		}if(current_pos.y+current_pos.h<border.y){
			current_pos.y=border.y+border.h-current_pos.h;
			flag=true;
		}else if(current_pos.y>border.y+border.h){
			current_pos.y=border.y;
			flag=true;
		}
	}
	return flag;
}

int sge_surface::get_warp(SDL_Rect rec, SDL_Rect &r1, SDL_Rect &r2, SDL_Rect &r3, SDL_Rect &r4)
{
	//We want to decode the pos rectangle into two or four rectangles.

	r1.x = r2.x = r3.x = r4.x = rec.x,  r1.y = r2.y = r3.y = r4.y = rec.y; 
	r1.w = r2.w = r3.w = r4.w = rec.w,  r1.h = r2.h = r3.h = r4.h = rec.h;

	int rects=0;

	if(warp_border){
	
		if(rec.x<border.x){
			r1.w = border.x - rec.x;
			r1.x = border.x + border.w - r1.w;
			r2.w = abs(rec.w - r1.w);           //SDL_Rect w/h is unsigned
			r2.x = border.x;
			rects=2; 
		}else if(rec.x+rec.w > border.x + border.w){
			r1.x = rec.x;
			r1.w = border.x + border.w - rec.x;
			r2.x = border.x;
			r2.w = abs(rec.w - r1.w);
			rects=2;
		}
		
		r3.x = r1.x; r3.w = r1.w;
		r4.x = r2.x; r4.w = r2.w;
		
		if(rec.y<border.y){
			if(rects==0){
				r1.h = border.y - rec.y;
				r1.y = border.y + border.h - r1.h;
				r2.h = abs(rec.h - r1.h);
				r2.y = border.y;
				rects=2;
			}else{
				r2.h = r1.h= border.y - rec.y;
				r2.y = r1.y= border.y + border.h - r1.h;
				r4.h = r3.h= abs(rec.h - r1.h);
				r4.y = r3.y= border.y;
				rects=4;
			}
		}else if(rec.y+rec.h > border.y + border.h){
			if(rects==0){
				r1.y = rec.y;
				r1.h = border.y + border.h - rec.y;
				r2.y = border.y;
				r2.h = abs(rec.h - r1.h);
				rects=2;
			}else{
				r2.y = r1.y = rec.y;
				r2.h = r1.h = border.y + border.h - rec.y;
				r4.y = r3.y = border.y;
				r4.h = r3.h = abs(rec.h - r1.h);
				rects=4;
			}
		}	
	}
	return rects;	
}

void sge_surface::warp_draw(void)
{
	SDL_Rect r1,r2,r3,r4;
	int rects=get_warp(current_pos,r1,r2,r3,r4);

	if(rects==2){
		sge_Blit(surface, dest, 0,0, r1.x, r1.y, r1.w, r1.h);
		sge_Blit(surface, dest, surface->w-r2.w,surface->h-r2.h, r2.x, r2.y, r2.w, r2.h);
	}
	else if(rects==4){
		sge_Blit(surface, dest, 0,0, r1.x, r1.y, r1.w, r1.h);
		sge_Blit(surface, dest, surface->w-r2.w,0, r2.x, r2.y, r2.w, r2.h);
		sge_Blit(surface, dest, 0,surface->h-r3.h, r3.x, r3.y, r3.w, r3.h);
		sge_Blit(surface, dest, surface->w-r4.w,surface->h-r4.h, r4.x, r4.y, r4.w, r4.h);
	}
	else
		sge_Blit(surface, dest, 0,0, current_pos.x, current_pos.y, surface->w, surface->h);
}

void sge_surface::warp_update(SDL_Rect rec)
{
	SDL_Rect r1,r2,r3,r4;
	int rects=get_warp(rec,r1,r2,r3,r4);
	
	if(rects>0){
		if(the_screen){  //Use the screen class?
			the_screen->add_rect(r1.x, r1.y, r1.w, r1.h);
			the_screen->add_rect(r2.x, r2.y, r2.w, r2.h);
			if(rects>2){
				the_screen->add_rect(r3.x, r3.y, r3.w, r3.h);
				the_screen->add_rect(r4.x, r4.y, r4.w, r4.h);
			}
		}else{
			sge_UpdateRect(dest, r1.x, r1.y, r1.w, r1.h);
			sge_UpdateRect(dest, r2.x, r2.y, r2.w, r2.h);
			if(rects>2){
				sge_UpdateRect(dest, r3.x, r3.y, r3.w, r3.h);
				sge_UpdateRect(dest, r4.x, r4.y, r4.w, r4.h);
			}
		}
	}
	else{
		if(the_screen)
			the_screen->add_rect(rec.x, rec.y, rec.w, rec.h);
		else
			sge_UpdateRect(dest, rec.x, rec.y, rec.w, rec.h);
	}
}

void sge_surface::warp_clear(Uint32 color)
{
	SDL_Rect r1,r2,r3,r4;
	int rects=get_warp(last_pos,r1,r2,r3,r4);
	
	if(rects>0){
		sge_FilledRect(dest, r1.x, r1.y, r1.x+r1.w-1, r1.y+r1.h-1, color);
		sge_FilledRect(dest, r2.x, r2.y, r2.x+r2.w-1, r2.y+r2.h-1, color);
		if(rects>2){
			sge_FilledRect(dest, r3.x, r3.y, r3.x+r3.w-1, r3.y+r3.h-1, color);
			sge_FilledRect(dest, r4.x, r4.y, r4.x+r4.w-1, r4.y+r4.h-1, color);
		}
	}
	else
		sge_FilledRect(dest, last_pos.x, last_pos.y, last_pos.x+last_pos.w-1, last_pos.y+last_pos.h-1,color);
}


void sge_surface::warp_clear(SDL_Surface *src, Sint16 srcX, Sint16 srcY)
{
	SDL_Rect r1,r2,r3,r4;
	int rects=get_warp(current_pos,r1,r2,r3,r4);

	if(rects>0){
		sge_Blit(src, dest, r1.x,r1.y, r1.x, r1.y, r1.w, r1.h);
		sge_Blit(src, dest, r2.x,r2.y, r2.x, r2.y, r2.w, r2.h);
		if(rects>2){
			sge_Blit(src, dest, r3.x,r3.y, r3.x, r3.y, r3.w, r3.h);
			sge_Blit(src, dest, r4.x,r4.y, r4.x, r4.y, r4.w, r4.h);
		}
	}
	else
		sge_Blit(src,dest, srcX, srcY, last_pos.x, last_pos.y, last_pos.w, last_pos.h);
}


//Draws the surface
void sge_surface::draw(void)
{
	if(!surface)
		return;
		
	current_pos.w=surface->w;
	current_pos.h=surface->h;

	if(warp_border)
		warp_draw();
	else
		sge_Blit(surface, dest, 0,0, current_pos.x, current_pos.y, surface->w, surface->h);
		
	prev_pos=last_pos;
	last_pos=current_pos;
}

void sge_surface::UpdateRects(void)
{
	Sint16 xoffs=last_pos.x-prev_pos.x, yoffs=last_pos.y-prev_pos.y;

	//if the prev and last area to update is very near
	//it's better to update both with one sge_UpdateRect call

	if(xoffs<0)
		xoffs=~xoffs+1;   //abs(xoffs)
	if(yoffs<0)
		yoffs=~yoffs+1;

	if(xoffs <= prev_pos.w/2 && yoffs <= prev_pos.h/2){
		Sint16 minx, miny, maxx, maxy, w=prev_pos.w+xoffs, h=prev_pos.h+yoffs;
	
		minx= (prev_pos.x<last_pos.x)? prev_pos.x : last_pos.x;
		miny= (prev_pos.y<last_pos.y)? prev_pos.y : last_pos.y;
	
		if(prev_pos.w!=last_pos.w){
			maxx= (prev_pos.x+prev_pos.w > last_pos.x+last_pos.w)? prev_pos.x+prev_pos.w : last_pos.x+last_pos.w;
			w=maxx-minx;
		}
		if(prev_pos.h!=last_pos.h){
			maxy= (prev_pos.y+prev_pos.h > last_pos.y+last_pos.h)? prev_pos.y+prev_pos.h : last_pos.y+last_pos.h;
			h=maxy-miny;
		}
		
		
		if(warp_border){
			SDL_Rect r;
			r.x=minx; r.y=miny; r.w=w; r.h=h;
			warp_update(r);
		}
		else if(the_screen)  //Use the screen class?
			the_screen->add_rect(minx, miny, w, h);
		else
			sge_UpdateRect(dest, minx, miny, w, h);
			
	}
	else{
		if(warp_border){
			warp_update(prev_pos);
			warp_update(last_pos);
		}
		else if(the_screen){
			the_screen->add_rect(prev_pos.x, prev_pos.y, prev_pos.w, prev_pos.h);	
			the_screen->add_rect(last_pos.x, last_pos.y, last_pos.w, last_pos.h);
		}
		else{
			sge_UpdateRect(dest, prev_pos.x, prev_pos.y, prev_pos.w, prev_pos.h);	
			sge_UpdateRect(dest, last_pos.x, last_pos.y, last_pos.w, last_pos.h);
		}
	}
}

void sge_surface::clear(Uint32 color)
{
	if(warp_border)
		warp_clear(color);
	else
		sge_FilledRect(dest, last_pos.x, last_pos.y, last_pos.x+last_pos.w-1, last_pos.y+last_pos.h-1,color);
}

void sge_surface::clear(SDL_Surface *src, Sint16 srcX, Sint16 srcY)
{
	if(warp_border)
		warp_clear(src,srcX,srcY);
	else
		sge_Blit(src,dest, srcX, srcY, last_pos.x, last_pos.y, last_pos.w, last_pos.h);
}



//==================================================================================
// sge_ssprite (derived from sge_surface)
// A simple sprite class
//==================================================================================
sge_ssprite::sge_ssprite(SDL_Surface *screen, SDL_Surface *img, Sint16 x, Sint16 y):
sge_surface(screen,img,x,y)
{
	//Create the first frame
	current_frame= new sge_frame;     //User has to catch bad_alloc himself
	current_frame->img = img;
	current_frame->cdata=NULL;
	frames.push_back(current_frame);
	
	current_fi=frames.begin();
	fi_start = current_fi;
	fi_stop = frames.end();
	
	//Default
	xvel=yvel=0;
	seq_mode=stop;

	bounce_border=true;
}

sge_ssprite::sge_ssprite(SDL_Surface *screen, SDL_Surface *img, sge_cdata *cdata, Sint16 x, Sint16 y):
sge_surface(screen,img,x,y)
{
	//Create the first frame
	current_frame= new sge_frame;     //User has to catch bad_alloc himself
	current_frame->img = img;
	current_frame->cdata=cdata;
	frames.push_back(current_frame);
	
	current_fi=frames.begin();
	fi_start = current_fi;
	fi_stop = frames.end();
	
	//Default
	xvel=yvel=0;
	seq_mode=stop;

	bounce_border=true;
}

sge_ssprite::~sge_ssprite(void)
{
	//Empty the list
	for(FI i=frames.begin(); i != frames.end(); i++)
			delete *i;
	
	frames.clear();	
}

bool sge_ssprite::check_border(void)
{
	if(!bounce_border)
		return sge_surface::check_border();

	bool flag=false;

	if(current_pos.x<border.x){
		current_pos.x=border.x;
		xvel=-xvel;
		flag=true;		
	}
	if(current_pos.x+current_pos.w > border.x+border.w){
		current_pos.x=border.x+border.w-current_pos.w;
		xvel=-xvel;
		flag=true;
	}
	if(current_pos.y<border.y){
		current_pos.y=border.y;
		yvel=-yvel;	
		flag=true;	
	}
	if(current_pos.y+current_pos.h > border.y+border.h){
		current_pos.y=border.y+border.h-current_pos.h;
		yvel=-yvel;
		flag=true;
	}
	return flag;
}


void sge_ssprite::add_frame(SDL_Surface *img)
{
	add_frame(img, NULL);
}

void sge_ssprite::add_frame(SDL_Surface *img, sge_cdata *cdata)
{
	//Create a new frame
	sge_frame *frame = new sge_frame;  //User has to catch bad_alloc himself
	frame->img = img;
	frame->cdata = cdata;
	frames.push_back(frame);
	
	fi_start = frames.begin();
	fi_stop = frames.end();
	
	seq_mode=loop;	
}

void sge_ssprite::skip_frame(int skips)
{
	if(skips > 0){
		for(int i=0; i<skips; i++){
			current_fi++;
			if(current_fi == fi_stop){
				if(seq_mode!=play_once)
					current_fi=fi_start;  //loop
				else{                   //stop
					seq_mode=stop;
					current_fi--;         //current_fi = fi_stop -1
					fi_start=current_fi;
				}
			}
		}
	}
	else if(skips < 0){
		for(int i=0; i<-skips; i++){
			if(current_fi == fi_start){
				if(seq_mode!=play_once)
					current_fi=fi_stop;   //loop
				else{                   //stop
					seq_mode=stop;
					current_fi++;         //+1
					fi_stop=current_fi;
				}	
			}
			current_fi--;
		}
	}
	else
		return;
	
	current_frame = *current_fi;	
	surface = current_frame->img;
	current_pos.w = surface->w;
	current_pos.h = surface->h;	
}

bool sge_ssprite::update(void)
{
	move(xvel,yvel);
	return !((xvel==0)&&(yvel==0));
}

void sge_ssprite::set_seq(int start, int stop, playing_mode mode)
{
	//Handle stupid user errors
	if(start < 0 || start > int(frames.size())-1)
		return;
	if(stop < start || stop > int(frames.size())-1)
		return;
		
	seq_mode=loop;
	if(mode==play_once)
		seq_mode=play_once;
	if(start==stop)
		seq_mode=sge_ssprite::stop;
		
	fi_start=fi_stop=frames.begin();
	
	for(int i=0; i<=stop; i++){
		if(i<start)
			fi_start++;
			
		fi_stop++;
		
		if(fi_stop == frames.end()){
			if(fi_start == frames.end())
				fi_start--;
			break;
		}		
	}
	
	current_fi = fi_start;
	
	current_frame = *current_fi;	
	surface = current_frame->img;
	current_pos.w = surface->w;
	current_pos.h = surface->h;	
}

void sge_ssprite::reset_seq(void)
{
	fi_start=frames.begin();
	fi_stop=frames.end();
	
	current_fi = fi_start;
	
	current_frame = *current_fi;	
	surface = current_frame->img;
	current_pos.w = surface->w;
	current_pos.h = surface->h;
	
	if(frames.size() > 1)
		seq_mode=loop;
	else
		seq_mode=stop;
}

void sge_ssprite::first_frame(void)
{
	current_fi = fi_start;
	
	current_frame = *current_fi;	
	surface = current_frame->img;
	current_pos.w = surface->w;
	current_pos.h = surface->h;
}

void sge_ssprite::last_frame(void)
{
	current_fi = fi_stop;
	current_fi--;
	
	current_frame = *current_fi;	
	surface = current_frame->img;
	current_pos.w = surface->w;
	current_pos.h = surface->h;
}



//==================================================================================
// sge_sprite (derived from sge_ssprite)
// A timed sprite class
//==================================================================================
bool sge_sprite::update(Uint32 ticks)
{
	if(tlast==0){
		tlast=ticks;
		return false;
	}
	
	Sint16 tmp;
	Uint32 time=ticks-tlast;
	tlast=ticks;	//Reset time
	
	bool ret=false;
	
	//Calculate new pos
	if(xppms!=0){
		xpos+=time*xppms;
		tmp=int(xpos);
		if(current_pos.x!=tmp){
			current_pos.x=tmp;
			ret=true;
		}
	}
	if(yppms!=0){	
  	ypos+=time*yppms;
  	tmp=int(ypos);
  	if(current_pos.y!=tmp){	
  	  current_pos.y=tmp;
			ret=true;
		}
  	}
	
	if(ret)             //Are we off-screen?
		check_border();
	
	//Calculate new frame
	if(fpms!=0){
		fpos+=time*fpms;
		tmp=int(fpos);
		if(tmp!=0){
			skip_frame(tmp);
			fpos-=tmp;
			ret=true;	
		}
	}		
		
	return ret;
}

bool sge_sprite::check_border(void)
{
	if(warp_border){
		if(sge_surface::check_warp()){
			xpos=current_pos.x;
			ypos=current_pos.y;
			return true;
		}
		return false;
	}
	if(!bounce_border)
		return false;

	bool flag=false;

	if(current_pos.x<border.x){
		current_pos.x=border.x;
		xpos=current_pos.x;
		xppms=-xppms;
		flag=true;		
	}
	else if(current_pos.x+current_pos.w > border.x+border.w){
		current_pos.x=border.x+border.w-current_pos.w;
		xpos=current_pos.x;
		xppms=-xppms;
		flag=true;
	}
	if(current_pos.y<border.y){
		current_pos.y=border.y;
		ypos=current_pos.y;
		yppms=-yppms;
		flag=true;		
	}
	else if(current_pos.y+current_pos.h > border.y+border.h){
		current_pos.y=border.y+border.h-current_pos.h;
		ypos=current_pos.y;
		yppms=-yppms;
		flag=true;
	}
	return flag;
}


#endif /* _SGE_NO_CLASSES */

