#include "FileLoader.h"
#include "DEBUGLOG.h"
#include <assert.h>
#include "Organization.h" // for APP_VERSION

const char *FileLoader::chunkid_strs[] = {
  "SongSettings",
  "Sample",
  "Instrument",
  "Pattern",
  "PatSeq",
};

FileLoader::FileLoader(uint8_t mcid, size_t sc) : master_chunkid(mcid), supported_chunksize(sc)
{
  fileversion[0] = 0; // default with no version string, since file has not been loaded
}

/* TODO from Tracker file loading routine: Handle chunkids that the app
 * doesn't recognize safely - skip them. */

int FileLoader::load(SDL_RWops *file, size_t chunksize, ChunkID chunkid)
{
  // include some generic top level debugging code here.
  assert(chunkid == master_chunkid);
  assert(fileversion[0] != 0); // fileversion has already been loaded
  if (chunksize > supported_chunksize)
    DEBUGLOG(
        "ChunkID: %s, chunksize $%04lX > supported_chunksize $%04lX! Perhaps your tracker software could use an update?\n",
        get_chunkid_str(chunkid), chunksize, supported_chunksize);
  else if (chunksize < supported_chunksize)
    DEBUGLOG(
        "ChunkID: %s, chunksize $%04lX < supported_chunksize $%04lX! Perhaps this track was created by an older version of the tracker software; This song was created by v%s, this tracker is v%s \n",
        get_chunkid_str(chunkid), chunksize, supported_chunksize, fileversion,
        APP_VERSION);

  return 0;
}

const char *FileLoader::get_chunkid_str(ChunkID cid)
{
  assert (cid < NUM_CHUNKIDS);
  if (cid >= NUM_CHUNKIDS)
    DEBUGLOG("Unrecognized Chunk ID: $%02x\n", cid);
  else
    return chunkid_strs[cid];
}


// Now with bounds checking :D
// Note: size includes NULL byte
void FileLoader::read_str_from_file(SDL_RWops *file, char *str_ptr, int size)
{
  assert(size > 1); // if it was one, that's only enough room for an empty string!
  do {
    SDL_RWread(file, str_ptr, 1, 1);
  } while ((--size) && *(str_ptr++) != 0);

  *str_ptr = 0; // need to write null byte incase we stopped looping cause size barrier was hit
}

/* Here's a small program to test how this works:
 *
#include <stdio.h>

void read_str_from_file(char *src, char *dest, int len)
{
do {
 *dest = *(src++);
 } while ( (--len) && *(dest++) != 0);

 *dest = 0;
 }

 int main()
 {
 char src[] = "Test";
 char dest[5];

 read_str_from_file(src, dest, 4);

 printf("%s\n", dest);
 return 0;
}*
*/
