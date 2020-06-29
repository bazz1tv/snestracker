#include "platform.h"
#include "kbd.h"
#include "tracker/Instruments.h"
#include "shared/Colors.h"
#include "shared/sdl_userevents.h"
#include <assert.h>
#include "utility.h"
#include "shared/sdl_dblclick.h"
#include "globals.h" // for mouse::
#include "PanelCommon.h"
#include "Samples.h"

Instrument::Instrument()
{
  reset();
}

void Instrument::reset()
{
  name[0] = 0;
  srcn = 0;
  adsr.adsr = DEFAULT_ADSR;
  vol = DEFAULT_VOL;
  pan = DEFAULT_PAN;
  semitone_offset = 0;
  finetune = 0;
  // leave metadata untouched
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
  if (i->pan != 0xff)
  {
    i->pan++;
    *i->metadata.changed = true;
  }
}

void Instrument::dec_pan(Instrument *i)
{
  if (i->pan != 0x00)
  {
    *i->metadata.changed = true;
    i->pan--;
  }
}

void Instrument::inc_finetune(Instrument *i)
{
  if (i->finetune < 127)
  {
    *i->metadata.changed = true;
    i->finetune++;
  }
}

void Instrument::dec_finetune(Instrument *i)
{
  if (i->finetune > -128)
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
          this->finetune == rhs.finetune);
}

#define INST_COREINFO_SIZE sizeof(Instrument::vol) + sizeof(Instrument::pan) + sizeof(Instrument::adsr)
#define INST_SONGMETA_SIZE sizeof(InstrumentChunkLoader::idx) + sizeof(Instrument::srcn)
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
        size_t minimum_chunksize = INST_SONGMETA_SIZE;
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
        read(file, &instruments[idx].srcn, 1, 1, &maxread);
        DEBUGLOG("\t\tidx: %d\n", idx);

        subchunksize -= INST_SONGMETA_SIZE;
        idx_loaded = true;
      }
      break;
      case SubChunkID::coreinfo:
      {
        DEBUGLOG("\tSubChunkID::coreinfo\n");
        size_t minimum_chunksize = INST_COREINFO_SIZE;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. cannot use\n", subchunkid);
          break;
        }

        struct Instrument *instr = &instruments[idx];

        read(file, &instr->vol, 1, 1, &maxread);
        read(file, &instr->pan, 1, 1, &maxread);
        read(file, &instr->adsr.adsr1, 1, 1, &maxread);
        read(file, &instr->adsr.adsr2, 1, 1, &maxread);

        // Ensure Hardware ADSR is activated since that's the only thing supported
        instr->adsr.adsr_active = 1;

        subchunksize -= INST_COREINFO_SIZE;
      }
      break;
      case SubChunkID::name:
      {
        DEBUGLOG("\tSubChunkID::name\n");
        assert(idx_loaded);
        size_t bytesread = ChunkLoader::read_str_from_file2(file, instruments[idx].name, subchunksize, INSTR_NAME_MAXLEN);
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
    word = INST_SONGMETA_SIZE; // we know the length in advance
    write(file, &word, 2, 1, &chunklen);
    write(file, &i, 1, 1, &chunklen); // write Instrument index (only 1 byt, &chunklene)
    write(file, &instr->srcn, 1, 1, &chunklen);
  }

  DEBUGLOG("\t\tSubChunkID::coreinfo\n");
  byte = SubChunkID::coreinfo;
  write(file, &byte, 1, 1, &chunklen);
  word = INST_COREINFO_SIZE; // we know the length in advance
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
}

