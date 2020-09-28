#pragma once
#include "shared/dsptypes.h"
#include "gui/Text_Edit_Rect.h"
#include "gui/Text.h"
#include "gui/Clickable_Text.h"
#include "gui/Button.h"
#include <vector>
#include "ChunkLoader.h"
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

/* My thought process on how SNES Tracker will work regarding editing and
 * playback: The currently editing pattern is special in that it is fully
 * uncompressed and all edits are directly reflected in APU ram. Playing
 * back the current pattern plays back this uncompressed version. No
 * rendering required. However, for song playback, all patterns will be
 * rendered to a compressed format.
 *
 * After mulling it over, I am starting to think it's safest and best to
 * do all editing in PC memory, and anytime playback is requested, the
 * song data is compressed into the playback version in APU ram */

/* For packing the playback data. I am thinking about 2 major things
 *
 * 1. Pack note/inst/vol/fx/fxparam by using unused bits to specify what
 * bytes actually come next (similar to XM file format)
 *
 * 2. changing the pattern length per track. For some pattern, for each
 * track, a search is done to determine the last row of data. That row is
 * marked the length for that track of the pattern. During playback, the
 * track should wait for the pattern to finished (and marked free if
 * relevant to do so).
 *
 * */
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
  NOTE_NONE=0, // Note that sometimes I check for 0 rather than NOTE_NONE
	/* So if you change this, you'd have to find those checks.. but I just
	 * don't see this changing, it's not a big deal..*/
  // SNES can go quite low (but with tremendously reduced accuracy),
  // might have to translate base octave higher. need to test if that's
  // useful. After having done some testing, it won't be useful to have a
  // note table lower than 4 octaves below base. Later I will code in a
  // pitch envelope to have fun with all kinds of pitches in the entire
  // pitch range.
  no(0),
  no(1),
  no(2),
  no(3),
  no(4), // BASE OCTAVE
  no(5),
  NOTE_C6, // SNES APU limit is C 2 octaves from base
  // Additions for Fine-tune
  NOTE_CSHARP6,
  NOTE_D6
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
  uint8_t note = NOTE_NONE;
  uint8_t instr = 0; /* an index into the instruments table. But how will we
  get the handle on it?*/
  uint8_t vol = 0; /* primarily volume. note that this column may also be used for effects.
  Maybe it should be called col1 or something?*/
  uint8_t fx = 0, fxparam = 0;
  /* It is possible SNES Tracker will deviate from traditional tracker
   * effects commands, or add additional effects as fit */
};

#define MAX_TRACKS 8
#define MAX_PATTERNS 0x80
#define MIN_PATTERN_LEN 1
#define MAX_PATTERN_LEN 0x80
#define DEFAULT_PATTERN_LEN 0x20

struct Pattern
{
  uint8_t len = DEFAULT_PATTERN_LEN;
  PatternRow trackrows[MAX_TRACKS][MAX_PATTERN_LEN];
  /* this hasn't been developed yet but I know we'll be needing track
   * memory. On 2nd thought, perhaps this is needed be only the playback
   * engine */
  /*struct Memory
  {
    int last_instr = 0;
  } mem[MAX_TRACKS];*/
};

/* TODO: have PatternMeta inherit Pattern rather than own */

// Pattern with sequencer meta data
struct PatternMeta
{
  Pattern p;
  // used is meta data that is not part of the structure mapped into SNES
  // APU RAM
  int used = 0; // number of sequence entries this pattern is in
};

struct PatternSequencer
{
  PatternSequencer();

  int num_entries = 1; // how many entries are in the sequencer
  std::vector<uint8_t> sequence; // sequence of patterns defining the song

  PatternMeta patterns[MAX_PATTERNS];
  // track-pattern rows. Like many other data types I have conjured, they
  // will need to later be converted to types that dynamically allocate
  // space. Or allocate max'es every time like is done now.
  struct {
    bool *changed;
  } metadata;
};

class PatternSequencerChunkLoader : public ChunkLoader
{
public:
  PatternSequencerChunkLoader(struct PatternSequencer *patseq);
  size_t load(SDL_RWops *file, size_t chunksize);
  size_t save(SDL_RWops *file);

  enum SubChunkID {
    coreinfo=0,
    Entries,
    NUM_SUBCHUNKIDS
  };
//private:
  struct PatternSequencer *patseq;
};

class PatternChunkLoader : public ChunkLoader
{
public:
  PatternChunkLoader(struct PatternMeta *patterns);
  size_t load(SDL_RWops *file, size_t chunksize);
  size_t save(SDL_RWops *file);

  enum SubChunkID {
    coreinfo=0,
    Track,
    NUM_SUBCHUNKIDS
  };
private:
  struct PatternMeta *patterns;
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
  Text title;

  Button clonebtn, seqbtn, clearbtn, insbtn, zapbtn;
  Button incpatbtn, decpatbtn, movePatUpbtn, movePatDownbtn;
  // callback funcs for the buttons
  static int clone(void *pspanel);
  static int seq(void *pspanel);
  static int clear(void *pspanel);
  static int insertPat(void *pspanel);
  static int incpat(void *pspanel);
  static int decpat(void *pspanel);
  static int movePatUp(void *pspanel);
  static int movePatDown(void *pspanel);
  static int zapPat(void *pspanel);

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

