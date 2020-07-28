#include "platform.h"
#include "kbd.h"

#include "Samples.h"
#include "Instruments.h"
#include "shared/Colors.h"
#include "shared/sdl_userevents.h"
#include <assert.h>
#include "utility.h"
#include "shared/SdlNfd.h"
#include "shared/sdl_dblclick.h"
#include "globals.h" // for ::mouse
#include "PanelCommon.h"



const int Sample_Panel::NUM_ROWS;
#define SAMPLE_NAME_GUI_CHAR_WIDTH 22

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
  loop = 0;
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
              return;
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
            return;
          }
        }

        struct Sample *s = &samples[idx];
        if (s->brr != NULL)
          free(s->brr);

        s->brr = brr;
        s->brrsize = subchunksize;
        subchunksize -= nb_read_total;

        Brr *lastblock = (Brr *) &(((uint8_t *)(brr))[s->brrsize - sizeof(Brr)]);
        s->loop = lastblock->loop ? true : false;
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


Sample_Panel::Sample_Panel(Sample* samples) :
  title("Samples:"),
  loadbtn("Load", Sample_Panel::load, this),
  savebtn("Save", Sample_Panel::save, this),
  clearbtn("Clear", Sample_Panel::clear, this),
	samples(samples)
{
  /* Disable unimpl'd buttons */
  //clearbtn.enabled = false;
}

Sample_Panel::~Sample_Panel()
{
}


void Sample_Panel::set_coords(int x, int y)
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
  clearbtn.rect.x = x;
  clearbtn.rect.y = y;

  y += CHAR_HEIGHT + (CHAR_HEIGHT/2);
  y += 2;
  rect.y = y;

  y += 2; //(CHAR_HEIGHT / 2);

  /* This init was postponed until now to avoid having to iterate through
   * all samples multiple times */
  for (int i=0; i < NUM_ROWS; i++)
  {
    sample_indices[i].str = sample_index_strings[i];

    conv_idx2ascii(rows_scrolled + i, sample_index_strings[i]);

    sample_indices[i].rect =
    {
      xx,
      y + (CHAR_HEIGHT * i),
      3 * CHAR_WIDTH,
      CHAR_HEIGHT
    };


    /* The GUI sample_names[i].str now points to the core (tracker)'s
     * instruments[selected].samples[i].str, meaning that when we load a file into the
     * tracker (core), these strings should automatically update after a
     * redraw */
    sample_names[i].str = samples[i].name;
    sample_names[i].strsize = SAMPLE_NAME_MAXLEN;
    sample_names[i].rect = sample_indices[i].rect; /* Base this rect off of the index rect */
    sample_names[i].rect.x += 3 * CHAR_WIDTH;
    sample_names[i].rect.w = SAMPLE_NAME_GUI_CHAR_WIDTH * CHAR_WIDTH;
    sample_names[i].max_visible_chars = SAMPLE_NAME_GUI_CHAR_WIDTH;
    sample_names[i].border = false;
  }

  rect.w = ((SAMPLE_NAME_GUI_CHAR_WIDTH + 3) * CHAR_WIDTH) + 2;
  rect.h = (CHAR_HEIGHT * (NUM_ROWS)) + 1;
}

int Sample_Panel::event_handler(const SDL_Event &ev)
{
  mousewheel_rows_event_handler(ev, &rows_scrolled, NUM_ROWS, NUM_SAMPLES, &rect);
  /* If the user clicks within a certain row rect. A row rect is comprised
   * of the index region (including padding), the spacer, and the
   * instrument name field (including padding).*/
  for (int i=0; i < NUM_ROWS; i++)
  {
    /* Check the rect of the indice. Unfortunately, unless I edit the TER
     * code to allow using an alternative Rect (Clickable_Rect code
     * really), then I can't composite an all-inclusive rect to do the
     * check once */
    SDL_Rect r = sample_indices[i].rect;
    /* Need to be careful this this rect does not overlap with the TER */
    r.w += sample_names[i].rect.x - (r.x + r.w) - 1;
    //assert(r.x + r.w < instr_names[i].rect.x);

    switch (ev.type)
    {
      case SDL_USEREVENT:
        if (ev.user.code == UserEvents::mouse_react)
        {
          SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click
          if (Utility::coord_is_in_rect(te->button.x,te->button.y, &r))
          {
            sample_names[i].do_thing(sample_names[i].data);
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
                                               &sample_names[i].rect))
              {
                if ((currow - rows_scrolled) != i)
                {
                  SDL_FillRect(::render->screen, &highlight_r, Colors::transparent);
                  currow = rows_scrolled + i;
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
                return ROW_UPDATED;
              }
            } break;
            default:break;
          }
        } break;
      default:break;
    }

    if (handle_text_edit_rect_event(ev, &sample_names[i]) == 2)
    {
      *samples[0].metadata.changed = true;
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
            if (MODONLY(mod, KMOD_SHIFT | CMD_CTRL_KEY))
            {
              dec_currow();
							return ROW_UPDATED;
            }
            break;
          case SDLK_DOWN:
            if (MODONLY(mod, KMOD_SHIFT | CMD_CTRL_KEY))
            {
              inc_currow();
							return ROW_UPDATED;
            }
            break;
          default:break;
        }
      } break;
  }

  loadbtn.check_event(ev);
  savebtn.check_event(ev);
  clearbtn.check_event(ev);

	return 0;
}

void Sample_Panel::dec_currow()
{
	if (currow > 0)
	{
		if ((currow - rows_scrolled) % NUM_ROWS == 0)
			rows_scrolled--;
		currow--;
	}
	else
	{
		currow = NUM_SAMPLES - 1;
		rows_scrolled = currow - (NUM_ROWS-1);
	}
}

