#include "platform.h"
#include "Pattern.h"
#include "PanelCommon.h"
#include "sdl_userevents.h"
#include "shared/Colors.h"
#include <assert.h>
#include "DEBUGLOG.h"
#include "shared/font.h"
#include "Instruments.h" // for Instrument_Panel
#include "shared/gui/MouseCursors.h"
#include "shared/Voice_Control.h"
#include "shared/kbd.h"

#include "apuram.h"


static int clone_seq_common(PatSeqPanel *psp);
static Pattern * get_current_pattern(PatSeqPanel *psp);
static PatternMeta * get_current_pattern_meta(PatSeqPanel *psp);

static void note2ascii(uint8_t note, char *c);
static void moveback(Pattern *pattern, int track, int pos, const Instrument_Panel *ip);

extern MouseCursors *mousecursors;

// TODO: Remove this hack
#include "Tracker.h"
extern Tracker *tracker;


PatternSequencer::PatternSequencer() : sequence(MAX_PATTERNS)
{
  patterns[0].used = 1;
}


/* For private use by the PatternSequencerChunkLoader */
class TrackChunkLoader : public ChunkLoader
{
public:
  TrackChunkLoader(struct Pattern *pat, int t) : ChunkLoader(0), pattern(pat), t(t) {}
  size_t load(SDL_RWops *file, size_t chunksize);
  size_t save(SDL_RWops *file);

  enum SubChunkID {
    coreinfo=0,
    Data,
    NUM_SUBCHUNKIDS
  };

  enum TrackType
  {
    voice=0,
    extra_fx
  };
private:
  struct Pattern *pattern;
  int t; // track number
};

size_t TrackChunkLoader::load(SDL_RWops *file, size_t chunksize)
{
  size_t maxread = 0;
  uint8_t tracktype = 0xFF;
  bool idx_loaded = false;

  //DEBUGLOG("\tLoading Track; chunksize: %d\n", chunksize);
  DEBUGLOG("\t\tTrackChunkLoader::load()\n");

  while (maxread < chunksize)
  {
    uint8_t subchunkid;
    uint16_t subchunksize;

    //DEBUGLOG("\t\tmaxread = %zu\n", maxread);

    if (read(file, &subchunkid, 1, 1, &maxread) == 0)
      break;
    if (read(file, &subchunksize, 2, 1, &maxread) == 0)
      break;

    //DEBUGLOG("\t\tsubchunksize = %hu\n", subchunksize);

    switch (subchunkid)
    {
      case SubChunkID::coreinfo:
      {
        DEBUGLOG("\t\t\tSubChunkID::coreinfo\n");
        size_t minimum_chunksize = 2, maximum_chunksize = 2;
        if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\t\t\tSubChunk %d is smaller than expected. Fail load chunk\n", subchunkid);
          return -1;
        }
        else if (subchunksize > maximum_chunksize)
        {
          DEBUGLOG("\t\t\t\tSubChunk %d is bigger than expected by %d bytes.\n",
            subchunkid, subchunksize - maximum_chunksize);
        }

        read(file, &tracktype, 1, 1, &maxread);
        read(file, &t, 1, 1, &maxread); //track idx
        idx_loaded = true;
        subchunksize -= 2;
      }
      break;
      case SubChunkID::Data:
      {
        assert(idx_loaded);
        DEBUGLOG("\t\t\tSubChunkID::Data\n");

        uint8_t rlecounter = 0;
        uint8_t a;
        size_t bytesread = 0;

        for (int tr=0; tr < MAX_PATTERN_LEN; tr++)
        {
          PatternRow *pr = &pattern->trackrows[t][tr];
          if (rlecounter)
            if (--rlecounter >= 0)
            {
              continue;
            }

          read(file, &a, 1, 1, &bytesread);
          if (a <= 0x7f) // positive
          {
            pr->note = a;
            read(file, &pr->instr, 1, 1, &bytesread);
            read(file, &pr->vol, 1, 1, &bytesread);
            read(file, &pr->fx, 1, 1, &bytesread);
            read(file, &pr->fxparam, 1, 1, &bytesread);
          }
          else
          {
            if ( a & ( 1 << CBIT_NOTE ) )
              read(file, &pr->note, 1, 1, &bytesread);

            if ( a & ( 1 << CBIT_INSTR ) )
            {
              read(file, &pr->instr, 1, 1, &bytesread);
            }

            if ( a & ( 1 << CBIT_VOL ) )
              read(file, &pr->vol, 1, 1, &bytesread);

            if ( a & ( 1 << CBIT_FX ) )
              read(file, &pr->fx, 1, 1, &bytesread);

            if ( a & ( 1 << CBIT_FXPARAM ) )
              read(file, &pr->fxparam, 1, 1, &bytesread);

            if ( a & ( 1 << CBIT_RLE_ONLY1 ) )
            {
              rlecounter = ( a & ( 1 << CBIT_RLE ) ) ? 2 : 1;
            }
            else if ( a & ( 1 << CBIT_RLE ) )
              read(file, &rlecounter, 1, 1, &bytesread);
          }
        }

        //DEBUGLOG("\t\tbytesread = %d\n", (uint16_t) bytesread);
        subchunksize -= bytesread;
        maxread += bytesread;
      }
      break;
      default:
        DEBUGLOG("\t\t\tUnknown SubChunkID: %d. skipping over..\n", subchunkid);
      break;
    }

    /* Skip the unrecognized part of the chunk */
    if (subchunksize)
    {
      DEBUGLOG("\t\t\tskipping past %d unknown bytes of chunk\n", subchunksize);
      SDL_RWseek(file, subchunksize, RW_SEEK_CUR);
      maxread += subchunksize;
    }
  }

  return maxread;
}

#define PATROW_EMPTY(pr) ( pr->note == 0 && pr->instr == 0 && pr->vol == 0 && pr->fx == 0 && pr->fxparam == 0 )
size_t TrackChunkLoader::save(SDL_RWops *file)
{
  assert(pattern && t < MAX_TRACKS);

  uint8_t byte;
  uint16_t word;
  uint16_t chunklen = 0;
  uint16_t headerlen = 0;
  Sint64 chunksize_location, chunkend_location;

  DEBUGLOG("\t\tTrackChunkLoader::save()\n");

  DEBUGLOG("\t\t\tSubChunkID::coreinfo\n");
  byte = SubChunkID::coreinfo;
  word = 2;
  write(file, &byte, 1, 1, &chunklen);
  write(file, &word, 2, 1, &chunklen);
  byte = TrackType::voice;
  write(file, &byte, 1, 1, &chunklen);
  write(file, &t, 1, 1, &chunklen);

  DEBUGLOG("\t\t\tSubChunkID::Data\n");
  uint16_t datachunklen = 0;
  byte = SubChunkID::Data;
  write(file, &byte, 1, 1, &headerlen);
  chunksize_location = SDL_RWtell(file);
  write(file, &chunklen, 2, 1, &headerlen);

  for (int tr=0; tr < MAX_PATTERN_LEN; tr++)
  {
    int ttrr;
    PatternRow *pr = &pattern->trackrows[t][tr];
    uint8_t cbyte = 0, rlebyte;
    // Lookahead: how many empty rows from this one until the next
    // filled row? If there's only 1 empty row, use RLE_ONLY1
    for (ttrr=tr+1; ttrr < MAX_PATTERN_LEN; ttrr++)
    {
      PatternRow *row = &pattern->trackrows[t][ttrr];
      if ( (!(PATROW_EMPTY(row))) || ttrr == (MAX_PATTERN_LEN - 1))
      {
        // we found a filled row or we made it to the end of pattern
        ttrr -= ( (PATROW_EMPTY(row)) ? 0 : 1);
        int num_empty = ttrr - tr;
        if (num_empty == 0)
          break;
        else if (num_empty == 1)
          cbyte |= ( 1<<CBIT_RLE_ONLY1 );
        else if (num_empty == 2)
          cbyte |= ( (1<<CBIT_RLE) | (1<<CBIT_RLE_ONLY1) );
        else
        {
          cbyte |= ( 1<<CBIT_RLE );
          rlebyte = num_empty;
        }

        break;
      }
    }

    if ( tr == 0 && (
          pr->note == NOTE_NONE &&
          pr->instr == 0 &&
          pr->vol == 0 && pr->fx == 0 && pr->fxparam == 0) )
    {
      cbyte |= 1<<CBIT;
    }
    // Only if every element is filled are we NOT going to use a
    // special compression byte. so let's check if every element is
    // filled first.
    else if (! (
          pr->note != NOTE_NONE &&
          pr->instr != 0 &&
          pr->vol != 0 && pr->fx != 0 && pr->fxparam != 0) )
    {
      cbyte |=
        (pr->note ? ( 1<<CBIT_NOTE ) : 0) |
        (pr->instr ? ( 1<<CBIT_INSTR ) : 0) |
        (pr->vol ? ( 1<<CBIT_VOL ) : 0) |
        (pr->fx ? ( 1<<CBIT_FX ) : 0) |
        (pr->fxparam ? ( 1<<CBIT_FXPARAM ) : 0);
    }

    if (cbyte)
    {
      cbyte |= ( 1<<CBIT );
      write(file, &cbyte, 1, 1, &datachunklen);
    }
    // we should now write the actual byte for any data that is
    // present
    if (pr->note)
      write(file, &pr->note, 1, 1, &datachunklen);
    if (pr->instr)
      write(file, &pr->instr, 1, 1, &datachunklen);
    if (pr->vol)
      write(file, &pr->vol, 1, 1, &datachunklen);
    if (pr->fx)
      write(file, &pr->fx, 1, 1, &datachunklen);
    if (pr->fxparam)
      write(file, &pr->fxparam, 1, 1, &datachunklen);
    if ( (cbyte & (( 1<<CBIT_RLE ) | ( 1<<CBIT_RLE_ONLY1 )) ) == ( 1<<CBIT_RLE ) )
      write(file, &rlebyte, 1, 1, &datachunklen);

    tr = ttrr; // skip over empty rows
  }

  DEBUGLOG("\t\t\tWriting chunksize\n");
  chunkend_location = SDL_RWtell(file);
  SDL_RWseek(file, chunksize_location, RW_SEEK_SET);
  SDL_RWwrite(file, &datachunklen, 2, 1);
  SDL_RWseek(file, chunkend_location, RW_SEEK_SET);

  return chunklen + headerlen + datachunklen; // return the chunk + header size
}

