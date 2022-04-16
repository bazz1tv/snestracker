#include <assert.h>
#include <DEBUGLOG.h>
#include "Samples.h"

Sample::Sample() : brr(NULL)
{
  clear();
}

Sample::~Sample()
{
  if (brr != NULL)
  {
    free(brr);
    brr = NULL;
    brrsize = 0;
  }
}

bool Sample::operator==(const Sample& rhs)
{
  return (!strcmp(this->name, rhs.name) &&
          this->brr == rhs.brr &&
          this->brrsize == rhs.brrsize &&
          this->rel_loop == rhs.rel_loop &&
          this->semitone_offset == rhs.semitone_offset &&
          this->finetune == rhs.finetune);
}

void Sample::inc_loop()
{
	if (brr != NULL)
  {
		rel_loop = rel_loop >= (brrsize - BRR_LEN) ? brrsize - BRR_LEN : rel_loop + BRR_LEN;
    *metadata.changed = true;
  }
}

void Sample::dec_loop()
{
	if (brr != NULL)
  {
		rel_loop = rel_loop >= BRR_LEN ? rel_loop - BRR_LEN : 0;
    *metadata.changed = true;
  }
}

void Sample::inc_finetune()
{
	if (brr != NULL && finetune < 127)
  {
		finetune++;
    *metadata.changed = true;
  }
}

void Sample::dec_finetune()
{
	if (brr != NULL && finetune > -128)
  {
		finetune--;
    *metadata.changed = true;
  }
}

bool Sample::identical(const Brr *brr)
{
  bool match = false;
  auto brrsize = get_brr_size(brr);
  if (this->brrsize == brrsize)
  {
    match = true;

    for (int i=0; this->brr[i].end == 0 && match == true; i++)
    {
      if (brr[i].end)
        DEBUGLOG("Checking END BLock\n");

      // Let's ignore the header because that data can change depending on whether
      // looping is enabled

      for (int y=0; y < 8; y++)
      {
        if (this->brr[i].samples[y] != brr[i].samples[y])
        {
          match = false;
          break;
        }
      }
    }
  }

  return match;
}

void Sample::clear()
{
  name[0] = 0;

  if (brr != NULL)
    free(brr);

  brr = NULL;
  brrsize = 0;
  rel_loop = 0;
  semitone_offset = 0;
  finetune = 0;
  metadata.loop = false;
}


SampleChunkLoader::SampleChunkLoader(struct Sample *s, bool ignoreSongMeta/*=false*/) :
  ChunkLoader(ChunkID::Sample), samples(s), ignoreSongMeta(ignoreSongMeta)
{}

#define SAMPLE_SONGMETA_SIZE sizeof(SampleChunkLoader::idx)
#define SAMPLE_COREINFO_SIZE sizeof(Sample::rel_loop)
#define SAMPLE_TUNE_SIZE sizeof(Sample::finetune) + sizeof(Sample::semitone_offset)

