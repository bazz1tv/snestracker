#pragma once
#include "shared/dsptypes.h"
#include "gui/Text_Edit_Rect.h"
#include "gui/Text.h"
#include "gui/Clickable_Text.h"
#include "gui/Button.h"
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
#define n(n,o) NOTE_ ## n ## o
#define no(o) \
  n(C, o), \
  n(CSHARP, o), \
  n(D, o), \
  n(DSHARP, o), \
  n(E, o), \
  n(F, o), \
  n(FSHARP, o), \
  n(G, o), \
  n(GSHARP, o), \
  n(A, o), \
  n(ASHARP, o), \
  n(B, o)
enum Note
{
  // SNES can go quite low (but with tremendously reduced accuracy),
  // might have to translate base octave higher. need to test if that's
  // useful
    no(1),
    no(2),
    no(3),
    no(4), // BASE OCTAVE
    no(5),
    NOTE_C6 // SNES APU limit is C 2 octaves from base
    //no(7),
    //no(8),
};
#undef n
#undef no

// the data model
struct PatternRow // defines one row of Pattern Data
{
  /*what data type to use?? Rather than hardcode a DSP pitch value, let's
   * just save the note value */
  Note note;
  int instr; /* an index into the instruments table. But how will we 
  get the handle on it?*/
  int vol_fx; /* primarily volume. note that this column may also be used for effects.
  Maybe it should be called col1 or something?*/
  int fx, fx_param;
  /* It is possible SNES Tracker will deviate from traditional tracker
   * effects commands, or add additional effects as fit */
};

#define MAX_TRACKS 8
#define MAX_PATTERNS 0x80
#define MAX_PATTERN_LEN 0x100

struct PatternSequencer
{
  int num_entries = 1;
  int sequence[MAX_PATTERNS]; // sequence of patterns defining the song
  PatternRow tprows[MAX_TRACKS][MAX_PATTERNS][MAX_PATTERN_LEN];
  // track-pattern rows. Like many other data types I have conjured, they
  // will need to later be converted to types that dynamically allocate
  // space. Or allocate max'es every time like is done now.
};

// GUI
struct PatSeqPanel // PatternSequencerPanel
{
  /*
   * +----+----+ ^ CLONE
   * | 00 | 00 | - SEQ
   * | 01 | 01 | | CLEAR
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

  PatSeqPanel(PatternSequencer *patseq);
  ~PatSeqPanel();

  void set_coords(int x, int y);
  int event_handler(const SDL_Event &ev);
  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);

  static const int VISIBLE_ROWS = 8;
  int currow = 0; /* This is not only controlled by the user, but also by
  the playback engine. At least that's the plan. */

  Button clonebtn, seqbtn, clearbtn;
  // callback funcs for the buttons
  static int clone(void *pspanel);
  static int seq(void *pspanel);
  static int clear(void *pspanel);

  Text index_text[VISIBLE_ROWS];
  char index_strings[VISIBLE_ROWS][4];
  // names.. really just the pattern numbers. I'm using the term 'names'
  // only to simplify porting from the Sample and Instrument panel code
  Clickable_Text names[VISIBLE_ROWS];
  char pattern_strings[VISIBLE_ROWS][3];
  int rows_scrolled = 0;
  SDL_Rect rect;
  SDL_Rect highlight_r; // the highlight rect of current select instr
  PatternSequencer *patseq;
};

struct PatternPanel
{
  static const int VISIBLE_ROWS = 0x20; // temp value to get started
  int currow = 0;
  char index_strings[VISIBLE_ROWS][3];
  SDL_Rect rect;
  SDL_Rect highlight_r;
  // Lots of clickable text
};
