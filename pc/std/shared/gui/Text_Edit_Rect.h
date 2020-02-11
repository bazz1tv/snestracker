#pragma once

#include "shared/gui/Clickable_Text.h"
#include "shared/gui/Cursor.h"
#include "shared/Colors.h"


class Text_Edit_Rect : public Clickable_Text
{
  Text_Edit_Rect(int width) : Clickable_Text("", clicked_callback, this)
  {
    if (width % CHAR_HEIGHT)
      width += CHAR_HEIGHT - (width % CHAR_HEIGHT);
    rect.w = width;
  }

  void draw(Uint32 &color, bool prefill=true, bool Vflip=false,
            bool Hflip=false, SDL_Surface *screen=::render->screen)
  {
    cursor.draw(screen, rect.x + (SDL_strlen(str) * CHAR_WIDTH), rect.y,
                Colors::green);
  }

  static int clicked_callback(void *data);
  /* The one cursor that will work across all T.E.Rs. */
  static Cursor cursor;
  // Stateful info
  bool editing = false;
};
