/*
*	SDL Graphics Extension
*	Text/TrueType classes
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

#include "SDL.h"
#include <stdlib.h>
#include <stdarg.h>
#include "sge_surface.h"
#include "sge_textpp.h"

#ifndef _SGE_NO_CLASSES


using namespace std;


//==================================================================================
// sge_TextEditor
//==================================================================================
sge_TextEditor::node* sge_TextEditor::new_node(node* p, node* n, Uint16 c)
{
	node* tmp = new node;
	
	tmp->prev=p;
	tmp->next=n;
	tmp->c=c;
	
	text_changed=true;
	return tmp;
}


bool sge_TextEditor::nswap(node* one, node* two)
{
	if(!one || !two || one==two)
		return false;
	
	//Check if the two nodes are connected
	if(one->prev==two){
		one->prev=two->prev;
		two->next=one->next;
		one->next=two;
		two->prev=one;
	}else if(one->next==two){
		one->next=two->next;
		two->prev=one->prev;
		one->prev=two;
		two->next=one;
	}else{
		node* p= one->prev;
		node* n= one->next;
	
		one->prev= two->prev;
		one->next= two->next;
	
		two->prev= p;
		two->next= n;
	}
	
	
	//Update connected nodes and list metadata
	if(!one->prev)
		start=one;
	else
		one->prev->next=one;

	if(!one->next)
		end=one;
	else
		one->next->prev=one;

	if(!two->prev)
		start=two;
	else
		two->prev->next=two;

	if(!two->next)
		end=two;
	else
		two->next->prev=two;
		
	text_changed=true;
	return true;
}


sge_TextEditor::sge_TextEditor(void)
{
	chars=0;
	mChars=0;
	cursor_char=124; //The charactar for the cursor - '|'
	cursor=end=start=new_node(NULL,NULL,cursor_char);
	text_changed=false;
}


sge_TextEditor::~sge_TextEditor(void)
{
	node *tmp;

	for(node* i=start; i;){
		tmp=i->next;
		delete i;
		i=tmp;
	}
}


bool sge_TextEditor::insert(Uint16 c)
{
	if(mChars && chars>=mChars)
		return false;

	if(cursor->prev){
		cursor->prev->next=new_node(cursor->prev,cursor,c);	
		cursor->prev=cursor->prev->next;
	}
	else{
		//New first node - update list metadata
		cursor->prev=start=new_node(NULL,cursor,c);
	}	
	
	chars++;
	return true;
}


bool sge_TextEditor::remove_left(void)
{
	if(cursor->prev){
		node* tmp=cursor->prev->prev;
		delete cursor->prev;
		cursor->prev=tmp;
		
		if(!cursor->prev)
			start=cursor;
		else
			tmp->next=cursor;
			
		chars--;
		text_changed=true;
		return true;
	}
	return false;
}


bool sge_TextEditor::remove_right(void)
{
	if(cursor->next){
		node* tmp=cursor->next->next;
		delete cursor->next;
		cursor->next=tmp;
		
		if(!cursor->next)
			end=cursor;
		else
			tmp->prev=cursor;
		
		chars--;
		text_changed=true;	
		return true;		
	}
	return false;
}


bool sge_TextEditor::move_left(void)
{
	return nswap(cursor,cursor->prev);
}


bool sge_TextEditor::move_right(void)
{
	return nswap(cursor,cursor->next);
}


bool sge_TextEditor::move_start(void)
{
	if(cursor->prev){
		cursor->prev->next= cursor->next;
		if(cursor->next)
			cursor->next->prev= cursor->prev;
		else
			end=cursor->prev;
		
		cursor->prev= NULL;
		cursor->next= start;
		start->prev= cursor;
		start= cursor;
		
		text_changed=true;
		return true;
	}
	
	return false;
}


bool sge_TextEditor::move_end(void)
{
	if(cursor->next){
		cursor->next->prev= cursor->prev;
		if(cursor->prev)
			cursor->prev->next= cursor->next;
		else
			start=cursor->next;
		
		cursor->next= NULL;
		cursor->prev= end;
		end->next= cursor;
		end= cursor;
		
		text_changed=true;
		return true;
	}
	
	return false;
}


string sge_TextEditor::get_string(bool wCursor)
{
	string ret;

	for(node* i=start; i; i=i->next){
		if(!wCursor && i==cursor)
			continue;
		
		ret += char(i->c);
	}
		
	return ret;
}

/*
basic_string<Uint16> sge_TextEditor::get_ustring(bool wCursor)
{
	basic_string<Uint16> ret;

	for(node* i=start; i; i=i->next){
		if(!wCursor && i==cursor)
			continue;
		
		ret += i->c;
	}
		
	return ret;	
}
*/

