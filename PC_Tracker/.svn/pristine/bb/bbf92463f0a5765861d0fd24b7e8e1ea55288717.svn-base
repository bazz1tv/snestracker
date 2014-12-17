/*
*	SDL Graphics Extension
*	Text/TrueType classes (header)
*
*	Started 990826 / 010207 (new version)
*
*	License: LGPL v2+ (see the file LICENSE)
*	(c)1999-2003 Anders Lindström
*
*	Uses the excellent FreeType 2 library, available at:
*	http://www.freetype.org/
*/

/*********************************************************************
 *  This library is free software; you can redistribute it and/or    *
 *  modify it under the terms of the GNU Library General Public      *
 *  License as published by the Free Software Foundation; either     *
 *  version 2 of the License, or (at your option) any later version. *
 *********************************************************************/

#ifndef sge_textpp_H
#define sge_textpp_H

#include "SDL.h"
#include "sge_internal.h"

#ifndef _SGE_NO_CLASSES

#include <string>
#include <stdio.h>
#include "sge_tt_text.h"
#include "sge_bm_text.h"
#include "sge_shape.h"

//==================================================================================
// Edits text from SDL_Event
//==================================================================================
class DECLSPEC sge_TextEditor
{
protected:
	//The text is stored in a double linked list

	//The elements in the linked list
	struct node{
		Uint16 c;    //Unicode char
		
		node* next;
		node* prev;
	};
	
	//List metadata
	node* start;
	node* end;
	node* cursor;   //The current node (the cursor)
	
	Uint16 cursor_char;     //The charactar for the cursor
	unsigned int chars;     //Size of the string (without the cursor)
	unsigned int mChars;    //Max chars, 0 is default (unlimited)
	
	//Create and fill a new node
	node* new_node(node* p, node* n, Uint16 c);
	
	//Swap two nodes
	bool nswap(node* one, node* two);
	
	//This indicates that the text has changed since last
	//set to false (used for derived classes)
	bool text_changed;
public:
	//Creator
	sge_TextEditor(void);
	//Destructor
	virtual ~sge_TextEditor(void);
	
	//Adds an char before the cursor
	bool insert(Uint16 c);
	//Removes the char left of cursor
	bool remove_left(void);
	//Removes the char right of cursor
	bool remove_right(void);
	
	//Move cursor left
	inline bool move_left(void);
	//Move cursor right
	inline bool move_right(void);
	//Move cursor to the start
	bool move_start(void);
	//Move cursor to the end
	bool move_end(void);
	
	//Returns text as latin1 or unicode with or without the cursor char
	std::string get_string(bool wCursor=true);
	//std::basic_string<Uint16> get_ustring(bool wCursor=true);
	//Returns a unicode c-style string (allocated with new)
	Uint16* get_ucstring(bool wCursor=true);
	
	//Process a SDL_Event
	//Returns true if the text changed
	virtual bool check(SDL_Event* event);
	
	//Change the cursor char
	void change_cursor(Uint16 c){cursor_char=c; cursor->c=c;}
	
	//Change the text
	void clear_text(void);
	void change_text(const std::string s);
	//void change_utext(const std::basic_string<Uint16> s);
	void change_uctext(Uint16 *text);
	void change_textf(const char *text, ...);             //printf c-style... urk
	
	//Set max chars (default: limited only by memory)
	void max_chars(unsigned int c){mChars=c;}
	
	//Returns the number of characters in the current string
	unsigned int get_chars(void){return chars;}
};



//==================================================================================
// A class for rendering text
//==================================================================================
class DECLSPEC sge_text: public sge_TextEditor
{
protected:
#ifndef _SGE_NOTTF
	sge_TTFont *tt_font;  //The truetype font
#else
	Uint8 *tt_font;
#endif

	//TT Font color
	SDL_Color color;
	SDL_Color background;
	
	sge_bmpFont *bm_font; //The bitmap font
	
	Uint8 alpha_level; //Alpha level (default: opaque)
	
	SDL_Surface* text_surface;	
	virtual void set_textSurface(SDL_Surface *new_surf){;}
	
	//Use what render?
	bool use_tt;
	
	//Show cursor when rendering text?
	bool sCursor;
	
public:
	//Constructor
	sge_text(void){tt_font=NULL; bm_font=NULL; text_surface=NULL; use_tt=true; text_surface=NULL; sCursor=false; alpha_level=SDL_ALPHA_OPAQUE;}
	virtual ~sge_text(void){if(text_surface){SDL_FreeSurface(text_surface);}}
	
