/*
*	SDL Graphics Extension
*	Collision routines
*
*	Started 000625
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)1999-2003 Anders Lindström
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#include "SDL.h"
#include <stdio.h>
#include <string.h>
#include <new>
#include "sge_collision.h"
#include "sge_surface.h"
#include "sge_shape.h"

using namespace std;

Uint8 sge_mask[8]={SGE_FLAG1,SGE_FLAG2,SGE_FLAG3,SGE_FLAG4,SGE_FLAG5,SGE_FLAG6,SGE_FLAG7,SGE_FLAG8};
SDL_Rect _ua;
Sint16 _cx=0,_cy=0;

int memand(Uint8 *s1, Uint8 *s2, int shift1, int shift2, int N);

//==================================================================================
// Makes a new collision map from img. Set colorkey first!
//==================================================================================
sge_cdata *sge_make_cmap(SDL_Surface *img)
{
	sge_cdata *cdata;
	Uint8 *map;
	Sint16 x,y;
	Sint32 offs;
	int i;
	
	cdata=new(nothrow) sge_cdata;
	if(!cdata){SDL_SetError("SGE - Out of memory");return NULL;}
	cdata->w=img->w; cdata->h=img->h;
	offs=(img->w*img->h)/8;
	cdata->map=new(nothrow) Uint8[offs+2];
	if(!cdata->map){SDL_SetError("SGE - Out of memory");return NULL;}
	memset(cdata->map,0x00,offs+2);
	
	map=cdata->map;
		
	i=0;
	for(y=0; y < img->h; y++){
		for(x=0; x < img->w; x++){
			if(i>7){i=0;map++;}
			if(sge_GetPixel(img, Sint16(x),Sint16(y))!=img->format->colorkey){
				*map=*map|sge_mask[i];	
			}
			i++;
		}	
	}
	return cdata;
}

//==================================================================================
// Checks bounding boxes for collision: 0-no collision 1-collision
//==================================================================================
int sge_bbcheck(sge_cdata *cd1,Sint16 x1,Sint16 y1, sge_cdata *cd2,Sint16 x2,Sint16 y2)
{
	Sint16 w1=cd1->w;
	Sint16 h1=cd1->h;
	Sint16 w2=cd2->w;
	Sint16 h2=cd2->h;

	if(x1 < x2){
		if(x1+w1 > x2){
			if(y1 < y2){
				if(y1+h1 > y2){
					_ua.x=x2;
					_ua.y=y2;
					return 1;
				}
			}
			else{
				if(y2+h2 > y1){
					_ua.x=x2;
					_ua.y=y1;
					return 1;
				}
			}
		}	
	}
	else{
		if(x2+w2 > x1){
			if(y2 < y1){
				if(y2+h2 > y1){
				  _ua.x=x1;
					_ua.y=y1;
					return 1;
				}
			}
			else{
				if(y1+h1 > y2){
				  _ua.x=x1;
					_ua.y=y2;
					return 1;
				}
			}
		}	
	}
	
	
	return 0;
}

//==================================================================================
// Checks bounding boxes for collision: 0-no collision 1-collision
//==================================================================================
int _sge_bbcheck(Sint16 x1,Sint16 y1,Sint16 w1,Sint16 h1, Sint16 x2,Sint16 y2,Sint16 w2,Sint16 h2)
{
	if(x1 < x2){
		if(x1+w1 > x2){
			if(y1 < y2){
				if(y1+h1 > y2){
					_ua.x=x2;
					_ua.y=y2;
					return 1;
				}
			}
			else{
				if(y2+h2 > y1){
					_ua.x=x2;
					_ua.y=y1;
					return 1;
				}
			}
		}	
	}
	else{
		if(x2+w2 > x1){
			if(y2 < y1){
				if(y2+h2 > y1){
				  _ua.x=x1;
					_ua.y=y1;
					return 1;
				}
			}
			else{
				if(y1+h1 > y2){
				  _ua.x=x1;
					_ua.y=y2;
					return 1;
				}
			}
		}	
	}
	
	
	return 0;
}

//==================================================================================
// AND N bits of s1 and s2
// Returns the number of the bit (or zero)
//==================================================================================
int memand(Uint8 *s1, Uint8 *s2, int shift1, int shift2, int N)
{
	int b,i1=shift1,i2=shift2;

  for(b=0; b<N; b++){
  	if(i1>7){i1=0;s1++;}
  	if(i2>7){i2=0;s2++;}
  	if( (*s1&sge_mask[i1]) && (*s2&sge_mask[i2]) ) return b+1;
  	i1++; i2++;	
  }
	return 0;
}

//==================================================================================
// Checks for pixel perfect collision: 0-no collision 1-collision
// sge_bbcheck MUST be called first!!!
//==================================================================================
int _sge_cmcheck(sge_cdata *cd1,Sint16 x1,Sint16 y1, sge_cdata *cd2,Sint16 x2,Sint16 y2)
{
	if(cd1->map==NULL || cd2->map==NULL)
		return 0;

  Sint16 w1=cd1->w;
	Sint16 h1=cd1->h;
	Sint16 w2=cd2->w;
	Sint16 h2=cd2->h;
	
	//masks
	
	Sint32 x1o=0,x2o=0,y1o=0,y2o=0,offs;  //offsets
	int i1=0,i2=0;
	
  Uint8 *map1=cd1->map;
	Uint8 *map2=cd2->map;
	
	//Calculate correct starting point
	if(_ua.x==x2 && _ua.y==y2){
		x1o=x2-x1;
		y1o=y2-y1;
		
		offs=w1*y1o+x1o;	
		map1+=offs/8;
		i1=offs%8;	
	}
	else if(_ua.x==x2 && _ua.y==y1){
		x1o=x2-x1;
		y2o=y1-y2;
			
		map1+=x1o/8;
		i1=x1o%8;
		
		offs=w2*y2o;
		map2+=offs/8;
		i2=offs%8;	
	}
	else if(_ua.x==x1 && _ua.y==y1){
		x2o=x1-x2;
		y2o=y1-y2;
		
		offs=w2*y2o+x2o;	
		map2+=offs/8;
		i2=offs%8;	
	}
	else if(_ua.x==x1 && _ua.y==y2){
		x2o=x1-x2;
		y1o=y2-y1;	
		
		offs=w1*y1o;
		map1+=offs/8;
		i1=offs%8;
		
		map2+=x2o/8;
		i2=x2o%8;	
	}
	else
		return 0;
		
	Sint16 y;

	Sint16 lenght;
	
	if(x1+w1 < x2+w2)
		lenght=w1-x1o;	
	else
		lenght=w2-x2o;
	
	//AND(map1,map2)
	for(y=_ua.y; y<=y1+h1 && y<=y2+h2; y++){
	
		offs=memand(map1,map2,i1,i2,lenght);
		if(offs){
			_cx=_ua.x+offs-1; _cy=y;
			return 1;
		}
		
		//goto the new line
		offs=(y-y1)*w1+x1o;
		map1=cd1->map; //reset pointer
		map1+=offs/8;
		i1=offs%8;
		
    offs=(y-y2)*w2+x2o;
    map2=cd2->map; //reset pointer
		map2+=offs/8;
		i2=offs%8;
	}
	
	
	return 0;
}

//==================================================================================
// Checks pixel perfect collision: 0-no collision 1-collision
// calls sge_bbcheck automaticly
//==================================================================================
int sge_cmcheck(sge_cdata *cd1,Sint16 x1,Sint16 y1, sge_cdata *cd2,Sint16 x2,Sint16 y2)
{
	if(!sge_bbcheck(cd1,x1,y1, cd2,x2,y2))
		return 0;
		
	if(cd1->map==NULL || cd2->map==NULL)
		return 1;	
	
	return _sge_cmcheck(cd1,x1,y1, cd2,x2,y2); 	
}


//==================================================================================
// Get the position of the last collision
//==================================================================================
Sint16 sge_get_cx(void)
{
	return _cx;
}
Sint16 sge_get_cy(void)
{
	return _cy;
}


//==================================================================================
// Removes collision map from memory
//==================================================================================
void sge_destroy_cmap(sge_cdata *cd)
{
	if(cd->map!=NULL)
		delete [] cd->map;
		
	delete cd;
}


//==================================================================================
// Checks bounding boxes for collision: 0-no collision 1-collision
// (sprites)
//==================================================================================
#ifndef _SGE_NO_CLASSES
int sge_bbcheck_shape(sge_shape *shape1, sge_shape *shape2)
{
	return _sge_bbcheck(shape1->get_xpos(), shape1->get_ypos(), shape1->get_w(), shape1->get_h(), shape2->get_xpos(), shape2->get_ypos(), shape2->get_w(), shape2->get_h());	
}
#endif


//==================================================================================
// Clears an area in a cmap
//==================================================================================
void sge_unset_cdata(sge_cdata *cd, Sint16 x, Sint16 y, Sint16 w, Sint16 h)
{
	Uint8 *map=cd->map;
	Sint16 offs,len;
	int i,n=0;
	
	offs=y*cd->w + x;
	map+=offs/8;
	i=offs%8;	
	
	while(h--){
		len=w;
		while(len--){
  		if(i>7){i=0;map++;}
  		*map&=~sge_mask[i];
  		i++;	
  	}
  	n++;
  	map=cd->map;
  	offs=(y+n)*cd->w + x;
  	map+=offs/8;
  	i=offs%8; 	
  }
}


//==================================================================================
// Fills an area in a cmap
//==================================================================================
void sge_set_cdata(sge_cdata *cd, Sint16 x, Sint16 y, Sint16 w, Sint16 h)
{
	Uint8 *map=cd->map;
	Sint16 offs,len;
	int i,n=0;
	
	offs=y*cd->w + x;
	map+=offs/8;
	i=offs%8;	
	
	while(h--){
		len=w;
		while(len--){
  		if(i>7){i=0;map++;}
  		*map|=sge_mask[i];
  		i++;	
  	}
  	n++;
  	map=cd->map;
  	offs=(y+n)*cd->w + x;
  	map+=offs/8;
  	i=offs%8; 	
  }
}
