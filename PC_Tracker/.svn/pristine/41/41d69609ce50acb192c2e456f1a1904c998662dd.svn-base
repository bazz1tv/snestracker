/*
*	SDL Graphics Extension
*	SGE shape (header)
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

#ifndef sge_shape_H
#define sge_shape_H

#include "SDL.h"
#include "sge_internal.h"

#ifndef _SGE_NO_CLASSES

// Remove "class 'std::list<>' needs to have dll-interface to be used" warnings
// from MS VisualC++
#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4251)
#endif


#include <list>
#include "sge_surface.h"
#include "sge_collision.h"

class DECLSPEC sge_shape;

//==================================================================================
// The screen class
//==================================================================================
class DECLSPEC sge_screen
{
protected:
	SDL_Surface *screen;       //The SDL screen surface
	std::list<SDL_Rect> rects; //The list of rectangles to be updated
	
	std::list<sge_shape*> shapes;   //The list of shapes to draw on screen
	std::list<sge_shape*> shapes_p; //The list of permanent shapes to draw on screen
	
	typedef std::list<SDL_Rect>::const_iterator RI;   //List iterator (for rects)
	typedef std::list<sge_shape*>::const_iterator SI; //List iterator (for shapes)
	
	bool HW, DB, FS;  //video memory, double-buffered or/and fullscreen?

public:
	sge_screen(SDL_Surface *screen);
	~sge_screen(void){rects.clear();shapes.clear();shapes_p.clear();}
	void add_rect(SDL_Rect rect);
	void add_rect(Sint16 x, Sint16 y, Uint32 w, Uint32 h);
	
	void add_shape(sge_shape *shape);
	void add_shape_p(sge_shape *shape); //Adds an shape permanently
	
	void remove_shape_p(sge_shape *shape);
	void clear_all(void);
	
	void update(void);
};


//==================================================================================
// sge_shape
// Abstract base class for different shapes (surfaces, sprites, ...)
//==================================================================================
class sge_shape
{
protected:
	SDL_Rect current_pos; //The *current* (maybe undrawn) position of the shape	
	SDL_Rect last_pos;    //The *last* drawn position of shape
	SDL_Rect prev_pos;    //The previous drawn position of shape (used to update a cleared area)

	SDL_Surface *dest;    //The surface to perform operations on
	
public:
	virtual ~sge_shape(void){} //Destructor
	virtual void draw(void)=0; //Draws the shape - prev_pos = last_pos; last_pos = the new position of shape
	
	//Updates the screen (last_pos+prev_pos)
	//If sge_screen is used this member will use it (the_screen) instead of doing it directly!
	virtual void UpdateRects(void)=0;
	
	//Some functions to clear (remove) shape
	virtual void clear(Uint32 color)=0; //Clears to color
	virtual void clear(SDL_Surface *src, Sint16 srcX, Sint16 srcY)=0; //Clears by blitting src
	
	inline SDL_Rect get_pos(void) const{return current_pos;}   //Returns the current position
	inline SDL_Rect get_last_pos(void) const{return last_pos;} //Returns the last updated position

	inline SDL_Surface* get_dest(void) const{return dest;} 
	
	/*
	//NW N NE
	//  \|/
	// W-C-E
	//  /|\
	//SW S SE
	//
	//Returns some usefull coords in shape (current)
	*/
	inline Sint16 c_x(void)  const{return current_pos.x+current_pos.w/2;}
	inline Sint16 c_y(void)  const{return current_pos.y+current_pos.h/2;}

	inline Sint16 nw_x(void) const{return current_pos.x;}
	inline Sint16 nw_y(void) const{return current_pos.y;}

	inline Sint16 n_x(void)  const{return current_pos.x+current_pos.w/2;}
	inline Sint16 n_y(void)  const{return current_pos.y;}

	inline Sint16 ne_x(void) const{return current_pos.x+current_pos.w-1;}
	inline Sint16 ne_y(void) const{return current_pos.y;}

	inline Sint16 e_x(void)  const{return current_pos.x+current_pos.w-1;}
	inline Sint16 e_y(void)  const{return current_pos.y+current_pos.h/2;}

	inline Sint16 se_x(void) const{return current_pos.x+current_pos.w-1;}
	inline Sint16 se_y(void) const{return current_pos.y+current_pos.h-1;}

	inline Sint16 s_x(void)  const{return current_pos.x+current_pos.w/2;}
	inline Sint16 s_y(void)  const{return current_pos.y+current_pos.h-1;}

	inline Sint16 sw_x(void) const{return current_pos.x;}
	inline Sint16 sw_y(void) const{return current_pos.y+current_pos.h-1;}

	inline Sint16 w_x(void)  const{return current_pos.x;}
	inline Sint16 w_y(void)  const{return current_pos.y+current_pos.h/2;}

	inline Sint16 get_xpos(void) const{return current_pos.x;}
	inline Sint16 get_ypos(void) const{return current_pos.y;}
	inline Sint16 get_w(void) const{return current_pos.w;}
	inline Sint16 get_h(void) const{return current_pos.h;}
};