Uint16* sge_TextEditor::get_ucstring(bool wCursor)
{
	Uint16* str=NULL;

	if(wCursor)
		str= new Uint16[chars+2];
	else
		str= new Uint16[chars+1];
	
	int k=0;
	
	for(node* i=start; i; i=i->next){
		if(!wCursor && i==cursor)
			continue;
		
		str[k++] = i->c;
	}
		
	str[k]=0;
	
	return str;
}


bool sge_TextEditor::check(SDL_Event* event)
{
	if(event->type!=SDL_KEYDOWN)
		return false;
		
	if(event->key.keysym.sym==SDLK_BACKSPACE)
		return remove_left();
	else if(event->key.keysym.sym==SDLK_DELETE)
		return remove_right();
	else if(event->key.keysym.sym==SDLK_LEFT)
		return move_left();
	else if(event->key.keysym.sym==SDLK_RIGHT)
		return move_right();
	else if(event->key.keysym.sym==SDLK_RETURN || event->key.keysym.sym==SDLK_KP_ENTER)
		return false;
	else if(event->key.keysym.sym==SDLK_HOME)
		return move_start();
	else if(event->key.keysym.sym==SDLK_END)
		return move_end();
	else if(event->key.keysym.unicode!=0)
		return insert(event->key.keysym.unicode);
	
	return false;
}


void sge_TextEditor::clear_text(void)
{
	if(!chars)
		return;
		
	node *tmp;

	for(node* i=start; i;){
		tmp=i->next;
		delete i;
		i=tmp;
	}
	
	cursor=end=start=new_node(NULL,NULL,cursor_char);
	
	chars=0;
	text_changed=true;
}


void sge_TextEditor::change_text(const string s)
{
	clear_text();
	
	for(unsigned int i=0; i<s.size(); i++)
		insert(char(s[i]));
}

/*
void sge_TextEditor::change_utext(const std::basic_string<Uint16> s)
{
	clear_text();
	
	for(unsigned int i=0; i<s.size(); i++)
		insert(s[i]);
}
*/


void sge_TextEditor::change_uctext(Uint16 *text)
{
	Uint16 *ch;

	clear_text();
	
	for(ch=text; *ch; ch++)
		insert(*ch);
}


void sge_TextEditor::change_textf(const char *text, ...)
{
	char buf[256];

	va_list ap;
	
	#ifdef __WIN32__
	va_start((va_list*)ap, text); //Stupid win32 crosscompiler
	#else
	va_start(ap, text);
	#endif
	
	vsprintf(buf, text, ap);
	va_end(ap);

	change_text(buf);
}




//==================================================================================
// sge_text
//==================================================================================
SDL_Surface* sge_text::get_textSurface(bool copy)
{
	if(copy)
		return sge_copy_surface(text_surface);
	else
		return text_surface;
}


bool sge_text::update_textSurface(bool force)
{
	if(text_changed || force){
#ifndef _SGE_NOTTF
		if(use_tt && tt_font){
			if(text_surface)
				SDL_FreeSurface(text_surface);
			
			Uint16* text=get_ucstring(sCursor);
			text_surface=sge_TTF_Render(tt_font, text, color, background, alpha_level);
			
			delete[] text;
			if(!text_surface)
				return false;
				
			SDL_SetColorKey(text_surface,SDL_SRCCOLORKEY,SDL_MapRGB(text_surface->format,background.r,background.g,background.b));
			
			if( alpha_level != SDL_ALPHA_OPAQUE )
				SDL_SetAlpha(text_surface, SDL_SRCALPHA, alpha_level);
				
			set_textSurface(text_surface);
		}else 
#endif
		if(bm_font){
			if(bm_font->CharPos  &&  bm_font->FontSurface->format->Amask){
				set_textSurface(NULL);
				return true;      /* These fonts can't be buffered (SFonts) */
			}

		
			if(text_surface)
				SDL_FreeSurface(text_surface);
			
			string text=get_string(sCursor);
			SDL_Rect size = sge_BF_TextSize(bm_font, (char*)(text.c_str()));	
			
			text_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, size.w, size.h, bm_font->FontSurface->format->BitsPerPixel, bm_font->FontSurface->format->Rmask, bm_font->FontSurface->format->Gmask, bm_font->FontSurface->format->Bmask, 0);
			
			if(!text_surface)
				return false;
			
			Uint32 bcol;
			
			if(bm_font->FontSurface->format->palette){
				//Set the palette
				SDL_Color c[2];
				c[0].r=bm_font->FontSurface->format->palette->colors[1].r+100; /* Whatever */
				c[0].g=bm_font->FontSurface->format->palette->colors[1].g+100;
				c[0].b=bm_font->FontSurface->format->palette->colors[1].b+100;
				
				c[1].r=bm_font->FontSurface->format->palette->colors[1].r;
				c[1].g=bm_font->FontSurface->format->palette->colors[1].g;
				c[1].b=bm_font->FontSurface->format->palette->colors[1].b;
				SDL_SetColors(text_surface, c, 0, 2);
				bcol = 0;
			}else{
				//Use the same background color as the font surface
				Uint8 r,g,b;
				SDL_GetRGB(bm_font->bcolor, bm_font->FontSurface->format, &r, &g, &b);
				bcol = SDL_MapRGB(text_surface->format, r,g,b);
				sge_ClearSurface(text_surface, bcol);
			}
			
			sge_BF_textout(text_surface, bm_font, (char*)(text.c_str()), 0, 0);
				
			SDL_SetColorKey(text_surface,SDL_SRCCOLORKEY, bcol);
			
			if( alpha_level != SDL_ALPHA_OPAQUE )
				SDL_SetAlpha(text_surface, SDL_SRCALPHA, alpha_level);
			
			set_textSurface(text_surface);
		}else
			return false;

		text_changed=false;
		return true;
	}
	
	return false;		
}


