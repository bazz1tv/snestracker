#include "SongSettings.h"
#include "Tracker.h"

/* OF course I am aware of the repetitive nature of this code impl. But
 * let's get this thing off the ground first and maybe some day we can
 * consider how it could be refactored into something more elegant */

extern Tracker *tracker;

SongSettingsPanel::SongSettingsPanel() :
	mvol_cbuf("00"),
	mvol_title("Main Vol"),
	mvol_valtext(mvol_cbuf),
	mvol_incbtn("+", inc_mvol, this, true),
	mvol_decbtn("-", dec_mvol, this, true),

  evol_cbuf("00"),
  evol_title("Echo Vol"),
  evol_valtext(evol_cbuf),
  evol_incbtn("+", inc_evol, this, true),
  evol_decbtn("-", dec_evol, this, true),

  edl_cbuf("00"),
  edl_title("Echo Delay"),
  edl_valtext(edl_cbuf),
  edl_incbtn("+", inc_edl, this, true),
  edl_decbtn("-", dec_edl, this, true),
  
  efb_cbuf("00"),
  efb_title("Echo Feedback"),
  efb_valtext(efb_cbuf),
  efb_incbtn("+", inc_efb, this, true),
  efb_decbtn("-", dec_efb, this, true)
{
}

void SongSettingsPanel::update_mvol()
{
	sprintf(mvol_cbuf, "$%02x", ::tracker->songsettings.mvol);
}

void SongSettingsPanel::update_evol()
{
  sprintf(evol_cbuf, "$%02x", ::tracker->songsettings.evol);
}

void SongSettingsPanel::update_edl()
{
  sprintf(edl_cbuf, "$%02x", ::tracker->songsettings.edl);
}

void SongSettingsPanel::update_efb()
{
  sprintf(efb_cbuf, "$%02x", ::tracker->songsettings.efb);
}

/* TODO: FIR */

void SongSettingsPanel :: set_coords(int x, int y)
{
  int yy = y;
  /* NOTE: I am using "Main Vol" string for all vars because it represents
   * the max length, the textual content is not important here; it was
   * easier to just copy/paste that one */

	mvol_title.rect.x = x;
	mvol_title.rect.y = y;
	mvol_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
	mvol_valtext.rect.y = y;
	mvol_incbtn.rect.x  = mvol_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
	mvol_incbtn.rect.y = y;
	mvol_decbtn.rect.x = mvol_incbtn.rect.x + CHAR_WIDTH + 5;
	mvol_decbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;

  evol_title.rect.x = x;
  evol_title.rect.y = y;
  evol_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
  evol_valtext.rect.y = y;
  evol_incbtn.rect.x  = evol_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
  evol_incbtn.rect.y = y;
  evol_decbtn.rect.x = evol_incbtn.rect.x + CHAR_WIDTH + 5;
  evol_decbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;
  
  edl_title.rect.x = x;
  edl_title.rect.y = y;
  edl_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
  edl_valtext.rect.y = y;
  edl_incbtn.rect.x  = edl_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
  edl_incbtn.rect.y = y;
  edl_decbtn.rect.x = edl_incbtn.rect.x + CHAR_WIDTH + 5;
  edl_decbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;
  
  efb_title.rect.x = x;
  efb_title.rect.y = y;
  efb_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
  efb_valtext.rect.y = y;
  efb_incbtn.rect.x  = efb_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
  efb_incbtn.rect.y = y;
  efb_decbtn.rect.x = efb_incbtn.rect.x + CHAR_WIDTH + 5;
  efb_decbtn.rect.y = y;

  //y += CHAR_HEIGHT + 5;
}

int SongSettingsPanel::handle_event(const SDL_Event &ev)
{
	mvol_incbtn.check_event(ev);
	mvol_decbtn.check_event(ev);

  evol_incbtn.check_event(ev);
  evol_decbtn.check_event(ev);

  edl_incbtn.check_event(ev);
  edl_decbtn.check_event(ev);

  efb_incbtn.check_event(ev);
  efb_decbtn.check_event(ev);
}

void SongSettingsPanel::draw(SDL_Surface *screen/*=::render->screen*/)
{
	update_mvol();
  update_evol();
  update_edl();
  update_efb();

	mvol_title.draw(screen);
	mvol_valtext.draw(screen);
	mvol_incbtn.draw(screen);
	mvol_decbtn.draw(screen);

  evol_title.draw(screen);
  evol_valtext.draw(screen);
  evol_incbtn.draw(screen);
  evol_decbtn.draw(screen);

  edl_title.draw(screen);
  edl_valtext.draw(screen);
  edl_incbtn.draw(screen);
  edl_decbtn.draw(screen);
  
  efb_title.draw(screen);
  efb_valtext.draw(screen);
  efb_incbtn.draw(screen);
  efb_decbtn.draw(screen);
}

int SongSettingsPanel::inc_mvol(void *i)
{
	SongSettingsPanel *ie = (SongSettingsPanel *)i;
	SongSettings::inc_vol(&::tracker->songsettings.mvol);
	ie->update_mvol();
}

int SongSettingsPanel::dec_mvol(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  SongSettings::dec_vol(&::tracker->songsettings.mvol);
  ie->update_mvol();
}

int SongSettingsPanel::inc_evol(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  SongSettings::inc_vol(&::tracker->songsettings.evol);
  ie->update_evol();
}

int SongSettingsPanel::dec_evol(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  SongSettings::dec_vol(&::tracker->songsettings.evol);
  ie->update_evol();
}

int SongSettingsPanel::inc_edl(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->songsettings.inc_edl();
  ie->update_edl();
}

int SongSettingsPanel::dec_edl(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->songsettings.dec_edl();
  ie->update_edl();
}

int SongSettingsPanel::inc_efb(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->songsettings.inc_efb();
  ie->update_efb();
}

int SongSettingsPanel::dec_efb(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->songsettings.dec_efb();
  ie->update_efb();
}
