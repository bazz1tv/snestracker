#pragma once

#include "shared/gui/Clickable_Text.h"
#include "shared/gui/cursor.h"
/* Let's try putting the edit logic into the window's logic handler. */
int handle_text_edit_rect_event(const SDL_Event &ev)
{
  switch (ev)
  {
  case ....:

  case ....:

  case ....:
  }
}

// We need a callback that handles what happens when this rect has been
// clicked, with state control (editing). proto code follows
//
// Add code to change the cursor to I when a Text_Edit_Rect rect is
// hovered over. 
int callback(void *data)
{
  // the rect has been clicked (it is clickable_text, after all.
  /* Different handling based on editing */
  if (!editing)
  {
    /* if we re not editing, then we will start editing. Position the
     * cursor at the char nearest where the click occurred.*/

    /* We need re-usable code for the blinking cursor. That should be
     * around from memhexdump. Check it out */
  }
  else
  {
  }
}

class Text_Edit_Rect : public Clickable_Text
{
  Text_Edit_Rect(int width) : Clickable_Text("", action,data)
  {
    if (width % CHAR_HEIGHT)
      width += CHAR_HEIGHT - (width % CHAR_HEIGHT);
    rect.w = width;
  }
  // Stateful info
  bool editing = false;
};
