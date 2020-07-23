#include "Menu_Bar.h"
#include "utility.h"
#include "platform.h"
#include "Instrument_Window.h"
#include "shared/windows/Options_Window/Options_Window.h"
#include "gui/Spc_Export_Window.h"

#include "gme/Wave_Writer.h"

void Menu_Bar::Tabs::draw()
{
  // tmpfix
  if (BaseD::grand_mode != logged_grand_mode)
  {
    logged_grand_mode = BaseD::grand_mode;

    if (BaseD::grand_mode == BaseD::GrandMode::MAIN)
    {
      mem.active = true;
      dsp.active = false;
      instr.active = false;
    }
    else if (BaseD::grand_mode == BaseD::GrandMode::DSP_MAP)
    {
      mem.active = false;
      dsp.active = true;
      instr.active = false;
    }
    else if (BaseD::grand_mode == BaseD::GrandMode::INSTRUMENT)
    {
      mem.active = false;
      dsp.active = false;
      instr.active = true;
    }    
  }
    

  mem.draw();
  dsp.draw();
  instr.draw();
}

int Menu_Bar::Edit_Context::open_options_window(void *data)
{
  ::options_window->show();
  ::options_window->raise();

  return 0;
}

void Menu_Bar::draw(SDL_Surface *screen)
{
  if (is_first_run)
  {
    context_menus.preload(10, 10);
    tabs.preload(context_menus.x, context_menus.y + context_menus.h + CHAR_HEIGHT*2);
    is_first_run = false;
    fprintf(stderr, "menubar DERP");
  }
  //sprintf(tmpbuf, " QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK - MM - DM - INSTR");
  //sdlfont_drawString(screen, 0, screen->h-9, tmpbuf, Colors::yellow);
  
  tabs.draw();
  context_menus.draw(screen);
  
}



void Menu_Bar::Track_Context::draw(SDL_Surface *screen)
{
  Clickable_Text *ct = (Clickable_Text*) &menu_items[1].clickable_text;
  if (::player->is_paused() || (BaseD::exp == BaseD::instr_window && BaseD::instr_window->start_stop.is_started) )
  {
    ct->str = "play";
  }
  else 
  {
    ct->str = "pause";
  }
  menu.draw(screen);
}

int Menu_Bar::File_Context::open_spc(void *data)
{
  ::player->pause(1,true,false);
  if (BaseD::nfd.get_multifile_read_path("spc,rsn,rar,7z") == NFD_OKAY)
  {
    DEBUGLOG("check_paths_and_reload\n");
    BaseD::check_paths_and_reload(BaseD::nfd.paths, BaseD::nfd.numpaths);
  }
  ::player->pause(0);
  return 0;
}

int Menu_Bar::File_Context::export_spc(void *data)
{
  // Do not show the window if the player has nothing loaded
  if (!::player->has_no_song)
    ::spc_export_window->show();

  return 0;
}

int Menu_Bar::File_Context::export_wav(void *data)
{
  //SDL_RWops *file;
  nfdchar_t *outPath=NULL;

  nfdresult_t result = NFD_SaveDialog( "wav", NULL, &outPath );
  SDL_RaiseWindow(::render->sdlWindow);
  if ( result == NFD_OKAY )
  {
    if (outPath !=NULL)
      fprintf(stderr, "%s\n", outPath);

    /* Begin writing to wave file */
    wave_open( ::player->sample_rate, outPath );
    wave_enable_stereo();

    ::player->exporting = true;
    BaseD::reload();
    ::player->pause(0, false, false);

    while ( (::player->emu()->tell()/1000) < BaseD::song_time )
    {
      /* Sample buffer */
      const unsigned int buf_size = 1024; /* can be any multiple of 2 */
      sample_t buf [buf_size];
      
      /* Fill sample buffer */
      Music_Player::fill_buffer(::player, buf, buf_size);
      
      /* Write samples to wave file */
      wave_write( buf, buf_size );
    }

    free(outPath);
    wave_close();
    ::player->exporting = false;
    return result;
  }
  else if ( result == NFD_CANCEL ) 
  {
    if (outPath)
      free(outPath);
    puts("User pressed cancel.");
    return result;
  }
  else
  {
    if (outPath)
    free(outPath);
    printf("Error: %s\n", NFD_GetError() );
    return NFD_ERROR;
  }
  
}

int Menu_Bar::Track_Context::toggle_pause (void *data)
{
  BaseD::toggle_pause();
  return 0;
}         
int Menu_Bar::Track_Context::restart_current_track (void *data)
{
  BaseD::restart_current_track();
  return 0;
}
int Menu_Bar::Track_Context::prev_track (void *data)
{
  BaseD::prev_track();
  return 0;
}        
int Menu_Bar::Track_Context::next_track (void *data)
{
  BaseD::next_track();
  return 0;
}
int Menu_Bar::Track_Context::create_profile(void *data)
{
  BaseD::tmp_profile = new BaseD::Profile(BaseD::g_cfg.playlist[BaseD::g_cur_entry]);
  return 0;
}


