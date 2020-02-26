#pragma once

#include "shared/gui/Text.h"
#include "shared/gui/Button.h"

struct Instrument_Panel;

struct InstrumentEditor
{
  InstrumentEditor(Instrument_Panel *instrpanel);

  void set_coords(int x, int y);
  int handle_event(const SDL_Event &ev);
  /*void one_time_draw(SDL_Surface *screen=::render->screen);*/
  void draw(SDL_Surface *screen=::render->screen);

  void update_vol();
  char vol_cbuf[4];
  Text vol_title, vol_valtext;
  Button vol_incbtn, vol_decbtn;
  static int incvol(void *i);
  static int decvol(void *i);

  void update_pan();
  char pan_cbuf[4];
  Text pan_title, pan_valtext;
  Button pan_incbtn, pan_decbtn;
  static int incpan(void *i);
  static int decpan(void *i);

  void update_finetune();
  char finetune_cbuf[5];
  Text finetune_title, finetune_valtext;
  Button finetune_incbtn, finetune_decbtn;
  static int incfinetune(void *i);
  static int decfinetune(void *i);

private:
  Instrument_Panel *instrpanel;
};
