#pragma once
#include <stdint.h>
#include "shared/Brr.h"
#include "shared/dsptypes.h"
#include "SDL.h"
#include "shared/gui/Text.h"
#include "shared/gui/Text_Edit_Rect.h"
#include "shared/gui/Button.h"
#include "shared/Render.h"
#include "shared/utility.h"
#include "ChunkLoader.h"
/* This sample number is hardcoded for now until sucessful testing is
 * done. Later, it will be made so that the limit can be dynamically
 * increased */
#define NUM_SAMPLES 0x100
#define SAMPLE_NAME_MAXLEN 22

struct Sample
{
  Sample();
  ~Sample();
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
  uint16_t brrsize;
  uint16_t rel_loop;
  /* signed offset in semitones to pitch the sample. This will be used
   * directly by the SNES driver. The tracker must impose the restraints
   * on the range allowed */
  int8_t semitone_offset; // TODO
	int8_t finetune;

	void inc_loop();
	void dec_loop();
	void inc_finetune();
	void dec_finetune();
};

/* Used for loading / storing sample data and metadata as chunks from/to a file. */
class SampleChunkLoader : public ChunkLoader
{
public:
  SampleChunkLoader(struct Sample *samples);
  size_t load(SDL_RWops *file, size_t chunksize);
  size_t save(SDL_RWops *file);

  enum SubChunkID {
    coreinfo=0,
    name,
    brr,
    NUM_SUBCHUNKIDS
  };
private:
  struct Sample *samples;
};

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
