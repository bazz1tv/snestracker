#include "shared/gui/Text_Edit_Rect.h"

Cursor Text_Edit_Rect::cursor;

/* Let's try putting the edit logic into the window's logic handler. */
int handle_text_edit_rect_event(const SDL_Event &ev, Text_Edit_Rect *ter)
{
  /*
  switch (ev.type)
  {
    case SDL_TEXTINPUT:
      if (SDL_strlen(event.text.text) == 0 || event.text.text[0] == '\n')
        break;

      if (SDL_strlen(text) + SDL_strlen(event.text.text) < sizeof(text))
        SDL_strlcat(text, event.text.text, sizeof(text));

      // need a handle on our TER
      ter->str += ev.text.text;
      break;

    case SDL_TEXTEDITING:
      break; 
  }*/
}

// We need a callback that handles what happens when this rect has been
// clicked, with state control (editing). proto code follows
//
// Add code to change the cursor to I when a Text_Edit_Rect rect is
// hovered over. 
int Text_Edit_Rect::clicked_callback(void *data)
{
  Text_Edit_Rect *ter = (Text_Edit_Rect *)data;
  // the rect has been clicked (it is clickable_text, after all.
  /* Different handling based on editing */
  if (!ter->editing)
  {
    /* if we re not editing, then we will start editing. Position the
     * cursor at the char nearest where the click occurred.*/

    /* We need re-usable code for the blinking cursor. That should be
     * around from memhexdump. Check it out */
    // start the cursor
    Text_Edit_Rect::cursor.start_timer();
    SDL_StartTextInput();
  }
  else
  {
    /* IF a click happened inside the rect while we were editing, it
     * ccould have been a number of things:
     *
     * . The user is repositioning the cursor.
     * . is highlighting text for a selection. We can determine this if
     * the button is continueously held down.
     * . ?*/
  }
}
