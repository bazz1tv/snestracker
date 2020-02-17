#pragma once

/* When a new Song is created. we need to perform a minimal SPC emulator
 * init and load the driver into SPC RAM. Then the user must be able to
 * get their BRR samples into the song. Then edit pattern data. Then play
 * back that song.
 */

/* BRR-Plus notes: The Loop point saved in DIR is an absolute address.
 * Assuming it is not a complex sample that loops into an external
 * address/sample, this loop location needs to be calculated as a
 * relative offset. That way, it can be dynamically loaded into snes
 * tracker.*/

/* There are 2 similar notions to document. The PatternSequencer, and the
 * Pattern. The PatternSequencer is the area that sequences different
 * patterns into a song; a linear list that maps which patterns are
 * played.
 *
 * The Pattern represents each unique pattern data
 * of the song; The main composing area where notes and effects are
 * specified.*/

class PatternSequencer_Panel
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

  Pattern_Panel();
  ~Pattern_Panel();

  Text pattern_indices[NUM_SAMPLES];
  Text pattern_vals[NUM_SAMPLES];
  static const int NUM_ROWS = 8;
  int currow = 0;

  // callback funcs for the buttons
  static int clone(void *spanel);
  static int seq(void *spanel);
  static int clear(void *spanel);

  char *pattern_index_strings;
  SDL_Rect rect;
  SDL_Rect highlight_r; // the highlight rect of current select instr
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
