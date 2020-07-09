#pragma once
#include "Samples.h"
#include "shared/gui/Text.h"
#include "shared/gui/Text_Edit_Rect.h"
#include "shared/gui/Button.h"

struct Sample_Panel
{
  Sample_Panel(Sample *samples);
  ~Sample_Panel();

  void run();

  enum {
    ROW_UPDATED=2
  };
  int event_handler(const SDL_Event &ev);

  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);
  void set_coords(int x, int y);
  Text title;

  Button loadbtn, savebtn, clearbtn;

  static const int NUM_ROWS = 8;
  int rows_scrolled = 0; // specified in rows

  Text sample_indices[NUM_ROWS];
  Text_Edit_Rect sample_names[NUM_ROWS];

  int currow = 0;
  void dec_currow();
  void inc_currow();
  // callback funcs for the buttons
  static int load(void *spanel);
  static int save(void *spanel);
  static int clear(void *spanel);

  // 4 is for eg. "01|\0"
  char sample_index_strings[NUM_ROWS][4];

  Sample *samples;
  SDL_Rect rect;
  SDL_Rect highlight_r; // the highlight rect of current select instr
};
