#pragma once
#include "shared/dsptypes.h"
/* When a new Song is created. we need to perform a minimal SPC emulator
 * init and load the driver into SPC RAM. Then the user must be able to
 * get their BRR samples into the song. Then edit pattern data. Then play
 * back that song.
 */

/* There are 2 similar notions to document. The PatternSequencer, and the
 * Pattern. The PatternSequencer is the area that sequences different
 * patterns into a song; a linear list that maps which patterns are
 * played.
 *
 * The Pattern represents each unique pattern data
 * of the song; The main composing area where notes and effects are
 * specified.*/

// the data model
struct PatternRow // defines one row of Pattern Data
{
  /*what data type to use??*/
  DspPitch note;
  int instr; /* an index into the instruments table. But how will we 
  get the handle on it?*/
  int vol_fx; /* primarily volume. note that this column may also be used for effects.
  Maybe it should be called col1 or something?*/
  int fx, fx_param;
  /* It is possible SNES Tracker will deviate from traditional tracker
   * effects commands, or add additional effects as fit */
};

#define MAX_PATTERNS 0x80
#define MAX_TRACKS 8
#define MAX_PATTERN_LEN 0x100

struct PatternSequence
{
  int patterns[MAX_PATTERNS];
  PatternRow tprows[MAX_PATTERNS][MAX_TRACKS][MAX_PATTERN_LEN];
  // track-pattern rows. Like many other data types I have conjured, they
  // will need to later be converted to types that dynamically allocate
  // space. Or allocate max'es every time like is done now.
};

struct PatSeqPanel // PatternSequencerPanel
{
  /*
   * +----+----+ ^
   * | 00 | 00 | -
   * | 01 | 01 | |
   * | 02 | 02 | -
   * | 03 | 02 |
   * +----+----+ v
   */
  /* This is almost the same as the Instr and Sample panels, except that
   * the right side will not be TERs, but just Text labels. Well I guess
   * they could be TERs as well. What comes to mind is Renoise's wonderful
   * ability to edit such values in several ways: click and drag, dbl
   * click and manually edit value by keyboard, or edit value by GUI
   * (e.g. slider / button). This could be impl'd at some point*/

  PatSeqPanel();
  ~PatSeqPanel();
  
  struct
  {
    Text index;
    Text val_text;
    int val;
  } pdata[MAX_PATTERNS];

  static const int NUM_ROWS = 8;
  int currow = 0; /* This is not only controlled by the user, but also by
  the playback engine. At least that's the plan. */

  // callback funcs for the buttons
  static int clone(void *pspanel);
  static int seq(void *pspanel);
  static int clear(void *pspanel);

  char pattern_index_strings[NUM_ROWS];
  SDL_Rect rect;
  SDL_Rect highlight_r; // the highlight rect of current select instr

  // just like Instruments own Samples, I elect to have PatSeq
};

class PatterniSequencerModel
{
};

class PatternSequencerController
{
};

class PatternGui
{
  /*
   * +----+----+ ^
   * | 00 | 00 | -
   * | 01 | 01 | |
   * | 02 | 02 | -
   * | 03 | 02 |
   * +----+----+ v
   */ 
};

class PatternModel
{
};

class PatternController
{
};