//==================================================================================
// sge_surface (derived from sge_shape)
// A class for moving/blitting surfaces
//==================================================================================
class DECLSPEC sge_surface: public sge_shape
{
protected:
	SDL_Surface *surface; //The source surface *NOT COPIED*

	//Do warp logic
	bool check_warp(void);
	
	//Handles outside screen problems (But not in this class)
	virtual bool check_border(void){return check_warp();}
	
	//The border box (default: the screen size)
	SDL_Rect border;
	
	//Should we warp around the border box? (not in this class
	//but some methods here must know)
	bool warp_border;
	
	//Decode a warp pos rectangle
	int get_warp(SDL_Rect rec, SDL_Rect &r1, SDL_Rect &r2, SDL_Rect &r3, SDL_Rect &r4);
	
	//Helper functions
	void warp_draw(void);
	void warp_update(SDL_Rect rec);
	void warp_clear(Uint32 color);
	void warp_clear(SDL_Surface *src, Sint16 srcX, Sint16 srcY);
	
public:
	sge_surface(SDL_Surface *dest, SDL_Surface *src, Sint16 x=0, Sint16 y=0);
	~sge_surface(void);
	
	//Draws the surface
	virtual void draw(void);
	
	virtual inline void clear(Uint32 color);
	virtual inline void clear(SDL_Surface *src, Sint16 srcX, Sint16 srcY);
	//virtual inline void clear(SDL_Surface *src){clear(src,last_pos.x,last_pos.y);}
	
	virtual void UpdateRects(void);
	
	//Move the surface
	virtual inline void move_to(Sint16 x, Sint16 y){current_pos.x=x; current_pos.y=y;check_border();}
	virtual inline void move(Sint16 x_step, Sint16 y_step){current_pos.x+=x_step; current_pos.y+=y_step; check_border();}

	//Get pointer to surface
	SDL_Surface* get_img(void) const{return surface;}
	
	//Sets the border box
	void set_border(SDL_Rect box){border=box;}
};



//==================================================================================
// The frame struct (for sge_ssprite)
//==================================================================================
struct sge_frame
{
	//The image
	SDL_Surface *img;
	
	//Collision data
	sge_cdata *cdata;
};


//==================================================================================
// sge_ssprite (derived from sge_surface)
// A simple sprite class
//==================================================================================
class DECLSPEC sge_ssprite: public sge_surface
{
public:enum playing_mode{loop, play_once, stop}; //This must be public	

protected:
	//Linked list with the frames
	//Obs! 'surface' always points to the current frame image
	std::list<sge_frame*> frames;
	typedef std::list<sge_frame*>::const_iterator FI; //List iterator (for frames)

	FI current_fi;
	FI fi_start;   //first frame in the playing sequence loop
	FI fi_stop;    //last frame + 1
	
	//The pointer to the current frame
	sge_frame *current_frame; //Should at all times be *current_fi
	
	//The speed of the sprite (pixels/update)
	Sint16 xvel, yvel;
	
	bool bounce_border;  //Do we want the sprite to bounce at the border?
	virtual bool check_border(void);
	
	//Playing sequence mode
	playing_mode seq_mode;
	
public:

	//Constructor and destructor
	sge_ssprite(SDL_Surface *screen, SDL_Surface *img, Sint16 x=0, Sint16 y=0);
	sge_ssprite(SDL_Surface *screen, SDL_Surface *img, sge_cdata *cdata, Sint16 x=0, Sint16 y=0);
	~sge_ssprite(void);
	
