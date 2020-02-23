#pragma once
#include <stdint.h>
#include "gui/Text_Edit_Rect.h"
#include "gui/Text.h"
#include "gui/Button.h"
#include "shared/Render.h"
#include "shared/dsptypes.h"
#include "Samples.h"
/* This sample number is hardcoded for now until sucessful testing is
 * done. Later, it will be made so that the limit can be dynamically
 * increased */
#define NUM_INSTR 0x40
#define INSTR_NAME_MAXLEN 22

struct Instrument
{
  Instrument() { name[0] = 0; }
  char name[INSTR_NAME_MAXLEN]; // the name of the instrument
  /* the app-level sample data. How that gets exported into the snes
   * driver data is a different story */
  Sample samples[NUM_SAMPLES];
  /* signed offset in semitones to pitch the sample. This will be used
   * directly by the SNES driver. The tracker must impose the restraints
   * on the range allowed */
  int8_t semitone_offset;
  /* used to pitch the sample for range of +/- one semitone. Tracker
   * software must impose retraints on the allowed range of values. Will
   * be used by SNES Driver */
  DspPitch fine_tune;
  /* 0x80 = dead center, 0xFF = hard right, 0x00 = hard left */
  int8_t pan;
  Adsr adsr; // The volume envelope that will be used for this instrument
  /* Aside from the ADSR hardware volume envelope, the voice stereo volume
   * may be adjusted real-time for additional effects.*/
  /* Note on volume envelopes as I learned from Milky Tracker. The graph
   * view X-axis is specified in ticks, up to 0x100 ticks. So each point
   * of the envelope is also specified in ticks */
  //VolumeEnvelope ve;
  //PanEnvelope pe;
};


/* That defined the Data model above. Now time to get that into a view */
struct Instrument_Panel
{
  /* Initialize the panel view from an X/Y coordinate. Additionally, we
   * need a reference to the instruments */
  Instrument_Panel(Instrument *instruments);
  ~Instrument_Panel();

  int event_handler(const SDL_Event &ev);
  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);
  void set_coords(int x, int y);

  // callback funcs for the buttons
  static int load(void *ipanel);
  static int save(void *ipanel);
  static int zap(void *ipanel);

  /*enum {
    LOAD=0,
    SAVE,
    ZAP,
    NUM_BUTTONS
  };*/

  Text title;
  Button loadbtn, savebtn, zapbtn;
  Text instr_indices[NUM_INSTR];

  // the number of instrument rows in GUI
  static const int NUM_ROWS = 8;
  int rows_scrolled = 0;
  // the current selected row
  int currow = 0;

  // 4 is for eg. "01|\0"
  char instr_index_strings[NUM_ROWS][4];
  Text_Edit_Rect instr_names[NUM_ROWS]; // temporarily hard coding the number of instruments
    /* a direct handle on the data, rather than accessing through an API */
  Instrument *instruments;
  /* Todo, calculate the panel rect */
  SDL_Rect rect; // define the boundaries of the entire panel
  SDL_Rect highlight_r; // the highlight rect of current select instr
};
