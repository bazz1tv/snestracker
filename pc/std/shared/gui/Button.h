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
  Button(const char * str, int (*action)(void *data)=NULL, void *data=NULL);
  
  void check_event(const SDL_Event &ev);

  void draw(SDL_Surface *screen);

  /* Based on the below state, alternative colors and positioning will
   * effect in draw() */
  enum
  {
    LEFTMOUSEBUTTON_HELD_IN_RECT=1,
  };
  unsigned int state :1;

  //Uint32 bg_color[2], fg_color[2];
  // This makes a complete waste of Clickable_Text color variable :( That
  // can be fixed later. Rapid prototype!
  SDL_Rect outer;
};
