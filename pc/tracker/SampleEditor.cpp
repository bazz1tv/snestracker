#include "SampleEditor.h"
#include "Tracker.h"
#include "apuram.h"

extern Tracker *tracker;

/* NOTE: Live sample loop point adjustment works as intended, but if the sample
used a filter that uses sample history, the live adjustment can cause it to
get dirty and create glitchy sounds until the next KON */
struct DirEntry{
  uint16_t sample;
  uint16_t loop;
} __attribute__((packed));

static int clickedLoopCheckbox(void *s)
{
  DEBUGLOG("clickedLoopCheckbox: ");
  Sample_Panel *sp = &::tracker->main_window.samplepanel;
  Sample *sample = (Sample *) &::tracker->song.samples[sp->currow];

  *sample->metadata.changed = true;

  Brr *endblock = getBrrEndBlock(sample->brr);
  endblock->loop = !endblock->loop;

  if (tracker->rendering())
  {
    DirEntry *dirent = (DirEntry *) &::IAPURAM[::tracker->apuram->dspdir_i << 8];

    dirent += sp->currow;
    Brr *brr = (Brr *) &::IAPURAM[dirent->sample];
    Brr *endblock = getBrrEndBlock(brr);
    endblock->loop = !endblock->loop;
    //DEBUGLOG("dspdir_i: 0x%04x dirent->sample: %04X, dirent->loop: %04X\n",
      //::tracker->apuram->dspdir_i << 8, dirent->sample, dirent->loop);
  }
  return 0;
}

SampleEditor::SampleEditor() :
	loop_cbuf("00"),
	loop_title("Loop..."),
	loop_valtext(loop_cbuf),
	loop_incbtn("+", incloop, this, true),
	loop_decbtn("-", decloop, this, true),
  loopCheckbox(NULL, clickedLoopCheckbox, NULL),


  finetune_cbuf("+000"),
  finetune_title("F.tune"),
  finetune_valtext(finetune_cbuf),
  finetune_incbtn("+", incfinetune, this, true),
  finetune_decbtn("-", decfinetune, this, true)
{
  /* Disable not-yet-supported features */
  finetune_incbtn.enabled = false;
  finetune_decbtn.enabled = false;
}

void SampleEditor :: update_loop()
{
  Sample_Panel *sp = &::tracker->main_window.samplepanel;
  Sample *cursamp = &::tracker->song.samples[ sp->currow ];
	sprintf(loop_cbuf, "$%04x", cursamp->rel_loop);

  // update what the checkbox refers to
  loopCheckbox.state = &cursamp->metadata.loop;
}

/* Same code as Instrument editor fine tune (detune). Maybe we can
 * abstract the function out to reduce code size?? TODO*/
void SampleEditor :: update_finetune()
{
  int8_t ft = ::tracker->song.samples[ ::tracker->main_window.samplepanel.currow ].finetune;
  char sign = '+';
  if (ft < 0)
    sprintf(finetune_cbuf, "%04d", ft);
  else
    sprintf(finetune_cbuf, "%c%03d", sign, ft);
}

void SampleEditor :: set_coords(int x, int y)
{
  /* (x,y)
   * Vol......7f +-
   * Pan......80 +-
   * F.Tune +000 +-*/

  int yy = y;

  this->x = x;
  this->y = y;

  y += CHAR_HEIGHT * 2;

	loop_title.rect.x = x;
	loop_title.rect.y = y;
	loop_valtext.rect.x = x + ((sizeof("Loop...")-1) * CHAR_WIDTH);
	loop_valtext.rect.y = y;
	loop_decbtn.rect.x  = loop_valtext.rect.x + (sizeof("$0000") * CHAR_WIDTH);
	loop_decbtn.rect.y = y;
	loop_incbtn.rect.x = loop_decbtn.rect.x + CHAR_WIDTH + 5;
	loop_incbtn.rect.y = y;

  loopCheckbox.rect.x = loop_incbtn.rect.x + (CHAR_WIDTH * 2);
  loopCheckbox.rect.y = y;

  y += CHAR_HEIGHT + 5;

  finetune_title.rect.x = x;
  finetune_title.rect.y = y;
  finetune_valtext.rect.x = x + ((sizeof("F.Tune ")) * CHAR_WIDTH);
  finetune_valtext.rect.y = y;
  finetune_decbtn.rect.x  = finetune_valtext.rect.x + (sizeof("+000") * CHAR_WIDTH);
  finetune_decbtn.rect.y = y;
  finetune_incbtn.rect.x = finetune_decbtn.rect.x + CHAR_WIDTH + 5;
  finetune_incbtn.rect.y = y;
}

