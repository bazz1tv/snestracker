#include "gui/cursor.h"


  
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
    /*if (timerid == timerid)
    {
      fprintf(stderr, "memcursor::timerID == cursor::timerid.. NOT FIXED\n");
    }*/

  }

  void Mem_Cursor::stop_timer()
  {
    //report::restore_color(mouse_hexdump::address);
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
  
  
 

  Uint32 Cursor::cursor_timerCB(Uint32 interval, void *param)
  {
    return ((Cursor *) param)->cursor_timer(interval); 
  }
  // timer callback, dont worry about this
  Uint32 Cursor::cursor_timer(Uint32 interval)
  {
    //fprintf(stderr, "cursor toggle, ");
    toggle = !toggle;
    return interval;
  }

  void Cursor::start_timer()
  {
    //fprintf(stderr, "cursor timer ON\n");
    // always start with the cursor showing
    toggle = 1;
    // is there a problem Removing an invalid timerid?? I think not..
    SDL_RemoveTimer(timerid);
    // i remove it when starting so I can have simple repeat-logic in the 
    // double click code
    timerid = SDL_AddTimer(CURSOR_TOGGLE_TIMEWINDOW, &cursor_timerCB, this);
    //fprintf(stderr, "TIMER ON %d\n", timerid);

    // fix a buf where both cursor::timerid and memcursor::timerid were
    // getting the same ID.. I think this is OK fix.
    
    /* DONT WORY ABOUT THIS FOR NOW
    if (timerid == memcursor::timerid)
    {
      //fprintf(stderr, "WTF");
      timerid = SDL_AddTimer(CURSOR_TOGGLE_TIMEWINDOW, &cursor_timer, &toggle);
    }*/
  }
  void Cursor::stop_timer()
  {
    //fprintf(stderr, "cursor timer OFF\n");
    SDL_RemoveTimer(timerid);
    // toggle must be set to 0 to prevent drawing to screen
    toggle = 0;
    //fprintf(stderr, "TIMER OFF\n");
  }

  void Cursor::draw(SDL_Surface *screen, int x, int y, Uint32 color)
  {
    if (toggle)
    {
        sdlfont_drawString(screen, x, y, "\x5B", color);
    }
  }

  void Cursor::draw(SDL_Surface *screen, Uint32 color)
  {
    if (toggle)
    {
        sdlfont_drawString(screen, rect.x, rect.y, "\x5B", color);
    }
  }

  //Uint8 Cursor::


