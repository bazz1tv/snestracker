#include "tracker/Instruments.h"
#include "shared/Colors.h"

/* The instrument panel is something like
 * Instruments  (Load) (Save) (Zap)
 * ------+-------------------------+
 *   00  |        some name        |
 *   01  |      some other name    |
 *   02  |       awesome instr     |
 *   ..  |       .. . .  . .       |
 * ------+-------------------------+
 */

const unsigned int Instrument_Panel::NUM_ROWS;
#define INSTR_NAME_GUI_CHAR_WIDTH 22


static char nibble_to_ascii(uint8_t nibble)
{
  if ( ((nibble) & 0x0f) < 0x0a )
    return '0' + nibble;
  return 'a' + (nibble - 0x0a);
}

Instrument_Panel::Instrument_Panel(Instrument *iptr) :
    title("Instruments"),
    loadbtn("Load", Instrument_Panel::load),
    savebtn("Save", Instrument_Panel::save),
    zapbtn("Zap", Instrument_Panel::zap),
    instruments(iptr)
{
  // 5 is for eg. "01 |\0"
  instr_index_strings = (char *) malloc( (sizeof(char) * 5) * NUM_INSTR );
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

  x += title.rect.w + (2*CHAR_WIDTH);
  loadbtn.rect.x = x;
  loadbtn.rect.y = y;

  x += loadbtn.rect.w + (2*CHAR_WIDTH);
  savebtn.rect.x = x;
  savebtn.rect.y = y;

  x += savebtn.rect.w + (2*CHAR_WIDTH);
  zapbtn.rect.x = x;
  zapbtn.rect.y = y;

  y += CHAR_HEIGHT;

  /* This init was postponed until now to avoid having to iterate through
   * all instruments multiple times */
  char *c = instr_index_strings;
  for (int i=0; i < NUM_INSTR; i++)
  {
    // convert index to ascii
    *(c++) = nibble_to_ascii(i >> 4);
    *(c++) = nibble_to_ascii(i);
    *(c++) = ' '; // These 2 bytes are a waste :(
    *(c++) = '|';
    *(c++) = 0;

    /* The GUI instr_names[i].str now points to the core (tracker)'s
     * instruments[i].str, meaning that when we load a file into the
     * tracker (core), these strings should automatically update after a
     * redraw */
    instr_names[i].str = instruments[i].name;
    instr_names[i].strsize = INSTR_NAME_MAXLEN;
    instr_names[i].rect = { i < NUM_ROWS ? xx : 0,
      i < NUM_ROWS ? y + (CHAR_HEIGHT * i)    : 0,
      INSTR_NAME_GUI_CHAR_WIDTH * CHAR_WIDTH,
      CHAR_HEIGHT};
    instr_names[i].max_visible_chars = INSTR_NAME_GUI_CHAR_WIDTH;
  }
}

int Instrument_Panel::event_handler(const SDL_Event &ev)
{
  /* If the user clicks within a certain row rect. A row rect is comprised
   * of the index region (including padding), the spacer, and the
   * instrument name field (including padding).*/
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
  highlight_r.w +=
    (instr_names[currow].rect.x - (highlight_r.x + highlight_r.w)) +
    instr_names[currow].rect.w;

  /* This color should not be hardcoded, but have a new entry in the GUI
   * Colors array for highlighted stuff. Reference MilkyTracker config */
  SDL_FillRect(screen, &highlight_r, Colors::magenta);

  for (i=0; i < NUM_INSTR; i++)
  {
    // Draw the index followed by the instrname
    // But beneath this, draw the highlighted currow rect
    /* In order to draw this rect, we need to the total horizontal width
     * of the index + instr_name, + some padding maybe. That should be
     * calculated at init and in the event handler */
    instr_indices[i].draw(screen);
    instr_names[i].draw(screen);
  }
}

int Instrument_Panel::load(void *null)
{
  return 0;
}

int Instrument_Panel::save(void *null)
{
  return 0;
}

int Instrument_Panel::zap(void *null)
{
  return 0;
}