int SampleEditor::handle_event(const SDL_Event &ev)
{
	loop_incbtn.check_event(ev);
	loop_decbtn.check_event(ev);
  loopCheckbox.check_event(ev);

  finetune_incbtn.check_event(ev);
  finetune_decbtn.check_event(ev);
  return 0;
}

void SampleEditor::draw(SDL_Surface *screen/*=::render->screen*/)
{
	update_loop();
  update_finetune();

  Uint32 present_color;
  {
    char tmpbuf[128];
    Sample *s = &::tracker->song.samples[::tracker->main_window.samplepanel.currow];
    if (s->brr)
    {
      sprintf(tmpbuf, "BRR Size: $%04x  ", s->brrsize);
      sdlfont_drawString(screen, x, y, tmpbuf);
      present_color = Colors::white;
      loop_incbtn.enabled = true;
      loop_decbtn.enabled = true;
      loopCheckbox.enabled = true;
    }
    else
    {
      sdlfont_drawString(screen, x, y, "No Sample Loaded!");
      present_color = Colors::nearblack;
      loop_incbtn.enabled = false;
      loop_decbtn.enabled = false;
      loopCheckbox.enabled = false;
    }
  }

	loop_title.draw(screen, present_color);
	loop_valtext.draw(screen, present_color);
	loop_incbtn.draw(screen);
	loop_decbtn.draw(screen);
  loopCheckbox.draw(screen);

  finetune_title.draw(screen, Colors::nearblack);
  finetune_valtext.draw(screen, Colors::nearblack);
  finetune_incbtn.draw(screen);
  finetune_decbtn.draw(screen);
}

static void loopHook(Sample_Panel *sp)
{
  Sample *cursamp = &::tracker->song.samples[ sp->currow ];
  if (tracker->rendering())
  {
    DirEntry *dirent = (DirEntry *) &::IAPURAM[::tracker->apuram->dspdir_i << 8];

    dirent += sp->currow;
    dirent->loop = dirent->sample + cursamp->rel_loop;

    DEBUGLOG("dspdir_i: 0x%04x dirent->sample: %04X, dirent->loop: %04X\n",
      ::tracker->apuram->dspdir_i << 8, dirent->sample, dirent->loop);
  }
}

int SampleEditor::incloop(void *i)
{
	SampleEditor *ie = (SampleEditor *)i;
	Sample *s = &::tracker->song.samples[::tracker->main_window.samplepanel.currow];
	s->inc_loop();

	ie->update_loop();
  loopHook(&::tracker->main_window.samplepanel);
	return 0;
}

int SampleEditor::decloop(void *i)
{
	SampleEditor *ie = (SampleEditor *)i;
	Sample *s = &::tracker->song.samples[::tracker->main_window.samplepanel.currow];
	s->dec_loop();

	ie->update_loop();
  loopHook(&::tracker->main_window.samplepanel);
	return 0;
}



int SampleEditor::incfinetune(void *i)
{
  SampleEditor *ie = (SampleEditor *)i;
  Sample *s = &::tracker->song.samples[::tracker->main_window.samplepanel.currow];
  s->inc_finetune();
  ie->update_finetune();
  return 0;
}

int SampleEditor::decfinetune(void *i)
{
  SampleEditor *ie = (SampleEditor *)i;
	Sample *s = &::tracker->song.samples[::tracker->main_window.samplepanel.currow];
	s->dec_finetune();
  ie->update_finetune();
  return 0;
}