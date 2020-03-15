#include "Menu_Bar.h"
#include "utility.h"
#include "platform.h"
#include "Options_Window.h"
#include "gui/Spc_Export_Window.h"

#include "gme/Wave_Writer.h"
#include "Tracker.h"

extern Tracker *tracker;

Menu_Bar::Menu_Bar()
{
}

Menu_Bar::~Menu_Bar()
{
}

int Menu_Bar::Edit_Context::copy(void *data)
{
  // STUB
}

int Menu_Bar::Edit_Context::paste(void *data)
{
  // STUB
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
    is_first_run = false;
  }

  context_menus.draw(screen);
}



void Menu_Bar::Track_Context::draw(SDL_Surface *screen)
{
  Clickable_Text *ct = (Clickable_Text*) &menu_items[1].clickable_text;
  if (::player->is_paused() )
  {
    ct->str = "play";
  }
  else 
  {
    ct->str = "pause";
  }
  menu.draw(screen);
}

int Menu_Bar::File_Context::new_song(void *data)
{
  // STUB
}

int Menu_Bar::File_Context::open_song(void *data)
{
	File_Context *fc = (File_Context *)data;
	nfdchar_t *filepath;
	/* Open the file */
	SDL_RWops *file;
	nfdresult_t rc = Utility::get_file_read_handle(&filepath, &file, "stp");
	if (rc != NFD_OKAY)
		return rc;

	rc = ::tracker->read_from_file(file);
	SDL_RWclose(file);

	if (rc == 0)
	{
		/*Successful. Update the filepath */
		if (fc->filepath)
			free(fc->filepath);

		fc->filepath = filepath;
	}
	return rc;
}

int Menu_Bar::File_Context::open_spc(void *data)
{
  ::player->pause(1,true,false);

  if (::nfd.get_multifile_read_path("spc,rsn,rar,7z") == NFD_OKAY)
  {
  }

  ::player->pause(0);
  return 0;
}

static int save_common(Menu_Bar::File_Context *fc, nfdchar_t *filepath)
{
	// now we have filepath: Open a Write RWOps handle on it
	SDL_RWops *file;

	file = SDL_RWFromFile(filepath, "wb");

	if (file == NULL)
	{
		char tmpbuf[250];
		sprintf(tmpbuf, "Warning: Unable to open file %s!\n %s", filepath, SDL_GetError() );
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
				"Could not open FILE!",
				tmpbuf,
				NULL);
		if (filepath)
			free(filepath);
		return -1;
	}

	::tracker->save_to_file(file);

	SDL_RWclose(file);

	fc->filepath = filepath;
	return 0;
}

static nfdresult_t query_user_save_filepath(nfdchar_t **filepath)
{
	// Open a dialog to query the user for a file to save to
	nfdresult_t result = NFD_SaveDialog( "stp", NULL, filepath );
	if ( result == NFD_OKAY )
	{
	}
	else if ( result == NFD_CANCEL )
	{
		if (*filepath)
		{
			free(*filepath);
			filepath = NULL;
		}
	}
	else
	{
		printf("Error opening %s for writing: %s\n", filepath, NFD_GetError() );
		if (*filepath)
		{
			free(*filepath);
			*filepath = NULL;
		}
	}

	return result;
}

int Menu_Bar::File_Context::save_song(void *data)
{
	File_Context *fc = (File_Context *)data;
	nfdchar_t *filepath = NULL;

	if (fc->filepath == NULL)
	{
		if (query_user_save_filepath(&filepath) != NFD_OKAY)
			return -1;
	}
	else
		filepath = fc->filepath;

	return save_common(fc, filepath);
}

int Menu_Bar::File_Context::save_as_song(void *data)
{
	File_Context *fc = (File_Context *)data;
	nfdchar_t *filepath = NULL;

	if (query_user_save_filepath(&filepath) != NFD_OKAY)
		return -1;

	return save_common(fc, filepath);
}

int Menu_Bar::File_Context::export_spc(void *data)
{
  // Do not show the window if the player has nothing loaded
  if (!::player->has_no_song)
    ::spc_export_window->show();

  return 0;
}


// This is the same func as Debugger's Menu_Bar. should rename the
// Debugger Menu class so we can reference those funcs
int Menu_Bar::File_Context::export_wav(void *data)
{
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
    //BaseD::reload();
    ::player->pause(0, false, false);

/* FIXME TODO
    while ( (::player->emu()->tell()/1000) < BaseD::song_time )
    {
      const unsigned int buf_size = 1024 // can be any multiple of 2
      sample_t buf [buf_size];

      Music_Player::fill_buffer(::player, buf, buf_size);

      wave_write( buf, buf_size );
    }
*/
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
  return 0;
}         

int Menu_Bar::Track_Context::restart_current_track (void *data)
{
  return 0;
}

void Menu_Bar::Context_Menus::preload(int x/*=x*/, int y/*=y*/)
{
  this->x = x; this->y = y;
  file_context.menu.preload(x, y);
  x +=  ( strlen(file_context.menu_items[0].clickable_text.str)
          * CHAR_WIDTH ) + CHAR_WIDTH*2;

  edit_context.menu.preload(x, y);
  x +=  ( strlen(file_context.menu_items[0].clickable_text.str)
          * CHAR_WIDTH ) + CHAR_WIDTH*2;

  track_context.menu.preload(x, y);
  x +=  ( strlen(track_context.menu_items[0].clickable_text.str)
          * CHAR_WIDTH ) + CHAR_WIDTH*2;

  window_context.menu.preload(x,y);
  x +=  ( strlen(window_context.menu_items[0].clickable_text.str)
          * CHAR_WIDTH ) + CHAR_WIDTH*2;
}

bool Menu_Bar::Context_Menus::check_left_click_activate(int &x, int &y,
        const Uint8 &button, const SDL_Event *ev)
{
  if (file_context.menu.check_left_click_activate(x, y, button, ev))
  {
    edit_context.menu.deactivate();
    track_context.menu.deactivate();
    window_context.menu.deactivate();
    return true;
  }

  if (edit_context.menu.check_left_click_activate(x, y, button, ev))
  {
    file_context.menu.deactivate();
    track_context.menu.deactivate();
    window_context.menu.deactivate();
    return true;
  }

  if (::player->has_no_song)
    return false;

  if (track_context.menu.check_left_click_activate(x, y, button, ev))
  {
    file_context.menu.deactivate();
    edit_context.menu.deactivate();
    window_context.menu.deactivate();
    return true;
  }
  if (window_context.menu.check_left_click_activate(x, y, button, ev))
  {
    file_context.menu.deactivate();
    edit_context.menu.deactivate();
    track_context.menu.deactivate();
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
      bool r = false; //tabs.check_mouse_and_execute(ev.button.x, ev.button.y);
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
      window_context.menu.is_active);
}
int Menu_Bar::Context_Menus::receive_event(SDL_Event &ev)
{
  int r;
  if ( ev.type == SDL_MOUSEBUTTONDOWN  && ev.button.button == SDL_BUTTON_LEFT || is_anything_active() )
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
  }
}

/*
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

  rect = {mem.rect.x, mem.rect.y, instr.rect.x + instr.rect.w, CHAR_HEIGHT};
}*/
