#pragma once

#include "shared/gui/Clickable_Text.h"
#include "shared/gui/Cursor.h"
#include "shared/Colors.h"

struct Text_Edit_Rect : public Clickable_Text
{
  Text_Edit_Rect(int width, const char *str, int strsize) :
          Clickable_Text(str, clicked_callback, this),
          strsize(strsize)
  {
    //if (width % CHAR_HEIGHT)
      //width += CHAR_HEIGHT - (width % CHAR_HEIGHT);
    rect.w = width * CHAR_WIDTH;
    rect.h = CHAR_HEIGHT;
  }

  void one_time_draw(SDL_Surface *screen);

  void draw(Uint32 color, bool prefill=true, bool Vflip=false,
            bool Hflip=false, SDL_Surface *screen=::render->screen);

  static int clicked_callback(void *data);
  /* The one cursor that will work across all T.E.Rs. */
  static Cursor cursor;
  static int comp_start_point;
  /* The one marked text thing */
  static char markedText[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
  static SDL_Rect markedRect;
  // Stateful info
  bool editing = false;
  bool needs_redraw = false;
  unsigned int strsize;
};

int handle_text_edit_rect_event(const SDL_Event &ev, Text_Edit_Rect *ter);
