#include "Instrument_Panel.h"
#include "Tracker.h"
#include "kbd.h"
#include "shared/Colors.h"
#include "shared/sdl_userevents.h"
#include "utility.h"
#include "shared/sdl_dblclick.h"
#include "PanelCommon.h"
#include "gui/DialogBox.h"
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

// Little helper
#define min(x, y) ((x) <= (y) ? (x) : (y))

Instrument_Panel::Instrument_Panel(Instrument *iptr, Sample_Panel *sp) :
    title("Instruments"),
    loadbtn("Load", Instrument_Panel::load, this),
    savebtn("Save", Instrument_Panel::save, this),
    dupbtn("Dup", Instrument_Panel::dup, this),
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
  dupbtn.rect.x = x;
  dupbtn.rect.y = y;

  x += dupbtn.rect.w + (2*CHAR_WIDTH);
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
    instr_names[i].strsize = min(INSTR_NAME_MAXLEN, INSTR_NAME_GUI_CHAR_WIDTH);
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
  dupbtn.check_event(ev);
  zapbtn.check_event(ev);
  return 0;
}

void Instrument_Panel::set_currow(int c)
{
  currow = c;
  samplepanel->currow = instruments[c].srcn;
  samplepanel->rows_scrolled = (samplepanel->currow / Sample_Panel::NUM_ROWS) * Sample_Panel::NUM_ROWS;

  // If the instrument's BRR sample is valid and the tracker is not playing the song
  if (samplepanel->samples[instruments[currow].srcn].brr != NULL && !::tracker->playback)
    ::tracker->renderCurrentInstrument();
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
  // Need to draw this title with the same background color as the rect_bg
  title.draw(screen, Colors::Interface::color[Colors::Interface::Type::text_fg],
             true, false, false, Colors::Interface::color[Colors::Interface::Type::instrpanelBG]);
  loadbtn.draw(screen);
  savebtn.draw(screen);
  dupbtn.draw(screen);
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

static bool AskDeleteSample(Instrument_Panel *ip)
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
      return true;
    break;
    case DialogBox::NO:
    default:
      return false;
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
  bool deletesample = false;

  if (*instr != Instrument())
  {
    char strbuf[100];
    sprintf(strbuf, OVERWRITE_STR, "overwrite", ip->currow + 1, instr->name);
    auto drc = DialogBox::SimpleYesNo("Overwrite Instrument?", strbuf);
    switch (drc)
    {
      case DialogBox::YES:
        if (*sample != Sample())
          deletesample = AskDeleteSample(ip);
      break;
      case DialogBox::NO:
      default:
        return -1;
    }
  }

  if (SdlNfd::get_file_handle("r", INSTRFILE_EXT) == NFD_OKAY)
  {
    if (deletesample)
      sample->clear();
    InstrumentFileLoader ifl(instr, samples);
    ifl.load(SdlNfd::file);

    /* Update the sample panel highlighted row to cover the srcn connected to this instrument */
    ip->samplepanel->currow = instr->srcn;
    *instr->metadata.changed = true;

    sample = &samples[instr->srcn];
    sample->metadata.loop = doesBrrLoop(sample->brr);

    ::tracker->renderCurrentInstrument();
  }
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

// duplicate instrument
int Instrument_Panel::dup(void *ipanel)
{
  Instrument_Panel *ip = (Instrument_Panel *)ipanel;
  fprintf(stderr, "Instrument_Panel::DUP\n");

  Instrument *instr = &ip->instruments[ip->currow];

  if (*instr == Instrument())
    return 1;
  
  for (int i = ip->currow + 1; i < NUM_INSTR; i++)
  {
    Instrument *instrCandidate = &ip->instruments[i];
    if (*instrCandidate == Instrument())
    {
      *instrCandidate = *instr;
      break;
    }
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
        if (AskDeleteSample(ip))
        {
          sample->clear();
          ::tracker->instr_render = false;
        }
        else if (samples[ip->instruments[ip->currow].srcn].brr != NULL && !::tracker->playback)
          ::tracker->renderCurrentInstrument();

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
