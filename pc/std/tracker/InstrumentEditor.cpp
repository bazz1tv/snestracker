#include "InstrumentEditor.h"
#include "Instruments.h"
#include "Tracker.h"

AdsrPanel::AdsrPanel(Instrument_Panel *ip) :
  attack_text("Attack"), decay_text("Decay"),
  sustainlevel_text("Sustain Lvl"), sustainrelease_text("Sustain Release"),
  instrpanel(ip)
{
}

void AdsrPanel::one_time_draw(SDL_Surface *screen)
{
  attack_text.draw(screen, Colors::gray);
  decay_text.draw(screen, Colors::gray);
  sustainlevel_text.draw(screen, Colors::gray);
  sustainrelease_text.draw(screen, Colors::gray);
}

void AdsrPanel::set_coords(int x, int y)
{
  int save_y = y;

  // Build graph below this. Just draw the bounding rect
  adsrgraph.bounds.y = y + (CHAR_HEIGHT*3);
  adsrgraph.bounds.x = x;
  adsrgraph.bounds.w = 500;
  adsrgraph.bounds.h = 120;
  adsrgraph.fg_color = Colors::green;
  adsrgraph.bg_color = Colors::nearblack;
  adsrgraph.border_color = Colors::white;

  //y += adsrgraph.bounds.h + (CHAR_HEIGHT*2);

  attack_text.rect.x = x;
  attack_text.rect.y = y;
  adsr_context_menus.attack_context.menu.preload(x, y + CHAR_HEIGHT);
// random sidenote: consider adding to sdlfont.h
// #define CTSTRLEN(str) sizeof(str)-1
  decay_text.rect.x = x + (CHAR_WIDTH*(4+sizeof("Attack")-1));
  decay_text.rect.y = y;
  adsr_context_menus.decay_context.menu.preload(decay_text.rect.x, y + CHAR_HEIGHT);

  sustainlevel_text.rect.x = decay_text.rect.x + (CHAR_WIDTH*(4+sizeof("Decay")-1));
  sustainlevel_text.rect.y = y;
  adsr_context_menus.sustain_level_context.menu.preload(
                        sustainlevel_text.rect.x, y + CHAR_HEIGHT);

  sustainrelease_text.rect.x = sustainlevel_text.rect.x +
                        (CHAR_WIDTH*(2+sizeof("Sustain Level")-1));
  sustainrelease_text.rect.y = y;
  adsr_context_menus.sustain_release_context.menu.preload(
                          sustainrelease_text.rect.x, y + CHAR_HEIGHT);
}

void AdsrPanel::check_event(const SDL_Event &ev)
{
  int r;
  if ((r=adsr_context_menus.receive_event(ev)))
  {
    switch (r)
    {
      case ADSR::Context_Menus::ATTACK_CHANGED:
        {
          Uint8 i = adsr_context_menus.attack_context.menu.currently_selected_item_index;
          /*Uint8 vreg = voice.n * 0x10 + dsp_reg::adsr1;
          Uint8 adsr1 = player->spc_read_dsp(vreg);
          player->spc_write_dsp(vreg, adsr1);*/
          uint8_t *adsr1 = &instrpanel->instruments[instrpanel->currow].adsr.adsr1;
          *adsr1 = *adsr1 & (~ADSR::ATTACK_MASK);
          *adsr1 |= ADSR::reverse_attack_index(i);
        }
        break;
      case ADSR::Context_Menus::DECAY_CHANGED:
        {
          Uint8 i = adsr_context_menus.decay_context.menu.currently_selected_item_index;
          /*Uint8 vreg = voice.n * 0x10 + dsp_reg::adsr1;
          Uint8 adsr1 = player->spc_read_dsp(vreg);
          player->spc_write_dsp(vreg, adsr1);*/
          uint8_t *adsr1 = &instrpanel->instruments[instrpanel->currow].adsr.adsr1;
          *adsr1 = *adsr1 & (~ADSR::DECAY_MASK);
          *adsr1 |= ADSR::reverse_decay_index(i);
        }
        break;
      case ADSR::Context_Menus::SUSTAIN_LEVEL_CHANGED:
        {
          Uint8 i = 7-adsr_context_menus.sustain_level_context.menu.currently_selected_item_index;
          /*
          Uint8 vreg = voice.n * 0x10 + dsp_reg::adsr2;
          Uint8 adsr2 = player->spc_read_dsp(vreg);
          player->spc_write_dsp(vreg, adsr2);*/
          uint8_t *adsr2 = &instrpanel->instruments[instrpanel->currow].adsr.adsr2;
          *adsr2 = *adsr2 & (~ADSR::SUSTAIN_LEVEL_MASK);
          *adsr2 |= ADSR::reverse_sustain_level_index(i);
        }
        break;
      case ADSR::Context_Menus::SUSTAIN_RELEASE_CHANGED:
        {
          Uint8 i = adsr_context_menus.sustain_release_context.menu.currently_selected_item_index;
          /*Uint8 vreg = voice.n * 0x10 + dsp_reg::adsr2;
          Uint8 adsr2 = player->spc_read_dsp(vreg);
          player->spc_write_dsp(vreg, adsr2);*/
          uint8_t *adsr2 = &instrpanel->instruments[instrpanel->currow].adsr.adsr2;
          *adsr2 = *adsr2 & (~ADSR::SUSTAIN_RELEASE_MASK);
          *adsr2 |= ADSR::reverse_sustain_release_index(i);
        }
        break;
      default:break;
    }
    return 0;
  }

  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      {
        if (adsr_context_menus.check_left_click_activate(ev.button.x, ev.button.y))
        {
          return 0;
        }
      }
    break;
    default:break;
  }
}

