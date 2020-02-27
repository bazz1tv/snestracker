#include "InstrumentEditor.h"
#include "Instruments.h"

InstrumentEditor::InstrumentEditor(Instrument_Panel *instrpanel) :
  vol_cbuf("7f"),
  vol_title("Vol"),
  vol_valtext(vol_cbuf),
  vol_incbtn("+", incvol, this, true),
  vol_decbtn("-", decvol, this, true),

  pan_cbuf("80"),
  pan_title("Pan"),
  pan_valtext(pan_cbuf),
  pan_incbtn("+", incpan, this, true),
  pan_decbtn("-", decpan, this, true),

  finetune_cbuf("+000"),
  finetune_title("F.tune"),
  finetune_valtext(finetune_cbuf),
  finetune_incbtn("+", incfinetune, this, true),
  finetune_decbtn("-", decfinetune, this, true),

  instrpanel(instrpanel)
{
}

void InstrumentEditor :: update_vol()
{
  sprintf(vol_cbuf, "%02x", instrpanel->instruments[instrpanel->currow].vol);
}

void InstrumentEditor :: update_pan()
{
  sprintf(pan_cbuf, "%02x", instrpanel->instruments[instrpanel->currow].pan);
}

void InstrumentEditor :: update_finetune()
{
  int8_t ft = instrpanel->instruments[instrpanel->currow].finetune;
  char sign = '+';
  if (ft < 0)
    sprintf(finetune_cbuf, "%04d", ft);
  else
    sprintf(finetune_cbuf, "%c%03d", sign, ft);
}

void InstrumentEditor :: set_coords(int x, int y)
{

  /* (x,y)
   * Vol......7f +-
   * Pan......80 +-
   * F.Tune +000 +-*/
  vol_title.rect.x = x;
  vol_title.rect.y = y;
  vol_valtext.rect.x = x + ((sizeof("Vol......")-1) * CHAR_WIDTH);
  vol_valtext.rect.y = y;
  vol_incbtn.rect.x  = vol_valtext.rect.x + (sizeof("7f") * CHAR_WIDTH);
  vol_incbtn.rect.y = y;
  vol_decbtn.rect.x = vol_incbtn.rect.x + CHAR_WIDTH + 5;
  vol_decbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;

  pan_title.rect.x = x;
  pan_title.rect.y = y;
  pan_valtext.rect.x = x + ((sizeof("Pan......")-1) * CHAR_WIDTH);
  pan_valtext.rect.y = y;
  pan_incbtn.rect.x  = pan_valtext.rect.x + (sizeof("80") * CHAR_WIDTH);
  pan_incbtn.rect.y = y;
  pan_decbtn.rect.x = pan_incbtn.rect.x + CHAR_WIDTH + 5;
  pan_decbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;

  finetune_title.rect.x = x;
  finetune_title.rect.y = y;
  finetune_valtext.rect.x = x + ((sizeof("F.Tune")) * CHAR_WIDTH);
  finetune_valtext.rect.y = y;
  finetune_incbtn.rect.x  = finetune_valtext.rect.x + (sizeof("+000") * CHAR_WIDTH);
  finetune_incbtn.rect.y = y;
  finetune_decbtn.rect.x = finetune_incbtn.rect.x + CHAR_WIDTH + 5;
  finetune_decbtn.rect.y = y;
}

int InstrumentEditor::handle_event(const SDL_Event &ev)
{
  vol_incbtn.check_event(ev);
  vol_decbtn.check_event(ev);

  pan_incbtn.check_event(ev);
  pan_decbtn.check_event(ev);

  finetune_incbtn.check_event(ev);
  finetune_decbtn.check_event(ev);
}

//void InstrumentEditor::one_time_draw(SDL_Surface *screen/*=::render->screen*/)
//{
//}

void InstrumentEditor::draw(SDL_Surface *screen/*=::render->screen*/)
{
  update_vol();
  update_pan();
  update_finetune();

  vol_title.draw(screen);
  vol_valtext.draw(screen);
  vol_incbtn.draw(screen);
  vol_decbtn.draw(screen);

  pan_title.draw(screen);
  pan_valtext.draw(screen);
  pan_incbtn.draw(screen);
  pan_decbtn.draw(screen);

  finetune_title.draw(screen);
  finetune_valtext.draw(screen);
  finetune_incbtn.draw(screen);
  finetune_decbtn.draw(screen);
}

int InstrumentEditor::incvol(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  Instrument *curinst = &ie->instrpanel->instruments[ie->instrpanel->currow];
  Instrument::inc_vol(curinst);
  ie->update_vol();
}

int InstrumentEditor::decvol(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  Instrument *curinst = &ie->instrpanel->instruments[ie->instrpanel->currow];
  Instrument::dec_vol(curinst);
  ie->update_vol();
}

int InstrumentEditor::incpan(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  Instrument *curinst = &ie->instrpanel->instruments[ie->instrpanel->currow];
  Instrument::inc_pan(curinst);
  ie->update_pan();
}

int InstrumentEditor::decpan(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  Instrument *curinst = &ie->instrpanel->instruments[ie->instrpanel->currow];
  Instrument::dec_pan(curinst);
  ie->update_pan();
}

int InstrumentEditor::incfinetune(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  Instrument *curinst = &ie->instrpanel->instruments[ie->instrpanel->currow];
  Instrument::inc_finetune(curinst);
  ie->update_finetune();
}

int InstrumentEditor::decfinetune(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  Instrument *curinst = &ie->instrpanel->instruments[ie->instrpanel->currow];
  Instrument::dec_finetune(curinst);
  ie->update_finetune();
}