	//Updates the internal status
	//Returns true if the sprite moved
	virtual inline bool update(void);
	
	//Sets the speed
	void set_vel(Sint16 x, Sint16 y){xvel=x;yvel=y;}
	void set_xvel(Sint16 x){xvel=x;}
	void set_yvel(Sint16 y){yvel=y;}
	
	//Gets the speed
	Sint16 get_xvel(void) const{return xvel;}
	Sint16 get_yvel(void) const{return yvel;}
	
	//Add a frame
	//Obs! Resets playing sequence
	void add_frame(SDL_Surface *img);
	void add_frame(SDL_Surface *img, sge_cdata *cdata);	
	
	//Change frame
	void skip_frame(int skips); //A negative 'skips' indicates backwards
	void next_frame(void){skip_frame(1);}
	void prev_frame(void){skip_frame(-1);}
	void first_frame(void);     //Does NOT change the sequence
	void last_frame(void);      //             "
	
	//Changes playing sequence (0- first frame)
	//playing_mode:
	// sge_ssprite::loop - loops forever
	// sge_ssprite::play_once - just once then stops
	// sge_ssprite::stop - is returned by get_PlayingMode() if stoped
	void set_seq(int start, int stop, playing_mode mode=loop);
	void reset_seq(void);
	playing_mode get_PlayingMode(void){return seq_mode;}
	
	//Get cdata for current frame
	sge_cdata* get_cdata(void){return current_frame->cdata;}
	
	//Get the current frame
	sge_frame* get_frame(void){return current_frame;}
	
	//Get the frame list
	//DO NOT ADD OR REMOVE ELEMENTS without using
	//reset_seq() when done!!
	std::list<sge_frame*>* get_list(void){return &frames;}
	
	//Set border mode:
	//Bounce - sprite bounces (default)
	//Warp - sprite warps at border
	void border_bounce(bool mode){bounce_border=mode; if(warp_border){warp_border=false;}}
	void border_warp(bool mode){warp_border=mode; if(bounce_border){bounce_border=false;}}
};


//==================================================================================
// sge_sprite (derived from sge_ssprite)
// A timed sprite class
//==================================================================================
class DECLSPEC sge_sprite: public sge_ssprite
{
protected:
	//Pixels/ms
	double xppms, yppms;
	
	//Frames/ms
	double fpms;

	//The float pos
	double xpos, ypos, fpos;
	
	//Ticks since last pos update
	Uint32 tlast;
	
	virtual bool check_border(void);
public:
	//Constructor
	sge_sprite(SDL_Surface *screen, SDL_Surface *img, Sint16 x=0, Sint16 y=0):
		sge_ssprite(screen,img,x,y){xppms=yppms=fpms=0;tlast=0;xpos=x;ypos=y;fpos=0;}
		
	sge_sprite(SDL_Surface *screen, SDL_Surface *img, sge_cdata *cdata, Sint16 x=0, Sint16 y=0):
		sge_ssprite(screen,img,cdata,x,y){xppms=yppms=fpms=0;tlast=0;xpos=x;ypos=y;fpos=0;}
	
	//Change the speeds
	void set_pps(Sint16 x, Sint16 y){xppms=x/1000.0; yppms=y/1000.0;}
	void set_xpps(Sint16 x){xppms=x/1000.0;}
	void set_ypps(Sint16 y){yppms=y/1000.0;}
	void set_fps(Sint16 f){fpms=f/1000.0;}
	
	//Get the speeds
	Sint16 get_xpps(void) const{return Sint16(xppms*1000);}
	Sint16 get_ypps(void) const{return Sint16(yppms*1000);}
	Sint16 get_fps(void) const{return Sint16(fpms*1000);}
			
	//Update position and frame
	//Returns true if something changed
	bool update(Uint32 ticks);
	bool update(void){return update(SDL_GetTicks());}
	
	//Correct move members for this class
	void move_to(Sint16 x, Sint16 y){sge_surface::move_to(x,y); xpos=current_pos.x; ypos=current_pos.y;}
	void move(Sint16 x_step, Sint16 y_step){sge_surface::move(x_step,y_step); xpos=current_pos.x; ypos=current_pos.y;}

	//Freeze time until next update
	void pause(void){tlast=0;}
};


#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif /* _SGE_NO_CLASSES */
#endif /* sge_shape_H */
