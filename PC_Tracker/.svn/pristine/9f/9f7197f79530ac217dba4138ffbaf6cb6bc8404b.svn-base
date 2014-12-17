#pragma once

#include "Globals.h"

/// To write Text requires the cFont class to be activated.
class TextObject : public GraphicsObject
{
public:
	string text;
	TTF_Font *font;
	SDL_Color fgColor;
	SDL_Color bgColor;
	SDL_bool drawtxt;


	TextObject();
	~TextObject();
	
	TextObject(string text);
	TextObject(string text, TTF_Font *font);
	TextObject(int x, int y);
	TextObject(int x, int y, string text);
	TextObject(int x, int y, string text, TTF_Font *font);


	void initDefaultColors();
	void SetBGColor(SDL_Color color);
	void SetFGColor(SDL_Color color);
	void SetColors(SDL_Color fg, SDL_Color bg);



	void RenderDifferentFont(TTF_Font *font);

	void preRender();
	void Render();
	void Render(TTF_Font *font);
	void Render_shaded();
	void Render_solid();
	void Render_blended();

	void SetFont(cFont *handle);

	void SetPos(int x, int y);

	void Update();
	void UpdateRect();
	void UpdateRect(int x, int y);

	void Draw();
	void Draw(SDL_Renderer *);
};