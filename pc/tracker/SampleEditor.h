#pragma once

#include "shared/gui/Text.h"
#include "shared/gui/Button.h"
#include "shared/gui/Checkbox.h"

struct SampleEditor
{
  SampleEditor();

  void set_coords(int x, int y);
  int handle_event(const SDL_Event &ev);
  void draw(SDL_Surface *screen=::render->screen);

	void update_loop();
	char loop_cbuf[sizeof("$1000")];
	Text loop_title, loop_valtext;
	Button loop_incbtn, loop_decbtn;
	Checkbox loopCheckbox;
	static int incloop(void *i);
	static int decloop(void *i);

	void update_finetune();
	char finetune_cbuf[5];
	Text finetune_title, finetune_valtext;
	Button finetune_incbtn, finetune_decbtn;
	static int incfinetune(void *i);
	static int decfinetune(void *i);
private:
	int x,y; // save coords of set_coords() for internal resizing
};