void Sample_Panel::inc_currow()
{
	if (currow >= (NUM_SAMPLES - 1))
	{
		currow = 0;
		rows_scrolled = 0;
	}
	else
	{
		if ((currow - rows_scrolled) % NUM_ROWS == (NUM_ROWS - 1))
			rows_scrolled++;
		currow++;
	}
}

void Sample_Panel::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
{
  Utility::DrawRect(&rect, 1);
}

void Sample_Panel::run()
{
}

void Sample_Panel::draw(SDL_Surface *screen/*=::render->screen*/)
{
  unsigned int i=0;
  /* First, draw the "Instruments" strings and top buttons */
  title.draw(screen);
  loadbtn.draw(screen);
  savebtn.draw(screen);
  clearbtn.draw(screen);
  
  //panel_clear_all_rows(sample_names, NUM_ROWS, ::render->screen);
  SDL_Rect r = {rect.x + 1, rect.y + 1, rect.w - 1, rect.h - 1};
  SDL_FillRect(screen, &r, Colors::transparent);

  /* This should really be put in init and event code, to decrease
   * redundant processing */
  if (currow >= rows_scrolled && currow < (rows_scrolled + NUM_ROWS))
  {
    highlight_r = sample_indices[currow - rows_scrolled].rect;
    highlight_r.y -= 1;
    highlight_r.w +=
      (sample_names[currow - rows_scrolled].rect.x - (highlight_r.x + highlight_r.w)) +
      sample_names[currow - rows_scrolled].rect.w;

    SDL_FillRect(screen, &highlight_r, Colors::Interface::color[Colors::Interface::Type::selections]);
  }

  for (i=0; i < NUM_ROWS; i++)
  {
    // Draw the index followed by the sample name
    // But beneath this, draw the highlighted currow rect
    /* In order to draw this rect, we need to the total horizontal width
     * of the index + instr_name, + some padding maybe. That should be
     * calculated at init and in the event handler */
    conv_idx2ascii(rows_scrolled + i, sample_index_strings[i]);
    sample_indices[i].draw(screen,
      Colors::Interface::color[Colors::Interface::Type::text_fg],
      false);
    sample_names[i].str = samples[rows_scrolled + i].name;
    sample_names[i].draw(Colors::Interface::color[Colors::Interface::Type::text_fg],
      false);
  }
}

#include "gui/DialogBox.h"
#define OVERWRITE_STR "Do you really want to %s this sample?\n\n sample #%02d, \"%s\""
using namespace SdlNfd;
/* In these following functions, we need an core instruments handle, and
 * the index into that table. Can get that through the Sample_Panel.
 * This is still GUI centric.*/
int Sample_Panel::load(void *spanel)
{
  Sample_Panel *sp = (Sample_Panel *)spanel;
  Sample *s = &sp->samples[sp->currow];

  fprintf(stderr, "Sample_Panel::LOAD\n");

  if (s->brr != NULL)
  {
    char strbuf[100];
    sprintf(strbuf, OVERWRITE_STR, "overwrite", sp->currow, s->name);
    auto drc = DialogBox::SimpleYesNo("Overwrite Sample?", strbuf);
    switch (drc)
    {
      case DialogBox::YES:
      break;
      case DialogBox::NO:
      default:
        return -1;
    }
  }

  if (SdlNfd::get_file_handle("r", "brr") == NFD_OKAY)
  {
    Sint64 brrsize = SDL_RWsize(file);

    DEBUGLOG("sample path:%s\n", outPath);
    if (brrsize <= 0)
    {
      DEBUGLOG("Could not detect filesize :( aborting loading sample\n");
      return -1;
    }

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
      free(brr);
      return -1;
    }

    if (s->brr != NULL)
      free(s->brr);

    s->brr = brr;
    s->brrsize = brrsize;

    strncpy(s->name, Utility::getFileName(outPath), SAMPLE_NAME_MAXLEN - 1);
    s->name[SAMPLE_NAME_MAXLEN-1] = 0;
    *s->metadata.changed = true;
  }

  return 0;
}

#include "gui/DialogBox.h"

int Sample_Panel::save(void *spanel)
{
  Sample_Panel *sp = (Sample_Panel *)spanel;
  fprintf(stderr, "Sample_Panel::SAVE\n");

  Sample *s = &sp->samples[sp->currow];

  if (s->brr == NULL/* || sample->brrsize == 0*/)
  {
    DialogBox::SimpleOK("No Sample data", "Please load a sample first");
    return -1;
  }

  if (SdlNfd::get_file_handle("w", "brr") == NFD_OKAY)
  {
    DEBUGLOG("\tsample path:%s\n", outPath);

    Sint64 nb = 0;
    nb = SDL_RWwrite(file, (char *)s->brr, 1, s->brrsize);

    if (nb != s->brrsize)
    {
      DEBUGLOG("\tCould not fully write BRR Data!\n");
      return -1;
    }
  }

  return 0;
}

int Sample_Panel::clear(void *spanel)
{
  Sample_Panel *sp = (Sample_Panel *)spanel;
  fprintf(stderr, "Sample_Panel::CLEAR\n");

  Sample *s = &sp->samples[sp->currow];

  if (s->brr != NULL)
  {
    char strbuf[100];
    sprintf(strbuf, OVERWRITE_STR, "clear", sp->currow, s->name);
    auto drc = DialogBox::SimpleYesNo("Clear Sample?", strbuf);
    switch (drc)
    {
      case DialogBox::YES:
        s->clear();
        *s->metadata.changed = true;
      break;
      case DialogBox::NO:
      default:
        return -1;
    }
  }

  return 0;
}
