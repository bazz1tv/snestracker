#pragma once

/*
 * Project is getting away from me. Need to pick an area to focus on: 
 * Song import (whatever that means)
 * project management (what?)
 * all stars (huh?)
 * feed me (??)
 * mehhhhhhhh. I guess I'm overtired.. */

#include "shared/gui/Text.h"
#include "shared/gui/Button.h"

/* TODO: Define the functions for inc/dec of each respective item, bpm
 * spd, (add already defined)
 Place bpm and spd in tracker for now */
struct Tracker;
struct PatternEditorPanel;

struct BpmSpdAddWidget
{
  BpmSpdAddWidget(Tracker *tracker, PatternEditorPanel *pep);

  void set_coords(int x, int y);
  int handle_event(const SDL_Event &ev);
  /*void one_time_draw(SDL_Surface *screen=::render->screen);*/
  void draw(SDL_Surface *screen=::render->screen);

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


/*
  struct Row
  {
    Row(const char *title,
        (*incaction)(void *data), void *incdata,
        (*decaction)(void *data), void *decdata);

    Text title, textval;
    Button incbtn, decbtn;
    unsigned int *val;
    char cbuf[4];
  } bpm, spd, add;
*/
private:
  /* The bpm and spd are really values that belong to a song. Find out
   * where to get them placed, because they really shouldn't belong to
   * this widget, rather the widget should have a handle on them. */
  /* To change the addval, we need a handle on: */
  Tracker *tracker;
  PatternEditorPanel *pep;
};

/*BpmSpdAddWidget::Row :: BpmSpdAddWidget::Row(const char *title,
  (*incaction)(void *data), void *incdata,
  (*decaction)(void *data), void *decdata) :

    title(title),
    incbtn("+", incaction, incdata),
    decbtn("-", decaction, decdata)
    val(default_value), minval(minval), maxval(maxval)
{
  sprintf(cbuf, "%d", val);
  textval.str = cbuf;
  textval.init_width_height();
}*/