PatternChunkLoader::PatternChunkLoader(struct PatternMeta *patterns) : 
  ChunkLoader(ChunkID::Pattern), patterns(patterns)
{}

size_t PatternChunkLoader::load(SDL_RWops *file, size_t chunksize)
{
  size_t maxread = 0;
  uint8_t idx = 0;
  bool idx_loaded = false;

  DEBUGLOG("PatternChunkLoader::load()\n");

  while (maxread < chunksize)
  {
    uint8_t subchunkid;
    uint16_t subchunksize;

    //DEBUGLOG("\tmaxread = %zu\n", maxread);

    if (read(file, &subchunkid, 1, 1, &maxread) == 0)
      break;
    if (read(file, &subchunksize, 2, 1, &maxread) == 0)
      break;

    //DEBUGLOG("\tsubchunksize = %hu\n", subchunksize);

    switch (subchunkid)
    {
      case SubChunkID::coreinfo:
      {
        DEBUGLOG("\tSubChunkID::coreinfo\n");
        size_t minimum_chunksize = 2, maximum_chunksize = 2;
        if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected.\n", subchunkid);
          return -1;
        }
        else if (subchunksize > maximum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected by %d bytes.\n",
            subchunkid, subchunksize - maximum_chunksize);
        }

        read(file, &idx, 1, 1, &maxread);
        idx_loaded = true;
        read(file, &patterns[idx].p.len, 1, 1, &maxread);

        subchunksize -= 2;
      }
      break;
      case SubChunkID::Track:
      {
        assert(idx_loaded);
        DEBUGLOG("\tSubChunkID::Track\n");

        TrackChunkLoader tfl(&patterns[idx].p, 0);
        size_t bytesread = tfl.load(file, subchunksize);
        //DEBUGLOG("\tsubchunksize = %d, bytesread = %d\n", subchunksize, bytesread);
        if (bytesread <= 0)
        {
          // ERROR
        }
        else
        {
          subchunksize -= bytesread;
          maxread += bytesread;
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
  return maxread;
}

size_t PatternChunkLoader::save(SDL_RWops *file)
{
  DEBUGLOG("PatternChunkLoader::save()\n");
  // PATTERNS
  // First calculate the number of used patterns in the song. This is not
  // sequence length, but the number of unique patterns. With that length
  // calculated, we can allocate the amount of RAM necessary for the
  // PatternLUT (detailed below comments).
  //uint8_t num_usedpatterns = 0;

  // NOTE: Unlike the render_to_apu, which only exports patterns that are
  // present in the sequencer, we will check here for ANY patterns that
  // are not fully blank, whether or not they are present in the sequencer
  for (uint8_t p=0; p < MAX_PATTERNS; p++)
  {
    PatternMeta *pm = &patterns[p];
    struct Pattern *pattern = &pm->p;
    if (pm->used == 0) // not in sequencer, but is there data?
    {
      bool hasdata = false;

      for (int t=0; t < MAX_TRACKS; t++)
        for (int tr=0; tr < pattern->len; tr++)
        {
          PatternRow *pr = &pattern->trackrows[t][tr];
          if (pr->note || pr->instr || pr->vol || pr->fx || pr->fxparam)
          {
            hasdata = true;
            break;
          }
        }
      if (!hasdata)
        continue;
    }

    /* Begin writing chunk for the pattern */
    uint8_t byte;
    uint16_t word;
    uint16_t chunklen = 0;
    Sint64 chunksize_location, chunkend_location;

    DEBUGLOG("\tPattern #%d, Writing top-level chunkid: %d\n", p, chunkid);

    // write this master chunk id
    byte = chunkid;
    SDL_RWwrite(file, &byte, 1, 1);
    // stub the master chunksize for later writing
    chunksize_location = SDL_RWtell(file);
    SDL_RWwrite(file, &chunklen, 2, 1);

    // Store CoreInfo
    DEBUGLOG("\t\tSubChunkID::coreinfo\n");
    byte = SubChunkID::coreinfo;
    write(file, &byte, 1, 1, &chunklen);
    word = sizeof(p) + sizeof(Pattern::len); // we only have the pattern idx so far
    assert(word == 2);
    write(file, &word, 2, 1, &chunklen);
    write(file, &p, 1, 1, &chunklen); // write pattern index
    write(file, &pattern->len, 1, 1, &chunklen);

    for (int t=0; t < MAX_TRACKS; t++)
    {
      /* Skip empty tracks */
      bool emptytrack = true;
      for (int tr=0; tr < pattern->len; tr++)
      {
        PatternRow *row = &pattern->trackrows[t][tr];
        if (!PATROW_EMPTY(row))
        {
          emptytrack = false;
          break;
        }
      }

      if (emptytrack)
        continue;

      // The track was not empty:

      Sint64 chunksize_location_track, chunkend_location_track;

      // Write the Track subchunkID
      DEBUGLOG("\t\tSubChunkID::Track (%d)\n", t);
      byte = SubChunkID::Track;
      write(file, &byte, 1, 1, &chunklen);
      chunksize_location_track = SDL_RWtell(file);
      write(file, &word, 2, 1, &chunklen); // write some junk to the len field for now

      TrackChunkLoader tfl(pattern, t);
      uint16_t chunklen_track = tfl.save(file);

      DEBUGLOG("\t\tWriting Track chunksize\n");
      chunkend_location_track = SDL_RWtell(file);
      SDL_RWseek(file, chunksize_location_track, RW_SEEK_SET);
      SDL_RWwrite(file, &chunklen_track, 2, 1);
      SDL_RWseek(file, chunkend_location_track, RW_SEEK_SET);

      chunklen += chunklen_track;
    }

    // END PATTERN
    DEBUGLOG("\tWriting chunksize\n");
    chunkend_location = SDL_RWtell(file);
    SDL_RWseek(file, chunksize_location, RW_SEEK_SET);
    SDL_RWwrite(file, &chunklen, 2, 1);
    SDL_RWseek(file, chunkend_location, RW_SEEK_SET);
  }
  return 0;
}

PatternSequencerChunkLoader::PatternSequencerChunkLoader(struct PatternSequencer *patseq) :
  ChunkLoader(ChunkID::PatternSequencer), patseq(patseq)
{}

size_t PatternSequencerChunkLoader::load(SDL_RWops *file, size_t chunksize)
{
  size_t maxread = 0;
  uint8_t idx = 0;
  bool idx_loaded = false;

  DEBUGLOG("PatternSequencerChunkLoader::load()\n");

  while (maxread < chunksize)
  {
    uint8_t subchunkid;
    uint16_t subchunksize;

    //DEBUGLOG("\tmaxread = %llu\n", maxread);

    if (read(file, &subchunkid, 1, 1, &maxread) == 0)
      break;
    if (read(file, &subchunksize, 2, 1, &maxread) == 0)
      break;

    //DEBUGLOG("\tsubchunksize = %llu\n", subchunksize);

    switch (subchunkid)
    {
      case SubChunkID::coreinfo:
      {
        DEBUGLOG("\tSubChunkID::coreinfo\n");
        size_t minimum_chunksize = 0;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }
      }
      break;
      case SubChunkID::Entries:
      {
        DEBUGLOG("\tSubChunkID::Entries\n");
        size_t minimum_chunksize = 1, maximum_chunksize = 256;
        if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }
        else if (subchunksize > maximum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected by %d bytes.\n",
            subchunkid, subchunksize - maximum_chunksize);
        }

        patseq->num_entries = subchunksize;
        for (int i=0; subchunksize; subchunksize--, i++)
        {
          read(file, &patseq->sequence[i], 1, 1, &maxread);
          patseq->patterns[patseq->sequence[i]].used++;
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

size_t PatternSequencerChunkLoader::save(SDL_RWops *file)
{
  uint8_t byte;
  uint16_t word;
  uint16_t chunklen = 0;
  Sint64 chunksize_location, chunkend_location;

  DEBUGLOG("PatternSequencerChunkLoader::save()\n");
  DEBUGLOG("\tWriting top-level chunkid: %d\n", chunkid);

  // write this master chunk id
  byte = chunkid;
  SDL_RWwrite(file, &byte, 1, 1);
  // stub the master chunksize for later writing
  chunksize_location = SDL_RWtell(file);
  SDL_RWwrite(file, &chunklen, 2, 1);

  DEBUGLOG("\t\tSubChunkID::Entries\n");
  byte = SubChunkID::Entries;
  write(file, &byte, 1, 1, &chunklen);
  word = patseq->num_entries; // we know the length in advance
  write(file, &word, 2, 1, &chunklen);
  // PATTERN SEQUENCER START
  for (int i=0; i < patseq->num_entries; i++)
    write(file, &patseq->sequence[i], 1, 1, &chunklen);

  DEBUGLOG("\tWriting chunksize\n");
  chunkend_location = SDL_RWtell(file);
  SDL_RWseek(file, chunksize_location, RW_SEEK_SET);
  SDL_RWwrite(file, &chunklen, 2, 1);
  SDL_RWseek(file, chunkend_location, RW_SEEK_SET);
  return 0;
}




const int PatSeqPanel::VISIBLE_ROWS;

PatSeqPanel::PatSeqPanel(PatternSequencer *patseq) :
  title("Pat. Sequencer"),
  clonebtn("Clone", PatSeqPanel::clone, this),
  seqbtn("New", PatSeqPanel::seq, this),
  clearbtn("Del", PatSeqPanel::clear, this),
  insbtn("Ins", PatSeqPanel::insertPat, this),
  zapbtn("Clr", PatSeqPanel::zapPat, this),
  incpatbtn("+", PatSeqPanel::incpat, this),
  decpatbtn("-", PatSeqPanel::decpat, this),
  movePatUpbtn(FONT_CODE_V_TRI_STR, PatSeqPanel::movePatUp, this),
  movePatDownbtn(FONT_CODE_V_TRI_STR, PatSeqPanel::movePatDown, this),

  patseq(patseq),
  lastTimeScrolled(0),

  // PatLenWidget stuff
  patlen_cbuf("$040"),
	patlen_title("Pat Len:"),
	patlen_valtext(patlen_cbuf),
	patlen_incbtn("+", inc_patlen, this, true),
	patlen_decbtn("-", dec_patlen, this, true)
{
}

PatSeqPanel::~PatSeqPanel()
{
}

#define min(x, y) ((x) <= (y) ? (x) : (y))

void PatSeqPanel::set_coords(int x, int y)
{
  int xx = x, yy = y;

  rect.x = x - 2; /* All rect.* are creating the bounding rect to be drawn
                     around the instruments list (not the buttons or main label. Unfortunately the
                     assignments are spread throughout this function rather than all being in one place,
                     due to the nature of the coordinates being manipulated to create the locations
                     of all pspanel entities */
  rect.y = y;


  //x += title.rect.w + (2*CHAR_WIDTH);
  title.rect.x = x;
  title.rect.y = y;

  y += (CHAR_HEIGHT * 2) + 2;

  

  clonebtn.rect.x = x;
  clonebtn.rect.y = y;
  x += clonebtn.rect.w + (CHAR_WIDTH) + (CHAR_WIDTH/2);
  
  seqbtn.rect.x = x;
  seqbtn.rect.y = y;
  x += seqbtn.rect.w + (CHAR_WIDTH) + (CHAR_WIDTH/2);

  insbtn.rect.x = x;
  insbtn.rect.y = y;

  y += (CHAR_HEIGHT * 2);

  rect_patTable.x = rect.x;
  rect_patTable.y = y;
  rect_patTable.w = (5 * CHAR_WIDTH) + 2;
  rect_patTable.h = (CHAR_HEIGHT * (VISIBLE_ROWS)) + 1;


  y += 2; //(CHAR_HEIGHT / 2);

  clearbtn.rect.x = x;
  clearbtn.rect.y = y;

  int zap_y = y + CHAR_HEIGHT + (CHAR_HEIGHT / 2) + 4;
  zapbtn.rect.x = x;
  zapbtn.rect.y = zap_y;

  /* This init was postponed until now to avoid having to iterate through
   * all instruments multiple times */
  for (int i=0; i < VISIBLE_ROWS; i++)
  {
    index_text[i].str = index_strings[i];
    conv_idx2ascii2(rows_scrolled + i, index_strings[i]);

    index_text[i].rect =
    {
      xx,
      y + (CHAR_HEIGHT * i),
      3 * CHAR_WIDTH, // eg 00|
      CHAR_HEIGHT
    };

    /* The GUI names[i].str now points to the core (tracker)'s
     * instruments[i].str, meaning that when we load a file into the
     * tracker (core), these strings should automatically update after a
     * redraw */
    names[i].str = pattern_strings[i];
    names[i].rect = index_text[i].rect;
    names[i].rect.x += 3 * CHAR_WIDTH;
    names[i].rect.w = 2 * CHAR_WIDTH;
  }

  // put the inc/dec pat buttons to the right of the rect.
  decpatbtn.rect.x = rect_patTable.x + rect_patTable.w + (CHAR_WIDTH) + (CHAR_WIDTH/2);
  decpatbtn.rect.y = rect_patTable.y + 2;
  incpatbtn.rect.x = decpatbtn.rect.x + incpatbtn.rect.w + 5;
  incpatbtn.rect.y = decpatbtn.rect.y;// + CHAR_HEIGHT;

  movePatUpbtn.rect.x = decpatbtn.rect.x;
  movePatUpbtn.rect.y = decpatbtn.rect.y + (CHAR_HEIGHT * 2);
  movePatDownbtn.rect.x = movePatUpbtn.rect.x;
  movePatDownbtn.rect.y = movePatUpbtn.rect.y + CHAR_HEIGHT + 5;

// PATTERN LENGTH PORTION:
  patlen_title.rect.x = xx;
	patlen_title.rect.y = rect_patTable.y + rect_patTable.h + (CHAR_HEIGHT * 2) + 1;// + (CHAR_HEIGHT / 2);
	patlen_valtext.rect.x = patlen_title.rect.x + ((strlen(patlen_title.str) + 1) * CHAR_WIDTH);
	patlen_valtext.rect.y = patlen_title.rect.y;
	patlen_decbtn.rect.x  = patlen_valtext.rect.x + (5 * CHAR_WIDTH);
	patlen_decbtn.rect.y = patlen_title.rect.y;
	patlen_incbtn.rect.x = patlen_decbtn.rect.x + CHAR_WIDTH + 5;
	patlen_incbtn.rect.y = patlen_title.rect.y;
// END PATLEN PORTION


  rect.w = (patlen_incbtn.rect.x + patlen_incbtn.rect.w) - rect.x;
  rect.h = (patlen_title.rect.y + patlen_title.rect.h) - rect.y;
}

static void apuSetPattern(int currow)
{
  if (::tracker->playback)
  {
    // send command to set the pattern
    ::player->spc_emu()->write_port(2, currow);
    ::player->spc_emu()->write_port(1, SPCCMD_SETPATTERN);
    while (::player->spc_emu()->read_port(1) != SPCCMD_SETPATTERN)
      SDL_Delay(1);
    ::player->spc_emu()->write_port(1, SPCCMD_NOP);
  }
}

int PatSeqPanel::event_handler(const SDL_Event &ev)
{
  if (mousewheel_rows_event_handler(ev, &rows_scrolled, VISIBLE_ROWS, patseq->num_entries, &rect))
  {
    // mark the last time you've scrolled
    lastTimeScrolled = SDL_GetTicks();
  }

  /* If the user clicks within a certain row rect. A row rect is comprised
   * of the index region (including padding), the spacer, and the
   * instrument name field (including padding).*/
  for (int i=0; i < min(VISIBLE_ROWS, (patseq->num_entries - rows_scrolled)); i++)
  {
    /* Check the rect of the indice. Unfortunately, unless I edit the TER
     * code to allow using an alternative Rect (Clickable_Rect code
     * really), then I can't composite an all-inclusive rect to do the
     * check once */
    SDL_Rect r = index_text[i].rect;
    /* Need to be careful this this rect does not overlap with the TER */
    r.w += names[i].rect.x - (r.x + r.w) - 1;
    assert(r.x + r.w < names[i].rect.x);

    switch (ev.type)
    {
      case SDL_MOUSEBUTTONDOWN:
        {
          switch (ev.button.button)
          {
            case SDL_BUTTON_LEFT:
              {
                if (Utility::coord_is_in_rect(ev.button.x,ev.button.y, &r)
                    || Utility::coord_is_in_rect(ev.button.x, ev.button.y,
                      &names[i].rect))
                {
                  if ((currow - rows_scrolled) != i)
                  {
                    //SDL_FillRect(::render->screen, &highlight_r, Colors::transparent);
                    set_currow(rows_scrolled + i);

                    apuSetPattern(currow);
                  }
                  return 1;
                }
              } break;
            default:break;
          }
        } break;
      default:break;
    }
  }

  switch(ev.type)
  {
    case SDL_KEYDOWN:
      {
        int scancode = ev.key.keysym.sym;
        int mod = ev.key.keysym.mod;

        /* You're currently editing a TER, so the only way you'll be
         * getting out of that is manually finishing (eg pressing enter or
         * double clicking on the field, or by clicking on some other area
         * that cancels focus on the TER */
        if (Text_Edit_Rect::cur_editing_ter)
          break;

        switch(scancode)
        {
          case SDLK_UP:
            if (MODONLY(mod, CMD_CTRL_KEY))
            {
              dec_currow();
              apuSetPattern(currow);
            }
            break;
          case SDLK_DOWN:
            if (MODONLY(mod, CMD_CTRL_KEY))
            {
              inc_currow();
              apuSetPattern(currow);
            }
            break;
          case SDLK_LEFT:
            if (MODONLY(mod, CMD_CTRL_KEY))
              decpat(this);
          break;
          case SDLK_RIGHT:
            if (MODONLY(mod, CMD_CTRL_KEY))
              incpat(this);
          break;
          case SDLK_F9:
            if (MODONLY(mod, CMD_CTRL_KEY))
              clear(this);
          break;
          case SDLK_F10:
            if (MODONLY(mod, CMD_CTRL_KEY))
            {
              //insert_pattern();
              PatternMeta *p = get_current_pattern_meta(this);

              if (!p || currow >= (MAX_PATTERNS - 1))
                break;
              // Add the entry to the sequencer by inserting it after currow
              patseq->sequence.insert(patseq->sequence.begin()+currow+1,
                  patseq->sequence[currow]);

              p->used += 1;

              if ((currow - rows_scrolled) % VISIBLE_ROWS == (VISIBLE_ROWS - 1))
                rows_scrolled++;

              patseq->num_entries++;
              currow++;
            }
          break;
          case SDLK_F11:
            if (MODONLY(mod, CMD_CTRL_KEY))
              clone_seq_common(this);
          break;
          case SDLK_F12:
          if (MODONLY(mod, CMD_CTRL_KEY))
            clone(this);
          break;
          // Mute / Solo kbd shortcuts
          case SDLK_1:
          case SDLK_2:
          case SDLK_3:
          case SDLK_4:
          case SDLK_5:
          case SDLK_6:
          case SDLK_7:
          case SDLK_8:
            if (MODONLY(mod, KMOD_ALT))
              Voice_Control::toggle_mute(scancode - SDLK_1 + 1);
            else if (MODONLY(mod, CMD_CTRL_KEY))
              Voice_Control::toggle_solo(scancode - SDLK_1 + 1);
          break;

          default:break;
        }
      } break;
  }
  clonebtn.check_event(ev);
  seqbtn.check_event(ev);
  clearbtn.check_event(ev);
  insbtn.check_event(ev);
  zapbtn.check_event(ev);
  incpatbtn.check_event(ev);
  decpatbtn.check_event(ev);
  movePatUpbtn.check_event(ev);
  movePatDownbtn.check_event(ev);

  // PatLenWidget stuff
	patlen_incbtn.check_event(ev);
	patlen_decbtn.check_event(ev);

  return 0;
}

void PatSeqPanel::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
{
  Utility::DrawRect(&rect_patTable, 1);
}

void PatSeqPanel::draw(SDL_Surface *screen/*=::render->screen*/)
{
  one_time_draw();

  // draw the title with the same background color as the bg rect
  title.draw(screen, Colors::Interface::color[Colors::Interface::Type::text_fg],
             true, false, false, Colors::Interface::color[Colors::Interface::Type::patseqpanelBG]);

  clonebtn.draw(screen);
  seqbtn.draw(screen);
  clearbtn.draw(screen);
  insbtn.draw(screen);
  zapbtn.draw(screen);
  incpatbtn.draw(screen);
  decpatbtn.draw(screen);
  movePatUpbtn.draw(screen);
  movePatDownbtn.draw(screen, true);

  // PatLenWidget Stuff
  update_patlen();
  // Need to draw this title with the same background color as the rect_bg
  patlen_title.draw(screen, Colors::Interface::color[Colors::Interface::Type::text_fg],
             true, false, false, Colors::Interface::color[Colors::Interface::Type::patseqpanelBG]);

	patlen_valtext.draw(screen, Colors::Interface::color[Colors::Interface::Type::text_fg],
             true, false, false, Colors::Interface::color[Colors::Interface::Type::patseqpanelBG]);

	patlen_incbtn.draw(screen);
	patlen_decbtn.draw(screen);
  // End PatLenWidget Stuff

  SDL_Rect r = {rect_patTable.x + 1, rect_patTable.y + 1, rect_patTable.w - 1, rect_patTable.h - 1};
  SDL_FillRect(screen, &r, Colors::transparent);

  /* This should really be put in init and event code, to decrease
   * redundant processing */
  if (currow >= rows_scrolled && currow < (rows_scrolled + VISIBLE_ROWS))
  {
    highlight_r = index_text[currow - rows_scrolled].rect;
    highlight_r.y -= 1;
    highlight_r.w +=
      (names[currow - rows_scrolled].rect.x - (highlight_r.x + highlight_r.w)) +
      names[currow - rows_scrolled].rect.w;

    SDL_FillRect(screen, &highlight_r, Colors::Interface::color[Colors::Interface::Type::selections]);
  }

  for (int i=0; i < min(VISIBLE_ROWS, (patseq->num_entries - rows_scrolled)); i++)
  {
    // Draw the index followed by the instrname
    // But beneath this, draw the highlighted currow rect
    /* In order to draw this rect, we need to the total horizontal width
     * of the index + instr_name, + some padding maybe. That should be
     * calculated at init and in the event handler */
    conv_idx2ascii(rows_scrolled + i, index_strings[i]);
    index_text[i].draw(screen,
        Colors::Interface::color[Colors::Interface::Type::text_fg],
        false);
    
    /*const SDL_Rect *tmp = &index_text[i].rect;
    sdlfont_drawString(screen,
        tmp->x + tmp->w - CHAR_WIDTH, index_text[i].rect.y,
        "|", Colors::Interface::color[Colors::Interface::Type::text_fg],
        Colors::Interface::color[Colors::Interface::Type::text_bg],
        false);*/

    conv_idx2ascii2(patseq->sequence[rows_scrolled + i], pattern_strings[i]);
    names[i].draw(Colors::Interface::color[Colors::Interface::Type::text_fg],
        false);
  }
}

int PatSeqPanel::movePatUp(void *pspanel)
{
  fprintf(stderr, "PatSeqPanel::movePatUP()\n");
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;

  if ( psp->currow == 0 )
    return 1;
  
  auto copy = patseq->sequence[psp->currow - 1];
  patseq->sequence[psp->currow - 1] = patseq->sequence[psp->currow];
  patseq->sequence[psp->currow] = copy;

  psp->set_currow ( psp->currow - 1 );
  *patseq->metadata.changed = true;

  return 0;
}

int PatSeqPanel::movePatDown(void *pspanel)
{
  fprintf(stderr, "PatSeqPanel::movePatDOWN()\n");
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;

  if ( psp->currow > (patseq->num_entries - 2) )
    return 1;
  
  auto copy = patseq->sequence[psp->currow + 1];
  patseq->sequence[psp->currow + 1] = patseq->sequence[psp->currow];
  patseq->sequence[psp->currow] = copy;

  psp->set_currow ( psp->currow + 1 );
  *patseq->metadata.changed = true;
  return 0;
}

int PatSeqPanel::insertPat(void *pspanel)
{
  fprintf(stderr, "PatSeqPanel::insertPat()\n");
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;

  patseq->sequence.insert(patseq->sequence.begin()+psp->currow+1, patseq->sequence[psp->currow]);
  patseq->num_entries++;
  psp->set_currow(psp->currow + 1);

  PatternMeta *pm = &patseq->patterns[patseq->sequence[psp->currow]];
  pm->used++;

  *patseq->metadata.changed = true;
  return 0;
}

int PatSeqPanel::zapPat(void *pspanel)
{
  fprintf(stderr, "PatSeqPanel::zapPat()\n");
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;

  PatternMeta *pm = &patseq->patterns[patseq->sequence[psp->currow]];
  auto lenBackup = pm->p.len;
  memset(pm, 0, sizeof(PatternMeta));
  pm->p.len = lenBackup;
  pm->used = 1;

  //clear(pspanel);
}

////////////////////// START PATTERN EDITOR STUFF ///////////////////////

static int get_unused_pattern_index(PatternSequencer *ps)
{
  for (int i=0; i < MAX_PATTERNS; i++)
  {
    bool skipPat = false;
    const PatternMeta *pm = &ps->patterns[i];
    const Pattern *p = &pm->p;

    if (pm->used)
      continue;

    for (int t=0; t < MAX_TRACKS; t++)
    {
      for (int r=0; r < MAX_PATTERN_LEN; r++)
      {
        const PatternRow *row = &p->trackrows[t][r];

        if (!PATROW_EMPTY(row))
        {
          skipPat = true;
          break;
        }
      }
      if (skipPat)
        break;
    }
    if (!skipPat)
      return i;
  }
  return -1;
}

int clone_seq_common(PatSeqPanel *psp)
{
  PatternSequencer *patseq = psp->patseq;
  /* Copy the "currow" pattern into the next pattern marked by "unused" */
  // Get the currow pattern
  //psp->currow, psp->patseq->{unused, num_entries, sequence, patterns}
  // Make sure whether there are no more free patterns
  int unused_index = get_unused_pattern_index(patseq);
  PatternMeta *up = unused_index == -1 ? NULL : &patseq->patterns[unused_index];

  if (!up || psp->currow >= (MAX_PATTERNS - 1))
    return -1;
  // We can copy all pattern data from patseq->patterns[patseq->sequence[psp->currow]]
  // to patseq->patterns[unused++]
  // Add the entry to the sequencer by inserting it after currow
  patseq->sequence.insert(patseq->sequence.begin()+psp->currow+1,
      unused_index);

  up->used += 1;
  patseq->num_entries++;
  psp->set_currow(psp->currow + 1);

  *patseq->metadata.changed = true;
  return 0;
}

int PatSeqPanel::clone(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::clone()\n");
  if(!clone_seq_common(psp))
    memcpy(&patseq->patterns[patseq->sequence[psp->currow]].p,
      &patseq->patterns[patseq->sequence[psp->currow - 1]].p,
      sizeof(Pattern));

  return 0;
}

int PatSeqPanel::seq(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::seq()\n");
  clone_seq_common(psp);
  // Set the new pattern's length to the same as the previous one
  PatternMeta *pm = &patseq->patterns[patseq->sequence[psp->currow]];
  pm->p.len = (pm - 1)->p.len; 
  return 0;
}

int PatSeqPanel::clear(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::clear()\n");

  if (patseq->num_entries == 1)
    return -1;

  PatternMeta *tp = &patseq->patterns[patseq->sequence[psp->currow]];
  tp->used--;
  if (tp->used < 0)
  {
    DEBUGLOG("Pattern sequence %d psp->currow \"used\" went below 0 ?!\n", psp->currow);
    tp->used = 0;
  }

  // Delete the patter entry at currow from the sequencer
  patseq->sequence.erase(patseq->sequence.begin()+psp->currow);
  if (patseq->num_entries == (psp->rows_scrolled + 1))
    psp->rows_scrolled--;
  patseq->num_entries--;
  if (psp->currow > 0 && psp->currow == patseq->num_entries)
    psp->currow--;
  if (psp->currow >= patseq->num_entries)
    psp->currow = patseq->num_entries - 1;

  *patseq->metadata.changed = true;

  /* Fixes #36 */
  ::tracker->main_window.pateditpanel.set_currow(::tracker->main_window.pateditpanel.currow);

  return 0;
}

int PatSeqPanel::incpat(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::incpat()\n");

  uint8_t *derp = &patseq->sequence[psp->currow];
  /* Increment the currow pattern */
  patseq->patterns[patseq->sequence[psp->currow]].used--;
  *derp += 1;
  if (*derp >= MAX_PATTERNS)
    *derp = MAX_PATTERNS - 1;

  patseq->patterns[patseq->sequence[psp->currow]].used++;
  *patseq->metadata.changed = true;

  /* Fixes #36 */
  ::tracker->main_window.pateditpanel.set_currow(::tracker->main_window.pateditpanel.currow);
  return 0;
}

int PatSeqPanel::decpat(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::decpat()\n");
  uint8_t *derp = &patseq->sequence[psp->currow];

  patseq->patterns[patseq->sequence[psp->currow]].used--;
  *derp -= 1;
  if (*derp == 0xff) // wrapped around?
    *derp = 0;

  patseq->patterns[patseq->sequence[psp->currow]].used++;
  *patseq->metadata.changed = true;

  /* Fixes #36 */
  ::tracker->main_window.pateditpanel.set_currow(::tracker->main_window.pateditpanel.currow);
  return 0;
}

void PatSeqPanel::set_currow(int row, bool updateScrolled/*=true*/)
{
  if (row >= patseq->num_entries)
    row = patseq->num_entries - 1;
  else if (row < 0)
    row = 0;

	currow = row;

  /* If the next pattern's length is shorter than the current row of the pattern
   * editor panel, then set it to the last row of that pattern */
  Pattern *p = get_current_pattern(this);
  PatternEditorPanel &pep = ::tracker->main_window.pateditpanel;
  if ( pep.currow >= p->len)
    pep.set_currow(p->len - 1);
  //else if ( pep.currow < pep.rows_scrolled )
    pep.set_currow(pep.currow);

  // (when the song is playing, If the user scrolled in the patseqpanel,
  // the update mechanism won't interfere for 2000 ms
  if ( updateScrolled && ( !::tracker->rendering() || (SDL_GetTicks() > (lastTimeScrolled + 2000)) ) )
  {
    if (currow < rows_scrolled)
      rows_scrolled = currow;
    else if (currow >= (rows_scrolled + VISIBLE_ROWS))
      rows_scrolled = currow - VISIBLE_ROWS + 1;
    else if (row == 0)
      rows_scrolled = 0;
  }
}

void PatSeqPanel::inc_currow()
{
  set_currow(currow + 1);
}

void PatSeqPanel::dec_currow()
{
  set_currow(currow - 1);
}

/* these functions query the proper handles on that real data. */
void PatSeqPanel :: update_patlen()
{
	uint8_t *len = &patseq->patterns[patseq->sequence[currow]].p.len;
	sprintf(patlen_cbuf, "$%03x", *len);
}

int PatSeqPanel::inc_patlen(void *bsaw)
{
	PatSeqPanel *b = (PatSeqPanel *)bsaw;
	//DEBUGLOG("inc_patlen; ");
	::tracker->inc_patlen();
	b->update_patlen();
	return 0;
}

int PatSeqPanel::dec_patlen(void *bsaw)
{
	PatSeqPanel *b = (PatSeqPanel *)bsaw;
	::tracker->dec_patlen();
	b->update_patlen();
	return 0;
}

///////////////////////////////////////////////////////////////////////
//////////////// BEGIN PATTERN EDITOR //////////////////////////////////
//

const int PatternEditorPanel::MAX_VISIBLE_ROWS;
int PatternEditorPanel::VISIBLE_ROWS = PatternEditorPanel::MAX_VISIBLE_ROWS;

PatternEditorPanel::PatternEditorPanel(PatSeqPanel *psp,
  Instrument_Panel *ip) :
    //VISIBLE_ROWS(MAX_VISIBLE_ROWS),
    cur_track(0), cur_octave(4), recording(0), addval(1),
    move_like_addval(0), pattern_wrap(1),
    psp(psp), ip(ip)
{
  SDL_PixelFormat *format = ::render->screen->format;
  flasher.clr[0] = SDL_MapRGB(format, 0x0, 0x0, 0x80);
  flasher.clr[1] = SDL_MapRGB(format, 0x0, 0x20, 0xf0);
  flasher.clr[2] = SDL_MapRGB(format, 0x20, 0x40, 0xe0);
  flasher.clr[3] = SDL_MapRGB(format, 0x10, 0x70, 0xc0);
  flasher.clr[4] = SDL_MapRGB(format, 0x15, 0xa0, 0xa0);
  flasher.clr[5] = SDL_MapRGB(format, 0x10, 0x70, 0xc0);
  flasher.clr[6] = SDL_MapRGB(format, 0x20, 0x40, 0xe0);
}

inline static void fxparam2ascii(int fx, int fxparam, char *c)
{
  c[2] = 0;
  if (fx == 0 && fxparam == 0)
  {
    c[0] = FONT_CENTERDOT_CHAR;
    c[1] = FONT_CENTERDOT_CHAR;
  }
  else
  {
    c[0] = Utility::nibble_to_ascii(fxparam>>4);
    c[1] = Utility::nibble_to_ascii(fxparam);
  }
}

inline static void fx2ascii(int fx, int fxparam, char *c)
{
  c[1] = 0;
  if (fx == 0 && fxparam == 0)
  {
    c[0] = FONT_CENTERDOT_CHAR;
  }
  else
    c[0] = Utility::nibble_to_ascii(fx);
}

inline static void vol2ascii(int vol, char *c)
{
  if (vol == 0)//< 0x10)
  {
    *(c++) = FONT_CENTERDOT_CHAR;
    *(c++) = FONT_CENTERDOT_CHAR;
    *(c++) = 0;
  }
  else if (vol >= 0x1 && vol <= 0x7f)
    conv_idx2ascii2(vol, c);
}

inline static void instr2ascii(int instr, char *c)
{
  c[2] = 0;
  int hi = instr >> 4;
  int lo = instr & 0x0f;

  if (hi == 0)
    *(c++) = FONT_CENTERDOT_CHAR;
  else
    *(c++) = Utility::nibble_to_ascii(hi);

  if (lo == 0 && !hi)
    *(c++) = FONT_CENTERDOT_CHAR;
  else
    *(c++) = Utility::nibble_to_ascii(lo);
}

void note2ascii(uint8_t note, char *c)
{
  if (note == NOTE_NONE)
  {
    /* shoutz to renoise */
    *(c++) = FONT_ELLIPSIS_CHAR;
    *(c++) = FONT_ELLIPSIS_CHAR;
    *(c++) = FONT_ELLIPSIS_CHAR;
    *(c++) = 0;
    return;
  }
  // get the octave by dividing by 12
  int octave = (int)(note - 1) / 12;
  int n = (int)((note - 1) % 12);
  switch (n)
  {
    case 0:
      *(c++) = 'C';
      *(c++) = '-';
      break;
    case 1:
      *(c++) = 'C';
      *(c++) = '#';
      break;
    case 2:
      *(c++) = 'D';
      *(c++) = '-';
      break;
    case 3:
      *(c++) = 'D';
      *(c++) = '#';
      break;
    case 4:
      *(c++) = 'E';
      *(c++) = '-';
      break;
    case 5:
      *(c++) = 'F';
      *(c++) = '-';
      break;
    case 6:
      *(c++) = 'F';
      *(c++) = '#';
      break;
    case 7:
      *(c++) = 'G';
      *(c++) = '-';
      break;
    case 8:
      *(c++) = 'G';
      *(c++) = '#';
      break;
    case 9:
      *(c++) = 'A';
      *(c++) = '-';
      break;
    case 10:
      *(c++) = 'A';
      *(c++) = '#';
      break;
    case 11:
      *(c++) = 'B';
      *(c++) = '-';
      break;
    default:break;
  }
  *(c++) = '0' + octave;
  *(c++) = 0;
}

void PatternEditorPanel::set_coords(int x, int y)
{
  int xx = x, yy = y;

  rect.x = x - 2; /* All rect.* are creating the bounding rect to be drawn
                     around the instruments list (not the buttons or main label. Unfortunately the
                     assignments are spread throughout this function rather than all being in one place,
                     due to the nature of the coordinates being manipulated to create the locations
                     of all pspanel entities */

  y += (CHAR_HEIGHT*4) + (CHAR_HEIGHT/2);
  y += 2;
  rect.y = y;

  y += 2;

  for (int t=0; t < MAX_TRACKS; t++)
  {
    int tx = xx + (CHAR_WIDTH * 3)
            + (CHAR_WIDTH * 5);
    {
      TrackHeader *th = &trackheader[t];
      char *s = th->strings;

      s[0] = Utility::nibble_to_ascii(t + 1);
      s[1] = 0;

      th->ctext.str = th->strings;
      th->ctext.rect = {
        tx + (t*((12*CHAR_WIDTH)-(CHAR_WIDTH/2))),// + (16*t),
        y,
        1*CHAR_WIDTH,
        CHAR_HEIGHT
      };
      // do the bounding rect later here
      th->outline.rect = {
        th->ctext.rect.x - (CHAR_WIDTH*6) + (CHAR_WIDTH/2),
        th->ctext.rect.y - 2,
        (CHAR_WIDTH*12) - (CHAR_WIDTH/2),
        th->ctext.rect.h + 2,
      };
			// store the track# and callback function for muting and soling the
			// voice via the track header
			th->outline.data = (void*) t;
			th->outline.action = Voice_Control::mute_solo_voice;
    }
  }

  y += CHAR_HEIGHT * 1;

  for (int i=0; i < MAX_PATTERN_LEN; i++)
  {
    conv_idx2ascii2(i, index_strings[i]);
    // load up (zero the default pattern and its ctext)
    // assume the tracker has zero'd the pattern data
  }

  for (int i=0; i < VISIBLE_ROWS; i++)
  {
    index_text[i].str = index_strings[i];
    index_text[i].rect = {
      xx,
      y + (CHAR_HEIGHT * i) + 4,
      3 * CHAR_WIDTH, // eg 00|
      CHAR_HEIGHT
    };
  }
  y += 4;
  for (int t=0; t < MAX_TRACKS; t++)
  {
    {
      GUITrackRow *gtr = &guitrackrow[t];

      PatternSequencer *patseq = psp->patseq;
      Pattern *pat = &patseq->patterns[0].p;
      /* Here we can load default pattern data */
      for (int r=0; r < VISIBLE_ROWS; r++)
      {
        PatternRow *patrow = &pat->trackrows[t][r];
        Clickable_Text *ctext;
        char *string;
        //--------------------------------------------------
        ctext  = &gtr->note_ctext[r];
        string = gtr->note_strings[r];

        note2ascii(patrow->note, string);

        ctext->str = string;
        ctext->rect = {
          x + (t*((12*CHAR_WIDTH)-(CHAR_WIDTH/2))) + (3*CHAR_WIDTH) + 3,
          y + (CHAR_HEIGHT * r),
          3 * CHAR_WIDTH, // eg C-4
          CHAR_HEIGHT
        };
        //--------------------------------------------------
        ctext  = &gtr->instr_ctext[r];
        string = gtr->instr_strings[r];

        instr2ascii(patrow->instr, string);

        ctext->str = string;
        ctext->rect = {
          x + (t*((12*CHAR_WIDTH)-(CHAR_WIDTH/2))) + (3*CHAR_WIDTH)+ (3*CHAR_WIDTH) + 3,
          y + (CHAR_HEIGHT * r),
          2 * CHAR_WIDTH, // eg C-4
          CHAR_HEIGHT
        };
        //--------------------------------------------------
        ctext  = &gtr->vol_ctext[r];
        string = gtr->vol_strings[r];

        // what can be used as the default volume value?
        vol2ascii(patrow->vol, string);

        ctext->str = string;
        ctext->rect = {
          x + (t*((12*CHAR_WIDTH)-(CHAR_WIDTH/2))) + (3*CHAR_WIDTH)+ ((3 + 2)*CHAR_WIDTH) + 3,
          y + (CHAR_HEIGHT * r),
          2 * CHAR_WIDTH, // eg C-4
          CHAR_HEIGHT
        };
        //--------------------------------------------------
        ctext  = &gtr->fx_ctext[r];
        string = gtr->fx_strings[r];

        // what can be used as the default volume value?
        fx2ascii(patrow->fx, patrow->fxparam, string);

        ctext->str = string;
        ctext->rect = {
          x + (t*((12*CHAR_WIDTH)-(CHAR_WIDTH/2))) + (3*CHAR_WIDTH)+ ((3 + 2 + 2)*CHAR_WIDTH) + 3,
          y + (CHAR_HEIGHT * r),
          1 * CHAR_WIDTH, // eg C-4
          CHAR_HEIGHT
        };
        //--------------------------------------------------
        ctext  = &gtr->fxparam_ctext[r];
        string = gtr->fxparam_strings[r];

        // what can be used as the default volume value?
        fxparam2ascii(patrow->fx, patrow->fxparam, string);

        ctext->str = string;
        ctext->rect = {
          x + (t*((12*CHAR_WIDTH)-(CHAR_WIDTH/2))) + (3*CHAR_WIDTH)+ ((3 + 2 + 2 + 1)*CHAR_WIDTH) + 3,
          y + (CHAR_HEIGHT * r),
          2 * CHAR_WIDTH, // eg C-4
          CHAR_HEIGHT
        };
        //--------------------------------------------------
      }
    }
  }

  const SDL_Rect *maxx = &trackheader[MAX_TRACKS - 1].outline.rect;


  rect.w = (maxx->x - rect.x) + maxx->w; // (3 * CHAR_WIDTH) + ((3 + 2 + 2 + 1 + 2 * CHAR_WIDTH) * MAX_TRACKS) + 2;

  for (int i=0; i < VISIBLE_ROWS; i++)
  {
    row_rects[i] = index_text[i].rect;
    row_rects[i].x -= 1;
    row_rects[i].w += rect.w - (3*CHAR_WIDTH) - 1;
  }

	// store of a copy of the rect representing all visible rows
	fullsize_r = rect;
	fullsize_r.h = (CHAR_HEIGHT * (1 + VISIBLE_ROWS)) + 6;
}

void PatternEditorPanel::set_visible_rows(int rows)
{
  VISIBLE_ROWS = rows;
  rect.h = (CHAR_HEIGHT * (1 + VISIBLE_ROWS)) + 6;
}

PatternMeta * get_current_pattern_meta(PatSeqPanel *psp)
{
  return &psp->patseq->patterns[psp->patseq->sequence[psp->currow]];
}

Pattern * get_current_pattern(PatSeqPanel *psp)
{
  return &psp->patseq->patterns[psp->patseq->sequence[psp->currow]].p;
}

void PatternEditorPanel::notehelper(int ndex, bool absmidi/*=false*/)
{
  Pattern *pat = get_current_pattern(psp);
  PatternRow *pw = &pat->trackrows[cur_track][currow];
  int n = NOTE_C0 + ndex + ( absmidi ? 0 : (cur_octave * 12) );
  if (n <= NOTE_D6)
  {
    if (recording)
    {
      // get current instrument
      pw->note = (Note)n;
      pw->instr = ip->currow + 1;
      inc_currow(addval);
      *psp->patseq->metadata.changed = true;
      //note2ascii(pw->note, guitrackrow[cur_track].note_strings[currow]);
    }
    //DEBUGLOG("YOU ARE HERE\n");
    /* TODO: Play the sample through the API just like in std instrument
     * window */
    if (::tracker->instr_render && !::tracker->playback)
    {
      ::player->spc_emu()->write_port(0, ip->currow);       // instr#
      ::player->spc_emu()->write_port(2, n - 1);                // note
      ::player->spc_emu()->write_port(3, cur_track);  // track
      ::player->spc_emu()->write_port(1, SPCCMD_PLAYINSTRUMENT);  // command
      while (::player->spc_emu()->read_port(1) != SPCCMD_PLAYINSTRUMENT)
        SDL_Delay(1);
      ::player->spc_emu()->write_port(1, SPCCMD_NOP);  // command
    }
  }
}

void PatternEditorPanel::instrhelper(int n)
{
  Pattern *pat = get_current_pattern(psp);
  PatternRow *pw = &pat->trackrows[cur_track][currow];

  if (n < 0)
    return;

  if (highlighted_subsection != INSTR_HI && highlighted_subsection != INSTR_LO)
  {
    DEBUGLOG("SHOULDN'T BE HERE!" FILE_LINE_FUNC_STR, FILE_LINE_FUNC_ARGS);
    return;
  }

  if (highlighted_subsection == INSTR_HI)
  {
    pw->instr &= 0x0f;
    pw->instr |= (n << 4);
  }
  else
  {
    pw->instr &= 0xf0;
    pw->instr |= n & 0x0f;
  }
  *psp->patseq->metadata.changed = true;
  inc_currow(addval);
  //instr2ascii(pw->note, guitrackrow[cur_track].instr_strings[currow]);
}

void PatternEditorPanel::volhelper(int n)
{
  Pattern *pat = get_current_pattern(psp);
  PatternRow *pw = &pat->trackrows[cur_track][currow];

  if (n < 0)
    return;

  if (highlighted_subsection != VOL_HI && highlighted_subsection != VOL_LO)
  {
    DEBUGLOG("SHOULDN'T BE HERE!"  FILE_LINE_FUNC_STR, FILE_LINE_FUNC_ARGS);
    return;
  }

  if (highlighted_subsection == VOL_HI)
  {
    pw->vol &= 0x0f;
    pw->vol |= (n << 4);
  }
  else
  {
    pw->vol &= 0xf0;
    pw->vol |= n & 0x0f;
  }

  *psp->patseq->metadata.changed = true;
  //vol2ascii(pw->vol, guitrackrow[cur_track].vol_strings[currow]);
}

void PatternEditorPanel::fxhelper(int n)
{
  Pattern *pat = get_current_pattern(psp);
  PatternRow *pw = &pat->trackrows[cur_track][currow];

  if (n < 0)
    return;

  if (highlighted_subsection != FX)
  {
    DEBUGLOG("SHOULDN'T BE HERE!"  FILE_LINE_FUNC_STR, FILE_LINE_FUNC_ARGS);
    return;
  }

  pw->fx = n;

  *psp->patseq->metadata.changed = true;

  //fx2ascii(pw->fx, pw->fxparam, guitrackrow[cur_track].fx_strings[currow]);
}

void PatternEditorPanel::fxparamhelper(int n)
{
  Pattern *pat = get_current_pattern(psp);
  PatternRow *pw = &pat->trackrows[cur_track][currow];

  if (n < 0)
    return;

  if (highlighted_subsection != FXPARAM_HI && highlighted_subsection != FXPARAM_LO)
  {
    DEBUGLOG("SHOULDN'T BE HERE!"  FILE_LINE_FUNC_STR, FILE_LINE_FUNC_ARGS);
    return;
  }

  if (highlighted_subsection == FXPARAM_HI)
  {
    pw->fxparam &= 0x0f;
    pw->fxparam |= (n << 4);
  }
  else
  {
    pw->fxparam &= 0xf0;
    pw->fxparam |= n & 0x0f;
  }
  *psp->patseq->metadata.changed = true;
  //fxparam2ascii(pw->fx, pw->fxparam, guitrackrow[cur_track].fxparam_strings[currow]);
}

void PatternEditorPanel::inc_curtrack()
{
  if (cur_track == (MAX_TRACKS - 1))
    cur_track = 0;
  else cur_track++;
}
void PatternEditorPanel::dec_curtrack()
{
  if (cur_track == 0)
    cur_track = MAX_TRACKS - 1;
  else cur_track--;
}
int PatternEditorPanel::event_handler(const SDL_Event &ev)
{
  Pattern *pat = get_current_pattern(psp);
  mousewheel_rows_event_handler(ev, &rows_scrolled, VISIBLE_ROWS, pat->len, &rect);

  /* If the user clicks within a certain row rect. A row rect is comprised
   * of the index region (including padding), the spacer, and the
   * instrument name field (including padding).*/
  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      {

				for (int i=0; i < MAX_TRACKS; i++)
				{
					uintptr_t newdata = (uintptr_t)trackheader[i].outline.data; // originally the voice number itself
					if (ev.button.button == SDL_BUTTON_RIGHT)
						newdata |= 0x08;
					trackheader[i].outline.check_mouse_and_execute(ev.button.x, ev.button.y, (void*)newdata);
				}

        switch (ev.button.button)
        {
          case SDL_BUTTON_LEFT:
            {
              for (int i=0; i < min(VISIBLE_ROWS, (pat->len - rows_scrolled)); i++)
              {
                if (Utility::coord_is_in_rect(ev.button.x,ev.button.y, &row_rects[i]))
                {
                  /* We've clicked inside the pattern editor, cancel any
                   * currently editing TER */
                  if (Text_Edit_Rect::cur_editing_ter)
                    Text_Edit_Rect::stop_editing(Text_Edit_Rect::cur_editing_ter);

                  // update the row highlighter (really we are "un"drawing
                  // the old highlighter. this may be unnecessary drawing
                  if ((currow - rows_scrolled) != i)
                  {
                    //SDL_FillRect(::render->screen, &highlight_r, Colors::transparent);
                    //SDL_FillRect(::render->screen, &subhighlight_r, Colors::transparent);
                    currow = rows_scrolled + i;
                  }

                  /* At this point, we know what row, but which sub
                   * component was clicked?? */
                  // let's just brute force it I s'pose
                  for (int t=0; t < MAX_TRACKS; t++)
                  {
                    const SDL_Rect *rr;
                    const GUITrackRow *guitr = &guitrackrow[t];

                    rr = &guitr->note_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      highlighted_subsection = NOTE;
                      cur_track = t;
                      return 1;
                    }

                    rr = &guitr->instr_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      SDL_Rect rhilo = *rr;
                      rhilo.w -= CHAR_WIDTH;
                      if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &rhilo))
                        highlighted_subsection = INSTR_HI;
                      else
                        highlighted_subsection = INSTR_LO;

                      cur_track = t;
                      return 1;
                    }

                    rr = &guitr->vol_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      SDL_Rect rhilo = *rr;
                      rhilo.w -= CHAR_WIDTH;
                      if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &rhilo))
                        highlighted_subsection = VOL_HI;
                      else
                        highlighted_subsection = VOL_LO;

                      cur_track = t;
                      return 1;
                    }

                    rr = &guitr->fx_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      cur_track = t;
                      highlighted_subsection = FX;
                      return 1;
                    }

                    rr = &guitr->fxparam_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      SDL_Rect rhilo = *rr;
                      rhilo.w -= CHAR_WIDTH;
                      if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &rhilo))
                        highlighted_subsection = FXPARAM_HI;
                      else
                        highlighted_subsection = FXPARAM_LO;

                      cur_track = t;
                      return 1;
                    }
                  }
                  return 1;
                }
              }
            } break;
          default:break;
        }
      } break;
    case SDL_KEYDOWN:
      {
        int scancode = ev.key.keysym.sym;
        int mod = ev.key.keysym.mod;

        if (Text_Edit_Rect::cur_editing_ter)
          break;

        events_kb_universal(scancode, mod);

        if (!recording)
          piano_kb(scancode, mod);
        else
        {
          // we are recording, a lot depends on what subhighlight we have
          recording_kb(scancode, mod);
        }

      } break;
    case SDL_USEREVENT:
    {
      if (ev.user.code == UserEvents::play_pitch)
      {
        uintptr_t tmp = (uintptr_t)ev.user.data1;
        unsigned char pitch = (unsigned char)tmp;
        DEBUGLOG("tmp=%d, pitch=%d\n", tmp, pitch);
        notehelper(pitch - 12, true);
      }
      else if (ev.user.code == UserEvents::keyoff)
      {
        ::player->spc_write_dsp(dsp_reg::koff, 0xff);
      }
    } break;
    default:break;
  }
  return 0;
}