void AdsrPanel::draw(SDL_Surface *screen)
{
  one_time_draw();
  run();
  adsr_context_menus.draw(::render->screen);
}

void AdsrPanel::draw_renderer()
{
  adsrgraph.draw_bg();
  adsrgraph.draw_lines();
  adsrgraph.draw_border();
}

void AdsrPanel::run()
{
  uint8_t *adsr1 = &instrpanel->instruments[instrpanel->currow].adsr.adsr1;
  uint8_t *adsr2 = &instrpanel->instruments[instrpanel->currow].adsr.adsr2;

  adsr_context_menus.update(*adsr1, *adsr2);
  // Update our lovely new graph system
  // the first point, based on the Attack envelope, decides how many ms
  // before final value 100 is reached (for 100% volume).
  // Now load the current attack float.
  int attack_i = adsr_context_menus.attack_context.menu.currently_selected_item_index;
  int decay_i = adsr_context_menus.decay_context.menu.currently_selected_item_index;
  int sustain_i = adsr_context_menus.sustain_level_context.menu.currently_selected_item_index;
  int release_i = adsr_context_menus.sustain_release_context.menu.currently_selected_item_index;

  float attack_ms = ADSR::attack_map[attack_i].ms;
  float decay_ms = ADSR::decay_map[7-sustain_i][decay_i].ms;
  float sustain_frac = ADSR::sustain_level_map[7-sustain_i].ms;
  float release_ms = ADSR::sustain_release_map[7-sustain_i][release_i].ms;

  adsrgraph.points[0] = {0,0};
  adsrgraph.points[1] = {(int)attack_ms, -100};
  adsrgraph.points[2] = {(int)(attack_ms + decay_ms), -(int)(sustain_frac * 100)};

  adsrgraph.points[3].x = (int)(attack_ms + decay_ms + release_ms);
  adsrgraph.points[3].y = 0;

  for (int i=0; i < 4; i++)
  {
    adsrgraph.points[i].x /= 10;
    adsrgraph.points[i].x += adsrgraph.bounds.x + 10;
    adsrgraph.points[i].y += adsrgraph.bounds.y + adsrgraph.bounds.h - 2;
  }

  if (release_ms == ADSR::INFINITE)
  {
    adsrgraph.points[3].x = (adsrgraph.bounds.x + adsrgraph.bounds.w);
    adsrgraph.points[3].y = adsrgraph.points[2].y;
  }

  if (adsrgraph.points[3].x > (adsrgraph.bounds.x + adsrgraph.bounds.w))
  {
    float x1 = adsrgraph.points[2].x;
    float x2 = adsrgraph.points[3].x;
    adsrgraph.points[3].x = adsrgraph.bounds.x + adsrgraph.bounds.w;
    /*(50,-100)
     * .        |
     *    .     |(60,?)
     *       .  |
     *          .
     *          |  .
     *          |     .
     *          |         . (100,0)
     *
     * Based on the horizontal percentage traveled, the final y coord can
     * be calculated between the start and end y coords.
     * EXAMPLE:
     *  y1 = 100                 y1 = 200
     *  y2 =   0                 y2 = 100
     *  50% (y1 - y2) + y2       50% (y1 - y2) + y2
     *  50% (100 - 0) + 0 = 50   50% (200-100) + 50 = 150
     */
    float y1 = adsrgraph.points[2].y;
    float y2 = adsrgraph.bounds.y + adsrgraph.bounds.h - 1;
    float percent = (adsrgraph.points[3].x - x1) / (x2 - x1);
    adsrgraph.points[3].y = (y1) + (percent * (y2 - y1));
  }
}

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

  tabs(this),
  adsrpanel(instrpanel),
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

  int yy = y;

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

  tabs.set_coords(vol_decbtn.rect.x + vol_decbtn.rect.w + (CHAR_WIDTH*15), yy - 3);
  adsrpanel.set_coords(vol_decbtn.rect.x + vol_decbtn.rect.w + (CHAR_WIDTH*7), yy + (3*CHAR_HEIGHT));
}

