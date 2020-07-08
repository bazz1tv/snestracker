#pragma once
#include "DEBUGLOG.h"
#include "SongSettings.h"
#include "Samples.h"
#include "Instruments.h"
#include "Pattern.h"
#include <assert.h>

/* Song File Format
~~~~~~~~~~~~~~~~~~~

The goal is to have a fileformat that continues to be within reason back/forward compatible,
easily extendable when new features and components are added.

All integers spanning more than 1 byte are stored little-endian unless
otherwise noted.

The following field starts the file

"STSong"   -- file magic header string. 6 bytes. not null terminated

The rest of the data is organized into chunks. Each chunk of data is comprised of
a 1-byte ID, a 16-bit length field, followed by the actual data corresponding to
that ID. The length field describes the length of data that comes immediately after it.

There are times when the ordering of chunks is important.
Currently the Version chunk must come immediately after the file header string.
The order presented below is the only supported ordering, although it might work
out of order (besides the version chunk).

Version Chunk
  coreinfo
    Song file version. 3 16-bit unsigned integers (Form into major.minor.micro)
    App version. The version of snestracker that saved this file. Same format as previous.
    [EXTENDABLE]
  commithash
    TODO
    [EXTENDABLE]
  [EXTENDABLE]

SongSettings Chunk
  songtitle
    non-null terminated string of song title
  bpmspd
    bpm/spd    -- 16-bit: (BPM << 6) | Spd
                  (0-300)   (1-32?)
                ----BPM---- ==SPD==
                0 0000 0000 0000 00
    [EXTENDABLE]
  volandecho
    mvol       -- 1 byte
    evol       -- 1 byte
    edl        -- 1 byte
    efb        -- 1 byte
    [EXTENDABLE]
  [EXTENDABLE]

Sample Chunk
  songmeta
    index       -- 1 byte, the index this sample belongs to
    [EXTENDABLE]
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
  songmeta
    index       -- 1 byte, the index this instrument belongs
    srcn        -- 1 byte, the sample number connected to this instrument
    flags       -- 1 byte, holds on-off state (eg echo). Added in 0.1.1
    [EXTENDABLE]
  coreinfo
    vol         -- 1 byte, volume
    pan         -- 1 byte
    adsr        -- 2 bytes, hardware ADSR envelope setting
    [EXTENDABLE]
  name
    non-null terminated string of instrument name
  tune
    finetune        -- 1 byte
    [EXTENDABLE]
  [EXTENDABLE]

Pattern Chunk
  coreinfo,
    index       -- 1 byte, pattern index
    len         -- 1 byte, pattern length
  Track (Chunk, will appear multiple times)
    coreinfo
      track type  -- allows to specify different kinds of tracks. Currently only "voice" is valid (0)
      index       -- track index (0-7)
    Data
      Compressed PatternRow data (when this data format is updated, update the Song version number)
    [EXTENDABLE]
  [EXTENDABLE]

Pattern Sequencer Chunk
  coreinfo (may not be present)
    [EXTENDABLE]
  Entries
    a sequence of byte sized pattern indices to make up the pattern sequencer.
  [EXTENDABLE]

[EXTENDABLE]
*/


// Song File info
//////////////////////////////////////////////////////////////////
#define SONGFILE_EXT "sts,stp"

#define SONGFILE_VER_MAJOR 0
#define SONGFILE_VER_MINOR 1
#define SONGFILE_VER_MICRO 1
// concats the 3 version values into a string
#define SONGFILE_VER_STRING(maj, min, mic) #maj "." #min "." #mic
// allows calling the above macro with macro arguments
#define SONGFILE_VER_STRING2(maj, min, mic) SONGFILE_VER_STRING(maj, min, mic)
// The final actual "public" macro for a version string
#define SONGFILE_VERSION SONGFILE_VER_STRING2(SONGFILE_VER_MAJOR, SONGFILE_VER_MINOR, SONGFILE_VER_MICRO)
//////////////////////////////////////////////////////////////////

struct Song
{
  Song()
  {
    int i;
    for (i=0; i < NUM_SAMPLES; i++)
      samples[i].metadata.changed = &changed;
    for (i=0; i < NUM_INSTR; i++)
      instruments[i].metadata.changed = &changed;
    patseq.metadata.changed = &changed;

    settings.metadata.changed = &changed;
  }
  void reset();
  void load(SDL_RWops *file);
  void save(SDL_RWops *file);

  Sample samples[NUM_SAMPLES];
  Instrument instruments[NUM_INSTR];
  PatternSequencer patseq;
  SongSettings settings;
//private:
  bool changed; // used to track changes to the song
/* "changed" is distributed to Song subcomponents in the following manner:
 -----------------
|                 |
|  Song::changed  |
|                 |
 -----------------
   |   |   |   |
   |   |   |   |
   |   |   |   |
   |   |    \   \------<-- *Sample::metadata.changed
   |   |     ------<------ *Instrument::metadata.changed
    \   \-------<--------- *PatternSequencer::metadata.changed
     ------------<-------- *SongSettings::metadata.changed
*/
};

class SongFileLoader
{
public:
  static const char constexpr HeaderStr[] = "STSong";
  STATIC_HEADER_CHECK(HeaderStr, FILE_HEADER_LEN);

  /* TODO: define outside class Song, and use that instead of all of these structures as dependency */
  SongFileLoader(Song *song);

  ~SongFileLoader();

  enum ret_t {
    HEADER_OK=0,
    HEADER_OLD,
    HEADER_BAD,
    FILE_LOADED,
    FILE_NOT_LOADED,
  };

  // convenience function
  size_t save(SDL_RWops *file);

  ret_t load(SDL_RWops *file);

private:
  ret_t readHeader(SDL_RWops *file);

/* Here lies a collection of subclassed FileLoaders that are associated with
handling a particular filetype. In this case, Song. */
  VersionChunkLoader *vcl;
  SongSettingsChunkLoader *sscl;
  SampleChunkLoader *scl;
  InstrumentChunkLoader *icl;
  PatternChunkLoader *pcl;
  PatternSequencerChunkLoader *pscl;


  int loadOld(SDL_RWops *file); // load the oldest file format
/* Need a handle on these guys for loadOld() */
  Song *song;
};