#define q(n) case SDLK_ ## n : \
  return 0x##n;

static int gethexkb(const int scancode, const int mod)
{
  if (mod)
    return -1;

  switch (scancode)
  {
    q(0);
    q(1);
    q(2);
    q(3);
    q(4);
    q(5);
    q(6);
    q(7);
    q(8);
    q(9);
    q(a);
    q(b);
    q(c);
    q(d);
    q(e);
    q(f);
    default:
    return -1;
  }
}
#undef q

void moveback(Pattern *pattern, int track, int pos, const Instrument_Panel *ip)
{
  if (pos == 0)
    return;
  PatternRow *patrow = pattern->trackrows[track];
	// decrement the row that will be erased's instr used if existent
	PatternRow *prm1 = &patrow[pos - 1];

	for (; pos < pattern->len; pos++)
    patrow[pos - 1] = patrow[pos];
}

static void moveforward(Pattern *pattern, int track, int pos)
{
  if (pos == (pattern->len - 1))
    return;
  PatternRow *patrow = pattern->trackrows[track];
  for (int p = pattern->len - 1; p > pos; p--)
    patrow[p] = patrow[p-1];
  patrow[pos] = PatternRow();
}

void PatternEditorPanel::recording_kb(const int scancode, const int mod)
{
  switch(highlighted_subsection)
  {
    case NOTE:
      piano_kb(scancode, mod);
    break;
    case INSTR_HI:
    case INSTR_LO:
      instrhelper(gethexkb(scancode, mod));
    break;
    case VOL_HI:
    case VOL_LO:
      volhelper(gethexkb(scancode, mod));
    break;
    case FX:
      fxhelper(gethexkb(scancode, mod));
    break;
    case FXPARAM_HI:
    case FXPARAM_LO:
      fxparamhelper(gethexkb(scancode, mod));
    break;
    default:
      DEBUGLOG("SHOULDN'T BE HERE!"  FILE_LINE_FUNC_STR, FILE_LINE_FUNC_ARGS);
    break;
  }

  switch (scancode)
  {
    case SDLK_DELETE:
		{
      switch (highlighted_subsection) {
        case NOTE:
        {
          Pattern *p = get_current_pattern(psp);
          p->trackrows[cur_track][currow] = PatternRow();
        }
        break;
        case INSTR_HI:
        case INSTR_LO:
        {
          Pattern *p = get_current_pattern(psp);
          p->trackrows[cur_track][currow].instr = 0;
        }
        break;
        case VOL_HI:
        case VOL_LO:
        {
          Pattern *p = get_current_pattern(psp);
          p->trackrows[cur_track][currow].vol = 0;
        }
        break;
        case FX:
        {
          Pattern *p = get_current_pattern(psp);
          p->trackrows[cur_track][currow].fx = 0;
          p->trackrows[cur_track][currow].fxparam = 0;
        }
        break;
        case FXPARAM_HI:
        case FXPARAM_LO:
        {
          Pattern *p = get_current_pattern(psp);
          p->trackrows[cur_track][currow].fx = 0;
          p->trackrows[cur_track][currow].fxparam = 0;
        }
        break;
        default:break;
      }
      // update that the song changed
      *psp->patseq->metadata.changed = true;
		}
    break;
    case SDLK_BACKSPACE:
      if (MODONLY(mod, KMOD_SHIFT))
        for (int t=0; t < MAX_TRACKS; t++)
          moveback(get_current_pattern(psp), t, currow, ip);
      else moveback(get_current_pattern(psp), cur_track, currow, ip);
      dec_currow();
      // update that the song changed
      *psp->patseq->metadata.changed = true;
    break;
    case SDLK_INSERT:
#ifdef PLATFORM_MACOSX          // On my Macbook Pro with El Capitan, the usual keycode for INSERT
    case SDLK_KP_ENTER: // Fn + Enter is giving this code, so let's make sure to process it on Mac.
#endif
      if (MODONLY(mod, KMOD_SHIFT))
        for (int t=0; t < MAX_TRACKS; t++)
          moveforward(get_current_pattern(psp), t, currow);
      else moveforward(get_current_pattern(psp), cur_track, currow);
      // update that the song changed
      *psp->patseq->metadata.changed = true;
    break;
    case SDLK_0:
      if (MODONLY(mod, KMOD_ALT))
        move_like_addval = !move_like_addval;
    break;
    case SDLK_MINUS:
      if (MODONLY(mod, KMOD_ALT))
        dec_addval();
    break;
    case SDLK_EQUALS:
      if (MODONLY(mod, KMOD_ALT))
        inc_addval();
    break;
    default:break;
  }
}

