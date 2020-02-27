#include "Pattern.h"
#include "PanelCommon.h"
#include "sdl_userevents.h"
#include "shared/Colors.h"
#include <assert.h>
#include "DEBUGLOG.h"
#include "shared/font.h"
#include "Instruments.h" // for Instrument_Panel
#include "shared/gui/MouseCursors.h"

static int clone_seq_common(PatSeqPanel *psp);
static Pattern * get_current_pattern(PatSeqPanel *psp);

#define MODONLY(mod, k) ( (mod) & (k) && !( (mod) & ~(k) ) )
#define MOD_ANY(mod) (mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI))

extern MouseCursors *mousecursors;

PatternSequencer::PatternSequencer() : sequence(MAX_PATTERNS)
{
  patterns[0].used = 1;
}

const int PatSeqPanel::VISIBLE_ROWS;

PatSeqPanel::PatSeqPanel(PatternSequencer *patseq) :
  clonebtn("Clone", PatSeqPanel::clone, this),
  seqbtn("Seq", PatSeqPanel::seq, this),
  clearbtn("Clr", PatSeqPanel::clear, this),
  incpatbtn("+", PatSeqPanel::incpat, this),
  decpatbtn("-", PatSeqPanel::decpat, this),

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

  x += clonebtn.rect.w + (CHAR_WIDTH) + (CHAR_WIDTH/2);
  seqbtn.rect.x = x;
  seqbtn.rect.y = y;

  x += seqbtn.rect.w + (CHAR_WIDTH) + (CHAR_WIDTH/2);
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

  // put the inc/dec pat buttons to the right of the rect.
  incpatbtn.rect.x = rect.x + rect.w + (CHAR_WIDTH) + (CHAR_WIDTH/2);
  incpatbtn.rect.y = rect.y + 4;
  decpatbtn.rect.x = incpatbtn.rect.x + incpatbtn.rect.w + 5;
  decpatbtn.rect.y = incpatbtn.rect.y;// + CHAR_HEIGHT;
}

int PatSeqPanel::event_handler(const SDL_Event &ev)
{
  mousewheel_rows_event_handler(ev, &rows_scrolled, VISIBLE_ROWS, patseq->num_entries, &rect);

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
            if (MODONLY(mod, KMOD_CTRL))
            {
              dec_currow();
            }
            break;
          case SDLK_DOWN:
            if (MODONLY(mod, KMOD_CTRL))
            {
              inc_currow();
            }
            break;
          case SDLK_LEFT:
            if (MODONLY(mod, KMOD_CTRL))
              decpat(this);
          break;
          case SDLK_RIGHT:
            if (MODONLY(mod, KMOD_CTRL))
              incpat(this);
          break;
          case SDLK_F9:
            if (MODONLY(mod, KMOD_CTRL))
              clear(this);
          break;
          case SDLK_F10:
            if (MODONLY(mod, KMOD_CTRL))
            {
              //insert_pattern();
              Pattern *p = get_current_pattern(this);

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
            if (MODONLY(mod, KMOD_CTRL))
              clone_seq_common(this);
          break;
          case SDLK_F12:
          if (MODONLY(mod, KMOD_CTRL))
            clone(this);
          break;

          default:break;
        }
      } break;
  }
  clonebtn.check_event(ev);
  seqbtn.check_event(ev);
  clearbtn.check_event(ev);
  incpatbtn.check_event(ev);
  decpatbtn.check_event(ev);
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
  incpatbtn.draw(screen);
  decpatbtn.draw(screen);

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

////////////////////// START PATTERN EDITOR STUFF ///////////////////////

static int get_unused_pattern_index(PatternSequencer *ps)
{
  for (int i=0; i < MAX_PATTERNS; i++)
    if (ps->patterns[i].used == 0)
      return i;
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
  Pattern *up = unused_index == -1 ? NULL : &patseq->patterns[unused_index];

  if (!up || psp->currow >= (MAX_PATTERNS - 1))
    return -1;
  // We can copy all pattern data from patseq->patterns[patseq->sequence[psp->currow]]
  // to patseq->patterns[unused++]
  // Add the entry to the sequencer by inserting it after currow
  patseq->sequence.insert(patseq->sequence.begin()+psp->currow+1,
      unused_index);

  up->used += 1;

  if ((psp->currow - psp->rows_scrolled) % psp->VISIBLE_ROWS == (psp->VISIBLE_ROWS - 1))
    psp->rows_scrolled++;

  patseq->num_entries++;
  psp->currow++;
  return 0;
}

int PatSeqPanel::clone(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::clone()\n");
  if(!clone_seq_common(psp))
    memcpy(&patseq->patterns[patseq->sequence[psp->currow]],
      &patseq->patterns[patseq->sequence[psp->currow - 1]],
      sizeof(Pattern));

  return 0;
}

int PatSeqPanel::seq(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  fprintf(stderr, "PatSeqPanel::seq()\n");
  clone_seq_common(psp);
  return 0;
}

int PatSeqPanel::clear(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::clear()\n");

  if (patseq->num_entries == 1)
    return -1;

  Pattern *tp = &patseq->patterns[patseq->sequence[psp->currow]];
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

  return 0;
}

int PatSeqPanel::incpat(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::incpat()\n");

  int *derp = &patseq->sequence[psp->currow];
  /* Increment the currow pattern */
  patseq->patterns[patseq->sequence[psp->currow]].used--;
  *derp += 1;
  if (*derp >= MAX_PATTERNS)
    *derp = MAX_PATTERNS - 1;

  patseq->patterns[patseq->sequence[psp->currow]].used++;
  return 0;
}

