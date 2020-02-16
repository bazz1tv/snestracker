#include "gui/Cursor.h"

Uint32 Cursor::cursor_timerCB(Uint32 interval, void *param)
{
  return ((Cursor *) param)->cursor_timer(interval); 
}

// timer callback, dont worry about this
Uint32 Cursor::cursor_timer(Uint32 interval)
{
  //fprintf(stderr, "Cursor callback!\n");
  toggle = !toggle;
  return interval;
}

void Cursor::start_timer()
{
  // always start with the cursor showing
  toggle = 1;
  // is there a problem Removing an invalid timerid?? I think not..
  SDL_RemoveTimer(timerid);
  // i remove it when starting so I can have simple repeat-logic in the
  // double click code
  timerid = SDL_AddTimer(CURSOR_TOGGLE_TIMEWINDOW, &cursor_timerCB, this);

  // fix a buf where both cursor::timerid and memcursor::timerid were
  // getting the same ID.. I think this is OK fix.
}
void Cursor::stop_timer()
{
  SDL_RemoveTimer(timerid);
  // toggle must be set to 0 to prevent drawing to screen
  toggle = 0;
}

void Cursor::draw(SDL_Surface *screen, int x, int y, Uint32 color,
  bool draw_transparent/*=false*/, const char *cursor_str/*="\x5b"*/)
{
  if (toggle)
      sdlfont_drawString(screen, x, y, cursor_str, color);
  else if (draw_transparent)
    sdlfont_drawString(screen, x, y, cursor_str, Colors::transparent);
}

void Cursor::draw(SDL_Surface *screen, Uint32 color,
  bool draw_transparent/*=false*/, const char *cursor_str/*="\x5B"*/)
{
  if (toggle)
    sdlfont_drawString(screen, rect.x, rect.y, cursor_str, color);
  else if (draw_transparent)
    sdlfont_drawString(screen, rect.x, rect.y, cursor_str, Colors::transparent);
}