void PatternEditorPanel::inc_addval()
{
  if (addval == 0x10)
    addval = 0;
  else addval++;
}

void PatternEditorPanel::dec_addval()
{
  if (addval == 0x00)
    addval = 0x10;
  else addval--;
}

void PatternEditorPanel::set_currow(int row)
{
  auto patlen = get_current_pattern(psp)->len;
	if ( ! ( row < patlen ) )
    row = patlen - 1;
	currow = row;
  DEBUGLOG("currow = %d, rows_scrolled = %d, rs + vr = %d\n", currow, rows_scrolled, rows_scrolled + VISIBLE_ROWS);
	if (currow == 0)
    rows_scrolled = 0;
  else if (currow >= (rows_scrolled + VISIBLE_ROWS))
  {
		rows_scrolled = (currow - VISIBLE_ROWS + 1) > 0 ? (currow - VISIBLE_ROWS + 1) : 0;
    DEBUGLOG("\tset rows_scrolled to %d\n", rows_scrolled);
  }
	else if (currow < VISIBLE_ROWS && ( rows_scrolled > currow) )
  {
    DEBUGLOG("\tset rows_scrolled to 0\n");
		rows_scrolled = 0;
  }
  else if ( ( rows_scrolled + VISIBLE_ROWS ) <= patlen &&
    currow < (VISIBLE_ROWS + rows_scrolled) && ( rows_scrolled <= (currow) ) )
  {
    DEBUGLOG("\tWon't change rows_scrolled\n");
  }
  else if ( ( rows_scrolled + VISIBLE_ROWS ) > patlen )
  /* the very last row is displayed and could be expressed with a lower scroll value */
  {
    DEBUGLOG("\there\n");
    int subtract = (patlen > VISIBLE_ROWS) ? patlen : VISIBLE_ROWS;
    rows_scrolled = subtract - VISIBLE_ROWS;
  }
  else
  {
    DEBUGLOG("\tELSE HIT\n");
    rows_scrolled = (currow - VISIBLE_ROWS + 1) > 0 ? (currow - VISIBLE_ROWS + 1) : 0;
  }
}