int Menu_Bar::Window_Context::restore_window_size(void *nada)
{
  SDL_SetWindowSize(::render->sdlWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
  return 0;
}

void Menu_Bar::Context_Menus::preload(int x/*=x*/, int y/*=y*/)
{
  this->x = x; this->y = y;
  file_context.menu.preload(x, y);
  x +=  ( strlen(file_context.menu_items[0].clickable_text.str) * CHAR_WIDTH ) + CHAR_WIDTH*2;

  edit_context.menu.preload(x, y);
  x +=  ( strlen(file_context.menu_items[0].clickable_text.str) * CHAR_WIDTH ) + CHAR_WIDTH*2;

  track_context.menu.preload(x, y);
  x +=  ( strlen(track_context.menu_items[0].clickable_text.str) * CHAR_WIDTH ) + CHAR_WIDTH*2;

  window_context.menu.preload(x,y);
  x +=  ( strlen(window_context.menu_items[0].clickable_text.str) * CHAR_WIDTH ) + CHAR_WIDTH*2;

  about_context.menu.preload(x,y);
  x +=  ( strlen(about_context.menu_items[0].clickable_text.str)
          * CHAR_WIDTH ) + CHAR_WIDTH*2;
}

bool Menu_Bar::Context_Menus::check_left_click_activate(int &x, int &y, const Uint8 &button, const SDL_Event *ev)
{
  if (file_context.menu.check_left_click_activate(x, y, button, ev))
  {
    edit_context.menu.deactivate();
    track_context.menu.deactivate();
    window_context.menu.deactivate();
    about_context.menu.deactivate();
    return true;
  }

  if (edit_context.menu.check_left_click_activate(x, y, button, ev))
  {
    file_context.menu.deactivate();
    track_context.menu.deactivate();
    window_context.menu.deactivate();
    about_context.menu.deactivate();
    return true;
  }

  if (::player->has_no_song)
    return false;

  if (track_context.menu.check_left_click_activate(x, y, button, ev))
  {
    file_context.menu.deactivate();
    edit_context.menu.deactivate();
    window_context.menu.deactivate();
    about_context.menu.deactivate();
    return true;
  }
  if (window_context.menu.check_left_click_activate(x, y, button, ev))
  {
    file_context.menu.deactivate();
    edit_context.menu.deactivate();
    track_context.menu.deactivate();
    about_context.menu.deactivate();
    return true;
  }
  if (about_context.menu.check_left_click_activate(x, y, button, ev))
  {
    file_context.menu.deactivate();
    edit_context.menu.deactivate();
    window_context.menu.deactivate();
    return true;
  }

  return false;
}

int Menu_Bar::receive_event(SDL_Event &ev)
{ 
  int r;
  
  if (ev.type == SDL_MOUSEBUTTONDOWN)
  {
    if (!::player->has_no_song)
    {
      bool r = tabs.check_mouse_and_execute(ev.button.x, ev.button.y);
      if (r) return r;
    }
  }

  r = context_menus.receive_event(ev);
  if (r) return r;

  return EVENT_INACTIVE;
}

bool Menu_Bar::Context_Menus::is_anything_active()
{
  return (file_context.menu.is_active || 
    edit_context.menu.is_active || 
    track_context.menu.is_active || 
    window_context.menu.is_active ||
    about_context.menu.is_active);
}
int Menu_Bar::Context_Menus::receive_event(SDL_Event &ev)
{
  int r;
  if ( ev.type == SDL_MOUSEBUTTONDOWN || is_anything_active() )
  {
    if (check_left_click_activate(ev.button.x, ev.button.y, ev.button.button, &ev))
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
  if ((r=edit_context.menu.receive_event(ev)))
  {
    if (r == Expanding_List::EVENT_MENU)
      return EVENT_FILE;
    return EVENT_ACTIVE;
  }
  if (!::player->has_no_song)
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
    if ((r=about_context.menu.receive_event(ev)))
    {
      if (r == Expanding_List::EVENT_MENU)
        return EVENT_ABOUT;
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
  edit_context.menu.draw(screen);
  if (!::player->has_no_song)
  {
    track_context.draw(screen);
    window_context.menu.draw(screen);
    about_context.menu.draw(screen);
  }
}

void Menu_Bar::Tabs::preload(int x, int y)
{
  // init Tabs
  mem.rect.x = x;
  mem.rect.y = y; // + h + CHAR_HEIGHT*2;
  //
  dsp.rect.x = mem.rect.x + mem.horiz_pixel_length() + CHAR_WIDTH;
  dsp.rect.y = mem.rect.y;
  //
  instr.rect.x = dsp.rect.x + dsp.horiz_pixel_length() + CHAR_WIDTH;
  instr.rect.y = mem.rect.y;

  rect = {mem.rect.x, mem.rect.y, instr.rect.x - mem.rect.x + instr.rect.w, CHAR_HEIGHT};
}

int Menu_Bar::About_Context::clicked_patreon(void *nada)
{
  DEBUGLOG("CLICKED PATREON\n");
  openUrl(PATREON_URL);
  return 0;
}

int Menu_Bar::About_Context::clicked_merch(void *nada)
{
  DEBUGLOG("CLICKED MERCH\n");
  openUrl(MERCH_URL);
  return 0;
}
