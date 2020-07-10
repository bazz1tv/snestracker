#pragma once
#include "gui/Text_Edit_Rect.h"
#include "gui/Text.h"
#include "gui/Button.h"
#include "shared/Render.h"
#include "Instruments.h"
struct Sample_Panel;
/* That defined the Data model above. Now time to get that into a view */
struct Instrument_Panel
{
  /* Initialize the panel view from an X/Y coordinate. Additionally, we
   * need a reference to the instruments */
  Instrument_Panel(Instrument *instruments, Sample_Panel *sp);
  ~Instrument_Panel();

  int event_handler(const SDL_Event &ev);
  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);
  void set_coords(int x, int y);

  void set_currow(int c);
  void inc_currow();
  void dec_currow();

  // callback funcs for the buttons
  static int load(void *ipanel);
  static int save(void *ipanel);
  static int dup(void *ipanel);
  static int zap(void *ipanel);

  Text title;
  Button loadbtn, savebtn, dupbtn, zapbtn;
  Text instr_indices[NUM_INSTR];

  // the number of instrument rows in GUI
  static const int NUM_ROWS = 8;
  int rows_scrolled = 0;
  // the current selected row
  int currow = 0;

  // 4 is for eg. "01|\0"
  char instr_index_strings[NUM_ROWS][4];
  Text_Edit_Rect instr_names[NUM_ROWS];
  /* a direct handle on the data, rather than accessing through an API */
  Instrument *instruments;
  // handle on the sample panel to update its currow when selecting an
  // instr
  Sample_Panel *samplepanel;
  /* Todo, calculate the panel rect */
  SDL_Rect rect; // define the boundaries of the entire panel
  SDL_Rect highlight_r; // the highlight rect of current select instr
};