void PatternEditorPanel::inc_currow(int howmany/*=1*/, bool wrap/*=true*/)
{
  int len = get_current_pattern(psp)->len;
  int sublen = len - howmany;
  if (sublen < 0) sublen = 0;
  if (currow < sublen)
  {
    currow += howmany;
    if (currow >= (rows_scrolled + VISIBLE_ROWS))
    {
      rows_scrolled = currow - VISIBLE_ROWS + 1;
    }
  }
  else
  {
    if (!wrap)
    {
      currow = len - 1;
      rows_scrolled = currow - (VISIBLE_ROWS-1);
      // bugfix #14 - avoid garbage for patterns < full screen length
      if (rows_scrolled < 0)
        rows_scrolled = 0;
    }
    else
    {
      currow = 0 + (howmany - (len - currow));
      rows_scrolled = 0;

      if (!pattern_wrap) // switch to the next pattern
        psp->inc_currow();
    }
  }
}

void PatternEditorPanel::dec_currow(int howmany/*=1*/, bool wrap/*=true*/)
{
  if (currow >= howmany)
  {
    currow -= howmany;
    if (currow < rows_scrolled)
      rows_scrolled = currow;
  }
  else
  {
    if (!wrap)
    {
      currow = 0;
      rows_scrolled = 0;
    }
    else
    {
      int len = get_current_pattern(psp)->len;
      currow = len - (howmany - currow);
      rows_scrolled = VISIBLE_ROWS > len ? 0 : currow - (VISIBLE_ROWS - 1);

      if (!pattern_wrap)
        psp->dec_currow();
    }
  }
}

