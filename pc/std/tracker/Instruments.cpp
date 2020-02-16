#include "tracker/Instruments.h"
#include "shared/Colors.h"
#include "shared/sdl_userevents.h"
#include <assert.h>
#include "utility.h"
#include "shared/sdl_dblclick.h"

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


static char nibble_to_ascii(uint8_t nibble)
{
  if ( ((nibble) & 0x0f) < 0x0a )
    return '0' + nibble;
  return 'a' + (nibble - 0x0a);
}

Instrument_Panel::Instrument_Panel(Instrument *iptr) :
    title("Instruments"),
    loadbtn("Load", Instrument_Panel::load, this),
    savebtn("Save", Instrument_Panel::save, this),
    zapbtn("Zap", Instrument_Panel::zap, this),
    instruments(iptr)
{
  // 5 is for eg. "01 |\0"
  instr_index_strings = (char *) malloc( (sizeof(char) * 4) * NUM_INSTR );
  if (instr_index_strings == NULL)
  {
    fprintf(stderr, "Could not allocate string index memory. "
            "This is not normal so we're quitting out");
    exit(1);
  }
}

Instrument_Panel::~Instrument_Panel()
{
  free(instr_index_strings);
}

void Instrument_Panel::set_coords(int x, int y)
{
  int xx = x, yy = y;
  title.rect.x = x;
  title.rect.y = y;

  y += CHAR_WIDTH + (CHAR_WIDTH / 2);

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
  char *c = instr_index_strings;
  for (int i=0; i < NUM_INSTR; i++)
  {
    instr_indices[i].str = c;
    // convert index to ascii
    *(c++) = nibble_to_ascii(i >> 4);
    *(c++) = nibble_to_ascii(i);
    *(c++) = '|';
    *(c++) = 0;

    instr_indices[i].rect = { i < NUM_ROWS ? xx : 0,
      i < NUM_ROWS ? y + (CHAR_HEIGHT * i)    : 0,
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
          if (Utility::coord_is_in_rect(ev.button.x,ev.button.y, &r)
              || Utility::coord_is_in_rect(ev.button.x, ev.button.y,
                                           &instr_names[i].rect))
          {
            if (currow != i)
            {
              SDL_FillRect(::render->screen, &highlight_r, Colors::transparent);
              currow = i;
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
              event2.user.data1 = 1;
              event2.user.data2 = 0;
              SDL_PushEvent(&event2);

              if (Text_Edit_Rect::cur_editing_ter)
                Text_Edit_Rect::stop_editing(Text_Edit_Rect::cur_editing_ter);
            }
            return 1;
          }
        } break;
      default:break;
    }

    if (handle_text_edit_rect_event(ev, &instr_names[i]) == 1)
      return 1;
  }

  loadbtn.check_event(ev);
  savebtn.check_event(ev);
  zapbtn.check_event(ev);
}

void Instrument_Panel::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
{
  Utility::DrawRect(&rect, 1);
}
void Instrument_Panel::draw(SDL_Surface *screen/*=::render->screen*/)
{
  unsigned int i=0;
  char *c = instr_index_strings;
  /* First, draw the "Instruments" strings and top buttons */
  title.draw(screen);
  loadbtn.draw(screen);
  savebtn.draw(screen);
  zapbtn.draw(screen);

  /* This should really be put in init and event code, to decrease
   * redundant processing */
  highlight_r = instr_indices[currow].rect;
  highlight_r.y -= 1;
  highlight_r.w +=
    (instr_names[currow].rect.x - (highlight_r.x + highlight_r.w)) +
    instr_names[currow].rect.w;

  /* This color should not be hardcoded, but have a new entry in the GUI
   * Colors array for highlighted stuff. Reference MilkyTracker config */
  SDL_FillRect(screen, &highlight_r, Colors::magenta);

  for (i=0; i < NUM_ROWS; i++)
  {
    // Draw the index followed by the instrname
    // But beneath this, draw the highlighted currow rect
    /* In order to draw this rect, we need to the total horizontal width
     * of the index + instr_name, + some padding maybe. That should be
     * calculated at init and in the event handler */
    instr_indices[i].draw(screen,
      Colors::Interface::color[Colors::Interface::Type::text_fg],
      false);
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
