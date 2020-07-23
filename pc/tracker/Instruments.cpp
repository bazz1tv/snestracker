#include "platform.h"
#include "tracker/Instruments.h"
#include <assert.h>
#include "globals.h" // for mouse::
#include "Samples.h"
#include "apuram.h"

Instrument::Instrument()
{
  reset();
}

void Instrument::reset()
{
  name[0] = 0;
  srcn = 0;
  adsr.adsr = DEFAULT_ADSR;
  adsr.sustain_rate = 13; // ~ 2.8 s
  vol = DEFAULT_VOL;
  pan = DEFAULT_PAN;
  semitone_offset = 0;
  finetune = 0;
  // leave metadata untouched
  echo = false;
}

Instrument::~Instrument()
{
}

void Instrument::inc_srcn(Instrument *i)
{
	if (i->srcn != 0xFF)
  {
		i->srcn++;
    *i->metadata.changed = true;
  }
}

void Instrument::dec_srcn(Instrument *i)
{
		if (i->srcn > 0)
    {
			i->srcn--;
      *i->metadata.changed = true;
    }
}

void Instrument::inc_vol(Instrument *i)
{
  if (i->vol != 0x7f)
  {
    i->vol++;
    *i->metadata.changed = true;
  }
}

void Instrument::dec_vol(Instrument *i)
{
  if (i->vol != 0x00)
  {
    i->vol--;
   *i->metadata.changed = true;
  }
}

void Instrument::inc_pan(Instrument *i)
{
  if (i->pan < 64)
  {
    *i->metadata.changed = true;
    i->pan++;
  }
}

void Instrument::dec_pan(Instrument *i)
{
  if (i->pan > -64)
  {
    *i->metadata.changed = true;
    i->pan--;
  }
}

void Instrument::inc_finetune(Instrument *i)
{
  if (i->finetune < 99)
  {
    *i->metadata.changed = true;
    i->finetune++;
  }
}

void Instrument::dec_finetune(Instrument *i)
{
  if (i->finetune > -99)
  {
    *i->metadata.changed = true;
    i->finetune--;
  }
}

bool Instrument::operator==(const Instrument& rhs)
{
  return (!strcmp(this->name, rhs.name) &&
          this->srcn == rhs.srcn &&
          this->adsr.adsr == rhs.adsr.adsr &&
          this->vol == rhs.vol &&
          this->pan == rhs.pan &&
          this->semitone_offset == rhs.semitone_offset &&
          this->finetune == rhs.finetune &&
          this->echo == rhs.echo);
}

#define MIN_INST_COREINFO_SIZE sizeof(Instrument::vol) + sizeof(Instrument::pan) + sizeof(Instrument::adsr)
#define MAX_INST_COREINFO_SIZE sizeof(Instrument::vol) + sizeof(Instrument::pan) + sizeof(Instrument::adsr)

#define MIN_INST_SONGMETA_SIZE sizeof(InstrumentChunkLoader::idx) + sizeof(Instrument::srcn)
#define MAX_INST_SONGMETA_SIZE sizeof(InstrumentChunkLoader::idx) + sizeof(Instrument::srcn)\
  + 1 /* flags */

#define INST_TUNE_SIZE sizeof(Instrument::finetune)

InstrumentChunkLoader::InstrumentChunkLoader(struct Instrument *i, bool ignoreSongMeta/*=false*/) :
  ChunkLoader(ChunkID::Instrument), instruments(i), ignoreSongMeta(ignoreSongMeta)
{}

