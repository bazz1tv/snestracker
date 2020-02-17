#include "Samples.h"
#include "Instruments.h"
#include "shared/Colors.h"
#include "shared/sdl_userevents.h"
#include <assert.h>
#include "utility.h"
#include "shared/sdl_dblclick.h"
#include "globals.h" // for ::mouse
#include "Panel.h"

const int Sample_Panel::NUM_ROWS;
#define SAMPLE_NAME_GUI_CHAR_WIDTH 22

Sample::Sample()
{
  name[0] = 0;
}

Sample_Panel::Sample_Panel(Instrument_Panel *ip) :
  title("Samples:"),
  loadbtn("Load", Sample_Panel::load, this),
  savebtn("Save", Sample_Panel::save, this),
  clearbtn("Clear", Sample_Panel::clear, this),
 
  instrpanel(ip)  
{
}

Sample_Panel::~Sample_Panel()
{
}


void Sample_Panel::set_coords(int x, int y)
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
    sample_names[i].str = instrpanel->instruments[instrpanel->currow].samples[i].name;
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
          if (Utility::coord_is_in_rect(ev.button.x,ev.button.y, &r)
              || Utility::coord_is_in_rect(ev.button.x, ev.button.y,
                                           &sample_names[i].rect))
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

    if (handle_text_edit_rect_event(ev, &sample_names[i]) == 1)
      return 1;
  }

  loadbtn.check_event(ev);
  savebtn.check_event(ev);
  clearbtn.check_event(ev);
}

void Sample_Panel::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
{
  Utility::DrawRect(&rect, 1);
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
  highlight_r = sample_indices[currow].rect;
  highlight_r.y -= 1;
  highlight_r.w +=
    (sample_names[currow].rect.x - (highlight_r.x + highlight_r.w)) +
    sample_names[currow].rect.w;

  /* This color should not be hardcoded, but have a new entry in the GUI
   * Colors array for highlighted stuff. Reference MilkyTracker config */
  SDL_FillRect(screen, &highlight_r, Colors::Interface::color[Colors::Interface::Type::selections]);

  for (i=0; i < NUM_ROWS; i++)
  {
    // Draw the index followed by the instrname
    // But beneath this, draw the highlighted currow rect
    /* In order to draw this rect, we need to the total horizontal width
     * of the index + instr_name, + some padding maybe. That should be
     * calculated at init and in the event handler */
    conv_idx2ascii(rows_scrolled + i, sample_index_strings[i]);
    sample_indices[i].draw(screen,
      Colors::Interface::color[Colors::Interface::Type::text_fg],
      false);
    sample_names[i].str =
      instrpanel->instruments[instrpanel->rows_scrolled
      + instrpanel->currow].samples[rows_scrolled + i].name;
    sample_names[i].draw(Colors::Interface::color[Colors::Interface::Type::text_fg],
      false);
  }
}

/* In these following functions, we need an core instruments handle, and
 * the index into that table. Can get that through the Sample_Panel.
 * This is still GUI centric.*/
int Sample_Panel::load(void *spanel)
{
  Sample_Panel *sp = (Sample_Panel *)spanel;
  Sample *samples = sp->instrpanel->instruments[sp->instrpanel->currow].samples;
  int currow = sp->currow;

  fprintf(stderr, "Sample_Panel::LOAD\n");
  return 0;
}

int Sample_Panel::save(void *spanel)
{
  Sample_Panel *sp = (Sample_Panel *)spanel;
  fprintf(stderr, "Sample_Panel::SAVE\n");
  return 0;
}

int Sample_Panel::clear(void *spanel)
{
  Sample_Panel *sp = (Sample_Panel *)spanel;
  fprintf(stderr, "Sample_Panel::CLEAR\n");
  return 0;
}
