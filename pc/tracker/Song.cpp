#include "Song.h"
#include "gui/DialogBox.h"
void Song::reset()
{
  // Reset Song Title
  settings.reset();

  // Reset all Samples (and free memory!)
  for (int i=0; i < NUM_SAMPLES; i++)
  {
    samples[i].clear();
  }

  // Reset all instruments
  for (int i=0; i < NUM_INSTR; i++)
  {
    instruments[i].reset();
  }

  // Reset all patterns
  memset(patseq.patterns, 0, sizeof(patseq.patterns));
  for (int i=0; i < MAX_PATTERNS; i++)
  {
    patseq.patterns[i].p.len = DEFAULT_PATTERN_LEN;
  }
  // Reset Pat Sequencer
  patseq.num_entries = 1;
  patseq.sequence[0] = 0;
  /* TODO: Don't have a "used" metadata for patterns, but just have a used()
   * that checks whether the index of that pattern is in the pattern sequencer.
   */
  patseq.patterns[0].used = 1;
  changed = false;
}

void Song::load(SDL_RWops *file)
{
  SongFileLoader sfl(this);
  sfl.load(file);
  changed = false;
}

void Song::save(SDL_RWops *file)
{
  SongFileLoader sfl(this);
  sfl.save(file);
  changed = false;
}

const char SongFileLoader::HeaderStr[];

SongFileLoader::SongFileLoader(Song *song) :

  vcl  ( new VersionChunkLoader(SONGFILE_VER_MAJOR, SONGFILE_VER_MINOR, SONGFILE_VER_MICRO) ),
  sscl ( new SongSettingsChunkLoader(&song->settings) ),
  scl  ( new SampleChunkLoader(song->samples) ),
  icl  ( new InstrumentChunkLoader(song->instruments) ),
  pcl  ( new PatternChunkLoader(song->patseq.patterns) ),
  pscl ( new PatternSequencerChunkLoader(&song->patseq) ),

  song(song)
{}

SongFileLoader::~SongFileLoader()
{
	delete pscl;
	delete pcl;
	delete icl;
	delete scl;
	delete sscl;
	delete vcl;
}

// convenience function
size_t SongFileLoader::save(SDL_RWops *file)
{
  /* TODO: Here (or on higher level) inform user if the data came from a file
  of an older file format version - inform that we are saving in the new format.
  Or, if the file loaded was too new, inform that file is being saved in the
  older format of current software */
  DEBUGLOG("Writing File HeaderString: %s\n", HeaderStr);
  SDL_RWwrite(file, HeaderStr, sizeof(HeaderStr) - 1, 1);
  vcl->save(file);  // version
  sscl->save(file); // song settings
  scl->save(file);  // samples
  icl->save(file);  // instruments
  pcl->save(file);  // patterns
  pscl->save(file); // pattern sequence
  return 0;
}