size_t InstrumentChunkLoader::load(SDL_RWops *file, size_t chunksize)
{
  size_t maxread = 0;

  DEBUGLOG("InstrumentChunkLoader::load()\n");

  /* Every time we encounter an instrument in a song, we need to reload the idx */
  if (!ignoreSongMeta)
    idx_loaded = false;

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
      case SubChunkID::songmeta:
      {
        DEBUGLOG("\tSubChunkID::songmeta\n");
        const size_t constexpr minimum_chunksize = MIN_INST_SONGMETA_SIZE;
        const size_t constexpr maximum_chunksize = MAX_INST_SONGMETA_SIZE;
        size_t bytesread = 0;
        if (subchunksize > maximum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }

        if (ignoreSongMeta)
        {
          DEBUGLOG("\t\tignoring...\n");
          break;
        }
        read(file, &idx, 1, 1, &bytesread);

        struct Instrument *instr = &instruments[idx];
        read(file, &instr->srcn, 1, 1, &bytesread);
        DEBUGLOG("\t\tidx: %d\n", idx);

        if (bytesread < subchunksize) // we have entries beyond the minimum needed
        {
          uint8_t flags;
          read (file, &flags, 1, 1, &bytesread);
          if (flags & INSTR_FLAG_ECHO)
            instr->echo = true;
        }

        subchunksize -= bytesread;
        maxread += bytesread;
        idx_loaded = true;
      }
      break;
      case SubChunkID::coreinfo:
      {
        DEBUGLOG("\tSubChunkID::coreinfo\n");
        const size_t constexpr minimum_chunksize = MIN_INST_COREINFO_SIZE;
        const size_t constexpr maximum_chunksize = MAX_INST_COREINFO_SIZE;
        size_t bytesread = 0;
        if (subchunksize > maximum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }

        struct Instrument *instr = &instruments[idx];

        read(file, &instr->vol, 1, 1, &bytesread);
        read(file, &instr->pan, 1, 1, &bytesread);
        // The old version 0.1.0 didn't use panning but stored this now invalid value
        if (instr->pan == -128) // migrate it
          instr->pan = Instrument::DEFAULT_PAN;
        read(file, &instr->adsr.adsr1, 1, 1, &bytesread);
        read(file, &instr->adsr.adsr2, 1, 1, &bytesread);
        // Ensure Hardware ADSR is activated since that's the only thing supported
        instr->adsr.adsr_active = 1;
        //
        if (bytesread < subchunksize) // we have entries beyond the minimum needed
        {
          
        }

        subchunksize -= bytesread;
        maxread += bytesread;
      }
      break;
      case SubChunkID::name:
      {
        DEBUGLOG("\tSubChunkID::name\n");
        assert(idx_loaded);
        size_t bytesread = ChunkLoader::read_str_from_file2(file, instruments[idx].name, subchunksize, INSTR_NAME_MAXLEN);

        // Truncate the sample name for the GUI's shorter length
        instruments[idx].name[INSTR_NAME_GUI_CHAR_WIDTH - 1] = 0;

        subchunksize -= bytesread;
        maxread += bytesread;
      }
      break;
      case SubChunkID::tune:
      {
        DEBUGLOG("\tSubChunkID::tune\n");
        size_t minimum_chunksize = INST_TUNE_SIZE;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }

        read(file, &instruments[idx].finetune, 1, 1, &maxread);

        subchunksize -= INST_TUNE_SIZE;
      }
      break;
      default:
        DEBUGLOG("\tUnknown SubChunkID: %d. skipping over..\n", subchunkid);
      break;
    }

    /* Skip the unrecognized part of the chunk */
    if (subchunksize)
    {
      DEBUGLOG("\tskipping past %d unread bytes of chunk\n", subchunksize);
      SDL_RWseek(file, subchunksize, RW_SEEK_CUR);
      maxread += subchunksize;
    }
  }
  return maxread;
}

/* Write the chunk for particular instrument indexed by i */
size_t InstrumentChunkLoader::save(SDL_RWops *file, int i)
{
  struct Instrument *instr = &instruments[i];

  uint8_t byte;
  uint16_t word;
  uint16_t chunklen = 0;
  Sint64 chunksize_location, chunkend_location;

  DEBUGLOG("InstrumentChunkLoader::save()\n");

  DEBUGLOG("\tInstrument #%d, Writing top-level chunkid: %d\n", i, chunkid);
  // write this master chunk id
  byte = chunkid;
  SDL_RWwrite(file, &byte, 1, 1);
  // stub the master chunksize for later writing
  chunksize_location = SDL_RWtell(file);
  SDL_RWwrite(file, &chunklen, 2, 1);

  if (ignoreSongMeta == false)
  {
    DEBUGLOG("\t\tSubChunkID::songmeta\n");
    byte = SubChunkID::songmeta;
    write(file, &byte, 1, 1, &chunklen);
    word = MAX_INST_SONGMETA_SIZE; // we know the length in advance
    write(file, &word, 2, 1, &chunklen);
    write(file, &i, 1, 1, &chunklen); // write Instrument index (only 1 byt, &chunklene)
    write(file, &instr->srcn, 1, 1, &chunklen);

    /* consolidate boolean flags into one byte */
    uint8_t flags = instr->echo ? INSTR_FLAG_ECHO : 0;
    write(file, &flags, 1, 1, &chunklen);
  }

  DEBUGLOG("\t\tSubChunkID::coreinfo\n");
  byte = SubChunkID::coreinfo;
  write(file, &byte, 1, 1, &chunklen);
  word = MAX_INST_COREINFO_SIZE; // we know the length in advance
  write(file, &word, 2, 1, &chunklen);
  write(file, &instr->vol, 1, 1, &chunklen);
  write(file, &instr->pan, 1, 1, &chunklen);
  write(file, &instr->adsr.adsr1, 1, 1, &chunklen);
  write(file, &instr->adsr.adsr2, 1, 1, &chunklen);

  word = strlen(instruments[i].name);
  if (word > 0)
  {
    DEBUGLOG("\t\tSubChunkID::name\n");
    byte = SubChunkID::name;
    write(file, &byte, 1, 1, &chunklen);
    write(file, &word, 2, 1, &chunklen);
    write(file, instruments[i].name,  word, 1, &chunklen);
  }


  DEBUGLOG("\t\tSubChunkID::tune\n");
  byte = SubChunkID::tune;
  write(file, &byte, 1, 1, &chunklen);
  word = INST_TUNE_SIZE; // we know the length in advance
  write(file, &word, 2, 1, &chunklen);
  write(file, &instr->finetune, 1, 1, &chunklen);

  DEBUGLOG("\tWriting chunksize\n");
  chunkend_location = SDL_RWtell(file);
  SDL_RWseek(file, chunksize_location, RW_SEEK_SET);
  SDL_RWwrite(file, &chunklen, 2, 1);
  SDL_RWseek(file, chunkend_location, RW_SEEK_SET);
  return 0;
}