void PatternEditorPanel::events_kb_universal(const int scancode, const int mod)
{
  switch(scancode)
  {
    case SDLK_w:
      if (MODONLY(mod, KMOD_ALT))
        pattern_wrap = !pattern_wrap;
    break;
    case SDLK_PAGEUP:
      dec_currow(VISIBLE_ROWS > 16 ? 16 : VISIBLE_ROWS, false);
    break;
    case SDLK_PAGEDOWN:
      inc_currow(VISIBLE_ROWS > 16 ? 16 : VISIBLE_ROWS, false);
    break;
    case SDLK_HOME:
      currow = 0;
      rows_scrolled = 0;
    break;
    case SDLK_END:
      currow = get_current_pattern(psp)->len - 1;
      rows_scrolled = currow - (VISIBLE_ROWS-1);
      // bugfix #14 - avoid garbage for patterns < full screen length
      if (rows_scrolled < 0) rows_scrolled = 0;
    break;
    case SDLK_UP:
      if (MOD_ANY(mod))
        break;
      dec_currow(move_like_addval ? addval : 1);
    break;
    case SDLK_DOWN:
    {
      if (MOD_ANY(mod))
        break;
      inc_currow(move_like_addval ? addval : 1);
    } break;
    case SDLK_LEFT:
      if (MOD_ANY(mod))
        break;
      if (highlighted_subsection > 0)
        highlighted_subsection--;
      else
      {
        dec_curtrack();
        highlighted_subsection = FXPARAM_LO;
      }
    break;
    case SDLK_RIGHT:
    {
      if (MOD_ANY(mod))
        break;
      if (highlighted_subsection < FXPARAM_LO)
        highlighted_subsection++;
      else
      {
        inc_curtrack();
        highlighted_subsection = NOTE;
      }
    } break;
    case SDLK_SPACE:
      if (!MOD_ANY(mod))
      {
        recording = !recording;
        mousecursors->set_cursor(CURSOR_MPAINT_WHITE_HAND - recording);
      }
    break;
    case SDLK_TAB:
    {
      if (MODONLY(mod, KMOD_SHIFT))
        dec_curtrack();
      else
        inc_curtrack();
    } break;
    case SDLK_F1:
      cur_octave = 0;
    break;
    case SDLK_F2:
      cur_octave = 1;
    break;
    case SDLK_F3:
    cur_octave = 2;
    break;
    case SDLK_F4:
    cur_octave = 3;
    break;
    case SDLK_F5:
    cur_octave = 4;
    break;
    case SDLK_F6:
    cur_octave = 5;
    break;
    default:break;
  }
}

