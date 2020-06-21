#pragma once
#include <stdint.h>
#include "SDL.h"

/* The ChunkLoader class works as follows
Each CPP Module that can be loaded from a file contains a ChunkLoader subclass
that inherits ChunkLoader. This subclass identifies with a particular
ChunkLoader::ChunkID that must be defined in advance in the parent class.
See Instruments.h for a working example, among the many others referenced from
Song.h.

When you go to load a file, first your own function somehow gets an SDL_RWops handle
on the file, reads in the proper header data string (must be FILE_HEADER_LEN bytes long),
and finally instantiates the relevant ChunkLoader subclasses, and calls ChunkLoader::loadchunks().
See Song.h's SongFileLoader class for a great working example.

Chunks themselves have their own header (preamble data), which is simply the
(uint8_t) chunkID followed by the (uint16_t) chunksize. There is currently no
support for different sizes of these fields.

All of the data is assumed little endian and there is no proper endian check during
loading or storing. This is not a problem for the supported platforms.

The top-level ChunkLoader loadchunks() will read the ID, and use it to call the
subclassed (ChunkLoader *) associated with that chunk id.
*/

/////// File Format Restrictions /////////
/* There is a fixed file header string length of 6 bytes, eg STSONG. This does
not include the NULL byte needed for buffers holding this string */
#define FILE_HEADER_LEN 6

/* static safety check to put in FileLoader classes (Song, Instrument). Ensures
that the file header string is the appropriate size */
#define STATIC_HEADER_CHECK_PRIVATE(HeaderStr, len)\
  static_assert(sizeof(HeaderStr) == (len + 1),\
  "File Header String must be " #len " bytes in length. (" #len "+1 for the NULL BYTE)" );\
  static_assert(HeaderStr[0] != '\0' && HeaderStr[1] != '\0' && HeaderStr[2] != '\0' &&\
                HeaderStr[3] != '\0' && HeaderStr[4] != '\0' && HeaderStr[5] != '\0',\
  "File Header String must use all " #len " bytes. Use spaces for unused characters")
/* The private macro is layered underneath the actual CHECK_HEADER macro, so that
macro arguments can be used */
#define STATIC_HEADER_CHECK(HeaderStr, len) STATIC_HEADER_CHECK_PRIVATE(HeaderStr, len)
//////////////////////////////////////////


class ChunkLoader
{
public:
  ChunkLoader(uint8_t chunkid);
  virtual ~ChunkLoader();

  /* A ChunkLoader subclass will load this member from its constructor, to associate
   * itself with a chunkid number. */
  const uint8_t chunkid;

  // scoped to 8-bits
  enum ChunkID {
    SongSettings=0,
    Sample,
    Instrument,
    Pattern,
    PatternSequencer,
    Version,
// Always add entries at the end of the list, in order not to disturb the order
    NUM_CHUNKIDS,
    UNUSED_CID=0xff
  };

  /* Active (instantiated) subclasses get automatically added to this mapping of
   * chunkid to a ChunkLoader */
  static ChunkLoader * ChunkIdMap[NUM_CHUNKIDS];
  /* Call this after positioning your file pointer to the first chunk (after reading
  the header string). It will automatically call the associated handler for chunkids
  it encounters, and skip unrecognized chunks */
  static size_t loadchunks(SDL_RWops *file);

  /* This isn't used yet but could become useful. It returns the location of a
  specified top-level chunkid, without disturbing the file pointer position */
  static size_t findChunk(SDL_RWops *file, uint8_t cid);

  /* Every subclassed ChunkLoader must implement these 2 functions. Note, in my
  practice it is easier to first implement the save(), then use that as the guide
  for load(). Also refer to my other ChunkLoaders for reference. */
  virtual size_t load(SDL_RWops *file, size_t chunksize) = 0;
  virtual size_t save(SDL_RWops *file) = 0;

  /* The following group of functions provide wrappers over SDL_RWread/write. They
  update an external bytecount variable. ChunkLoader Subclasses use these functions
  from their load() / save() to keep track of bytes read or written as they perform
  their subchunk loading. */
  static size_t read (struct SDL_RWops* context,
                      void*             ptr,
                      size_t            size,
                      size_t            maxnum,
                      size_t            *bytecount);

  static size_t write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint32_t *chunksize_counter);
  static size_t write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint16_t *chunksize_counter);

  /* Reads a (non \0 delimited) string of chunksize bytes into a buffer of bufsize
  (including presence of null byte) bytes. The null byte will be appended to the string */
  /* This function is used for subchunks that carry a sole string */
  static size_t read_str_from_file2(SDL_RWops *file, char *str_ptr, int chunksize, int bufsize);

protected:
  /* Allows subclasses to signal to the ChunkLoader to stop loading chunks. For example,
  when the user is prompted whether to attempt loading a file of a newer file format, and chooses
  'NO' */
  static bool StopLoading;
};

/* Here lies the VersionChunkLoader class that can be used for different file types.
It is currently working for Song files and can accomodate any filetype where you
intend on classifying its version as a major.minor.micro version specification.
See how it can be used from SongFileLoader in Song.h */
class VersionChunkLoader : public ChunkLoader
{
public:
  typedef uint16_t version_t; // currently each number of the version is 16-bits
  VersionChunkLoader(version_t maj, version_t min, version_t mic);

  size_t save(SDL_RWops *file);
  size_t load(SDL_RWops *file, size_t chunksize);

  enum SubChunkID {
    coreinfo=0,
    commithash, // TODO
    NUM_SUBCHUNKIDS
  };
private:
  version_t maj, min, mic;
};