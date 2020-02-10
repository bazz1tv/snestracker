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

class PatternSequencerGui
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