SongFileLoader::ret_t SongFileLoader::load(SDL_RWops *file)
{
  DEBUGLOG("Reading File's HeaderString\n");
  auto rc = readHeader(file);
  if (rc == HEADER_OLD)
  {
    // TODO: call older loading function
    loadOld(file);
    return FILE_LOADED;
  }
  else if (rc == HEADER_OK)
  {
    /* TODO: add a return value to loadchunks() to ensure file successfully
     * loaded */

    ChunkLoader::loadchunks(file);
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

SongFileLoader::ret_t SongFileLoader::readHeader(SDL_RWops *file)
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
  /* The older format used header string "STSONG" */
  if (strcmp((const char *)buf, "STSONG") == 0)
  {
    DEBUGLOG("\tFound older (pre-version) song file\n");
    ret = HEADER_OLD;
    DialogBox::SimpleOK("Older File Format Detected",
            "This file will be migrated to the new format the next time you save.");
  }
  // The newer header is "SONGST"
  else if (strcmp( (const char *)buf, HeaderStr) == 0)
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

#include "apuram.h"

// function for loadOld
// Now with bounds checking :D
// Note: size includes NULL byte
static size_t read_str_from_file(SDL_RWops *file, char *str_ptr, int size);

int SongFileLoader::loadOld(SDL_RWops *file)
{
  struct SongSettings &songsettings(song->settings);
  struct Sample *samples(song->samples);
  struct Instrument *instruments(song->instruments);
  struct PatternSequencer &patseq(song->patseq);

  size_t rc;
  // Read Song title until NULL is read in
  // corner case: no title == 0
  read_str_from_file(file, songsettings.song_title_str, SongSettings::SONGTITLE_SIZE);

  uint16_t bpmspd;
  rc = SDL_RWread(file, &bpmspd, 2, 1);
  if (rc == 0)
  {
    DEBUGLOG("Could not read from file: %s\n", SDL_GetError());
    return -1;
  }
  // Check for valid BPM/SPD
  songsettings.bpm = bpmspd >> 6;
  if (songsettings.bpm < SongSettings::MIN_BPM || songsettings.bpm > SongSettings::MAX_BPM)
  {
    DEBUGLOG("Invalid BPM: %d. Setting to default %d\n", songsettings.bpm, SongSettings::DEFAULT_BPM);
    songsettings.bpm = SongSettings::DEFAULT_BPM;
  }
  songsettings.spd = (uint8_t)(bpmspd & 0b111111);
  if (songsettings.spd < SongSettings::MIN_SPD || songsettings.spd > SongSettings::MAX_SPD)
  {
    DEBUGLOG("Invalid SPD: %d. Setting to default %d\n", songsettings.spd, SongSettings::DEFAULT_SPD);
    songsettings.spd = SongSettings::DEFAULT_SPD;
  }

  // number of samples
  uint8_t numsamples;
  SDL_RWread(file, &numsamples, 1, 1);
  for (int i=0; i < numsamples; i++)
  {
    uint8_t idx;
    SDL_RWread(file, &idx, 1, 1);

    Sample *s = &samples[idx];
    read_str_from_file(file, s->name, sizeof(Sample::name));

    SDL_RWread(file, &samples[idx].rel_loop, 2, 1);

    uint16_t brrsize;
    SDL_RWread(file, &brrsize, 2, 1);
    Brr *brr = (Brr *) malloc(brrsize);

    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = (char *)brr;
    while (nb_read_total < brrsize && nb_read != 0) {
      nb_read = SDL_RWread(file, buf, 1, (brrsize - nb_read_total));
      nb_read_total += nb_read;
      buf += nb_read;
    }

    if (nb_read_total != brrsize)
    {
      DEBUGLOG("Error Reading Sample %d!?\n", i);
      free(brr);
      return -1;
    }

    if (s->brr != NULL)
      free(s->brr);

    s->brr = brr;
    s->brrsize = brrsize;
  }

  uint8_t numinstr = 0;
  SDL_RWread(file, &numinstr, 1, 1);
  for (int i=0; i < numinstr; i++)
  {
    uint8_t idx;
    SDL_RWread(file, &idx, 1, 1);
    Instrument *instr = &instruments[idx];
    /* TODO: store/read the instr # */

    read_str_from_file(file, instr->name, sizeof(Instrument::name));

    /* TODO: Store/Read the instr used. Better yet, calculate it after the
     * patterns have been loaded */

    // Time to load instrument info
    SDL_RWread(file, &instr->vol, 1, 1);
    SDL_RWread(file, &instr->pan, 1, 1);
    if (instr->pan == -128)
      instr->pan = 0;
    SDL_RWread(file, &instr->srcn, 1, 1);
    SDL_RWread(file, &instr->adsr.adsr1, 1, 1);
    SDL_RWread(file, &instr->adsr.adsr2, 1, 1);
    SDL_RWread(file, &instr->semitone_offset, 1, 1);
    SDL_RWread(file, &instr->finetune, 1, 1);
  }

  // PATTERNS
  //
  uint8_t numpatterns = 0;

  SDL_RWread(file, &numpatterns, 1, 1);
  for (int p=0; p < numpatterns; p++)
  {
    uint8_t idx;
    SDL_RWread(file, &idx, 1, 1);
    PatternMeta *pm = &patseq.patterns[idx];

    Pattern *pattern = &pm->p;
    SDL_RWread(file, &pattern->len, 1, 1);
    for (int t=0; t < MAX_TRACKS; t++)
    {
      uint8_t rlecounter = 0;
      uint8_t a;

      for (int tr=0; tr < pattern->len; tr++)
      {
        PatternRow *pr = &pattern->trackrows[t][tr];
        if (rlecounter)
          if (--rlecounter >= 0)
          {
            /* with the proper reset() written, we won't need to
             * rewrite 0 values here */
            /*pr->note = NOTE_NONE;
            pr->instr = 0;
            pr->vol = 0;
            pr->fx = 0;
            pr->fxparam = 0;*/
            continue;
          }

        SDL_RWread(file, &a, 1, 1);
        if (a <= 0x7f) // positive
        {
          pr->note = a;
          SDL_RWread(file, &pr->instr, 1, 1);
          SDL_RWread(file, &pr->vol, 1, 1);
          SDL_RWread(file, &pr->fx, 1, 1);
          SDL_RWread(file, &pr->fxparam, 1, 1);
        }
        else
        {
          /* TODO: since reset() written, we won't need to
           * rewrite 0 values here ( the else statements will be removed )*/
          if ( a & ( 1 << CBIT_NOTE ) )
            SDL_RWread(file, &pr->note, 1, 1);
          //else pr->note = NOTE_NONE;

          if ( a & ( 1 << CBIT_INSTR ) )
          {
            SDL_RWread(file, &pr->instr, 1, 1);
          }
          //else pr->instr = 0;

          if ( a & ( 1 << CBIT_VOL ) )
            SDL_RWread(file, &pr->vol, 1, 1);
          //else pr->vol = 0;

          if ( a & ( 1 << CBIT_FX ) )
            SDL_RWread(file, &pr->fx, 1, 1);
          //else pr->fx = 0;

          if ( a & ( 1 << CBIT_FXPARAM ) )
            SDL_RWread(file, &pr->fxparam, 1, 1);
          //else pr->fxparam = 0;

          if ( a & ( 1 << CBIT_RLE_ONLY1 ) )
          {
            rlecounter = ( a & ( 1 << CBIT_RLE ) ) ? 2 : 1;
          }
          else if ( a & ( 1 << CBIT_RLE ) )
            SDL_RWread(file, &rlecounter, 1, 1);
        }
      }
    }
  }
  // PAttern Sequencer
  patseq.num_entries = 0; // WARNING: could be dangerous
  for (int i=0; 1; i++)
  {
    rc = SDL_RWread(file, &patseq.sequence[i], 1, 1);
    if (rc == 0) // EOF
      break;

    patseq.patterns[patseq.sequence[i]].used++;
    patseq.num_entries++;
  }
  return 0;
}

size_t read_str_from_file(SDL_RWops *file, char *str_ptr, int size)
{
  size_t count = 0;
  assert(size > 1); // if it was one, that's only enough room for an empty string!
  do {
    SDL_RWread(file, str_ptr, 1, 1);
    count++;
  } while ((--size) && *(str_ptr++) != 0);

  *str_ptr = 0; // need to write null byte incase we stopped looping cause size barrier was hit
  return count;
}