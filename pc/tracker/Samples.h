#pragma once
#include <stdint.h>
#include "shared/Brr.h"
#include "shared/dsptypes.h"
#include "SDL.h"
#include "ChunkLoader.h"
/* This sample number is hardcoded for now until sucessful testing is
 * done. Later, it will be made so that the limit can be dynamically
 * increased */
#define NUM_SAMPLES 0x100
#define SAMPLE_NAME_MAXLEN 22
// Shortened the length for new GUI layout (v0.2.1)
#define SAMPLE_NAME_GUI_CHAR_WIDTH (22 - 3)

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

  bool identical(const Brr *brr);
  void clear();

  struct {
    bool *changed;
    bool loop;      // whether or not this sample BRR end block should set loop on
  } metadata;

  bool operator==(const Sample& rhs);
  inline bool operator!=(const Sample& rhs) { return !(*this == rhs); }
};

/* Used for loading / storing sample data and metadata as chunks from/to a file. */
class SampleChunkLoader : public ChunkLoader
{
public:
  SampleChunkLoader(struct Sample *samples, bool ignoreSongMeta=false);
  size_t load(SDL_RWops *file, size_t chunksize);
  size_t save(SDL_RWops *file);
  size_t save(SDL_RWops *file, int sampIdx);
  // Helper for non-Song loading
  size_t save(SDL_RWops *file, struct Sample *s);

  inline void setIdx(uint8_t idx) { this->idx = idx; idx_loaded = true; }

  enum SubChunkID {
    coreinfo=0,
    songmeta,
    name,
    brr,
    tune,
    NUM_SUBCHUNKIDS
  };

  int instr_srcn;
private:
  struct Sample *samples;
  bool ignoreSongMeta;
  uint8_t idx = 0; //sample index
  bool idx_loaded = false;
};
