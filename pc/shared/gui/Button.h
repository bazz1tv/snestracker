#pragma once
#include "gui/Clickable_Text.h"

struct Button : public Clickable_Text
{
  /* The difference Button from Clickable_Text is as follows:
   * Event handler code that adjusts the drawing when the left mouse
   * button is held down. The adjustments are a slightly lower vertical
   * text position (perhaps also slightly right horiz position), and a
   * darker background color. They also receive a callback function to
   * call when the mouse button is released.*/
  Button(const char * str, int (*action)(void *data)=NULL, void *data=NULL, bool holdrepeat=false);
  
  void check_event(const SDL_Event &ev);

  void draw(SDL_Surface *screen, bool flipV=false);

  static Uint32 held_callback(Uint32 interval, void *b);
  static void trigger_callback(void *b);

  SDL_Rect outer;

private:
  /* Based on the below state, alternative colors and positioning will
   * effect in draw() */
  enum
  {
    LEFTMOUSEBUTTON_HELD_IN_RECT=1,
  };
  unsigned int state :1;
  /* Whether or not when a button has been for heldtime ms, will it start
   * spewing continuous actions every triggerfreq ms */
  /* In order to check that the button has been held, we can use the
   * already present 'state' variable, along with a timer */
  unsigned int holdrepeat :1; // this is a flag

  /* How long to wait until repeating actions, and how often actions are
   * repeated */
  static const unsigned int holdtime, triggerfreq;
  static Uint32 hold_tid; // timer ID
  static void remove_hold_timer();
};
