#pragma once

#include "shared/gui/Clickable_Text.h"
#include "shared/gui/Cursor.h"
#include "shared/Colors.h"


struct Text_Edit_Rect : public Clickable_Text
{
  Text_Edit_Rect(int width) : Clickable_Text("", clicked_callback, this)
  {
    //if (width % CHAR_HEIGHT)
      //width += CHAR_HEIGHT - (width % CHAR_HEIGHT);
    rect.w = width * CHAR_WIDTH;
    rect.h = 2 + CHAR_HEIGHT + 2;
  }

  void one_time_draw();

  void draw(Uint32 &color, bool prefill=true, bool Vflip=false,
            bool Hflip=false, SDL_Surface *screen=::render->screen);

  static int clicked_callback(void *data);
  /* The one cursor that will work across all T.E.Rs. */
  static Cursor cursor;
  static int comp_start_point;
  /* The one marked text thing */
  static char markedText[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
  static SDL_Rect markedRect;
  char *str;
  // Stateful info
  bool editing = false;
  bool needs_redraw = false;
};