#ifndef _SGE_NOTTF
void sge_text::set_ttFont(sge_TTFont *font, Uint8 r, Uint8 g, Uint8 b, Uint8 br, Uint8 bg, Uint8 bb)
{
	color.r=r; color.g=g; color.b=b;
	background.r=br; background.g=bg; background.b=bb;
	
	tt_font=font;
	
	use_tt=true;
	update_textSurface(true);
}	
#endif


void sge_text::set_bmFont(sge_bmpFont *font)
{
	bm_font=font;
	
	use_tt=false;
	update_textSurface(true);
}


SDL_Rect sge_text::render_text(SDL_Surface *surface, Sint16 x, Sint16 y)
{
#ifndef _SGE_NOTTF
	if(use_tt && tt_font){
		SDL_Rect ret;
		ret.x=ret.y=ret.w=ret.h=0;
		
		SDL_Surface *tmp;
		Uint16* text=get_ucstring(sCursor);	
		tmp=sge_TTF_Render(tt_font, text, color, background, alpha_level);
		delete[] text;
		if(!tmp)
			return ret;
			
		SDL_SetColorKey(tmp,SDL_SRCCOLORKEY,SDL_MapRGB(tmp->format,background.r,background.g,background.b));
		if( alpha_level != SDL_ALPHA_OPAQUE )
				SDL_SetAlpha(tmp, SDL_SRCALPHA, alpha_level);
				
		sge_Blit(tmp, surface, 0, 0, x, y, tmp->w, tmp->h);
		ret.x=x; ret.y=y; ret.w=tmp->w; ret.h=tmp->h;
		
		SDL_FreeSurface(tmp);
		return ret;
	}else 
#endif
	if(bm_font){
		string text=get_string(sCursor);
		
		if( alpha_level != SDL_ALPHA_OPAQUE  &&  !bm_font->FontSurface->format->Amask)
			SDL_SetAlpha(bm_font->FontSurface,SDL_SRCALPHA, alpha_level);
			
		return sge_BF_textout(surface, bm_font, (char*)(text.c_str()), x, y);
	}else{
		SDL_Rect ret;
		ret.x=ret.y=ret.w=ret.h=0;
		return ret;
	}	
}



//==================================================================================
// sge_TextSurface
//==================================================================================
void sge_TextSurface::set_textSurface(SDL_Surface *new_surf)
{
	if(bm_font){
		if(bm_font->CharPos  &&  bm_font->FontSurface->format->Amask){
			SDL_Rect size = sge_BF_TextSize(bm_font, (char*)(get_string(sCursor).c_str()));
			current_pos.w = size.w;
			current_pos.h = size.h;
			
			return;
		}
	}
	
	surface=new_surf; 
	current_pos.w=surface->w; 
	current_pos.h=surface->h;
}

void sge_TextSurface::draw(void)
{
	if(bm_font){
		if(bm_font->CharPos  &&  bm_font->FontSurface->format->Amask){  /* These fonts can't be buffered and must be rendered */
			SDL_Rect size = render_text(dest, current_pos.x, current_pos.y);
			
			current_pos.w = size.w;
			current_pos.h = size.h;
			
			prev_pos=last_pos;
			last_pos=current_pos;
			
			return;
		}
	}

	update_textSurface();
	if(text_surface)
		sge_surface::draw();
}	



//==================================================================================
// sge_TextSsprite
//==================================================================================
void sge_TextSsprite::set_textSurface(SDL_Surface *new_surf)
{	
	if(bm_font){
		if(bm_font->CharPos  &&  bm_font->FontSurface->format->Amask){
			SDL_Rect size = sge_BF_TextSize(bm_font, (char*)(get_string(sCursor).c_str()));
			current_pos.w = size.w;
			current_pos.h = size.h;
			
			return;
		}
	}

	//Update the first frame
	sge_frame *tmp=frames.front();
	if(tmp->img==surface){
		surface=new_surf; 
		current_pos.w=surface->w; 
		current_pos.h=surface->h;
	}
	tmp->img=new_surf;
}