int PatSeqPanel::decpat(void *pspanel)
{
  PatSeqPanel *psp = (PatSeqPanel *)pspanel;
  PatternSequencer *patseq = psp->patseq;
  fprintf(stderr, "PatSeqPanel::decpat()\n");
  int *derp = &patseq->sequence[psp->currow];

  patseq->patterns[patseq->sequence[psp->currow]].used--;
  *derp -= 1;
  if (*derp < 0)
    *derp = 0;

  patseq->patterns[patseq->sequence[psp->currow]].used++;
}

void PatSeqPanel::inc_currow()
{
  if (currow >= (patseq->num_entries - 1))
  {
    currow = 0;
    rows_scrolled = 0;
  }
  else
  {
    if ((currow - rows_scrolled) % VISIBLE_ROWS == (VISIBLE_ROWS - 1))
      rows_scrolled++;

    currow++;
  }
}

void PatSeqPanel::dec_currow()
{
  if (currow > 0)
  {
    if ((currow - rows_scrolled) % VISIBLE_ROWS == 0)
      rows_scrolled--;
    currow--;
  }
  else
  {
    currow = patseq->num_entries - 1;
    rows_scrolled = patseq->num_entries >= VISIBLE_ROWS ? currow - (VISIBLE_ROWS-1) : 0;
  }
}

///////////////////////////////////////////////////////////////////////
//////////////// BEGIN PATTERN EDITOR //////////////////////////////////
//

const int PatternEditorPanel::MAX_VISIBLE_ROWS;
int PatternEditorPanel::VISIBLE_ROWS = PatternEditorPanel::MAX_VISIBLE_ROWS;

PatternEditorPanel::PatternEditorPanel(PatSeqPanel *psp,
  Instrument_Panel *ip) :
    //VISIBLE_ROWS(MAX_VISIBLE_ROWS),
    cur_track(0), cur_octave(4), recording(0), addval(4),
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

static void note2ascii(Note note, char *c)
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
      th->outline = {
        th->ctext.rect.x - (CHAR_WIDTH*6) + (CHAR_WIDTH/2),
        th->ctext.rect.y - 2,
        (CHAR_WIDTH*12) - (CHAR_WIDTH/2),
        th->ctext.rect.h + 2,
      };
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
      Pattern *pat = &patseq->patterns[0];
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

  const SDL_Rect *maxx = &trackheader[MAX_TRACKS - 1].outline;


  rect.w = (maxx->x - rect.x) + maxx->w; // (3 * CHAR_WIDTH) + ((3 + 2 + 2 + 1 + 2 * CHAR_WIDTH) * MAX_TRACKS) + 2;

  for (int i=0; i < VISIBLE_ROWS; i++)
  {
    row_rects[i] = index_text[i].rect;
    row_rects[i].x -= 1;
    row_rects[i].w += rect.w - (3*CHAR_WIDTH) - 1;
  }
}

void PatternEditorPanel::set_visible_rows(int rows)
{
  VISIBLE_ROWS = rows;
  rect.h = (CHAR_HEIGHT * (1 + VISIBLE_ROWS)) + 6;
}

Pattern * get_current_pattern(PatSeqPanel *psp)
{
  return &psp->patseq->patterns[psp->patseq->sequence[psp->currow]];
}

void PatternEditorPanel::notehelper(int ndex)
{
  Pattern *pat = get_current_pattern(psp);
  PatternRow *pw = &pat->trackrows[cur_track][currow];
  int n = NOTE_C0 + ndex + (cur_octave * 12);
  if (n <= NOTE_C6)
  {
    if (recording)
    {
      // get current instrument
      pw->note = (Note)n;
      pw->instr = ip->currow + 1;
      inc_currow(addval);
      //note2ascii(pw->note, guitrackrow[cur_track].note_strings[currow]);
    }
    else
    {
      /* TODO: Play the sample through the API just like in std instrument
       * window */
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
    default:break;
  }
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

static void moveback(Pattern *pattern, int track, int pos)
{
  if (pos == 0)
    return;
  PatternRow *patrow = pattern->trackrows[track];
  for (; pos < pattern->len; pos++)
  {
    patrow[pos - 1] = patrow[pos];
  }
}

static void moveforward(Pattern *pattern, int track, int pos)
{
  if (pos == 0)
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
      get_current_pattern(psp)->trackrows[cur_track][currow] = PatternRow();
    break;
    case SDLK_BACKSPACE:
      if (MODONLY(mod, KMOD_SHIFT))
        for (int t=0; t < MAX_TRACKS; t++)
          moveback(get_current_pattern(psp), t, currow);
      else moveback(get_current_pattern(psp), cur_track, currow);
      dec_currow();
    break;
    case SDLK_INSERT:
      if (MODONLY(mod, KMOD_SHIFT))
        for (int t=0; t < MAX_TRACKS; t++)
          moveforward(get_current_pattern(psp), t, currow);
      else moveforward(get_current_pattern(psp), cur_track, currow);
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
      rows_scrolled = currow - (VISIBLE_ROWS - 1);

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
      if (MODONLY(mod, KMOD_CTRL))
        pattern_wrap = !pattern_wrap;
    break;
    case SDLK_PAGEUP:
      dec_currow(16, false);
    break;
    case SDLK_PAGEDOWN:
      inc_currow(16, false);
    break;
    case SDLK_HOME:
      currow = 0;
      rows_scrolled = 0;
    break;
    case SDLK_END:
      currow = get_current_pattern(psp)->len - 1;
      rows_scrolled = currow - (VISIBLE_ROWS-1);
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

    trackheader[t].ctext.draw(
        Colors::Interface::color[Colors::Interface::Type::text_fg], false);
    Utility::DrawRect(&trackheader[t].outline, 1);

    for (int r=0; r < min(VISIBLE_ROWS, (pat->len - rows_scrolled)); r++)
    {
      index_text[r].str = index_strings[rows_scrolled + r];
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
