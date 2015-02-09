#include "Menu_Bar.h"
#include "utility.h"
#include "File_System_Context.h"
#include "platform.h"
//#include <stdlib.h>

void Menu_Bar::Track_Context::draw(SDL_Surface *screen)
{
  Clickable_Text *ct = (Clickable_Text*) &menu_items[1].clickable_text;
  if (BaseD::player->is_paused())
  {
    ct->str = "play";
    //ct->str.clear();
    //ct->str.append( "resume" );
  }
  else 
  {
    ct->str = "pause";
  }
  menu.draw(screen);
}

int Menu_Bar::File_Context::open_spc(void *data)
{
  if (BaseD::nfd.get_multifile_read_path("spc,rsn,rar") == NFD_OKAY)
  {
    BaseD::check_paths_and_reload(BaseD::nfd.paths, BaseD::nfd.numpaths);
  }
}

int Menu_Bar::Track_Context::toggle_pause (void *data)
{
  
  BaseD::toggle_pause();
  
}         
int Menu_Bar::Track_Context::restart_current_track (void *data)
{
  BaseD::restart_current_track();
}
int Menu_Bar::Track_Context::prev_track (void *data)
{
  BaseD::prev_track();
}        
int Menu_Bar::Track_Context::next_track (void *data)
{
  BaseD::next_track();
}

int Menu_Bar::Window_Context::memory(void *data)
{
  BaseD::switch_mode(BaseD::GrandMode::MAIN);
}
int Menu_Bar::Window_Context::DSP(void *data)
{
  BaseD::switch_mode(BaseD::GrandMode::DSP_MAP);
}
int Menu_Bar::Window_Context::Instrument(void *data)
{
  BaseD::switch_mode(BaseD::GrandMode::INSTRUMENT);
}

void Menu_Bar::draw(SDL_Surface *screen)
{
  if (is_first_run)
  {
    context_menus.preload(10, 10);
    is_first_run = false;
    fprintf(stderr, "menubar DERP");
  }
  //sprintf(tmpbuf, " QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK - MM - DM - INSTR");
  //sdlfont_drawString(screen, 0, screen->h-9, tmpbuf, Colors::yellow);
  context_menus.draw(screen);
}

void Menu_Bar::Context_Menus::preload(int x/*=x*/, int y/*=y*/)
{
  this->x = x; this->y = y;
  file_context.menu.preload(x, y);
  x +=  ( file_context.menu_items[0].clickable_text.str.length() * CHAR_WIDTH ) + CHAR_WIDTH*2;

  track_context.menu.preload(x, y);
  x +=  ( track_context.menu_items[0].clickable_text.str.length() * CHAR_WIDTH ) + CHAR_WIDTH*2;

  window_context.menu.preload(x,y);
  x +=  ( window_context.menu_items[0].clickable_text.str.length() * CHAR_WIDTH ) + CHAR_WIDTH*2;
}

bool Menu_Bar::Context_Menus::check_left_click_activate(int &x, int &y, const Uint8 &button)
{
  if (file_context.menu.check_left_click_activate(x, y, button))
  {
    track_context.menu.deactivate();
    window_context.menu.deactivate();
    return true;
  }

  if (BaseD::player->has_no_song)
    return false;

  if (track_context.menu.check_left_click_activate(x, y, button))
  {
    file_context.menu.deactivate();
    window_context.menu.deactivate();
    return true;
  }
  if (window_context.menu.check_left_click_activate(x, y, button))
  {
    file_context.menu.deactivate();
    track_context.menu.deactivate();
    return true;
  }

  return false;
}

int Menu_Bar::Context_Menus::receive_event(SDL_Event &ev)
{
  int r;
  if (ev.type != SDL_MOUSEBUTTONUP && (ev.type == SDL_MOUSEBUTTONDOWN || 
    (file_context.menu.is_active || track_context.menu.is_active || window_context.menu.is_active) ) )
  {
    if (check_left_click_activate(ev.button.x, ev.button.y, ev.button.button))
    {
      return EVENT_ACTIVE;
    }
  }

  if ((r=file_context.menu.receive_event(ev)))
  {
    if (r == Expanding_List::EVENT_MENU)
      return EVENT_FILE;
    return EVENT_ACTIVE;
  }
  if (!BaseD::player->has_no_song)
  {
    if ((r=track_context.menu.receive_event(ev)))
    {
      if (r == Expanding_List::EVENT_MENU)
        return EVENT_TRACK;
      return EVENT_ACTIVE;
    }
    if ((r=window_context.menu.receive_event(ev)))
    {
      if (r == Expanding_List::EVENT_MENU)
        return EVENT_WINDOW;
      return EVENT_ACTIVE;
    }
  }

  return EVENT_INACTIVE;
}
void Menu_Bar::Context_Menus::update(Uint8 adsr1, Uint8 adsr2)
{
  // Don't need this because there is no currently selected item, just actions
  /*file_context.menu.update_current_item(Menu_Bar::get_attack_index(adsr1));
  window_context.menu.update_current_item(Menu_Bar::get_sustain_level_index(adsr2));
  track_context.menu.update_current_item(Menu_Bar::get_decay_index(adsr1));*/
  //track_context.update();
}

void Menu_Bar::Context_Menus::draw(SDL_Surface *screen)
{
  file_context.menu.draw(screen);
  if (!BaseD::player->has_no_song)
  {
    track_context.draw(screen);
    window_context.menu.draw(screen);
  }
}