int InstrumentEditor::handle_event(const SDL_Event &ev)
{
  vol_incbtn.check_event(ev);
  vol_decbtn.check_event(ev);

  pan_incbtn.check_event(ev);
  pan_decbtn.check_event(ev);

  finetune_incbtn.check_event(ev);
  finetune_decbtn.check_event(ev);

  if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
    tabs.check_mouse_and_execute(ev.button.x, ev.button.y);

  if (tabs.adsr.active)
    adsrpanel.check_event(ev);
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

  tabs.draw();

  // conditionally draw the active tab
  if (tabs.adsr.active)
  {
    adsrpanel.draw();
  }
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

InstrumentEditor::Tabs::Tabs(InstrumentEditor *ie) :
  adsr(3,3, "ADSR", switchto_adsr, ie, true),
  vol(3,3, "Vol", switchto_vol, ie),
  pan(3,3, "Pan", switchto_pan, ie)
{
}

void InstrumentEditor::Tabs::set_coords(int x, int y)
{
  // init Tabs
  adsr.rect.x = x;
  adsr.rect.y = y; // + h + CHAR_HEIGHT*2;
  //
  vol.rect.x = adsr.rect.x + adsr.horiz_pixel_length() + CHAR_WIDTH;
  vol.rect.y = adsr.rect.y;
  //
  pan.rect.x = vol.rect.x + vol.horiz_pixel_length() + CHAR_WIDTH;
  pan.rect.y = adsr.rect.y;
}

bool InstrumentEditor::Tabs::check_mouse_and_execute(int x, int y)
{
  if (adsr.check_mouse_and_execute(x,y))
    return true;
  if (vol.check_mouse_and_execute(x,y))
    return true;
  if (pan.check_mouse_and_execute(x,y))
    return true;
  return false;
}

void InstrumentEditor::Tabs::draw()
{
  adsr.draw();
  vol.draw();
  pan.draw();
}

int InstrumentEditor::Tabs::switchto_adsr(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  ie->tabs.adsr.active = true;
  ie->tabs.vol.active = false;
  ie->tabs.pan.active = false;
  Tracker::prerenders.insert((DrawRenderer *)&ie->adsrpanel);
}

int InstrumentEditor::Tabs::switchto_vol(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  ie->tabs.adsr.active = false;
  ie->tabs.vol.active = true;
  ie->tabs.pan.active = false;
  Tracker::prerenders.erase((DrawRenderer *)&ie->adsrpanel);
}

int InstrumentEditor::Tabs::switchto_pan(void *i)
{
  InstrumentEditor *ie = (InstrumentEditor *)i;
  ie->tabs.adsr.active = false;
  ie->tabs.vol.active = false;
  ie->tabs.pan.active = true;
  Tracker::prerenders.erase((DrawRenderer *)&ie->adsrpanel);
}