	void set_currow(int row, bool updateScrolled=true);
  void inc_currow();
  void dec_currow();
private:
  Uint32 lastTimeScrolled;

public:
  // v0.2.1
  SDL_Rect rect_patTable;
  // PatLenWidget
  char patlen_cbuf[5];
  Text patlen_title, patlen_valtext;
  Button patlen_incbtn, patlen_decbtn;

  static int inc_patlen(void *bsaw);
  static int dec_patlen(void *bsaw);

  void update_patlen();
  // END PatLenWidget
};

struct Instrument_Panel;

struct PatternEditorPanel
{
  PatternEditorPanel(PatSeqPanel *psp, Instrument_Panel *ip);
  //~PatternEditorPanel();

  void set_coords(int x, int y);
  int event_handler(const SDL_Event &ev);
  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);

  void set_visible_rows(int rows);

  static const int MAX_VISIBLE_ROWS = 0x28;
  static int VISIBLE_ROWS;
  int currow = 0; /* This is not only controlled by the user, but also by
  the playback engine. At least that's the plan. */
  enum Highlight {
    NOTE,
    INSTR_HI,
    INSTR_LO,
    VOL_HI,
    VOL_LO,
    FX,
    FXPARAM_HI,
    FXPARAM_LO,
  };
  /*Highlight*/int highlighted_subsection=NOTE;

  int rows_scrolled = 0;
	SDL_Rect rect;
	SDL_Rect fullsize_r; // copy of rect that represents MAX_VISIBLE_ROWS dimensions
  SDL_Rect highlight_r; // the highlight rect of the row
  SDL_Rect subhighlight_r; // the highlight rect of which column (note, instr, vol etc.)
  SDL_Rect row_rects[MAX_VISIBLE_ROWS];

  struct TrackHeader {
		TrackHeader() : outline(Clickable_Rect(NULL, NULL)) { }
    Clickable_Text ctext;
    char strings[sizeof("1")];
    Clickable_Rect outline;
  } trackheader[MAX_TRACKS];

  Text index_text[MAX_VISIBLE_ROWS];
  char index_strings[MAX_PATTERN_LEN][sizeof("00")];

  struct GUITrackRow {
    Clickable_Text note_ctext[MAX_VISIBLE_ROWS];
    char note_strings[MAX_VISIBLE_ROWS][sizeof("C-4")];

    Clickable_Text instr_ctext[MAX_VISIBLE_ROWS];
    char instr_strings[MAX_VISIBLE_ROWS][sizeof("0f")];

    Clickable_Text vol_ctext[MAX_VISIBLE_ROWS];
    char vol_strings[MAX_VISIBLE_ROWS][sizeof("0f")];

    Clickable_Text fx_ctext[MAX_VISIBLE_ROWS];
    char fx_strings[MAX_VISIBLE_ROWS][sizeof("0")];

    Clickable_Text fxparam_ctext[MAX_VISIBLE_ROWS];
    char fxparam_strings[MAX_VISIBLE_ROWS][sizeof("00")];
  } guitrackrow[MAX_TRACKS];

  unsigned int cur_track :3;
  unsigned int cur_octave :3;
  unsigned int recording :1;   // recording or not
  /* by how much the cursor increments after inputting a value. I might also
   * elect to have the cursor move by this value when navigating (barring
   * 0)*/
  unsigned int addval :5; // valid range 0-0x10 (inclusive) so 5bits
  /* Whether the nav will refer to addval for movement increments */
  unsigned int move_like_addval :1;
  unsigned int pattern_wrap :1;

/* The psp is our doorway to pattern data */
  PatSeqPanel *psp;
/* The instrument panel lets us reference the currently selected instr */
  const Instrument_Panel *ip;

  void inc_addval();
  void dec_addval();
  void set_currow(int row);
	void inc_currow(int howmany=1, bool wrap=true);
  void dec_currow(int howmany=1, bool wrap=true);
private:
  void inc_curtrack();
  void dec_curtrack();

  // needs to be renamed
  void notehelper(int ndex, bool absmidi=false);
  void instrhelper(int n);
  void volhelper(int n);
  void fxhelper(int n);
  void fxparamhelper(int n);

  void events_kb_universal(const int scancode, const int mod);
  void piano_kb(int scancode, const int mod);
  void recording_kb(const int scancode, const int mod);

  /* Provide color flashing for recording mode of the subhighlight rect.
   * This is a shout to goattrk2, though I never learned to use it :\ */
  struct
  {
  public:
    // provides a cycled color
    inline Uint32 color()
    {
      cycle();
      return clr[cnt >> 1];
    }

    // these colors get set directly from PatternEditorPanel constructor
    Uint32 clr[7];

  private:
    /* cycles through the colors. The framerate is 1/2 the rate draw() is
     * called. The bit shifts are just fancy divide by 2's. If you want a
     * constant framerate for this independent of the tracker framerate,
     * you need to create a timer and all that jazz. */
    inline void cycle() {
      cnt++;
      if (cnt >> 1 == 8)
        cnt = 0;
    }

    int cnt=0;
  } flasher;
};
