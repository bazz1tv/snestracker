#include "Sample_Panel.h"
#include "platform.h"
#include "kbd.h"
#include "Instruments.h"
#include "shared/Colors.h"
#include "shared/sdl_userevents.h"
#include <assert.h>
#include "utility.h"
#include "shared/SdlNfd.h"
#include "shared/sdl_dblclick.h"
#include "globals.h" // for ::mouse
#include "PanelCommon.h"
#include "Tracker.h"

const int Sample_Panel::NUM_ROWS;

// Little helper
#define min(x, y) ((x) <= (y) ? (x) : (y))

Sample_Panel::Sample_Panel(Sample* samples) :
  title("Samples"),
  loadbtn("Load", Sample_Panel::load, this),
  savebtn("Save", Sample_Panel::save, this),
  clearbtn("Zap", Sample_Panel::clear, this),
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

    sample_names[i].strsize = min(SAMPLE_NAME_MAXLEN, SAMPLE_NAME_GUI_CHAR_WIDTH);
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
  // Need to draw this title with the same background color as the rect_bg
  title.draw(screen, Colors::Interface::color[Colors::Interface::Type::text_fg],
      true, false, false, Colors::Interface::color[Colors::Interface::Type::samplepanelBG]);
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

    uint16_t loop = 0;
    if (brrsize % 9 != 0)
    {
      /* This might be an AddmusicM header / c700 file */
      DEBUGLOG("This might be c700\n");
      /* This might be a c700 BRR file */
      if (brrsize % 9 == 2)
      {
        DEBUGLOG("This might be c700\n");
        //Brr *c700 = &buf[2];
        Brr end;

        auto startLocation = SDL_RWtell(file);

        SDL_RWseek(file, brrsize - 9, RW_SEEK_SET);
        SDL_RWread(file, &end, 1, 1);
        SDL_RWseek(file, startLocation, RW_SEEK_SET);
        if (end.end)
        {
          SDL_RWread(file, &loop, 2, 1);
          brrsize -= 2;
        }
        else
        {
          // No end block, invalid file
          DEBUGLOG("\t No end block.\n");
          return -1;
        }
      }
      else
      {
        DEBUGLOG("invalid file\n");
        return -1; // invalid file
      }
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



    s->clear();
    s->brr = brr;
    s->brrsize = brrsize;
    s->rel_loop = loop;

    strncpy(s->name, Utility::getFileName(outPath), SAMPLE_NAME_MAXLEN - 1);
    s->name[SAMPLE_NAME_MAXLEN-1] = 0;
    *s->metadata.changed = true;
    s->metadata.loop = doesBrrLoop(s->brr);
    if (::tracker->song.instruments[::tracker->main_window.instrpanel.currow].srcn == sp->currow)
      ::tracker->renderCurrentInstrument();
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
        tracker->instr_render = false;
      break;
      case DialogBox::NO:
      default:
        return -1;
    }
  }

  return 0;
}
