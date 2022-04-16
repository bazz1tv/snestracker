#include "ChunkLoader.h"
#include "DEBUGLOG.h"
#include <assert.h>
#include "Organization.h" // for APP_VERSION, APP_NAME

/* I hate doing this, but having the ChunkLoader depend on DialogBox so I can
report to user direct from here for easier coding burden */
#include "gui/DialogBox.h"

ChunkLoader * ChunkLoader::ChunkIdMap[NUM_CHUNKIDS];
bool ChunkLoader::StopLoading;

// Adds child class impl of a chunkid
ChunkLoader::ChunkLoader(uint8_t chunkid) : chunkid(chunkid)
{
  ChunkIdMap[chunkid] = this;
}

// removes child class impl from the map
ChunkLoader::~ChunkLoader()
{
  ChunkIdMap[chunkid] = NULL;
}

size_t ChunkLoader::loadchunks(SDL_RWops *file)
{
  uint8_t chunkid;
  uint16_t chunksize;

  StopLoading = false;

  while (!StopLoading)
  {
    //DEBUGLOG("Top-Level LoadChunk\n");
    if (SDL_RWread(file, &chunkid, 1, 1) == 0)
    {
      DEBUGLOG("EOF\n");
      break;
    }
    if (SDL_RWread(file, &chunksize, 2, 1) == 0)
    {
      DEBUGLOG("EOF\n");
      break;
    }

    if (chunkid >= NUM_CHUNKIDS) // this chunkid is not recognized
    {
      DEBUGLOG("UNRECOGNIZED CHUNK: %d, skipping\n", chunkid);
      SDL_RWseek(file, chunksize, RW_SEEK_CUR);
      continue;
    }

    //DEBUGLOG("Chunk: %d, size: %d\n", chunkid, chunksize);
    if (ChunkIdMap[chunkid] == NULL)
    {
      DEBUGLOG("UNMAPPED CHUNK: %d, skipping\n", chunkid);
      SDL_RWseek(file, chunksize, RW_SEEK_CUR);
    }
    else
    {
      /* TODO: make use of a return value from load() for error feedback */
      ChunkIdMap[chunkid]->load(file, chunksize);
    }
  }

  return 0;
}

/* Return codes: 0 == FAIL, not 0 == location of chunk.
(There can never be at chunk at 0 because that is where the file header string
is) */
size_t ChunkLoader::findChunk(SDL_RWops *file, uint8_t cid)
{
  uint8_t scannedCid = UNUSED_CID;
  size_t bytesRead = 0;

  // log our starting location
  Sint64 startLocation, matchLocation = 0;
  startLocation = SDL_RWtell(file);

  SDL_RWseek(file, FILE_HEADER_LEN, RW_SEEK_SET); // go to beginning of file, skipping header

  // while we havent found a match and haven't read the whole chunk
  uint16_t scannedSize;
  for (size_t notEOF=1; scannedCid != cid && notEOF == true;
       SDL_RWseek(file, scannedSize, RW_SEEK_CUR) )
  {
    matchLocation = SDL_RWtell(file);
    notEOF = read(file, &scannedCid, 1, 1, &bytesRead);
    if (scannedCid >= NUM_CHUNKIDS)
    {
      // Something is seriously wrong here :( File appears corrupted
      DEBUGLOG("Invalid chunk id: %d, the file could be corrupted.\n", scannedCid);
      break;
    }
    notEOF = read(file, &scannedSize, 2, 1, &bytesRead);
  }
  // Go back to where we started
  SDL_RWseek(file, startLocation, RW_SEEK_SET);

  return scannedCid == cid ? matchLocation : 0;
}

size_t ChunkLoader::read (struct SDL_RWops* context,
                         void*             ptr,
                         size_t            size,
                         size_t            maxnum,
                         size_t            *bytecount)
{
  size_t ret = SDL_RWread(context, ptr, size, maxnum);
  if (ret != maxnum)
  {
    DEBUGLOG("Couldn't fully read data; Read %lu of %lu bytes\n", ret * size, maxnum * size);
  }
  *bytecount += ret * size;
  return ret;
}

