#include "TextObject.h"

void TextObject::SetBGColor(SDL_Color color)
{
	bgColor = color;
}
void TextObject::SetFGColor(SDL_Color color)
{
	fgColor = color;
}

void TextObject::SetColors(SDL_Color fg, SDL_Color bg)
{
	fgColor = fg;
	bgColor = bg;
}

void TextObject::initDefaultColors()
{
	bgColor = SetColor( 0, 0, 100 );	// Default Background Color : Dark Blue
	fgColor = SetColor( 255, 255, 255 );	// Default Text Color : White
}

/// Makes the surface and updates the Rect
void TextObject::Render()
{
	surface = cFont::CreateText(text.c_str(), font,FONT_TYPE_BLENDED,fgColor,bgColor);
	UpdateRect();
}

void TextObject::Render_blended()
{
	surface = cFont::CreateText(text.c_str(), font,FONT_TYPE_BLENDED,fgColor,bgColor);
	UpdateRect();
}

void TextObject::Render_shaded()
{
	surface = cFont::CreateText(text.c_str(), font,FONT_TYPE_SHADED,fgColor,bgColor);
	UpdateRect();
}

void TextObject::Render_solid()
{
	surface = cFont::CreateText(text.c_str(), font,FONT_TYPE_SOLID,fgColor,bgColor);
	UpdateRect();
}


void TextObject::Render(TTF_Font *font)
{
	surface = cFont::CreateText(text.c_str(), font,FONT_TYPE_BLENDED,fgColor,bgColor);
	UpdateRect();
}

void TextObject::Draw(SDL_Surface *dest)
{
	SDL_BlitSurface(surface, NULL, dest, &rect );
}

void TextObject::Draw()
{
	Draw(Screen); 
}

/// Update the surface with the object's internal text and font
TextObject::TextObject()
{
	font = NULL;
	text = "";
	surface = NULL;
	initDefaultColors();
}

TextObject::TextObject(string text)
{
	initDefaultColors();
	this->text = text;	
}

/// Write text with a different font
/// This won't change the text object's default font
void TextObject::RenderDifferentFont(TTF_Font *different_font)
{
	Render(different_font);
}


/// Update's the surface and internal font and text members
TextObject::TextObject(string text, TTF_Font *font)
{
	initDefaultColors();
	this->font = font;
	this->text = text;
}

TextObject::TextObject(int x, int y)
{
	initDefaultColors();
	SetPos(x,y);
}

TextObject::TextObject(int x, int y, string text)
{
	initDefaultColors();
	this->text = text;
	SetPos(x,y);
}

TextObject::TextObject(int x, int y, string text, TTF_Font *font)
{
	initDefaultColors();
	this->font = font;
	this->text = text;
	SetPos(x,y);
	
}

/// Set the active font engine handle
/*void TextObject::SetFontEngineHandle(cFont *handle)
{
	this->font_engine_handle = handle;
}*/



void TextObject::UpdateRect(int x, int y)
{
	rect = SetRect(x,y,surface->w, surface->h);
}

void TextObject::UpdateRect()
{
	rect = SetRect(rect.x, rect.y, surface->w, surface->h);
}


void TextObject::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void TextObject::Update()
{
	UpdateRect();
}