size_t SampleChunkLoader::load(SDL_RWops *file, size_t chunksize)
{
  size_t maxread = 0;

  DEBUGLOG("SampleChunkLoader::load()\n");

  /* Every time we encounter a sample in a song, we need to reload the idx */
  if (!ignoreSongMeta)
    idx_loaded = false;

  while (maxread < chunksize)
  {
    uint8_t subchunkid;
    uint16_t subchunksize;

    //DEBUGLOG("maxread = %llu\n", maxread);

    if (read(file, &subchunkid, 1, 1, &maxread) == 0)
      break;
    if (read(file, &subchunksize, 2, 1, &maxread) == 0)
      break;

    //DEBUGLOG("subchunksize = %llu\n", subchunksize);

    switch (subchunkid)
    {
      case SubChunkID::songmeta:
      {
        DEBUGLOG("\tSubChunkID::songmeta\n");
        size_t minimum_chunksize = SAMPLE_SONGMETA_SIZE;
        if (subchunksize > minimum_chunksize)
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
        read(file, &idx, 1, 1, &maxread);

        DEBUGLOG("\t\tidx: %d\n", idx);

        subchunksize -= SAMPLE_SONGMETA_SIZE;
        idx_loaded = true;
      }
      break;
      case SubChunkID::coreinfo:
      {
        DEBUGLOG("\tSubChunkID::coreinfo\n");
        size_t minimum_chunksize = SAMPLE_COREINFO_SIZE;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }

        read(file, &samples[idx].rel_loop, 2, 1, &maxread);
        subchunksize -= SAMPLE_COREINFO_SIZE;
      }
      break;
      case SubChunkID::name:
      {
        DEBUGLOG("\tSubChunkID::name\n");
        assert(idx_loaded);
        size_t bytesread = ChunkLoader::read_str_from_file2(file, samples[idx].name, subchunksize, SAMPLE_NAME_MAXLEN);

        // Truncate the sample name for the GUI's shorter length
        // Shenanigans from 11/8/2020 Twitch Stream. Ariel: if {song ends=true, then noIt’sNotThisSongNeverEnds; song ends = false;}
        samples[idx].name[SAMPLE_NAME_GUI_CHAR_WIDTH - 1] = 0;

        subchunksize -= bytesread;
        maxread += bytesread;
      }
      break;

      case SubChunkID::brr:
      {
        DEBUGLOG("\tSubChunkID::brr\n");
        assert(idx_loaded);
        Brr *brr = (Brr *) malloc(subchunksize);

        Sint64 nb_read_total = 0, nb_read = 1;
        char* buf = (char *)brr;
        while (nb_read_total < subchunksize) {
          nb_read = SDL_RWread(file, buf, 1, (subchunksize - nb_read_total));
          nb_read_total += nb_read;
          buf += nb_read;
        }
        maxread += nb_read_total;

        if (nb_read_total != subchunksize)
        {
          DEBUGLOG("\t\tError Reading Sample %d!?\n", idx);
          DEBUGLOG("\t\tnb_read_total = %llu, subchunksize = %llu\n", nb_read_total, subchunksize);
          free(brr);
          break;
        }

        if (ignoreSongMeta)
        {
          /* When we load an instrument up, we need to check if this sample is already in a slot. */
          // findIdentical()
          for (int i=0; i < NUM_SAMPLES; i++)
          {
            if (samples[i].identical(brr))
            {
              // found a matching brr already in the sample table. free this copy
              free(brr);
              instr_srcn = i; // mark for the instrument file loader :O
              // skip the rest of thos whole chunk and return
              SDL_RWseek(file, chunksize - maxread, RW_SEEK_CUR);
              return 0;
            }
          }

          // not a match. Find the earliest blank slot
          // findEmpty()
          instr_srcn = -1;
          for (int i=0; i < NUM_SAMPLES; i++)
          {
            if (samples[i].brr == NULL)
            {
              instr_srcn = i;
              idx = i;
              break;
            }
          }
          if (instr_srcn == -1)
          {
            // skip the rest of thos whole chunk and return
            SDL_RWseek(file, chunksize - maxread, RW_SEEK_CUR);
            return 0;
          }
        }

        struct Sample *s = &samples[idx];
        s->clear();

        s->brr = brr;
        s->brrsize = subchunksize;
        subchunksize -= nb_read_total;

        s->metadata.loop = doesBrrLoop(s->brr);
      }
      break;
      case SubChunkID::tune:
      {
        DEBUGLOG("\tSubChunkID::tune\n");
        size_t minimum_chunksize = SAMPLE_TUNE_SIZE;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }

        read(file, &samples[idx].finetune, 1, 1, &maxread);
        read(file, &samples[idx].semitone_offset, 1, 1, &maxread);

        subchunksize -= SAMPLE_TUNE_SIZE;
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
  return maxread;
}

size_t SampleChunkLoader::save(SDL_RWops *file, int i)
{
  uint8_t byte;
  uint16_t word;
  uint16_t chunklen = 0;
  Sint64 chunksize_location, chunkend_location;
  DEBUGLOG("SampleChunkLoader::save()\n");

  DEBUGLOG("\tsample #%d, Writing top-level chunkid: %d\n", i, chunkid);

  byte = chunkid;
  SDL_RWwrite(file, &byte, 1, 1);
  chunksize_location = SDL_RWtell(file);
  SDL_RWwrite(file, &chunklen, 2, 1);

  if (ignoreSongMeta == false)
  {
    DEBUGLOG("\t\tSubChunkID::songmeta\n");
    byte = SubChunkID::songmeta;
    write(file, &byte, 1, 1, &chunklen);
    word = SAMPLE_SONGMETA_SIZE; // we know the length in advance
    write(file, &word, 2, 1, &chunklen);
    write(file, &i, 1, 1, &chunklen); // write sample index (only 1 byt, &chunklene)
  }

  /* It is very important that the BRR chunk be the very first chunk after the
  optional first songmeta chunk. This is because when there is no songmeta chunk
  (STI instruments), the idx will not be retrieved from a songmeta chunk. Instead,
  the idx is calculated from the brr-chunk handling code. Although it would be
  easy to not have this order requirement, it's easier to have it xD. */
  DEBUGLOG("\t\tSubChunkID::brr\n");
  byte = SubChunkID::brr;
  write(file, &byte, 1, 1, &chunklen);
  word = samples[i].brrsize;
  write(file, &word, 2, 1, &chunklen);
  write(file, samples[i].brr, word, 1, &chunklen);

  DEBUGLOG("\t\tSubChunkID::coreinfo\n");
  byte = SubChunkID::coreinfo;
  write(file, &byte, 1, 1, &chunklen);
  word = SAMPLE_COREINFO_SIZE; // we know the length in advance
  write(file, &word, 2, 1, &chunklen);
  write(file, &samples[i].rel_loop, 2, 1, &chunklen);

  byte = SubChunkID::name;
  write(file, &byte, 1, 1, &chunklen);
  word = strlen(samples[i].name);
  if (word > 0)
  {
    DEBUGLOG("\t\tSubChunkID::name\n");
    write(file, &word, 2, 1, &chunklen);
    write(file, samples[i].name,  word, 1, &chunklen);
  }

  DEBUGLOG("\t\tSubChunkID::tune\n");
  byte = SubChunkID::tune;
  write(file, &byte, 1, 1, &chunklen);
  word = SAMPLE_TUNE_SIZE; // we know the length in advance
  write(file, &word, 2, 1, &chunklen);
  write(file, &samples[i].finetune, sizeof(Sample::finetune), 1, &chunklen);
  write(file, &samples[i].semitone_offset, sizeof(Sample::semitone_offset), 1, &chunklen);

  DEBUGLOG("\tWriting chunksize\n");
  chunkend_location = SDL_RWtell(file);
  SDL_RWseek(file, chunksize_location, RW_SEEK_SET);
  SDL_RWwrite(file, &chunklen, 2, 1);

  SDL_RWseek(file, chunkend_location, RW_SEEK_SET);
  return 0;
}

size_t SampleChunkLoader::save(SDL_RWops *file)
{
  DEBUGLOG("SampleChunkLoader::save() (all)\n");
  for (uint16_t i=0; i < NUM_SAMPLES; i++)
  {
    if (samples[i].brr != NULL)
      save(file, i);
  }
  return 0;
}

size_t SampleChunkLoader::save(SDL_RWops *file, struct Sample *s)
{
  struct Sample *backup = samples;
  samples = s;
  save(file, 0);
  samples = backup;
  return 0;
}
