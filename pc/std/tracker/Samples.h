#pragma once
#include <stdint.h>
#include "shared/Brr.h"
#include "shared/dsptypes.h"

/* This sample number is hardcoded for now until sucessful testing is
 * done. Later, it will be made so that the limit can be dynamically
 * increased */
#define NUM_SAMPLES 0x10
#define SAMPLE_NAME_MAXLEN 22

struct Sample
{
  char name[SAMPLE_NAME_MAXLEN]; // name of the sample
  /* pointer to first BRR block. can act as an iterator until end block is
   * found */
  /* Todo: Would like to include a WAV data. This is either the high
   * fidelity version of the sample that can be worked with (e.g.
   * EQing), or the lofi-BRR-decoded WAV also for working with */
  //Wav *wav;
  Brr *brr; // raw brr sample data. Have to iterate it to END block
  /* offset into sample where loop starts, specified by number of BRR
   * blocks or [by samples and must be divisible by 16]*/
  uint16_t rel_loop;
  /* signed offset in semitones to pitch the sample. This will be used
   * directly by the SNES driver. The tracker must impose the restraints
   * on the range allowed */
  //int8_t semitone_offset;
  //DspPitch fine_tune; // used to pitch the sample for range of +/- one semitone
private:
  /* The raw stereo volume that will be calculated from the sample /
   * instrument volume and panning settings. This might be something that
   * is actually calculated from the SNES driver and will be removed from
   * here*/
  //DspVol vol_l, vol_r;
};

//include the full from the CPP file
struct Instrument_Panel;

struct Sample_Panel
{
  /* Need a handle on the instrument panel to reference to selected
   * instrument. This access could be limited to the panel's currow member */
  Sample_Panel(Instrument_Panel *instrpanel);
  void event_handler(const SDL_Event &ev);
  void draw(SDL_Surface *screen=::render->screen);
  Text title;
  Button loadbtn, savebtn, clearbtn;
  Text_Edit_Rect *sample_name[NUM_SAMPLES];
  Sample *samples;
  static const int NUM_ROWS = 8;
  int currow = 0;
  // callback funcs for the buttons
  static int load(void *spanel);
  static int save(void *spanel);
  static int clear(void *spanel);
};