/* TODO: integrate chunklen (featured below) into the ChunkLoader class, and have
ChunkLoader::write() automatically update it. Then user just needs to make sure
they clear the chunklen when calculating a new chunklen */

size_t InstrumentChunkLoader::save(SDL_RWops *file)
{
  DEBUGLOG("InstrumentChunkLoader::save() (all)\n");
  for (uint16_t i=0; i < NUM_INSTR; i++)
  {
    if (instruments[i] != ::Instrument()) // empty instrument
      save(file, i);
  }
  return 0;
}

size_t InstrumentChunkLoader::save(SDL_RWops *file, struct Instrument *instr)
{
  struct Instrument *backup = instruments;
  instruments = instr;
  save(file, 0);
  instruments = backup;
  return 0;
}

/////////////////// INSTRUMENT FILE LOADER //////////////////

#include "gui/DialogBox.h"
const char InstrumentFileLoader::HeaderStr[];

InstrumentFileLoader::InstrumentFileLoader(Instrument *instrument, Sample *sample) :

  vcl  ( new VersionChunkLoader(INSTRFILE_VER_MAJOR, INSTRFILE_VER_MINOR, INSTRFILE_VER_MICRO) ),
  scl  ( new SampleChunkLoader(sample, true) ),
  icl  ( new InstrumentChunkLoader(instrument, true) ),

  instrument(instrument)
{}

InstrumentFileLoader::~InstrumentFileLoader()
{
  delete icl;
  delete scl;
  delete vcl;
}

// convenience function
size_t InstrumentFileLoader::save(SDL_RWops *file)
{
  /* TODO: Here (or on higher level) inform user if the data came from a file
  of an older file format version - inform that we are saving in the new format.
  Or, if the file loaded was too new, inform that file is being saved in the
  older format of current software */
  DEBUGLOG("Writing File HeaderString: %s\n", HeaderStr);
  SDL_RWwrite(file, HeaderStr, sizeof(HeaderStr) - 1, 1);
  vcl->save(file);  // version
  scl->save(file, 0);  // samples
  icl->save(file, 0);  // instruments
  return 0;
}

InstrumentFileLoader::ret_t InstrumentFileLoader::load(SDL_RWops *file)
{
  DEBUGLOG("Reading File's HeaderString\n");
  auto rc = readHeader(file);
  if (rc == HEADER_OK)
  {
    icl->setIdx(0);
    scl->setIdx(0);
    ChunkLoader::loadchunks(file);
    if (scl->instr_srcn == -1)
    {
      DEBUGLOG("Sample limit reached you crazy mofo!\n");
    }
    else
    {
      instrument->srcn = scl->instr_srcn;
    }
    return FILE_LOADED;
  }
  else
  {
    assert(rc == HEADER_BAD);
    auto drc = DialogBox::SimpleYesNo("Unrecognized header",
      "This file might be incompatible or corrupted. Load anyways?");
    switch (drc)
    {
      case DialogBox::YES:
        fprintf(stderr, "USER SAID YES\n");
        ChunkLoader::loadchunks(file);
        return FILE_LOADED;
      default:
        return HEADER_BAD;
    }
  }
}

InstrumentFileLoader::ret_t InstrumentFileLoader::readHeader(SDL_RWops *file)
{
  uint8_t buf[sizeof(HeaderStr)];
  size_t rc;

  rc = SDL_RWread(file, buf, sizeof(HeaderStr) - 1, 1);

  if (rc == 0)
  {
    DEBUGLOG("\tCould not read from file: %s\n", SDL_GetError());
    return HEADER_BAD;
  }

  buf[sizeof(HeaderStr) - 1] = 0;

  ret_t ret;
  // The newer header is "InstrumentST"
  if (strcmp( (const char *)buf, HeaderStr) == 0)
  {
    DEBUGLOG("\tHeader is of 'Current' chunk format.\n");
    ret = HEADER_OK;
  }
  // bad file header
  else
  {
    DEBUGLOG("\tUnrecognized header! This file might be incompatible or corrupted. Load anyways?\n");
    ret = HEADER_BAD;
  }

  return ret;
}