#pragma once

#include "shared/gui/Text.h"
#include "shared/gui/Button.h"

struct Tracker;
struct PatternEditorPanel;

struct BpmSpdAddWidget
{
  BpmSpdAddWidget(Tracker *tracker, PatternEditorPanel *pep);

  void set_coords(int x, int y);
  int handle_event(const SDL_Event &ev);
  /*void one_time_draw(SDL_Surface *screen=::render->screen);*/
  void draw(SDL_Surface *screen=::render->screen, Uint32 bgColor=Colors::transparent);

  void updatebpm();
  void updatespd();
  void updateadd();

  char bpm_cbuf[4];
  Text bpm_title, bpm_valtext;
  Button bpm_incbtn, bpm_decbtn;
  
  char spd_cbuf[4];
  Text spd_title, spd_valtext;
  Button spd_incbtn, spd_decbtn;
  
  char add_cbuf[4];
  Text add_title, add_valtext;
  Button add_incbtn, add_decbtn;
  
  static int incbpm(void *bsaw);
  static int decbpm(void *bsaw);
  static int incspd(void *bsaw);
  static int decspd(void *bsaw);
  static int incadd(void *bsaw);
  static int decadd(void *bsaw);

  SDL_Rect rect;
private:
  Tracker *tracker;
  PatternEditorPanel *pep;
};
