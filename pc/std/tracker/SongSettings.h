#pragma once

#include "shared/gui/Text.h"
#include "shared/gui/Button.h"

/* This is a graphical panel to set song-wide volume settings, and who
 * knows what might get thrown in here along the way */

/* I have no implemented sliders yet, so let's keep going with buttons
 *
 * Let's, for now -- assume we want equal volume settings for the MAIN
 * volume faders, so just have one fader.
 *
 * I'm not going to support the phase inversion right now, but it would be
 * as as simple as adding a checkbox to invert phase.
 *
 * Main Vol      $40 +/-  (0 - 7F)
 *
 * Echo Vol      $10 +/-  (0 - 7F)
 * Echo Delay    $0a +/-  (range 0 - F)
 * Echo Feedback $40 +/-  (0 - ff)
 *
 * FIR Filter  [Expanding List of presets]
 *   C0: $7f
 *   C2: $00
 *   C3: $00
 *   C4: $00
 *   C5: $00
 *   C6: $00
 *   C7: $00
 *---------------------------------------------------
 * The ESA should be abstracted away into the render_to_apu() Tracker
 * function.
 *
 * Add new instrument setting that is echo enable*/

struct SongSettings
{
  uint8_t mvol = 0x40, evol = 0x06;
  uint8_t esa = 0x88, edl = 0x05, efb = 0x40;
  uint8_t fir[8] = { 0x7f, 0, 0, 0, 0, 0, 0, 0 };

  /* One function for both evol and mvol */
  static void inc_vol(uint8_t *vol)
  {
    *vol += (*vol < 0x7F) ? 1 : 0;
  }
  static void dec_vol(uint8_t *vol)
  {
    *vol -= (*vol > 0) ? 1 : 0;
  }

  void inc_edl()
  {
    edl += (edl < 0x0F) ? 1 : 0;
  }
  void dec_edl()
  {
    edl -= (edl > 0) ? 1 : 0;
  }
 
  // Allow wrapping for EFB
  void inc_efb()
  {
    efb++;
  }
  void dec_efb()
  {
    efb--;
  }
};

struct SongSettingsPanel
{
  SongSettingsPanel();

  void set_coords(int x, int y);
  int handle_event(const SDL_Event &ev);
  void draw(SDL_Surface *screen=::render->screen);

	// Main Volume (MVOL)
  void update_mvol();
	char mvol_cbuf[sizeof("$10")];
	Text mvol_title, mvol_valtext;
	Button mvol_incbtn, mvol_decbtn;
	static int inc_mvol(void *i);
	static int dec_mvol(void *i);

  // Echo Volume (EVOL)
  void update_evol();
  char evol_cbuf[sizeof("$10")];
  Text evol_title, evol_valtext;
  Button evol_incbtn, evol_decbtn;
  static int inc_evol(void *i);
  static int dec_evol(void *i);

  // Echo Delay (EDL)
  void update_edl();
  char edl_cbuf[sizeof("$10")];
  Text edl_title, edl_valtext;
  Button edl_incbtn, edl_decbtn;
  static int inc_edl(void *i);
  static int dec_edl(void *i);
  /* EDL notes
   * $800 bytes increment from 1 - $f, with 0 setting using only 4 bytes.
   * so be sure it points somewhere free! It will overwrite those 4 bytes
   * where ESA points.
   * max space occupied: $7800 bytes at $0F setting */


  // Echo Feedback (EFB)
  void update_efb();
  char efb_cbuf[sizeof("$10")];
  Text efb_title, efb_valtext;
  Button efb_incbtn, efb_decbtn;
  static int inc_efb(void *i);
  static int dec_efb(void *i);

  /* TODO FIR FILTER */
};