void PatternEditorPanel::piano_kb(const int scancode, const int mod)
{
	if (MOD_ANY(mod))
		return;

  switch (scancode)
  {
    case SDLK_z:
      {
        notehelper(0);
      } break;
    case SDLK_s:
      {
        notehelper(1);
      } break;
    case SDLK_x:
      {
        notehelper(2);
      } break;
    case SDLK_d: /*marked */
      {
        notehelper(3);
      } break;
    case SDLK_c: /*marked */
      {
        notehelper(4);
      } break;
    case SDLK_v:
      {
        notehelper(5);
      } break;
    case SDLK_g:
      {
        notehelper(6);
      } break;
    case SDLK_b: /*marked */
      {
        notehelper(7);
      } break;
    case SDLK_h:
      {
        notehelper(8);
      } break;
    case SDLK_n:
      {
        notehelper(9);
      } break;
    case SDLK_j:
      {
        notehelper(10);
      } break;
    case SDLK_m:
      {
        notehelper(11);
      } break;
    case SDLK_COMMA:
      {
        notehelper(12);
      } break;
    case SDLK_l:
      notehelper(13);
      break;
    case SDLK_PERIOD:
      notehelper(14);
      break;
    case SDLK_SEMICOLON:
      notehelper(15);
      break;
    case SDLK_SLASH:
      notehelper(16);
      break;
    case SDLK_q:
      {
        notehelper(12);
      } break;
    case SDLK_2: /*marked */
      {
        notehelper(13);
      } break;
    case SDLK_w:
      {
        notehelper(14);
      } break;
    case SDLK_3: /*marked */
      {
        notehelper(15);
      } break;
    case SDLK_e: /*marked */
      {
        notehelper(16);
      } break;
    case SDLK_r:
      {
        notehelper(17);
      } break;
    case SDLK_5: /*marked */
      {
        notehelper(18);
      } break;
    case SDLK_t:
      {
        notehelper(19);
      } break;
    case SDLK_6: /*marked */
      {
        notehelper(20);
      } break;
    case SDLK_y:
      {
        notehelper(21);
      } break;
    case SDLK_7: /*marked */
      {
        notehelper(22);
      } break;
    case SDLK_u:
      {
        notehelper(23);
      } break;
    case SDLK_i:
      {
        notehelper(24);
      } break;
    case SDLK_9: /*marked */
      {
        notehelper(25);
      } break;
    case SDLK_o:
      {
        notehelper(26);
      } break;
    case SDLK_0: /*marked */
      {
        notehelper(27);
      } break;
    case SDLK_p:
      {
        notehelper(28);
      } break;
    case SDLK_LEFTBRACKET:
      {
        notehelper(29);
      } break;
    case SDLK_EQUALS:
      {
        notehelper(30);
      } break;
    case SDLK_RIGHTBRACKET:
      {
        notehelper(31);
      } break;

    default:break;
  }
}

