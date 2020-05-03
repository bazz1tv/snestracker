#include "gui/Mem_Cursor.h"

Uint32 Mem_Cursor::cursor_timerCB(Uint32 interval, void *param)
{
  return ((Mem_Cursor *) param)->cursor_timer(interval);
}

Uint32 Mem_Cursor::cursor_timer(Uint32 interval)
{
  flags ^= FLAG_TOGGLED;
  return interval;
}

void Mem_Cursor::start_timer()
{
  if (flags & FLAG_DISABLED)
    return;

  // always start with the cursor showing

  // is there a problem Removing an invalid timerid?? I think not..
  SDL_RemoveTimer(timerid);
  // i remove it when starting so I can have simple repeat-logic in the
  // double click code
  timerid = SDL_AddTimer(interval, &cursor_timerCB, this);
  flags=FLAG_ACTIVE | FLAG_TOGGLED;
}

void Mem_Cursor::stop_timer()
{
  flags &= ~(FLAG_TOGGLED | FLAG_ACTIVE);
  SDL_RemoveTimer(timerid);
  // flags must be set to 0 to prevent drawing to screen
}

char Mem_Cursor::is_active() { return flags & FLAG_ACTIVE; }
char Mem_Cursor::is_disabled() { return flags & FLAG_DISABLED; }
char Mem_Cursor::is_toggled() { return flags & FLAG_TOGGLED; }
void Mem_Cursor::disable(char c/*=1*/)
{
  if (c)
  {
    if (is_active())
      flags |= FLAG_WAS_ACTIVE_BEFORE_DISABLING;
    else flags &= ~FLAG_WAS_ACTIVE_BEFORE_DISABLING;
    stop_timer();
    flags |= FLAG_DISABLED;
  }
  else
  {
    flags &= ~FLAG_DISABLED;
    if (flags & FLAG_WAS_ACTIVE_BEFORE_DISABLING)
      start_timer();
  }
}

void Mem_Cursor::toggle_disable()
{
  disable(!is_disabled());
}