size_t InstrumentChunkLoader::save(SDL_RWops *file, struct Instrument *instr)
{
  struct Instrument *backup = instruments;
  instruments = instr;
  save(file, 0);
  instruments = backup;
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


/* The instrument panel is something like
 * Instruments  (Load) (Save) (Zap)
 * ------+-------------------------+
 *   00  |        some name        |
 *   01  |      some other name    |
 *   02  |       awesome instr     |
 *   ..  |       .. . .  . .       |
 * ------+-------------------------+
 */

const int Instrument_Panel::NUM_ROWS;
#define INSTR_NAME_GUI_CHAR_WIDTH 22

Instrument_Panel::Instrument_Panel(Instrument *iptr, Sample_Panel *sp) :
    title("Instruments"),
    loadbtn("Load", Instrument_Panel::load, this),
    savebtn("Save", Instrument_Panel::save, this),
    zapbtn("Zap", Instrument_Panel::zap, this),
    instruments(iptr),
    samplepanel(sp)
{
}

Instrument_Panel::~Instrument_Panel()
{
}

void Instrument_Panel::set_coords(int x, int y)
{
  int xx = x, yy = y;
  title.rect.x = x;
  title.rect.y = y;

  y += CHAR_HEIGHT + (CHAR_HEIGHT / 2);

  rect.x = x - 2; /* All rect.* are creating the bounding rect to be drawn
  around the instruments list (not the buttons or main label. Unfortunately the
  assignments are spread throughout this function rather than all being in one place,
  due to the nature of the coordinates being manipulated to create the locations
  of all ipanel entities */

  //x += title.rect.w + (2*CHAR_WIDTH);
  x += (CHAR_WIDTH * 0) + 2;
  loadbtn.rect.x = x;
  loadbtn.rect.y = y;

  x += loadbtn.rect.w + (2*CHAR_WIDTH);
  savebtn.rect.x = x;
  savebtn.rect.y = y;

  x += savebtn.rect.w + (2*CHAR_WIDTH);
  zapbtn.rect.x = x;
  zapbtn.rect.y = y;

  y += CHAR_HEIGHT + (CHAR_HEIGHT/2);
  y += 2;
  rect.y = y;

  y += 2; //(CHAR_HEIGHT / 2);

  /* This init was postponed until now to avoid having to iterate through
   * all instruments multiple times */
  for (int i=0; i < NUM_ROWS; i++)
  {
    instr_indices[i].str = instr_index_strings[i];
    conv_idx2ascii(rows_scrolled + i + 1, instr_index_strings[i]);

    instr_indices[i].rect =
    {
      xx,
      y + (CHAR_HEIGHT * i),
      3 * CHAR_WIDTH,
      CHAR_HEIGHT};


    /* The GUI instr_names[i].str now points to the core (tracker)'s
     * instruments[i].str, meaning that when we load a file into the
     * tracker (core), these strings should automatically update after a
     * redraw */
    instr_names[i].str = instruments[i].name;
    instr_names[i].strsize = INSTR_NAME_MAXLEN;
    instr_names[i].rect = instr_indices[i].rect;
    instr_names[i].rect.x += 3 * CHAR_WIDTH;
    instr_names[i].rect.w = INSTR_NAME_GUI_CHAR_WIDTH * CHAR_WIDTH;
    instr_names[i].max_visible_chars = INSTR_NAME_GUI_CHAR_WIDTH;
    instr_names[i].border = false;
  }

  rect.w = ((INSTR_NAME_GUI_CHAR_WIDTH + 3) * CHAR_WIDTH) + 2;
  rect.h = (CHAR_HEIGHT * (NUM_ROWS)) + 1;

}

int Instrument_Panel::event_handler(const SDL_Event &ev)
{
  mousewheel_rows_event_handler(ev, &rows_scrolled, NUM_ROWS, NUM_INSTR, &rect);

  /* If the user clicks within a certain row rect. A row rect is comprised
   * of the index region (including padding), the spacer, and the
   * instrument name field (including padding).*/
  for (int i=0; i < NUM_ROWS; i++)
  {
    /* Check the rect of the indice. Unfortunately, unless I edit the TER
     * code to allow using an alternative Rect (Clickable_Rect code
     * really), then I can't composite an all-inclusive rect to do the
     * check once */
    SDL_Rect r = instr_indices[i].rect;
    /* Need to be careful this this rect does not overlap with the TER */
    r.w += instr_names[i].rect.x - (r.x + r.w) - 1;
    //assert(r.x + r.w < instr_names[i].rect.x);

    switch (ev.type)
    {
      case SDL_USEREVENT:
        if (ev.user.code == UserEvents::mouse_react)
        {
          SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click
          if (Utility::coord_is_in_rect(te->button.x,te->button.y, &r))
          {
            instr_names[i].do_thing(instr_names[i].data);
            return 1;
          }
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        {
          switch (ev.button.button)
          {
            case SDL_BUTTON_LEFT:
            {
              if (Utility::coord_is_in_rect(ev.button.x,ev.button.y, &r)
                  || Utility::coord_is_in_rect(ev.button.x, ev.button.y,
                                               &instr_names[i].rect))
              {
                if ((currow - rows_scrolled) != i)
                {
                  SDL_FillRect(::render->screen, &highlight_r, Colors::transparent);
                  set_currow(rows_scrolled + i);
                  // Need to reset the double click code if this click was
                  // registered
                  //dblclick::reset_dblclicktimer();
                  /* Dirty way to implement the following functionality: if
                   * you click on an unselected entity, that counts as one
                   * click towards a double click, but if the second click is
                   * on a different entity, the logic must think only 1 click
                   * happened. The dirtyness comes from how the dblclick
                   * engine works. It's kind of an enigma atm. */
                  dblclick::numclicks = 0;
                  SDL_Event event2;

                  event2.type = SDL_USEREVENT;
                  event2.user.code = UserEvents::mouse_reset;
                  event2.user.data1 = (void *) 1;
                  event2.user.data2 = (void *) 0;
                  SDL_PushEvent(&event2);

                  if (Text_Edit_Rect::cur_editing_ter)
                    Text_Edit_Rect::stop_editing(Text_Edit_Rect::cur_editing_ter);
                }
                return 1;
              }
            } break;
            default:break;
          }
        } break;
      default:break;
    }

    if (handle_text_edit_rect_event(ev, &instr_names[i]) == 2)
    {
      *instruments[0].metadata.changed = true;
      return 1;
    }
  }

  switch (ev.type)
  {
    case SDL_KEYDOWN:
      {
        int scancode = ev.key.keysym.sym;
        int mod = ev.key.keysym.mod;

        switch(scancode)
        {
          case SDLK_UP:
            if (MODONLY(mod, KMOD_SHIFT))
            {
              dec_currow();
            }
            break;
          case SDLK_DOWN:
            if (MODONLY(mod, KMOD_SHIFT))
            {
              inc_currow();
            }
            break;
          default:break;
        }
      } break;
  }

  loadbtn.check_event(ev);
  savebtn.check_event(ev);
  zapbtn.check_event(ev);
}

void Instrument_Panel::set_currow(int c)
{
	currow = c;
	samplepanel->currow = instruments[c].srcn;
	samplepanel->rows_scrolled = (samplepanel->currow / Sample_Panel::NUM_ROWS) * Sample_Panel::NUM_ROWS;
}

void Instrument_Panel::dec_currow()
{
	if (currow > 0)
	{
		if ((currow - rows_scrolled) % NUM_ROWS == 0)
			rows_scrolled--;
		set_currow(currow - 1);
	}
	else
	{
		set_currow(NUM_INSTR - 1);
		rows_scrolled = currow - (NUM_ROWS-1);
	}

}

void Instrument_Panel::inc_currow()
{
	if (currow >= (NUM_INSTR - 1))
	{
		set_currow(0);
		rows_scrolled = 0;
	}
	else
	{
		if ((currow - rows_scrolled) % NUM_ROWS == (NUM_ROWS - 1))
			rows_scrolled++;
		set_currow(currow + 1);
	}
}

void Instrument_Panel::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
{
  Utility::DrawRect(&rect, 1);
}

void Instrument_Panel::draw(SDL_Surface *screen/*=::render->screen*/)
{
  unsigned int i=0;
  /* First, draw the "Instruments" strings and top buttons */
  title.draw(screen);
  loadbtn.draw(screen);
  savebtn.draw(screen);
  zapbtn.draw(screen);

  SDL_Rect r = {rect.x + 1, rect.y + 1, rect.w - 1, rect.h - 1};
  SDL_FillRect(screen, &r, Colors::transparent);

  /* This should really be put in init and event code, to decrease
   * redundant processing */
  if (currow >= rows_scrolled && currow < (rows_scrolled + NUM_ROWS))
  {
    highlight_r = instr_indices[currow - rows_scrolled].rect;
    highlight_r.y -= 1;
    highlight_r.w +=
    (instr_names[currow - rows_scrolled].rect.x - (highlight_r.x + highlight_r.w)) +
    instr_names[currow - rows_scrolled].rect.w;

    SDL_FillRect(screen, &highlight_r, Colors::Interface::color[Colors::Interface::Type::selections]);
  }

  for (i=0; i < NUM_ROWS; i++)
  {
    // Draw the index followed by the instrname
    // But beneath this, draw the highlighted currow rect
    /* In order to draw this rect, we need to the total horizontal width
     * of the index + instr_name, + some padding maybe. That should be
     * calculated at init and in the event handler */
    conv_idx2ascii(rows_scrolled + i + 1, instr_index_strings[i]);
    instr_indices[i].draw(screen,
      Colors::Interface::color[Colors::Interface::Type::text_fg],
      false);
    instr_names[i].str = instruments[rows_scrolled + i].name;
    instr_names[i].draw(Colors::Interface::color[Colors::Interface::Type::text_fg],
      false);
  }
}

static void AskDeleteSample(Instrument_Panel *ip)
{
  Instrument *instr = &ip->instruments[ip->currow];
  Sample *sample = &ip->samplepanel->samples[instr->srcn];
  char strbuf[100];
  sprintf(strbuf, "Do you wish to delete the sample for this instrument as well?\n\n"
    "sample #%02d, \"%s\"", instr->srcn, sample->name);
  auto rc = DialogBox::SimpleYesNo("Delete sample?", strbuf, false);

  switch (rc)
  {
    case DialogBox::YES:
      sample->clear();
    break;
    case DialogBox::NO:
    default:
      return;
  }
}

#include "SdlNfd.h"
#define OVERWRITE_STR "Do you really want to %s this instrument?\n\n instrument #%02d, \"%s\""
/* In these following functions, we need an core instruments handle, and
 * the index into that table. Can get that through the Instrument_Panel.
 * This is still GUI centric.*/
int Instrument_Panel::load(void *ipanel)
{
  Instrument_Panel *ip = (Instrument_Panel *)ipanel;
  Instrument *instruments = ip->instruments;

  fprintf(stderr, "LOAD\n");

  /* First, check if the current instrument slot is non-default data, and warn
  the user you would be overwriting the old data */
  Instrument *instr = &ip->instruments[ip->currow];
  Sample *samples = ip->samplepanel->samples;
  Sample *sample = &samples[instr->srcn];

  if (*instr != Instrument())
  {
    char strbuf[100];
    sprintf(strbuf, OVERWRITE_STR, "overwrite", ip->currow + 1, instr->name);
    auto drc = DialogBox::SimpleYesNo("Overwrite Instrument?", strbuf);
    switch (drc)
    {
      case DialogBox::YES:
        AskDeleteSample(ip);
      break;
      case DialogBox::NO:
      default:
        return -1;
    }
  }

  if (SdlNfd::get_file_handle("r", INSTRFILE_EXT) == NFD_OKAY)
  {
    InstrumentFileLoader ifl(instr, samples);
    ifl.load(SdlNfd::file);
  }

  /* Update the sample panel highlighted row to cover the srcn connected to this instrument */
  ip->samplepanel->currow = instr->srcn;
  *instr->metadata.changed = true;
  return 0;
}

int Instrument_Panel::save(void *ipanel)
{
  Instrument_Panel *ip = (Instrument_Panel *)ipanel;
  fprintf(stderr, "SAVE\n");

  /* First make sure this instrument actually references a real BRR sample */
  Instrument *instr = &ip->instruments[ip->currow];
  Sample *sample = &ip->samplepanel->samples[instr->srcn];

  if (sample->brr == NULL/* || sample->brrsize == 0*/)
  {
    DialogBox::SimpleOK("No Sample data",
      "This instrument references a blank sample.\n\n"
      "Please load and connect a valid sample to this instrument first.");
    return -1;
  }

  if (SdlNfd::get_file_handle("w", INSTRFILE_EXT) == NFD_OKAY)
  {
    DEBUGLOG("\tinstrument path:%s\n", SdlNfd::outPath);
    InstrumentFileLoader ifl(instr, sample);
    ifl.save(SdlNfd::file);
  }
  return 0;
}

int Instrument_Panel::zap(void *ipanel)
{
  Instrument_Panel *ip = (Instrument_Panel *)ipanel;
  fprintf(stderr, "ZAP\n");

  Instrument *instr = &ip->instruments[ip->currow];
  Sample *samples = ip->samplepanel->samples;
  Sample *sample = &samples[instr->srcn];

  if (*instr != Instrument())
  {
    char strbuf[100];
    sprintf(strbuf, OVERWRITE_STR, "zap", ip->currow + 1, instr->name);
    auto drc = DialogBox::SimpleYesNo("Zap Instrument?", strbuf);
    switch (drc)
    {
      case DialogBox::YES:
        AskDeleteSample(ip);
        instr->reset();
        *instr->metadata.changed = true;
      break;
      case DialogBox::NO:
      default:
        return -1;
    }
  }

  return 0;
}