	//Get a pointer to the text surface or (if copy=true) returns
	//a copy (don't forget to free it later)
	SDL_Surface* get_textSurface(bool copy=false);
	
	//Updates the textsurface if the text has changed (or if force=true)
	//and returns true if the surface was updated
	bool update_textSurface(bool force=false);
	
#ifndef _SGE_NOTTF
	void set_ttFont(sge_TTFont *font, Uint8 r, Uint8 g, Uint8 b, Uint8 br=0, Uint8 bg=0, Uint8 bb=0);
#endif
	
	bool get_color(SDL_Color *fg){if( tt_font ) {fg->r = color.r; fg->g = color.g; fg->b = color.b; return true;} else return false;}
	bool get_bg(SDL_Color *bg)   {if( tt_font ) {bg->r = background.r; bg->g = background.g; bg->b = background.b; return true;} else return false;}

	void set_bmFont(sge_bmpFont *bm_font);
	
	//Should a cursor be drawn?
	void show_cursor(bool mode){if(mode!=sCursor){text_changed=true;} sCursor=mode;}
	
	//Render text to a surface
	SDL_Rect render_text(SDL_Surface *surface, Sint16 x, Sint16 y);
	
	void use_TTrender(void){if(tt_font){use_tt=true;}}
	void use_BMrender(void){if(bm_font){use_tt=false;}}
	
	//set alpha level (default: opaque)
	void set_alpha(Uint8 alpha){alpha_level = alpha;}
};


//==================================================================================
//sge_TextSurface (derived public from sge_text and sge_surface)
//sge_TextSsprite (derived public from sge_text and sge_ssprite)
//sge_TextSprite  (derived public from sge_text and sge_sprite)
//==================================================================================
class DECLSPEC sge_TextSurface: public sge_text, public sge_surface 
{
protected:
	virtual void set_textSurface(SDL_Surface *new_surf);

public:
	sge_TextSurface(SDL_Surface *screen, Sint16 x=0, Sint16 y=0):
		sge_surface(screen,screen,x,y)
		{\
			surface=NULL;
			current_pos.w=0; current_pos.h=0;
		}
		
	sge_TextSurface(SDL_Surface *screen, const std::string text, Sint16 x=0, Sint16 y=0):
		sge_surface(screen,screen,x,y)
		{
			change_text(text);
			current_pos.w=0; current_pos.h=0;
		}
		
	virtual void draw(void);
};


class DECLSPEC sge_TextSsprite: public sge_text, public sge_ssprite 
{
protected:
	virtual void set_textSurface(SDL_Surface *new_surf);

public:
	sge_TextSsprite(SDL_Surface *screen, Sint16 x=0, Sint16 y=0):
		sge_ssprite(screen,screen,x,y)
		{\
			surface=NULL;
			current_pos.w=0; current_pos.h=0;
		}
		
	sge_TextSsprite(SDL_Surface *screen, const std::string text, Sint16 x=0, Sint16 y=0):
		sge_ssprite(screen,screen,x,y)
		{
			change_text(text);
			current_pos.w=0; current_pos.h=0;
		}
		
	virtual void draw(void);
};


class DECLSPEC sge_TextSprite: public sge_text, public sge_sprite 
{
protected:
	virtual void set_textSurface(SDL_Surface *new_surf);

public:
	sge_TextSprite(SDL_Surface *screen, Sint16 x=0, Sint16 y=0):
		sge_sprite(screen,screen,x,y)
		{\
			surface=NULL;
			current_pos.w=0; current_pos.h=0;
		}
		
	sge_TextSprite(SDL_Surface *screen, const std::string text, Sint16 x=0, Sint16 y=0):
		sge_sprite(screen,screen,x,y)
		{
			change_text(text);
			current_pos.w=0; current_pos.h=0;
		}
		
	virtual void draw(void);
};


//==================================================================================
// A helper function for lazy users: blocking text input for sge_TextSurface
// objects.
// Flags is the same as for BM and TTF input (which now uses this function)
//==================================================================================
DECLSPEC int sge_text_input(sge_TextSurface *tc, Uint8 flags);

#endif /* _SGE_NO_CLASSES */
#endif /* sge_textpp_H */
