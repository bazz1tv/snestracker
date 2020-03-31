#pragma once
#include <stdint.h>
#include "SDL.h"

class FileLoader
{
public:
  FileLoader(uint8_t mcid, size_t sc);
  
  enum ChunkID {
    SongSettings=0,
    Sample,
    Instrument,
    Pattern,
    PatSeq,
    NUM_CHUNKIDS
  };

  virtual int load(SDL_RWops *file, size_t chunksize, ChunkID cid);
  uint8_t master_chunkid;
  size_t supported_chunksize;
  
  static const constexpr int MAX_VERSIONLEN = 15;
  char fileversion[MAX_VERSIONLEN]; // the embedded version info in the file being loaded

  /* WARNING: no bounds checking!? */
  static void read_str_from_file(SDL_RWops *file, char *str_ptr, int len);

  static const char *chunkid_strs[];
  static const char *get_chunkid_str(ChunkID cid);
};
