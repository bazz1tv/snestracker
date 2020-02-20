#include "Pattern.h"
#include "PanelCommon.h"
#include "sdl_userevents.h"
#include "shared/Colors.h"
#include <assert.h>

const int PatSeqPanel::VISIBLE_ROWS;
//#define SAMPLE_NAME_GUI_CHAR_WIDTH 22

PatSeqPanel::PatSeqPanel(PatternSequencer *patseq) :
  clonebtn("Clone", PatSeqPanel::clone, this),
  seqbtn("Seq", PatSeqPanel::seq, this),
  clearbtn("Clr", PatSeqPanel::clear, this),
  patseq(patseq)
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

  //x += title.rect.w + (2*CHAR_WIDTH);
  clonebtn.rect.x = x;
  clonebtn.rect.y = y;

  x += clonebtn.rect.w + (2*CHAR_WIDTH);
  seqbtn.rect.x = x;
  seqbtn.rect.y = y;

  x += seqbtn.rect.w + (2*CHAR_WIDTH);
  clearbtn.rect.x = x;
  clearbtn.rect.y = y;

  y += CHAR_HEIGHT + (CHAR_HEIGHT/2);
  y += 2;
  rect.y = y;

  y += 2; //(CHAR_HEIGHT / 2);

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

  rect.w = (5 * CHAR_WIDTH) + 2;
  rect.h = (CHAR_HEIGHT * (VISIBLE_ROWS)) + 1;
}

int PatSeqPanel::event_handler(const SDL_Event &ev)
{
  mousewheel_rows_event_handler(ev, &rows_scrolled, VISIBLE_ROWS, patseq->num_entries, &rect);

  /* If the user clicks within a certain row rect. A row rect is comprised
   * of the index region (including padding), the spacer, and the
   * instrument name field (including padding).*/
  for (int i=0; i < min(patseq->num_entries, VISIBLE_ROWS); i++)
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
                    SDL_FillRect(::render->screen, &highlight_r, Colors::transparent);
                    currow = rows_scrolled + i;
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
  clonebtn.check_event(ev);
  seqbtn.check_event(ev);
  clearbtn.check_event(ev);
}

void PatSeqPanel::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
{
  Utility::DrawRect(&rect, 1);
}

void PatSeqPanel::draw(SDL_Surface *screen/*=::render->screen*/)
{
  one_time_draw();
  /* First, draw the "Instruments" strings and top buttons */
  //title.draw(screen);
  clonebtn.draw(screen);
  seqbtn.draw(screen);
  clearbtn.draw(screen);

  SDL_Rect r = {rect.x + 1, rect.y + 1, rect.w - 1, rect.h - 1};
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

  for (int i=0; i < min(patseq->num_entries, VISIBLE_ROWS); i++)
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

int PatSeqPanel::clone(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;

  fprintf(stderr, "PatSeqPanel::clone()\n");
  return 0;
}

int PatSeqPanel::seq(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  fprintf(stderr, "PatSeqPanel::seq()SAVE\n");
  return 0;
}

int PatSeqPanel::clear(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  fprintf(stderr, "PatSeqPanel::clear()\n");
  return 0;
}
