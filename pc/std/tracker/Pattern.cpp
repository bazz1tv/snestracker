#include "Pattern.h"
#include "PanelCommon.h"
#include "sdl_userevents.h"
#include "shared/Colors.h"
#include <assert.h>
#include "DEBUGLOG.h"
#include "shared/font.h"

const int PatSeqPanel::VISIBLE_ROWS;

PatternSequencer::PatternSequencer() : sequence(MAX_PATTERNS)
{
  patterns[0].used = true;
}

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

  switch(ev.type)
  {
    case SDL_KEYDOWN:
      {
        int scancode = ev.key.keysym.sym;
        int mod = ev.key.keysym.mod;

        switch(scancode)
        {
          case SDLK_UP:
            if (!(mod & KMOD_SHIFT) && mod & KMOD_CTRL)
            {
              //dec_currow();
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
            break;
          case SDLK_DOWN:
            if (!(mod & KMOD_SHIFT) && mod & KMOD_CTRL)
            {
              //inc_currow();
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

static Pattern * get_unused_pattern(PatternSequencer *ps)
{
  for (int i=0; i < MAX_PATTERNS; i++)
    if (ps->patterns[i].used == 0)
      return &ps->patterns[i];
  return NULL;
}

static int get_unused_pattern_index(PatternSequencer *ps)
{
  for (int i=0; i < MAX_PATTERNS; i++)
    if (ps->patterns[i].used == 0)
      return i;
  return -1;
}

static int clone_seq_common(PatSeqPanel *psp)
{
  PatternSequencer *patseq = psp->patseq;
  /* Copy the "currow" pattern into the next pattern marked by "unused" */
  // Get the currow pattern
  //psp->currow, psp->patseq->{unused, num_entries, sequence, patterns}
  // Make sure whether there are no more free patterns
  int unused_index = get_unused_pattern_index(patseq);
  Pattern *up = &patseq->patterns[unused_index];

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
    memcpy(&patseq->patterns[psp->currow],
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

///////////////////////////////////////////////////////////////////////
//////////////// BEGIN PATTERN EDITOR //////////////////////////////////
//

const int PatternEditorPanel::VISIBLE_ROWS;

PatternEditorPanel::PatternEditorPanel(PatSeqPanel *psp) : psp(psp) {}

inline static void fxparam2ascii(int fx, int fxparam, char *c)
{
  c[2] = 0;
  if (fx == 0 && fxparam == 0)
  {
    c[0] = FONT_CENTERDOT_CHAR; //'.';
    c[1] = FONT_CENTERDOT_CHAR; //'.';
  }
  else
  {
    c[0] = Utility::nibble_to_ascii(fxparam>>4);
    c[1] = Utility::nibble_to_ascii(fxparam);
  }
}

inline static void fx2ascii(int fx, char *c)
{
  c[1] = 0;
  if (fx == 0)
  {
    c[0] = FONT_CENTERDOT_CHAR; //'.';
  }
  else
    c[0] = Utility::nibble_to_ascii(fx);
}

inline static void vol2ascii(int vol, char *c)
{
  if (vol < 0x10)
  {
    *(c++) = FONT_CENTERDOT_CHAR; //'.';
    *(c++) = FONT_CENTERDOT_CHAR; //'.';
    *(c++) = 0;
  }
  else if (vol >= 0x10 && vol <= 0x50)
    conv_idx2ascii2(vol - 0x10, c);
}

inline static void instr2ascii(int instr, char *c)
{
  if (instr == 0)
  {
    *(c++) = FONT_CENTERDOT_CHAR; //'.';
    *(c++) = FONT_CENTERDOT_CHAR; //'.';
    *(c++) = 0;
  }
  else
    conv_idx2ascii2(instr, c);
}

inline static void note2ascii(Note note, char *c)
{
  if (note == NOTE_NONE)
  {
    *(c++) = FONT_CENTERDOT_CHAR; //'.';
    *(c++) = FONT_CENTERDOT_CHAR; //'.';
    *(c++) = FONT_CENTERDOT_CHAR; //'.';
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
        fx2ascii(patrow->fx, string);

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
  rect.h = (CHAR_HEIGHT * (1 + VISIBLE_ROWS)) + 7;


  for (int i=0; i < VISIBLE_ROWS; i++)
  {
    row_rects[i] = index_text[i].rect;
    row_rects[i].x -= 1;
    row_rects[i].w += rect.w - (3*CHAR_WIDTH) - 1;
  }
}

static Pattern * get_current_pattern(PatSeqPanel *psp)
{
  return &psp->patseq->patterns[psp->patseq->sequence[psp->currow]];
}

static void helper(int ndex, PatternEditorPanel *pep)
{
  Pattern *pat = get_current_pattern(pep->psp);
  PatternRow *pw = &pat->trackrows[pep->cur_track][pep->currow];
  int n = NOTE_C0 + ndex + (pep->cur_octave * 12);
  if (n <= NOTE_C6)
  {
    pw->note = n;
    note2ascii(pw->note, pep->guitrackrow[pep->cur_track].note_strings[pep->currow]);
  }
}

/* The event handler must do the following:
 * If an index is clicked,
 *
 * there is a row-wide highlight_rect, and also a track-specific
 * sub-highlight rect that highlights either the note, instr, vol, fx, or
 * fx-param sub-sections. We need to track which sub-section that is highlighted so that when the main row is changed by a click on an index, the currently specified sub-section is highlighted. */

/*int which_row(const SDL_Event &ev, PatternEditorPanel *pep)
{
  // for all rows, create a rect that spans the entire width of that row,
   // to optimize the checks.
  for (int i=0; i < min(VISIBLE_ROWS, (pat->len - rows_scrolled)); i++)
  {
    // start from the leftmost rect
    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &row_rects[i]))
      return i;
    return -1;
  }
}*/

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
                  // update the row highlighter (really we are "un"drawing
                  // the old highlighter. this may be unnecessary drawing
                  if ((currow - rows_scrolled) != i)
                  {
                    SDL_FillRect(::render->screen, &highlight_r, Colors::transparent);
                    SDL_FillRect(::render->screen, &subhighlight_r, Colors::transparent);
                    currow = rows_scrolled + i;
                  }

                  /* At this point, we know what row, but which sub
                   * component was clicked?? */
                  // let's just brute force it I s'pose
                  for (int t=0; t < MAX_TRACKS; t++)
                  {
                    const SDL_Rect *rr;
                    const GUITrackRow *guitr = &guitrackrow[t];
                    bool hit = false;

                    rr = &guitr->note_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      hit = true;
                      cur_track = t;
                      highlighted_subsection = NOTE;
                      return 1;
                    }

                    rr = &guitr->instr_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      hit = true;
                      cur_track = t;
                      highlighted_subsection = INSTR;
                      return 1;
                    }

                    rr = &guitr->vol_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      hit = true;
                      cur_track = t;
                      highlighted_subsection = VOL;
                      return 1;
                    }

                    rr = &guitr->fx_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      hit = true;
                      cur_track = t;
                      highlighted_subsection = FX;
                      return 1;
                    }

                    rr = &guitr->fxparam_ctext[i].rect;
                    if (Utility::coord_is_in_rect(ev.button.x, ev.button.y, rr))
                    {
                      hit = true;
                      cur_track = t;
                      highlighted_subsection = FXPARAM;
                      /*subhighlight_r.x -= 1;
                      subhighlight_r.y -= 1;
                      subhighlight_r.w += 2;
                      subhighlight_r.h += 2;*/
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


        switch(scancode)
        {
          case SDLK_UP:
            if (mod & KMOD_SHIFT || mod & KMOD_CTRL)
              break;
            if (currow > 0)
              currow--;
            else currow = get_current_pattern(psp)->len - 1;
          break;
          case SDLK_DOWN:
          {
            if (mod & KMOD_SHIFT || mod & KMOD_CTRL)
              break;
            if (currow < (get_current_pattern(psp)->len - 1))
              currow++;
            else currow = 0;
          } break;
          case SDLK_LEFT:
          if (mod & KMOD_SHIFT || mod & KMOD_CTRL)
            break;
          if (highlighted_subsection > 0)
            highlighted_subsection--;
          else
          {
            dec_curtrack();
            highlighted_subsection = FXPARAM;
          }
          break;
          case SDLK_RIGHT:
          {
            if (mod & KMOD_SHIFT || mod & KMOD_CTRL)
              break;
            if (highlighted_subsection < FXPARAM)
              highlighted_subsection++;
            else
            {
              inc_curtrack();
              highlighted_subsection = NOTE;
            }
          } break;
          case SDLK_TAB:
          {
            if (mod & KMOD_SHIFT)
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
          case SDLK_z:
          {
            helper(0, this);
          } break;
          case SDLK_s:
          {
            helper(1, this);
          } break;
          case SDLK_x:
          {
            helper(2, this);
          } break;
          case SDLK_d:
          {
            helper(3, this);
          } break;
          case SDLK_c:
          {
            helper(4, this);
          } break;
          case SDLK_v:
          {
            helper(5, this);
          } break;
          case SDLK_g:
          {
            helper(6, this);
          } break;
          case SDLK_b:
          {
            helper(7, this);
          } break;
          case SDLK_h:
          {
            helper(8, this);
          } break;
          case SDLK_n:
          {
            helper(9, this);
          } break;
          case SDLK_j:
          {
            helper(10, this);
          } break;
          case SDLK_m:
          {
            helper(11, this);
          } break;
          case SDLK_COMMA:
          {
            helper(12, this);
          } break;
          case SDLK_q:
          {
            helper(12, this);
          } break;
          case SDLK_2:
          {
            helper(13, this);
          } break;
          case SDLK_w:
          {
            helper(14, this);
          } break;
          case SDLK_3:
          {
            helper(15, this);
          } break;
          case SDLK_e:
          {
            helper(16, this);
          } break;
          case SDLK_r:
          {
            helper(17, this);
          } break;
          case SDLK_5:
          {
            helper(18, this);
          } break;
          case SDLK_t:
          {
            helper(19, this);
          } break;
          case SDLK_6:
          {
            helper(20, this);
          } break;
          case SDLK_y:
          {
            helper(21, this);
          } break;
          case SDLK_7:
          {
            helper(22, this);
          } break;
          case SDLK_u:
          {
            helper(23, this);
          } break;
          case SDLK_i:
          {
            helper(24, this);
          } break;
          case SDLK_9:
          {
            helper(25, this);
          } break;
          case SDLK_o:
          {
            helper(26, this);
          } break;
          case SDLK_0:
          {
            helper(27, this);
          } break;
          case SDLK_p:
          {
            helper(28, this);
          } break;
          case SDLK_LEFTBRACKET:
          {
            helper(29, this);
          } break;
          case SDLK_EQUALS:
          {
            helper(30, this);
          } break;
          case SDLK_RIGHTBRACKET:
          {
            helper(31, this);
          } break;

          default:break;
        }
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
      case INSTR:
        subhighlight_r = guitrackrow[cur_track].instr_ctext[currow - rows_scrolled].rect;
      break;
      case VOL:
        subhighlight_r = guitrackrow[cur_track].vol_ctext[currow - rows_scrolled].rect;
      break;
      case FX:
        subhighlight_r = guitrackrow[cur_track].fx_ctext[currow - rows_scrolled].rect;
      break;
      case FXPARAM:
        subhighlight_r = guitrackrow[cur_track].fxparam_ctext[currow - rows_scrolled].rect;
      break;
      default:break;
    }

    subhighlight_r.y -= 1;

    SDL_FillRect(screen, &highlight_r, Colors::Interface::color[Colors::Interface::Type::selections]);
    SDL_FillRect(screen, &subhighlight_r, Colors::Interface::color[Colors::Interface::Type::subselections]);
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

      PatternRow *patrow = &pat->trackrows[t][r];
      Clickable_Text *ctext;
      char *string;

      //--------------------------------------------------
      ctext  = &gtr->note_ctext[r];
      string = gtr->note_strings[r];
      note2ascii(patrow->note, string);
      //--------------------------------------------------
      ctext  = &gtr->instr_ctext[r];
      string = gtr->instr_strings[r];
      instr2ascii(patrow->instr, string);
      //--------------------------------------------------
      ctext  = &gtr->vol_ctext[r];
      string = gtr->vol_strings[r];
      vol2ascii(patrow->vol, string);
      //--------------------------------------------------
      ctext  = &gtr->fx_ctext[r];
      string = gtr->fx_strings[r];
      fx2ascii(patrow->fx, string);
      //--------------------------------------------------
      ctext  = &gtr->fxparam_ctext[r];
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
