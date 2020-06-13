#include "tracker/Instruments.h"
#include "shared/Colors.h"
#include "shared/sdl_userevents.h"
#include <assert.h>
#include "utility.h"
#include "shared/sdl_dblclick.h"
#include "globals.h" // for mouse::
#include "PanelCommon.h"
#include "Samples.h"

Instrument::Instrument() : used(0), srcn(0), vol(0x50), pan(0x80),
   semitone_offset(0), finetune(0)
{
  name[0] = 0;
  adsr.adsr1 = 0xff;
  adsr.adsr2 = 0xe0;
}

Instrument::~Instrument()
{
}

void Instrument::inc_srcn(Instrument *i)
{
	if (i->srcn != 0xFF)
		i->srcn++;
}

void Instrument::dec_srcn(Instrument *i)
{
		if (i->srcn > 0)
			i->srcn--;
}

void Instrument::inc_vol(Instrument *i)
{
  if (i->vol != 0x7f)
    i->vol++;
}

void Instrument::dec_vol(Instrument *i)
{
  if (i->vol != 0x00)
    i->vol--;
}

void Instrument::inc_pan(Instrument *i)
{
  if (i->pan != 0xff)
    i->pan++;
}

void Instrument::dec_pan(Instrument *i)
{
  if (i->pan != 0x00)
    i->pan--;
}

void Instrument::inc_finetune(Instrument *i)
{
  if (i->finetune < 127)
    i->finetune++;
}

void Instrument::dec_finetune(Instrument *i)
{
  if (i->finetune > -128)
    i->finetune--;
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
  loadbtn.enabled = false;
  savebtn.enabled = false;
  zapbtn.enabled = false;
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

    if (handle_text_edit_rect_event(ev, &instr_names[i]) == 1)
      return 1;
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
            if (mod & KMOD_SHIFT && !(mod & KMOD_CTRL))
            {
              dec_currow();
            }
            break;
          case SDLK_DOWN:
            if (mod & KMOD_SHIFT && !(mod & KMOD_CTRL))
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

/* In these following functions, we need an core instruments handle, and
 * the index into that table. Can get that through the Instrument_Panel.
 * This is still GUI centric.*/
int Instrument_Panel::load(void *ipanel)
{
  Instrument_Panel *ip = (Instrument_Panel *)ipanel;
  Instrument *instruments = ip->instruments;
  int currow = ip->currow;

  fprintf(stderr, "LOAD\n");
  return 0;
}

int Instrument_Panel::save(void *ipanel)
{
  Instrument_Panel *ip = (Instrument_Panel *)ipanel;
  fprintf(stderr, "SAVE\n");
  return 0;
}

int Instrument_Panel::zap(void *ipanel)
{
  Instrument_Panel *ip = (Instrument_Panel *)ipanel;
  fprintf(stderr, "ZAP\n");
  return 0;
}
