#pragma once
#include <stdint.h>
#include "ChunkLoader.h"
#include "shared/dsptypes.h"
/* This number is hardcoded for now until sucessful testing is
 * done. Later, it will be made so that the limit can be dynamically
 * increased */
#define NUM_INSTR 0x40
#define INSTR_NAME_MAXLEN 22
// Shortened the length for new GUI layout (v0.2.1)
#define INSTR_NAME_GUI_CHAR_WIDTH (22 - 3)
#define APU_MAXVOL 0x7f

// TODO : take out meta info from Instrument into a InstrumentMeta
// datatype that inherits from Instrument. that should allow simplifying
// render_to_apu into memcpy on Instrument type into APU RAM
struct Instrument
{
  Instrument();
  ~Instrument();
  void reset();
  /* TODO: Move this metadata outside of the core instrument */
  char name[INSTR_NAME_MAXLEN]; // the name of the instrument
  /* the app-level sample data. How that gets exported into the snes
   * driver data is a different story */
	uint8_t srcn; // like a DIR offset
	Adsr adsr; // The volume envelope that will be used for this instrument
  static const uint16_t constexpr DEFAULT_ADSR = 0xe0ff;
	/* Aside from the ADSR hardware volume envelope, the voice stereo volume
	 * may be adjusted real-time for additional effects.*/
	/* Note on volume envelopes as I learned from Milky Tracker. The graph
	 * view X-axis is specified in ticks, up to 0x100 ticks. So each point
	 * of the envelope is also specified in ticks */
	//VolumeEnvelope ve;
	//PanEnvelope pe;
	//  bool phaseflip=false;
  uint8_t vol;
  static const uint8_t constexpr DEFAULT_VOL = 0x50;
  /* -64 = hard left, 0 = dead center, 64 = hard right,  */
  int8_t pan;
  static const uint8_t constexpr DEFAULT_PAN = 0x00;
  /* signed offset in semitones to pitch the sample. This will be used
   * directly by the SNES driver. The tracker must impose the restraints
   * on the range allowed */
  int8_t semitone_offset;
  /* used to pitch the sample for range of +/- one semitone. Tracker
   * software must impose retraints on the allowed range of values. Will
   * be used by SNES Driver */
  int8_t finetune;

  bool operator==(const Instrument& rhs);
  inline bool operator!=(const Instrument& rhs) { return !(*this == rhs); }

	static void inc_srcn(Instrument *i);
	static void dec_srcn(Instrument *i);
  static void inc_vol(Instrument *i);
  static void dec_vol(Instrument *i);
  static void inc_pan(Instrument *i);
  static void dec_pan(Instrument *i);
  static void inc_finetune(Instrument *i);
  static void dec_finetune(Instrument *i);

  struct {
    bool *changed;
  } metadata;

  /* whether or not echo is enabled for this instrument. Currently uses
   * echo settings specified in your SongSettings */
  bool echo;
};

struct ApuInstr
{
  uint8_t vol;
  int8_t finetune;
  int8_t pan;
  uint8_t srcn;
  uint8_t adsr1, adsr2;
  uint8_t flags; // echo
  int8_t semitone_offset;
};

class InstrumentChunkLoader : public ChunkLoader
{
public:
  InstrumentChunkLoader(struct Instrument *instruments, bool ignoreSongMeta=false);
  size_t load(SDL_RWops *file, size_t chunksize);
  // save specific instrument to file handle. This does not include a File Header
  size_t save(SDL_RWops *file, int instNumber);
  size_t save(SDL_RWops *file); // Save all instruments to file handle
  // Helper for non-Song loading
  size_t save(SDL_RWops *file, struct Instrument *instr);

  inline void setIdx(uint8_t idx) { this->idx = idx; idx_loaded = true; }