size_t ChunkLoader::write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint32_t *chunksize_counter)
{
  size_t ret = SDL_RWwrite(context, ptr, size, num);
  if (ret != num)
  {
    DEBUGLOG("Couldn't fully write data; Wrote %lu of %lu bytes\n", ret * size, num * size);
  }
  *chunksize_counter += ret * size;
  return ret;
}
size_t ChunkLoader::write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint16_t *chunksize_counter)
{
  size_t ret = SDL_RWwrite(context, ptr, size, num);
  if (ret != num)
  {
    DEBUGLOG("Couldn't fully write data; Wrote %lu of %lu bytes\n", ret * size, num * size);
  }
  *chunksize_counter += ret * size;
  return ret;
}

/* Reads a (non \0 delimited) string of chunksize bytes into a buffer of bufsize
(including presence of null byte) bytes. The null byte will be appended to the string */
size_t ChunkLoader::read_str_from_file2(SDL_RWops *file, char *str_ptr, int chunksize, int bufsize)
{
  size_t count = 0;
  assert(chunksize > 0 && bufsize > 1); // if it was one, that's only enough room for an empty string!
  do {
    SDL_RWread(file, str_ptr++, 1, 1);
    count++;
  } while ((--chunksize) && (--bufsize - 1));

  *str_ptr = 0; // need to write null byte incase we stopped looping cause size barrier was hit
  return count;
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



#define VERSION_COREINFO_SIZE ( (sizeof(maj) + sizeof(min) + sizeof(mic)) * 2 )

VersionChunkLoader::VersionChunkLoader(version_t maj, version_t min, version_t mic) :
  ChunkLoader(ChunkID::Version), maj(maj), min(min), mic(mic)
{}

size_t VersionChunkLoader::save(SDL_RWops *file)
{
  uint8_t byte;
  uint16_t word;
  uint16_t chunklen = 0;
  Sint64 chunksize_location, chunkend_location;

  DEBUGLOG("VersionChunkLoader::save()\n");

///////////// 1 //////////////////////
  DEBUGLOG("\tWriting top-level chunkid: %d\n", chunkid);
  // write this master chunk id
  byte = chunkid;
  SDL_RWwrite(file, &byte, 1, 1);

  // stub the master chunksize for later writing
  chunksize_location = SDL_RWtell(file);
  SDL_RWwrite(file, &chunklen, 2, 1);
///////////// 1 //////////////////////

///////////// 2.1 ////////////////////
  DEBUGLOG("\t\tSubChunkID::coreinfo\n");
  // write the ID
  byte = SubChunkID::coreinfo;
  write(file, &byte, 1, 1, &chunklen);
  // write the chunksize
  word = VERSION_COREINFO_SIZE;
  write(file, &word, 2, 1, &chunklen);
  // write the chunk data (file format version)
  write(file, &maj, 2, 1, &chunklen);
  write(file, &min, 2, 1, &chunklen);
  write(file, &mic, 2, 1, &chunklen);
  // write the app version
  word = APP_VER_MAJOR;
  write(file, &word, 2, 1, &chunklen);
  word = APP_VER_MINOR;
  write(file, &word, 2, 1, &chunklen);
  word = APP_VER_MICRO;
  write(file, &word, 2, 1, &chunklen);
///////////// 2.1 ////////////////////
  DEBUGLOG("\tWriting chunksize\n");
  chunkend_location = SDL_RWtell(file);
  SDL_RWseek(file, chunksize_location, RW_SEEK_SET);
  SDL_RWwrite(file, &chunklen, 2, 1);
  SDL_RWseek(file, chunkend_location, RW_SEEK_SET);
  return 0;
}

size_t VersionChunkLoader::load(SDL_RWops *file, size_t chunksize)
{
  size_t maxread = 0;

  DEBUGLOG("VersionChunkLoader::load()\n");

  while (maxread < chunksize)
  {
    uint8_t subchunkid;
    uint16_t subchunksize;

    //DEBUGLOG("maxread = %zu\n", maxread);

    if (read(file, &subchunkid, 1, 1, &maxread) == 0)
      break;
    if (read(file, &subchunksize, 2, 1, &maxread) == 0)
      break;

    //DEBUGLOG("subchunksize = %hu\n", subchunksize);

    switch (subchunkid)
    {
      case SubChunkID::coreinfo:
      {
        DEBUGLOG("\tSubChunkID::coreinfo\n");
        size_t minimum_chunksize = VERSION_COREINFO_SIZE;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          /* I suppose we *could* try to see if we can get the maj version.. but for now just
          give up */
          break;
        }

        decltype(maj) loadedFileMaj, loadedAppMaj;
        decltype(min) loadedFileMin, loadedAppMin;
        decltype(mic) loadedFileMic, loadedAppMic;
        // read file format version
        read(file, &loadedFileMaj, 2, 1, &maxread);
        read(file, &loadedFileMin, 2, 1, &maxread);
        read(file, &loadedFileMic, 2, 1, &maxread);
        // read snestracker app version
        read(file, &loadedAppMaj, 2, 1, &maxread);
        read(file, &loadedAppMin, 2, 1, &maxread);
        read(file, &loadedAppMic, 2, 1, &maxread);
        subchunksize -= VERSION_COREINFO_SIZE;

        DEBUGLOG("\t\tIn-File:\n"
                 "\t\t\tFile Version: %d.%d.%d\n"
                 "\t\t\t" APP_NAME " Version: %d.%d.%d\n",
                 loadedFileMaj, loadedFileMin, loadedFileMic,
                 loadedAppMaj, loadedAppMin, loadedAppMic
                );
        DEBUGLOG("\t\tCurrently running:\n"
                 "\t\t\tLatest Supported File Version: %d.%d.%d\n"
                 "\t\t\t" APP_NAME " Version: %d.%d.%d\n",
                 maj, min, mic,
                 APP_VER_MAJOR, APP_VER_MINOR, APP_VER_MICRO
                );

        // When opening a file version that is newer than the software,
        if (loadedFileMaj > maj || ( loadedFileMaj == maj && loadedFileMin > min ) ||
            ( loadedFileMaj == maj && loadedFileMin == min && loadedFileMic > mic) )
        {
          // loaded file is a newer format version
          // ask user if they want to try loading the file anyways? Yes/No
          auto rc = DialogBox::SimpleYesNo("Newer File Format Detected",
            "There's a chance this file may be incompatible with your current version of "
            APP_NAME ". Try loading anyways?");

          /* We need a way to warn the user when they go to save the file in the future
          that there will likely be information loss of data from the newer format. */
          /*
          FEATURE-REQUEST: Have the SAVE routine attempt to keep all of the unknown
          chunks or chunk extensions in place when saving files based from newer file versions! It's
          more trouble than it's worth for now, though */

          switch (rc)
          {
            case DialogBox::YES:
              fprintf(stderr, "USER SAID YES\n");
              // DO NOTHING - the file will load as normal
            break;
            case DialogBox::NO:
              fprintf(stderr, "USER SAID NO\n");
              // TODO: Fire a signal to the top-level FileLoader to stop loading
              StopLoading = true;
            break;
            case DialogBox::ERROR:
              fprintf(stderr, "ERROR\n");
              // Not sure how to handle this scenario yet. For now, try to load the
              // file anyways. (do nothing here)
            break;
            default:
              fprintf(stderr, "Unknown dialog return code %d\n", rc);
            break;
          }
        }
        else if (loadedFileMaj == maj && loadedFileMin == min && loadedFileMic == mic)
        {
          // same version (do nothing)
        }
        else
        {
          // earlier version
          /* In general, just tell the user this was made on an older version of
          SNES Tracker and that it may not make use of newer features. OR, NOT? */
          DialogBox::SimpleOK("Older File Format Detected",
            "This file will be migrated to the new format the next time you save.");


          if (loadedFileMaj < maj)
          {
            /* Since there is no such version available yet, I'm only stubbing this */

            /* I'm establishing that we only be "scared" when the major version is
            not a match - implying "ABI" incompatibility. Otherwise, load as normal */

            /* In this case, either use an older loader / importer associated with this
            incompatible version, or tell the user this probably will break snestracker
            to attempt loading it */
          }
        }
      }
      break;
      default:
        DEBUGLOG("\tUnknown SubChunkID: %d. skipping over..\n", subchunkid);
      break;
    }

    /* Skip the unrecognized part of the chunk */
    if (subchunksize)
    {
      DEBUGLOG("\tskipping past %d unknown bytes of chunk\n", subchunksize);
      SDL_RWseek(file, subchunksize, RW_SEEK_CUR);
      maxread += subchunksize;
    }
  }
  return 0;
}