void sge_TextSsprite::draw(void)
{
	if(bm_font){
		if(bm_font->CharPos  &&  bm_font->FontSurface->format->Amask){  /* These fonts can't be buffered and must be rendered */
			SDL_Rect size = render_text(dest, current_pos.x, current_pos.y);
			
			current_pos.w = size.w;
			current_pos.h = size.h;
			
			prev_pos=last_pos;
			last_pos=current_pos;
			
			return;
		}
	}
	
	update_textSurface(); 
	if(text_surface){
		sge_surface::draw();
	}
}

//==================================================================================
// sge_TextSprite
//==================================================================================
void sge_TextSprite::set_textSurface(SDL_Surface *new_surf)
{
	if(bm_font){
		if(bm_font->CharPos  &&  bm_font->FontSurface->format->Amask){
			SDL_Rect size = sge_BF_TextSize(bm_font, (char*)(get_string(sCursor).c_str()));
			current_pos.w = size.w;
			current_pos.h = size.h;
			
			return;
		}
	}
	
	//Update the first frame
	sge_frame *tmp=frames.front();
	if(tmp->img==surface){
		surface=new_surf; 
		current_pos.w=surface->w; 
		current_pos.h=surface->h;
	}
	tmp->img=new_surf;	
}

void sge_TextSprite::draw(void)
{
	if(bm_font){
		if(bm_font->CharPos  &&  bm_font->FontSurface->format->Amask){  /* These fonts can't be buffered and must be rendered */
			SDL_Rect size = render_text(dest, current_pos.x, current_pos.y);
			
			current_pos.w = size.w;
			current_pos.h = size.h;
			
			prev_pos=last_pos;
			last_pos=current_pos;
			
			return;
		}
	}
	
	update_textSurface(); 
	if(text_surface){
		sge_surface::draw();
	}
}


//==================================================================================
// A helper function for lazy users: blocking text input for sge_TextSurface
// objects.
// Flags is the same as for BM and TTF input (which now uses this function)
//==================================================================================
extern Uint8 _sge_update;

int sge_text_input(sge_TextSurface *tc, Uint8 flags)
{
	if( !tc )
		return -4;
	
	Uint8 update = _sge_update;
	
	SDL_Surface *screen = tc->get_dest();
	SDL_Surface *buffer = NULL;
	
	SDL_Color bg;
	bool is_ttf = tc->get_bg(&bg);       /* No bc color indicates bitmap font */
	
	if( flags&SGE_FLAG1  ||  !is_ttf ){  /* Keep background? */
		buffer = SDL_DisplayFormat(screen);
		if(buffer==NULL){
			SDL_SetError("SGE - Out of memory");return -3;
		}
 	}
	
	/* Enable keyrepeat */
	if(!(flags&SGE_FLAG3))
 		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL+50);
	
	int uflag = SDL_EnableUNICODE(1);
 
	Sint16 x = tc->get_xpos();
	Sint16 y = tc->get_ypos();
	
	/* Draw the text for the first time */
	tc->draw();
	tc->UpdateRects();
 
	/* Main loop */ 
	SDL_Event event;
	int quit = 0;
	do{ 
 		SDL_WaitEvent(&event);
		if( event.type == SDL_QUIT ){
			quit=-1;
			break;
		}
		else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ){
			quit=-2;
			break;
		}
		else if(event.type==SDL_KEYDOWN && (event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER)){
			quit=0;
			break;
		}
		
		/* Let the text class handle the event*/ 
		if( tc->check(&event) ){ 
			/* The text has changed */
			if(buffer)
				tc->clear(buffer, x,y); //Remove the text 
			else
				tc->clear(SDL_MapRGB(screen->format, bg.r, bg.g, bg.b));
				
			tc->draw(); //Draw the new text 
			sge_Update_ON(); 
			tc->UpdateRects(); //Update screen 
			sge_Update_OFF();
		}
	}while(true);

 	if(buffer){
		tc->clear(buffer, x,y); //Remove the text
		SDL_FreeSurface(buffer);
	}

	if( !(flags&SGE_FLAG2) ){
		//Draw text without cursor
		tc->show_cursor(false);
		tc->draw();
	}
	
	sge_Update_ON(); 
	tc->UpdateRects(); //Update screen 

	_sge_update = update;

	SDL_EnableUNICODE(uflag);  //Restore unicode setting

	if( quit < 0 )
		return quit;
	else
		return tc->get_chars();
}


#endif /* _SGE_NO_CLASSES */