  enum SubChunkID {
    coreinfo=0,
    songmeta,
    name,
    tune,
    NUM_SUBCHUNKIDS
  };
private:
  struct Instrument *instruments;
  bool ignoreSongMeta;
  uint8_t idx = 0; //instrument index
  bool idx_loaded = false;
};

////////// STI INSTRUMENTS //////////////////////////////////////
/* Instrument File Format
~~~~~~~~~~~~~~~~~~~~~~~~~

It uses the same structures and routines invented for the Song File Format (Song.h)
Basically it is the same format, just with the removal of unneeded chunks and
songmeta subchunks.

All integers spanning more than 1 byte are stored little-endian unless
otherwise noted.

The following field starts the file

"STInst"   -- file magic header string. 6 bytes. not null terminated

The rest of the data is organized into chunks. Each chunk of data is comprised of
a 1-byte ID, a 16-bit length field, followed by the actual data corresponding to
that ID. The length field describes the length of data that comes immediately after it.

There are times when the ordering of chunks is important.
Currently the Version chunk must come immediately after the file header string.
And, the Sample brr subchunk must be first.
The order presented below is the only supported ordering.

Version Chunk
  coreinfo
    file version. 3 16-bit unsigned integers (Form into major.minor.micro)
    App version. The version of snestracker that saved this file. Same format as previous.
    [EXTENDABLE]
  commithash
    TODO
    [EXTENDABLE]
  [EXTENDABLE]

Sample Chunk
  brr
    raw sample data
  coreinfo
    rel_loop    -- 2 bytes, the relative offset of loop start
    [EXTENDABLE]
  name
    non-null terminated string of sample name
  tune
    finetune        -- 1 byte (impl TODO)
    semitone_offset -- 1 byte (impl TODO) (REMOVABLE)
    [EXTENDABLE]
  [EXTENDABLE]

Instrument Chunk
  coreinfo
    vol         -- 1 byte, volume
    pan         -- 1 byte (impl TODO)
    adsr        -- 2 bytes, hardware ADSR envelope setting
    [EXTENDABLE]
  name
    non-null terminated string of instrument name
  tune
    finetune        -- 1 byte (impl TODO)
    [EXTENDABLE]
  [EXTENDABLE]
*/

// Instrument File info
//////////////////////////////////////////////////////////////////
#define INSTRFILE_EXT "sti"

#define INSTRFILE_VER_MAJOR 0
#define INSTRFILE_VER_MINOR 0
#define INSTRFILE_VER_MICRO 1
// concats the 3 version values into a string
#define INSTRFILE_VER_STRING(maj, min, mic) #maj "." #min "." #mic
// allows calling the above macro with macro arguments
#define INSTRFILE_VER_STRING2(maj, min, mic) INSTRFILE_VER_STRING(maj, min, mic)
// The final actual "public" macro for a version string
#define INSTRFILE_VERSION INSTRFILE_VER_STRING2(INSTRFILE_VER_MAJOR, INSTRFILE_VER_MINOR, INSTRFILE_VER_MICRO)
//////////////////////////////////////////////////////////////////

struct Sample;
class SampleChunkLoader;
class InstrumentFileLoader
{
public:
  static const char constexpr HeaderStr[] = "STInst";
  STATIC_HEADER_CHECK(HeaderStr, FILE_HEADER_LEN);

  InstrumentFileLoader(Instrument *instrument, Sample *sample);
  ~InstrumentFileLoader();

  enum ret_t {
    HEADER_OK=0,
//    HEADER_OLD,
    HEADER_BAD,
    FILE_LOADED,
    FILE_NOT_LOADED,
  };

  size_t save(SDL_RWops *file);
  ret_t load(SDL_RWops *file);

private:
  ret_t readHeader(SDL_RWops *file);

/* Here lies a collection of subclassed FileLoaders that are associated with
 * this file type. */
  VersionChunkLoader *vcl;
  SampleChunkLoader *scl;
  InstrumentChunkLoader *icl;

  Instrument *instrument;
};