void PatternEditorPanel::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
{
  auto backup = VISIBLE_ROWS;
  set_visible_rows(MAX_VISIBLE_ROWS);
  // Draw a black inside rect to cover the BG Rect
  SDL_FillRect(screen, &rect, Colors::transparent);
  Utility::DrawRect(&rect, 1);

  set_visible_rows(backup);
  // draw a white border around the current visible rows, but also the max visible as well
  Utility::DrawRect(&rect, 1);
}

void PatternEditorPanel::draw(SDL_Surface *screen/*=::render->screen*/)
{
  Pattern *pat = get_current_pattern(psp);
  one_time_draw();
  /* First, draw the "Instruments" strings and top buttons */
  //title.draw(screen);

  SDL_Rect r = {rect.x + 1, rect.y + 1, rect.w - 1, rect.h - 1};
  SDL_FillRect(screen, &r, Colors::transparent);

  if (currow >= rows_scrolled && currow < (rows_scrolled + VISIBLE_ROWS))
  {
    highlight_r = row_rects[currow - rows_scrolled];
    highlight_r.y -= 1;

    switch (highlighted_subsection) {
      case NOTE:
        subhighlight_r = guitrackrow[cur_track].note_ctext[currow - rows_scrolled].rect;
      break;
      case INSTR_HI:
      subhighlight_r = guitrackrow[cur_track].instr_ctext[currow - rows_scrolled].rect;
      subhighlight_r.w -= CHAR_WIDTH + 1;
      break;
      case INSTR_LO:
        subhighlight_r = guitrackrow[cur_track].instr_ctext[currow - rows_scrolled].rect;
        subhighlight_r.x += CHAR_WIDTH;
        subhighlight_r.w -= CHAR_WIDTH + 1;
      break;
      case VOL_HI:
      subhighlight_r = guitrackrow[cur_track].vol_ctext[currow - rows_scrolled].rect;
      subhighlight_r.w -= CHAR_WIDTH + 1;
      break;
      case VOL_LO:
        subhighlight_r = guitrackrow[cur_track].vol_ctext[currow - rows_scrolled].rect;
        subhighlight_r.x += CHAR_WIDTH;
        subhighlight_r.w -= CHAR_WIDTH + 1;
      break;
      case FX:
        subhighlight_r = guitrackrow[cur_track].fx_ctext[currow - rows_scrolled].rect;
      break;
      case FXPARAM_HI:
      subhighlight_r = guitrackrow[cur_track].fxparam_ctext[currow - rows_scrolled].rect;
      subhighlight_r.w -= CHAR_WIDTH + 1;
      break;
      case FXPARAM_LO:
        subhighlight_r = guitrackrow[cur_track].fxparam_ctext[currow - rows_scrolled].rect;
        subhighlight_r.x += CHAR_WIDTH;
        subhighlight_r.w -= CHAR_WIDTH + 1;
      break;
      default:break;
    }

    subhighlight_r.y -= 1;

    SDL_FillRect(screen, &highlight_r,
      recording ? Colors::Interface::color[Colors::Interface::Type::recording]
                : Colors::Interface::color[Colors::Interface::Type::selections]);
    SDL_FillRect(screen, &subhighlight_r,
      recording ? flasher.color()
                : Colors::Interface::color[Colors::Interface::Type::subselections]);
  }

  for (int t=0; t < MAX_TRACKS; t++)
  {
    GUITrackRow *gtr = &guitrackrow[t];
		Uint32 color = Voice_Control::is_muted(t) ? Colors::nearblack : Colors::voice[t];

    trackheader[t].ctext.draw(color, false);
    Utility::DrawRect(&trackheader[t].outline.rect, 1);

    for (int r=0; r < min(VISIBLE_ROWS, (pat->len - rows_scrolled)); r++)
    {
      index_text[r].str = index_strings[rows_scrolled + r];
      if ( (rows_scrolled + r) % 8 == 0)
        index_text[r].draw(screen,
          Colors::yellow,
          false);
      else
        index_text[r].draw(screen,
          Colors::Interface::color[Colors::Interface::Type::text_fg],
          false);

      PatternRow *patrow = &pat->trackrows[t][rows_scrolled + r];
      char *string;

      //--------------------------------------------------
      string = gtr->note_strings[r];
      note2ascii(patrow->note, string);
      //--------------------------------------------------
      string = gtr->instr_strings[r];
      instr2ascii(patrow->instr, string);
      //--------------------------------------------------
      string = gtr->vol_strings[r];
      vol2ascii(patrow->vol, string);
      //--------------------------------------------------
      string = gtr->fx_strings[r];
      fx2ascii(patrow->fx, patrow->fxparam, string);
      //--------------------------------------------------
      string = gtr->fxparam_strings[r];
      fxparam2ascii(patrow->fx, patrow->fxparam, string);
      //--------------------------------------------------

      gtr->note_ctext[r].draw(
        Colors::Interface::color[Colors::Interface::Type::note], false);
      gtr->instr_ctext[r].draw(
        Colors::Interface::color[Colors::Interface::Type::instr], false);
      gtr->vol_ctext[r].draw(
        Colors::Interface::color[Colors::Interface::Type::vol], false);
      gtr->fx_ctext[r].draw(
        Colors::Interface::color[Colors::Interface::Type::fx], false);
      gtr->fxparam_ctext[r].draw(
        Colors::Interface::color[Colors::Interface::Type::fxparam], false);
    }
  }
}
