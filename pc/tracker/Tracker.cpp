#include "Tracker.h"
#include <getopt.h>
#include "utility.h"
#include "Screen.h"
#include "sdl_userevents.h"
#include "kbd.h"
#include "apuram.h"
#include "shared/SdlNfd.h"
#include "shared/fps.h"
#include "Song.h"
#include "gui/DialogBox.h"

#define L_FLAG 0
#define R_FLAG 1

std::unordered_set<DrawRenderer *> Tracker::prerenders, Tracker::postrenders;

Tracker::Tracker(int &argc, char **argv) :
main_window(argc,argv, this)
{
	/* eventually I want to make sub-windows just an overlay in the one main
	 * window, rather than having separate windows. That's my choice. But
	 * since I have already impl'd some separate window functionality, might
	 * as well keep it and make it an optional thing?? */
	// MARK START of subwindow code
  ::options_window = &options_window;
  ::spc_export_window = &spc_export_window;
  cur_exp = &main_window;

	// Make subwindow location deviate slightly from mainwindow position to
	// make it easier to toggle and position windows.
  int x,y;
  SDL_GetWindowPosition(::render->sdlWindow, &x, &y);
  fprintf(stderr, "x: %d, y: %d\n", x, y);
  x += 20;
  y += 20;
  SDL_SetWindowPosition(::options_window->render.sdlWindow, x, y);

  int i=0;
  Window::map[i++] = ::options_window;
  Window::map[i++] = ::spc_export_window;
  Window::map[i] = NULL;
	// MARK END of subwindow code

  int should_be_zero = SDL_GetCurrentDisplayMode(0, &monitor_display_mode);

    if(should_be_zero != 0)
      // In case of error...
      SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

    else
      // On success, print the current display mode.
      SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, 
        monitor_display_mode.w, monitor_display_mode.h, monitor_display_mode.refresh_rate);

	/* Init mouse cursors. This could probably be on the stack, but during
	 * my testing it became dynamic and I just kept it */
  mousecursors = new MouseCursors;
  ::mousecursors = mousecursors;
  mousecursors->set_cursor(CURSOR_MPAINT_WHITE_HAND);

	// We need to load the APU emulator at some point. Why not here, right
	// now?
	/* APU EMU LOAD CODE */
	char tb[260];
	int len;
	assert(::file_system);

	strcpy(tb, ::file_system->data_path);
	strcat(tb, SPCDRIVER_FILENAME);
	assert(::player);
	handle_error( ::player->load_file(tb) );

	::IAPURAM = player->spc_emu()->ram();
	apuram = (TrackerApuRam *)::IAPURAM;
#ifndef NDEBUG
	//::IAPURAM[6] = 5;
	//assert(apuram->ticks == 5);
#endif
	/* END APU EMU LOAD CODE */

  frame.set(30);
}

Tracker::~Tracker()
{
  DEBUGLOG("~Tracker");
  delete mousecursors;
  mousecursors = NULL;
}

void Tracker::run()
{
  // gotta call this once to initialize important stuffz
  main_window.one_time_draw();

  cur_exp->draw();

gameloop:
  ::quitting = false;
  SDL_ShowWindow(::render->sdlWindow);

  // exp is changed from BaseD
  while (!::quitting)
  {
    frame.start();

    cur_exp->run();
    cur_exp->draw();

    if (sub_window_experience)
    {
      sub_window_experience->run();
      sub_window_experience->draw();
    }

    menu_bar.draw(::render->screen);

    SDL_UpdateTexture(::render->sdlTexture, NULL, ::render->screen->pixels, ::render->screen->pitch);
    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0);
    SDL_RenderClear(::render->sdlRenderer);

    // Certain things want to draw directly to renderer before the screen
    // texture is copied
    for (const auto& elem : prerenders)
      elem->draw_renderer();

    SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);

    // Other things may want to draw directly to renderer after the screen
    // texture is copied
    for (const auto& elem : postrenders)
      elem->draw_renderer();

    // Optionally, let's draw on top of EVERYTHING ELSE, any auxiliary
    // mouse FX
    mousecursors->draw_aux();

    SDL_RenderPresent(::render->sdlRenderer);

    /* The reason I handle events at the end of the loop rather than
     * before display code is so I can easily poll events for the rest of
     * the frame time, otherwise I would have to calculate how much frame
     * time to allocate for event polling, which wouldn't be so bad I
     * imagine. but this works.. */
    handle_events();

    frame.end();
  }

  if (song.changed)
  {
    int rc = DialogUnsavedChanges();
    if (rc < 0) // couldn't save file or user pressed cancel
      goto gameloop;
    // file saved or user pressed 'No'
  }

  sub_window_experience = NULL;

  if (!player->is_paused() && player->track_started)
  {
    player->fade_out(false);
    player->pause(1, false, false);
  }
}

int Tracker::DialogUnsavedChanges()
{
  DEBUGLOG("SONG CHANGED\n");
  const char *buttonStr[3] = { "Yes", "No", "Cancel" };
  char msg[512];
  snprintf(msg, 512 - 1,
    "Would you like to save?"
  );
askagain:
  int button = DialogBox::Custom("Unsaved changes!",
    msg, 3, buttonStr);

  if (button == 0) // YES
  {
    DEBUGLOG("\tUser pushed YES\n");
    if (Menu_Bar::File_Context::save_song(NULL) < 0)
    {
      return -1; // couldn't save
    }
    return 0; // saved
  }
  else if (button == 1)
  {
    DEBUGLOG("\tUser pushed NO\n");
    // do nothing
    return 0; // did nothing
  }
  else if (button == 2) // CANCEL
  {
    DEBUGLOG("\tUser pushed CANCEL\n");
    return -1; // cancel
  }
  else
  {
    DEBUGLOG("\tUNKNOWN BUTTON VALUE %d\n", button);
    goto askagain;
  }

  //return 0;
}

/* Print SNES Tracker, version, followed by str */
void Tracker::updateWindowTitle(const char *str)
{
  /* put the filename in the window title */
  snprintf(windowStr, PATH_MAX, PROG_NAME_VERSION_STRING " - %s", APP_VERSION, str);
  SDL_SetWindowTitle(::render->sdlWindow, windowStr);
}

#define SET_PLAYBACK_BUTTONS(truefalse) \
main_window.patseqpanel.patlen_decbtn.enabled = truefalse;\
main_window.patseqpanel.patlen_incbtn.enabled = truefalse;\
main_window.patseqpanel.clonebtn.enabled = truefalse;\
main_window.patseqpanel.seqbtn.enabled = truefalse;\
main_window.patseqpanel.clearbtn.enabled = truefalse;\
main_window.patseqpanel.insbtn.enabled = truefalse;\
main_window.patseqpanel.zapbtn.enabled = truefalse;\
main_window.patseqpanel.incpatbtn.enabled = truefalse;\
main_window.patseqpanel.decpatbtn.enabled = truefalse;\
main_window.patseqpanel.movePatUpbtn.enabled = truefalse;\
main_window.patseqpanel.movePatDownbtn.enabled = truefalse;\
main_window.samplepanel.loadbtn.enabled = truefalse;\
main_window.instrpanel.loadbtn.enabled = truefalse

bool Tracker::rendering() { return playback || instr_render; }

void Tracker::handle_events()
{
  SDL_Event ev;
  int ev_frame = 0; /* track how many events are processed. Created primarily
  for the soft click frame support detailed below */
  int mbd_frame = -2; /* track the event frame that an SDL_MOUSEBUTTONDOWN
  event occurred in. This is used to detect when event pair of SDL_MOUSEBUTTONDOWN
  is followed immediately by an SDL_MOUSEBUTTONUP in the same graphical frame,
  an otherwise perhaps impossible circumstance that only occurs for touchpad
  "Soft" clicks. These variables help detect when such an event pair occurs and
  pushes the MOUSEBUTTONUP event to be processed in the next graphical frame's event handler,
  so that we can graphically have time to display reactions to the MOUSEBUTTONDOWN
  event, and then later to the MOUSEBUTTONUP event. This at least gives us
  1 frame of graphical change for display entities that update their visual
  based on when the mouse button is down and then up. If I didn't do this,
  there would be no graphical change for event pairs of this nature (once again,
  that is touchpad "soft" clicks, when you simply lightly tap the touchpad rather
  than a hard push down and release).*/
  /* The -2 default value for mbd_frame is merely to disassociate the
   * starting value from the code that will check this value between
   * ev_frame */
  SDL_Event mbu_postpone_ev; /* When the aforementioned event pair anomaly occurs,
  a copy of the mousebuttonup (mbu) event is placed here for later pushing onto
  the event queue after exiting the event poller loop*/

  //static int scrolled_this_gframe = 0; // if we scrolled in this graphical frame

  mbu_postpone_ev.type = SDL_QUIT; /* set the default to anything that isnt mousebuttonup,
  that way we can reliably check later if the type has changed to SDL_MOUSEBUTTONUP
  we know that we must then push this event copy to the queue for next frame's
  processing */

  // Poll events for the remainder of this graphical frame time while the queue
  // has events.
  while (frame.time() && SDL_PollEvent(&ev))
  {
    /* ignore events immediately after NFD dialog (#12) */
    if (SdlNfd::active())
      continue;

    SDL_Event orig_ev = ev;
    Window *win = Window::getWindow(SDL_GetMouseFocus());
    //DEBUGLOG("GRabbed Window = %d\n", win ? win->render.windowID : -1);
    Render *render = win ? &win->render : ::render;

    switch(ev.type)
    {
      case SDL_QUIT:
        SDL_HideWindow(::render->sdlWindow);
        ::quitting = true;
        break;
      case SDL_WINDOWEVENT:
      {
        switch(ev.window.event)
        {
          case SDL_WINDOWEVENT_RESIZED:
          {
            int w = ev.window.data1;
            int h = ev.window.data2;

            DEBUGLOG("window resize: w = %d, h = %d\n", w, h);
            if (render->logical == false)
            {
              render->sx = (float)(w) / (float)(render->w);
              render->sy = (float)(h) / (float)(render->h);
            }
          } break;
          case SDL_WINDOWEVENT_FOCUS_LOST:
          {
            DEBUGLOG("Window %d Lost keyboard focus\n", ev.window.windowID);
            if (ev.window.windowID == ::render->windowID)
            {
              // OFF context menus
              menu_bar.context_menus.deactivate_all();
            }
          }
          break;
          case SDL_WINDOWEVENT_FOCUS_GAINED:
          {
            DEBUGLOG("Window %d Gained keyboard focus\n", ev.window.windowID);
            sub_window_experience = NULL;
            Window *window = Window::getWindow(ev.window.windowID);
            if (window && window->oktoshow)
            {
              DEBUGLOG("Window_map \"%s\" gained experience. :D\n", window->title.c_str());
              sub_window_experience = (Experience *)window;
            }
          }
          break;
          case SDL_WINDOWEVENT_SHOWN:
          {
            SDL_Log("Window %d shown", ev.window.windowID);
            Window *window = Window::getWindow(ev.window.windowID);
            if (window)
            {
              if (!window->oktoshow)
              {
                window->hide();
                /* maybe right here, instead of raising the main window, we should
                  have a history of displayed windows.. and the last one should be raised.
                */
                sub_window_experience = NULL;
              }
              else
              {
                sub_window_experience = NULL;
                DEBUGLOG("Window_map \"%s\" gained experience. :D\n", window->title.c_str());
                sub_window_experience = (Experience *)window;
                window->raise();
              }
            }
          }
          break;
          case SDL_WINDOWEVENT_CLOSE:
          {
            SDL_Log("Window %d closed", ev.window.windowID);
            if (ev.window.windowID == ::render->windowID)
            {
              // quit app

              SDL_Event quit_ev;
              quit_ev.type = SDL_QUIT;
              SDL_PushEvent(&quit_ev);
            }
            else
            {
              Window *window = Window::getWindow(ev.window.windowID);
              if (window && window->oktoshow)
              {
                  window->hide();
                  // maybe right here, instead of raising the main window, we should
                  //  have a history of displayed windows.. and the last one should be raised.

                  sub_window_experience = NULL;
                  SDL_RaiseWindow(::render->sdlWindow);
                  break;
              }
            }
          }
          break;

          default:break;
        }
      } break;
      case SDL_DROPFILE:
      {
        char *dropped_filedir = ev.drop.file;

        ::nfd.free_pathset();
        SDL_free(dropped_filedir);    // Free dropped_filedir memory
        SDL_RaiseWindow(::render->sdlWindow);
      } break;
      case SDL_USEREVENT:
      {
        switch (ev.user.code)
        {
          case UserEvents::sound_stop:
            if (!instr_render)
            {
						  ::player->post_fadeout();
              ::player->pause(true, false, false);
            }
            else
            {
              renderCurrentInstrument();
            }
						break;
          case UserEvents::callback:
          {
            //DEBUGLOG("CALLBACK; ");
            typedef void (*FuncPtr_t)(void *);
            FuncPtr_t p = FuncPtr_t(ev.user.data1);
            p(ev.user.data2);
          }
          break;
					case UserEvents::report_tracker_incrow:
            if(playback)
						  main_window.pateditpanel.inc_currow();
					break;
					case UserEvents::report_tracker_setrow:
            if(playback)
						  main_window.pateditpanel.set_currow((intptr_t)ev.user.data1);
					break;
					case UserEvents::report_tracker_setpattern:
            if(playback)
						  main_window.patseqpanel.set_currow((intptr_t)ev.user.data1);
					break;
        }
      } break;
      case SDL_MOUSEMOTION:
      {
        mouse::prescaled_x = ev.motion.x;
        mouse::prescaled_y = ev.motion.y;
        ev.motion.x = static_cast<int> ((float)(ev.motion.x) / render->sx);
        ev.motion.y = static_cast<int> ((float)(ev.motion.y) / render->sy);
        mouse::x = ev.motion.x;
        mouse::y = ev.motion.y;
      } break;
      case SDL_MOUSEBUTTONDOWN:
      {
        ev.button.x = static_cast<int> ((float)(ev.button.x) / render->sx);
        ev.button.y = static_cast<int> ((float)(ev.button.y) / render->sy);
        //DEBUGLOG("MOUSEBUTTONDOWN; ");
        /* In the case of touchpad soft clicking, it is expected that the
         * MOUSEBUTTONUP is sent as the immediate next event after the
         * mousebuttondown. In this case, it does not allow for the
         * graphical updates between down and up mouse presses to take
         * place, because they are all processed within the same frame.
         * For this reason, I will check if the next event is
         * MOUSEBUTTONUP, and if so, I will postpone it to the following
         * graphical frame */
        mbd_frame = ev_frame; // save the frame number the mbd occurred
      } break;
      case SDL_MOUSEBUTTONUP:
      {
        ev.button.x = static_cast<int> ((float)(ev.button.x) / render->sx);
        ev.button.y = static_cast<int> ((float)(ev.button.y) / render->sy);
        //DEBUGLOG("MOUSEBUTTONUP; ");
        /* If the last frame was the mbd, we know we have found the soft
         * click event pair. copy this mbu event to be postponed after
         * this event loop exits. Also don't process this event */
        if (mbd_frame == (ev_frame - 1))
        {
          mbu_postpone_ev = ev;
          goto next_event;
        }
      } break;
      case SDL_MOUSEWHEEL:
      {
        //DEBUGLOG("gframe: %d, ev_frame: %d\n", gframe, ev_frame);
        /*if (scrolled_this_gframe)
        {
          goto next_event;
        }
        scrolled_this_gframe = 300;*/
      } break;
      case SDL_KEYDOWN:
      {
        int scancode = ev.key.keysym.sym;
        int mod = ev.key.keysym.mod;
        switch (scancode)
        {
          case SDLK_ESCAPE:
            ::player->spc_write_dsp(dsp_reg::koff, 0xff);
          break;
          case SDLK_q:
            if ( MODONLY(mod, CMD_CTRL_KEY) )
            {
              SDL_Event quit_ev;
              quit_ev.type = SDL_QUIT;
              SDL_PushEvent(&quit_ev);
            }
          break;
          case SDLK_LEFT:
            if ( MODONLY(mod, KMOD_SHIFT | CMD_CTRL_KEY) )
              mousecursors->prev();
          break;
          case SDLK_RIGHT:
            if ( MODONLY(mod, KMOD_SHIFT | CMD_CTRL_KEY) )
              mousecursors->next();
          break;
					case SDLK_RETURN:
          {
            if (ev.key.repeat != 0) // Do not process key held down repeat events for this key
              break;
            bool repeat_pattern = false;
            bool startFromPlayhead = false;
						if (Text_Edit_Rect::cur_editing_ter != NULL)
							break;

            if (MODONLY(mod, KMOD_ALT))
            {
              alt_return_was_held = true;
              /* mark copies of the patseqpanel->currow and pateditpanel->currow for later restoration */
              psp_currow_stash  = main_window.patseqpanel.currow;
              pep_currow_stash = main_window.pateditpanel.currow;
              psp_rows_scrolled_stash  = main_window.patseqpanel.rows_scrolled;
              pep_rows_scrolled_stash = main_window.pateditpanel.rows_scrolled;
              startFromPlayhead = true;
            }
            else
            {
              if (mod & CMD_CTRL_KEY)
                startFromPlayhead = true;
              if (mod & KMOD_SHIFT)
                repeat_pattern = true;
            }

						playback = !playback;
						if (playback)
            {
							render_to_apu(repeat_pattern, startFromPlayhead);
              // prevent user from decreasing pattern length, etc
              SET_PLAYBACK_BUTTONS(false);
            }
						else
            {
							::player->fade_out(true);
              // pause taken care of in sound_stop userevent called from fadeout thread
              // Re-enable the pattern length decrement button
              SET_PLAYBACK_BUTTONS(true);
            }
          }
					break;
					case SDLK_BACKQUOTE:
          {
            if (mod & KMOD_SHIFT) /* Switch between logical scaling and "raw" scaling */
            {
              render->logical = !render->logical;
              if (render->logical)
              {
                SDL_RenderSetLogicalSize(render->sdlRenderer, render->w, render->h);
                /* No need to use raw scaling factors because the app is fed pre-scaled
                coordinates (eg. mouse coords) */
                render->sx = render->sy = 1.0;
              }
              else
              {
                int w,h;
                // Remove Logical Scaling
                SDL_RenderSetLogicalSize(render->sdlRenderer, 0, 0);
                // Restore "raw" scaling factors
                SDL_GetWindowSize(render->sdlWindow, &w, &h);
                render->sx = (float)(w) / (float)(render->w);
                render->sy = (float)(h) / (float)(render->h);
              }
            }
            else /* Toggle between SDL stock Scaling filters */
            {
              static uint8_t filter_type = 0;
              char filter_str[2];

              // update the filter index
              filter_type = filter_type == 2 ? 0 : filter_type + 1;
              // create the proper string from the index
              filter_str[0] = '0' + filter_type;
              filter_str[1] = 0;
              // set the filter
              SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, filter_str);
            }

            /* The new scaling filter will only apply to textures created after the filter was set,
            so we must destroy the current main screen texture and screen surface and recreate them.
            The code below was copied and adjusted from main.cpp */
            SDL_DestroyTexture(::render->sdlTexture);
            SDL_FreeSurface(::render->screen);
            ::render->screen = SDL_CreateRGBSurface(0, ::render->w, ::render->h, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
            SDL_SetColorKey(::render->screen, SDL_TRUE, 0);
            SDL_SetSurfaceBlendMode(::render->screen, SDL_BLENDMODE_BLEND);

            ::render->sdlTexture = SDL_CreateTexture(::render->sdlRenderer,
                                                    SDL_PIXELFORMAT_ARGB8888,
                                                    SDL_TEXTUREACCESS_STREAMING,
                                                    ::render->w, ::render->h);

            SDL_SetTextureBlendMode(::render->sdlTexture, SDL_BLENDMODE_BLEND);

            if (::render->screen == NULL || render->sdlTexture == NULL)
            {
              DEBUGLOG("I couldn't allocate a screen or Texture : %s\n", SDL_GetError());
              return -1;
            }
          }
					break;
          default:
            //DEBUGLOG("scancode = %08x\n", scancode);
          break;
        }
      } break;
      case SDL_KEYUP:
      {
        int scancode = ev.key.keysym.sym;
        int mod = ev.key.keysym.mod;
        switch (scancode)
        {
          case SDLK_RETURN:
            if(alt_return_was_held)
            {
              alt_return_was_held = false;
              playback = false;
              ::player->fade_out(true);
              // pause taken care of in sound_stop userevent called from fadeout thread
              // Re-enable the pattern length decrement button
              SET_PLAYBACK_BUTTONS(true);
              //SDL_FlushEvent(SDL_USEREVENT);
              main_window.patseqpanel.currow = psp_currow_stash;
              main_window.pateditpanel.currow = pep_currow_stash;
              main_window.patseqpanel.rows_scrolled = psp_rows_scrolled_stash;
              main_window.pateditpanel.rows_scrolled = pep_rows_scrolled_stash;
              //DEBUGLOG("set psp->currow to %d, pep->currow to %d\n",
                //patseqpanel_currow_stash, pateditpanel_currow_stash);
            }
          break;
        }
      }
      break;
      default:break;
    }

    mousecursors->handle_event(ev);

    if (menu_bar.receive_event(ev))
      continue;
 
    if (sub_window_experience)
    {
      sub_window_experience->receive_event(orig_ev);
    }
    else cur_exp->receive_event(ev);
next_event:
    ev_frame++;
  }

  /* If we have copied an mbu event to mbu_postpone_ev, then push it to
   * the event queue for next graphical frame to pick up */
  if (mbu_postpone_ev.type == SDL_MOUSEBUTTONUP)
    SDL_PushEvent(&mbu_postpone_ev);
}

void Tracker::inc_patlen()
{
	uint8_t *len = &song.patseq.patterns[
	    song.patseq.sequence[ main_window.patseqpanel.currow ] ].p.len;

	if (*len < MAX_PATTERN_LEN)
  {
		*len += 1;
    song.changed = true;
  }
}

void Tracker::dec_patlen()
{
	uint8_t *len = &song.patseq.patterns[
	    song.patseq.sequence[ main_window.patseqpanel.currow ] ].p.len;

	if (*len > MIN_PATTERN_LEN)
	{
		*len -= 1;
		auto *currow = &main_window.pateditpanel.currow;
		/* If we've decremented past the currow, keep the currow on the last
		 * row */
		*currow = *currow >= *len ? *len - 1 : *currow;
		/* Also, if we have rows_scrolled, and
		 * (rows_scrolled + VISIBLE_ROWS) > *len, decrement rows_scrolled */
		auto *rows_scrolled = &main_window.pateditpanel.rows_scrolled;
		if (*rows_scrolled && (*rows_scrolled + main_window.pateditpanel.VISIBLE_ROWS) > *len)
			*rows_scrolled -= 1;

    song.changed = true;
	}
}

// SNES APU timer 0 and 1 frequency rate in seconds
#define TIMER01_FREQS 0.000125
int Tracker::calcTicks()
{
  /* Convert BPM to ticks */
  // Ticks = 60 / ( BPM * 4 * Spd * freqS )
  double ticks = 60.0 / ( (double)song.settings.bpm * 4.0 * (double)song.settings.spd * TIMER01_FREQS );
  int ticksi = (int) floor(ticks + 0.5);
  if (ticksi == 256)
    ticksi = 0; // max timer setting is 0
  else if (ticksi > 256)
  {
    DEBUGLOG("Ticks value too high\n");
    ticksi = 255;
  }

  return ticksi;
}

void Tracker::renderCurrentInstrument()
{
  DEBUGLOG("renderCurrentInstrument(); instr_render: %d, playback: %d\n",
    instr_render, playback);

  ::player->pause(0, false, false);
  ::player->start_track(0);
  // SPC player fade to virtually never end (24 hours -> ms)
  ::player->emu()->set_fade(24 * 60 * 60 * 1000);
  // note, this does indeed fit into 32-bit even with the samplerate calcs
  // done from set_fade()

  // This is absolutely crucial for the tracker to sync properly with the
  // APU emu!!! v v v  Otherwise the emu runs too fast ahead of the audio
  ::player->ignore_silence();
  /* BPM AND SPD */
  /* Quick thoughts on Timer : We could add a checkmark to use the high
   * frequency timer. Also could have a mode where you specify ticks and
   * see the actual BPM */
  
  apuram->ticks = calcTicks();
  apuram->spd = song.settings.spd;
  /* END BPM AND SPD */

  // Find the position in SPC RAM after driver code
  uint16_t freeram_i = SPCDRIVER_CODESTART + SPCDRIVER_CODESIZE;


  // Only render the current instrument

  /* PlayInstrument */
  auto instr_num = main_window.instrpanel.currow;
  const Instrument *instr = &song.instruments[instr_num];

  if (song.samples[instr->srcn].brr == NULL)
    return; // forget it..

  //DEBUGLOG("instr->srcn = %d\n", instr->srcn);
  /* Another strategy would be to position the DIR at the base of the
   * offset rather than push it up further. Would need to check how many
   * DIR entries are needed if there's room or not */
  uint16_t dir_i, dspdir_i;
  dir_i = freeram_i + ((freeram_i % 0x100) ? (0x100 - (freeram_i % 0x100)) : 0);
  dspdir_i = dir_i / 0x100;

  uint16_t instrtable_i = dir_i + ( (instr->srcn + 1) * 0x4);
  apuram->instrtable_ptr = instrtable_i ;

  /* We have got to load these samples in first, so the DIR table knows
   * where the samples are */
  /* DIR is specified in multiples of 0x100. So if we're shy of that, we
   * need to move it up. I think a smarter program would mark that unused
   * area as free for something */

  /* DIR can be at max 0x400 bytes in size, but any unused space in DIR
   * can be used for other data */
  // Write the sample and loop information to the DIR. Then write the DSP
  // DIR value to DSP
  uint16_t cursample_i = dir_i + ( (instr->srcn + 1) * 0x4 ) + ( (instr_num + 1) * 2 );

  uint16_t *dir = (uint16_t *) &::IAPURAM[dir_i + (instr->srcn * 4)];
  *dir = cursample_i;
  *(dir+1) = cursample_i + song.samples[instr->srcn].rel_loop;

  size_t s=0;
  for (; s < song.samples[instr->srcn].brrsize; s++)
  {
    uint8_t *bytes = (uint8_t *)song.samples[instr->srcn].brr;
    ::IAPURAM[cursample_i + s] = bytes[s];
  }
  cursample_i += s;
  
  /* Could add a (SHA1) signature to Sample struct so that we can
   * identify repeat usage of the same sample and only load it once to
   * SPC RAM. For now, don't do this!! We're trying to get to first
   * working tracker status here! Plus, it's possible the user wants the
   * 2 identical samples to be treated individually (maybe their doing
   * something complicated) */
  uint16_t *it = (uint16_t *) &::IAPURAM[instrtable_i + (instr_num * 2)];
  *it = cursample_i;

  // Time to load instrument info
  ::IAPURAM[cursample_i++] = instr->vol;
  ::IAPURAM[cursample_i++] = instr->finetune;
  ::IAPURAM[cursample_i++] = instr->pan;
  ::IAPURAM[cursample_i++] = instr->srcn;
  ::IAPURAM[cursample_i++] = instr->adsr.adsr1;
  ::IAPURAM[cursample_i++] = instr->adsr.adsr2;
  ::IAPURAM[cursample_i++] = (instr->echo ? INSTR_FLAG_ECHO : 0);
  ::IAPURAM[cursample_i++] = instr->semitone_offset;

  apuram->dspdir_i = dspdir_i;
  ::player->spc_write_dsp(dsp_reg::dir, dspdir_i);
  // INSTRUMENTS END

  // set flag whether to repeat the pattern, and also set the bit to skip
  // the echobuf clear
  apuram->extflags |= (1 << EXTFLAGS_SKIP_ECHOBUF_CLEAR);
  // PATTERN SEQUENCER END

  // SONG SETTINGS
  apuram->mvol_val = song.settings.mvol;
  apuram->evol_val = song.settings.evol;
  /* calculate ESA */
  /* The ESA will be, based on EDL, pushed all the way to the end of RAM,
   * so control bit 7 ($F1) must be reset to enable the RAM region of IPL
   * ROM. Note that the asm RAM clear routine can be executed even with
   * IPL active (write-only)*/
  // if EDL is 0, just stick the 4 bytes of echo buffer at $FF00
  apuram->esa_val = calcESAfromEDL(song.settings.edl);
  apuram->edl_val = song.settings.edl;
  apuram->efb_val = song.settings.efb;
  uint8_t *coeff = &apuram->c0_val;
  for (int i=0; i < 8; i++)
    coeff[i] = song.settings.fir[i];
  // SONG SETTINGS END

  instr_render = true;
}

void Tracker::render_to_apu(bool repeat_pattern/*=false*/, bool startFromPlayhead/*=false*/)
{
  DEBUGLOG("render_to_apu(); playback: %d\n", playback);
  ::player->pause(0, false, false);
	::player->start_track(0);
	// SPC player fade to virtually never end (24 hours -> ms)
	::player->emu()->set_fade(24 * 60 * 60 * 1000);
	// note, this does indeed fit into 32-bit even with the samplerate calcs
	// done from set_fade()

	// This is absolutely crucial for the tracker to sync properly with the
	// APU emu!!! v v v  Otherwise the emu runs too fast ahead of the audio
	::player->ignore_silence();
	/* BPM AND SPD */
  /* Quick thoughts on Timer : We could add a checkmark to use the high
   * frequency timer. Also could have a mode where you specify ticks and
   * see the actual BPM */
  
	apuram->ticks = calcTicks();
	apuram->spd = song.settings.spd;
	/* END BPM AND SPD */

  // Find the position in SPC RAM after driver code
	uint16_t freeram_i = SPCDRIVER_CODESTART + SPCDRIVER_CODESIZE;

  uint16_t numinstr = 0;
  apuRender.highest_instr = 0;
  /* TODO: Could optimize this into bitflags */
	uint8_t used_instr[NUM_INSTR];
  memset(used_instr, 0, sizeof(used_instr));

  // PATTERNS
  // First calculate the number of used patterns in the song. This is not
  // sequence length, but the number of unique patterns. With that length
  // calculated, we can allocate the amount of RAM necessary for the
  // PatternLUT (detailed below comments).
  uint8_t num_usedpatterns = 0;
  apuRender.highest_pattern = 0;
  uint16_t patternlut_i = freeram_i; // position pattern data at start of free ram
  uint16_t patternlut_size;

  apuram->patterntable_ptr = patternlut_i;

  /* Track the number of used patterns and the highest pattern number */
  for (int p=0; p < MAX_PATTERNS; p++)
  {
    PatternMeta *pm = &song.patseq.patterns[p];
    if (pm->used == 0)
      continue;
    num_usedpatterns++;
    apuRender.highest_pattern = p;
  }
  // Note we don't optimize the pattern LUT by consolidating unused
  // patterns. This was chosen for debugging purposes for Version 1
  patternlut_size = (apuRender.highest_pattern + 1) * 2; // WORD sized address table
  /* OK I'm thinking about how aside from the pattern data itself, how
   * that pattern data will be accessed. We can store a Pattern lookup
   * table that has the 16-bit addresses of each pattern in ascending
   * order. The pattern sequencer table can simply store the pattern
   * number, and that number is converted to an index into the lookup
   * table to get the address of the pattern. That address will be stored
   * into a DP address for indirect access to pattern data. There may be
   * another DP pointer for accessing Row data row by row.*/
  uint16_t *patlut = (uint16_t *) &::IAPURAM[patternlut_i];
  uint16_t pat_i = patternlut_i + patternlut_size; // index into RAM for actual pattern data
  for (int p=0; p <= apuRender.highest_pattern; p++)
  {
    PatternMeta *pm = &song.patseq.patterns[p];
    if (pm->used == 0)
    {
      if (p <= apuRender.highest_pattern)
        *(patlut++) = 0xd00d;
      continue;
    }

    /* Here we need to iterate through the Pattern to form the compressed
     * version. For now, let's try to follow the XM version without RLE
     * compression */
    *(patlut++) = pat_i; // put the address of this pattern into the patlut
    Pattern *pattern = &pm->p;
    /* I just thought of another problem here. How do we access the
     * individual tracks of a pattern if they are going to be in a
     * compressed form (non-fixed length), other than storing the offset
     * of each track? Since they will be stored sequentially, We could iterate to the end of each pattern when the pattern is first loaded to be played by the driver (in snes_driver code), and store those offsets into RAM. I
     * suppose? that would save (8*2) bytes Per pattern! that's a lot!
     */
    // BEFORE THIS LOOP IS WHERE WE CAN WRITE A PATTERN HEADER FOR SNES
    // When we do that, make sure to update pat_i or use a new variable
    // for the next codeblock
    ::IAPURAM[pat_i++] = pattern->len; // TODO write code that handles len of 0 == 256
    /* The format of Pattern data is as follows:
     *
     * Len - byte - the length of the pattern in rows (same value for each
     * track)
     *
     * If every column is filled, then next bytes are the note, instr, vol,
     * fx, and fxparam for that row. Otherwise, the CBIT is set and a new set
     * of rules apply, based on XM file format.
     *
     * When the CBIT is on, see the CBITS definition for which bits indicate
     * what the next bytes are. The new addition are the RLE bits; defined in
     * the code path below
     * */
    for (int t=0; t < MAX_TRACKS; t++)
    {
      uint8_t last_instr = 0;
      for (int tr=0; tr < pattern->len; tr++)
      {
        int ttrr;
        PatternRow *pr = &pattern->trackrows[t][tr];
        uint8_t cbyte = 0, rlebyte;
        /* Lookahead: how many empty rows from this one until the next
         * filled row? If there's only 1 empty row, use RLE_ONLY1 */
#define PATROW_EMPTY(pr) ( pr->note == 0 && pr->instr == 0 && pr->vol == 0 && pr->fx == 0 && pr->fxparam == 0 )
        for (ttrr=tr+1; ttrr < pattern->len; ttrr++)
        {
          PatternRow *row = &pattern->trackrows[t][ttrr];
          if ( (!(PATROW_EMPTY(row))) || ttrr == (pattern->len - 1))
          {
            // we found a filled row or we made it to the end of pattern
            ttrr -= ( (PATROW_EMPTY(row)) ? 0 : 1);
            int num_empty = ttrr - tr;

            if (num_empty == 0)
              break;
            else if (num_empty == 1)
              cbyte |= ( 1<<CBIT_RLE_ONLY1 );
            else if (num_empty == 2)
              cbyte |= ( (1<<CBIT_RLE) | (1<<CBIT_RLE_ONLY1) );
            else
            {
              cbyte |= ( 1<<CBIT_RLE );
              rlebyte = num_empty;
            }

            break;
          }
        }

        // The very first row of a track has nothing? Then set the compression
        // bit
        if ( tr == 0 && (
              pr->note == NOTE_NONE &&
              pr->instr == 0 &&
              pr->vol == 0 && pr->fx == 0 && pr->fxparam == 0) )
        {
          cbyte |= 1<<CBIT;
        }
#define DIFF_LAST_INSTR (last_instr == 0 || last_instr != pr->instr)
        // Only if every element is filled are we NOT going to use a
        // special compression byte. so let's check if every element is
        // filled first.
        // Lit. "If not every element is filled"
        else if (! (
              pr->note != NOTE_NONE &&
              pr->instr != 0 &&
              pr->vol != 0 && pr->fx != 0 && pr->fxparam != 0) )
        {
          cbyte |=
            (pr->note ? ( 1<<CBIT_NOTE ) : 0) |
            (pr->instr && DIFF_LAST_INSTR ? ( 1<<CBIT_INSTR ) : 0) |
            (pr->vol ? ( 1<<CBIT_VOL ) : 0) |
            (pr->fx ? ( 1<<CBIT_FX ) : 0) |
            (pr->fxparam ? ( 1<<CBIT_FXPARAM ) : 0);
        }

        if (cbyte)
          ::IAPURAM[pat_i++] = ( 1<<CBIT ) | cbyte;
        /* we should now write the actual byte for any data that is
         * present */
        if (pr->note)
          ::IAPURAM[pat_i++] = pr->note - 1;
        if (pr->instr && DIFF_LAST_INSTR)
        {
          last_instr = pr->instr;
          // however, the "actual" instrument is 0-based in the driver
          ::IAPURAM[pat_i++] = pr->instr - 1;

          /* V2: Also update the running list of used instruments */
          //DEBUGLOG("SET USED INSTR: %d\n", pr->instr);
          if (used_instr[pr->instr - 1] == 0)
          {
            used_instr[pr->instr - 1] = 1;
            numinstr++;
            if (apuRender.highest_instr < (pr->instr - 1)) apuRender.highest_instr = pr->instr - 1;
          }
        }
        if (pr->vol)
          ::IAPURAM[pat_i++] = pr->vol;
        if (pr->fx)
          ::IAPURAM[pat_i++] = pr->fx;
        if (pr->fxparam)
          ::IAPURAM[pat_i++] = pr->fxparam;
        if ( (cbyte & (( 1<<CBIT_RLE ) | ( 1<<CBIT_RLE_ONLY1 )) ) == ( 1<<CBIT_RLE ) )
          ::IAPURAM[pat_i++] = rlebyte;

        tr = ttrr; // skip over empty rows
      }
    }
  }
  // PATTERNS END

  // PATTERN SEQUENCER START
  // set the start sequence index to the currently selected one in the
  // tracker (eg. play from current pattern)
  apuram->sequencer_i =  main_window.patseqpanel.currow;
  uint16_t patseq_i = pat_i;
  apuram->sequencer_ptr = patseq_i;
  for (int i=0; i < song.patseq.num_entries; i++)
    ::IAPURAM[patseq_i++] = song.patseq.sequence[i];
  ::IAPURAM[patseq_i++] = 0xff; // mark end of sequence
  // going to check in apu driver for a negative number to mark end


  /* Now that we know what instruments are used, let's check the samples
   * they reference to calculate the used_samples */
  /* TODO: Could optimize this into bitflags */
  uint16_t numsamples = 0;
  apuRender.highest_sample = 0;
  uint8_t used_samples[NUM_SAMPLES];
  memset(used_samples, 0, sizeof(used_samples));
  for (int i=0; i <= apuRender.highest_instr; i++)
  {
    if (used_instr[i])
    {
      auto srcn = song.instruments[i].srcn;
      numsamples++;
      if (apuRender.highest_sample < srcn)
        apuRender.highest_sample = srcn;
      used_samples[srcn] = 1;
    }
  }

  /* Another strategy would be to position the DIR at the base of the
   * offset rather than push it up further. Would need to check how many
   * DIR entries are needed if there's room or not */
  uint16_t dir_i, dspdir_i;
  dir_i = patseq_i + ((patseq_i % 0x100) ? (0x100 - (patseq_i % 0x100)) : 0);
  dspdir_i = dir_i / 0x100;

	uint16_t instrtable_i = dir_i + ( (apuRender.highest_sample + 1) * 0x4);
	//                             {applied size of DIR}  {INSTR TABLE SIZE}
	uint16_t sampletable_i = dir_i + ( (apuRender.highest_sample + 1) * 0x4) + ( (apuRender.highest_instr + 1) * 0x2);

	apuram->instrtable_ptr = instrtable_i;

	/* We have got to load these samples in first, so the DIR table knows
	 * where the samples are */
	/* DIR is specified in multiples of 0x100. So if we're shy of that, we
	 * need to move it up. I think a smarter program would mark that unused
	 * area as free for something */

	/* DIR can be at max 0x400 bytes in size, but any unused space in DIR
	 * can be used for other data */
	// Write the sample and loop information to the DIR. Then write the DSP
	// DIR value to DSP
	uint16_t cursample_i = sampletable_i;

	/* TODO: Currently, the samples are half-assed optimized. A cool idea I
	 * have to remap and optimize from blank sample entries is to, as these
	 * entries are encountered, add to some sort of mapping data structure
	 * so that instruments can be updated. The same could be done for
	 * instrument mappings, now that I think of it. but for now, let's not
	 * optimize  */
	for (int i=0; i <= apuRender.highest_sample; i++)
	{
		if (used_samples[i] == 0)
		{
			if (i <= apuRender.highest_sample)
			{
				// be neat and mark the unused unoptimized entries
				uint16_t *dir = (uint16_t *) &::IAPURAM[dir_i + (i * 4)];
				*dir = 0xdead;
				*(dir+1) = 0xbeef;
			}
			continue;
		}
		uint16_t *dir = (uint16_t *) &::IAPURAM[dir_i + (i * 4)];
		*dir = cursample_i;
		*(dir+1) = cursample_i + song.samples[i].rel_loop;

		size_t s=0;
		for (; s < song.samples[i].brrsize; s++)
		{
			uint8_t *bytes = (uint8_t *)song.samples[i].brr;
			::IAPURAM[cursample_i + s] = bytes[s];
		}
		cursample_i += s;
	}

	for (int i=0; i <= apuRender.highest_instr; i++)
	{
		Instrument *instr = &song.instruments[i];
		if (used_instr[i] == 0)
		{
			if (i <= apuRender.highest_instr)
			{
				// be neat and mark the unused unoptimized entries
				uint16_t *it = (uint16_t *) &::IAPURAM[instrtable_i + (i*2)];
				*it = 0xf00d; /* I'm feeding the $BANANA register. Anyone remember
				that one from the Yoshi Register doc? :D */
			}
			continue;
		}

		/* Could add a (SHA1) signature to Sample struct so that we can
		 * identify repeat usage of the same sample and only load it once to
		 * SPC RAM. For now, don't do this!! We're trying to get to first
		 * working tracker status here! Plus, it's possible the user wants the
		 * 2 identical samples to be treated individually (maybe their doing
		 * something complicated) */
		uint16_t *it = (uint16_t *) &::IAPURAM[instrtable_i + (i*2)];
		*it = cursample_i;

		// Time to load instrument info
		::IAPURAM[cursample_i++] = instr->vol;
    ::IAPURAM[cursample_i++] = instr->finetune;
		::IAPURAM[cursample_i++] = instr->pan;
		::IAPURAM[cursample_i++] = instr->srcn;
		::IAPURAM[cursample_i++] = instr->adsr.adsr1;
		::IAPURAM[cursample_i++] = instr->adsr.adsr2;
    ::IAPURAM[cursample_i++] = (instr->echo ? INSTR_FLAG_ECHO : 0);
		::IAPURAM[cursample_i++] = instr->semitone_offset;
	}
	apuram->dspdir_i = dspdir_i;
	::player->spc_write_dsp(dsp_reg::dir, dspdir_i);
	// INSTRUMENTS END

  // set flag whether to repeat the pattern, and also set the bit to skip
  // the echobuf clear
  apuram->extflags |= (repeat_pattern << EXTFLAGS_REPEATPATTERN) | (1 << EXTFLAGS_SKIP_ECHOBUF_CLEAR)
                    | (startFromPlayhead << EXTFLAGS_START_FROM_PLAYHEAD);

//  if (startFromPlayhead)
  // This is only actually used when startFromPlayhead is true
  apuram->startPatRow = main_window.pateditpanel.currow;
  //DEBUGLOG("startPatRow: %d\n", apuram->startPatRow);
	// PATTERN SEQUENCER END

  // SONG SETTINGS
  apuram->mvol_val = song.settings.mvol;
  apuram->evol_val = song.settings.evol;
  /* calculate ESA */
  /* The ESA will be, based on EDL, pushed all the way to the end of RAM,
   * so control bit 7 ($F1) must be reset to enable the RAM region of IPL
   * ROM. Note that the asm RAM clear routine can be executed even with
   * IPL active (write-only)*/
  // if EDL is 0, just stick the 4 bytes of echo buffer at $FF00
  apuram->esa_val = calcESAfromEDL(song.settings.edl);
  apuram->edl_val = song.settings.edl;
  apuram->efb_val = song.settings.efb;
  uint8_t *coeff = &apuram->c0_val;
  for (int i=0; i < 8; i++)
    coeff[i] = song.settings.fir[i];
  // SONG SETTINGS END
	
	// send the command to start the song
	player->spc_emu()->write_port(1, SPCCMD_PLAYSONG);
	/* This way for SPC Export. Won't have to send the play command to
	 * the APU port manually anymore */
	if (!playback)
	{
		apuram->sequencer_i = 0; // since this will probably be for SPC Export
    // play from beginning of sequence
		sound_stop();
	}

  DEBUGLOG("apuRender.highest_pattern: %d, apuRender.highest_instr: %d, apuRender.highest_sample: %d\n",
    apuRender.highest_pattern, apuRender.highest_instr, apuRender.highest_sample);
}

uint8_t calcESAfromEDL(uint8_t edl)
{
  return edl ? ( 0x10000 - (edl * 0x800) ) >> 8 : 0xff;
}

/*
Other ideas:

Project -- a project can consist of 1 or more songs. Songs glean from a
global bank of samples, instruments, and (if desired) patterns.

Mixer panel -- button like "Inst. Ed.", has slider gui elements to control
track volumes for the song, along with Main and Echo Volumes.

Interpolate parameter -- with tracker on a certain pattern/row, being able
to perform a special mouse click on allowed elements that will activate
interpolation, then -- moving to a forward pattern/row, FX elements will
be inserted that interpolate the start and end values of the selected
paramter (eg. echo volume)

FX Tracks -- there may be cases where this is not enough room for
inserting desired FX. In this case, why not have ability to add FX tracks?
Could do it like Renoise and just add fx columns under the desired track.

Support video game SFX - some games will want to play songs on tracks (eg.
7-8) and when sending the command to play a certain sound effect, that
track (if it has music) should mute the music for the time of the sfx
being played until it's finished.

Meta Instruments (SFX?) -
  can take up 1 or more tracks,
  selectable ability to run on a special global meta instrument timer.
	has priority over the physical tracks.
*/

void Tracker::reset()
{
  // stop the player incase it's playing
  playback = false;
  instr_render = false;
  ::player->fade_out(false); // immediate fade-out (no thread)
  ::player->pause(true, false, false);
  /* It was shown that the program would crash if a file was opened while
   * the tracker was playing; It was because there were remnant
   * User Events pertaining to the Tracker reporting mechanism from the
   * SPC driver. To bypass this issue, drain all UserEvents when we
   * perform a tracker reset (eg. when opening a file). */
  SDL_FlushEvent(SDL_USEREVENT);

	// Reset Important GUI Elements
	if (Text_Edit_Rect::cur_editing_ter)
		Text_Edit_Rect::stop_editing(Text_Edit_Rect::cur_editing_ter);

  song.reset();

	// Reset Panel currows
  main_window.pateditpanel.set_currow(0);
	main_window.patseqpanel.set_currow(0);
	main_window.instrpanel.set_currow(0);
	main_window.samplepanel.currow = 0;
	main_window.samplepanel.rows_scrolled = 0;

	// Reset Other GUI elements
  Voice_Control::unmute_all();
  /* enable all critical playback buttons that may have been disabled if we are in playback state */
  SET_PLAYBACK_BUTTONS(true);

	/* HACKS */
	/* Since the BPM and SPD widgets do not constantly poll (they normally
	 * only update graphically when manually altered via +/- buttons, we
	 * need to manually update it */
	main_window.bsawidget.updatebpm();
	main_window.bsawidget.updatespd();

  updateWindowTitle("New Song");
}

/* TODO: Add sanitization where necessary */
int Tracker::read_from_file(SDL_RWops *file)
{
  reset(); // reset tracker

  song.load(file);

	/* HACKS */
	/* Since the BPM and SPD widgets do not constantly poll (they normally
	 * only update graphically when manually altered via +/- buttons, we
	 * need to manually update it */
	main_window.bsawidget.updatebpm();
	main_window.bsawidget.updatespd();

  if (song.samples[song.instruments[main_window.instrpanel.currow].srcn].brr != NULL && !::tracker->playback)
    ::tracker->renderCurrentInstrument();
	return 0;
}

void Tracker::save_to_file(SDL_RWops *file)
{
  song.save(file);
}
/* Define the Packed Pattern Format.
 * For this, I am electing to use the XM packing scheme, with the addition
 * of an RLE for absences of 1 row, 2 rows, and > 2 rows up to 255, which.
 * covers the full range given that the first row is taken by first entry
 */

/* Notes:
 * When encoding WAV samples to BRR, you may want to Low pass filter
 * around 10-16 KHz. SNES is capable of max 16 KHz tones (based on 32KHz
 * sample rate), and even those are going to be worst quality of all tones
 * capable. So, to reduce noise in signal, consider trying that low pass
 * filter! */

/* For envelope FX, rules:
 * ok heres something i thought up:
 * fx name: N (noise)
 * params:    0: off
 *         1-fe: choose envelope (+on)
 *           ff: envelope on
 */

/* Inspired from RIFF headers, but simpler; To maintain an extendable file
 * format, different structures / elements need an ID byte to preclude the
 * data. There was some concern of whether the 255 ID constraint of 1 byte
 * would be exceeded, but Expansion Bytes can be set aside to extend the
 * available IDS. for Example, let's pretend the EXPANSION byte is 255.
 * Then when that byte is identified, code will understand the following
 * byte is an EXPANSION ID byte, which will enable another 255 IDS to be
 * utilized. AND, as if that isn't enough, more than 1 EXPANSION byte
 * could be present (eg. EXPANSION1, EXPANSION2, ...) */

/* Another cool idea; Sidechaining a voice from another voice. By using
 * some form of VxENVX or VxOUTX. Basically, the intensity of envelope or
 * sample amplitude, affects another specified voice or instrument's
 * volume value! But, sidechaining to volume is not the only possibility,
 * you could sidechain pitch or some other parameter. In fact, you could
 * sidechain not only from volume either, but from pitch or some other
 * parameter. I wonder what sonically attractive sidechaining
 * possibilities come to me?
 *
 * Sidechain
 * ~~~~~~~~~
 *
 * Intensity: 65%
 *
 * From:                         To:
 *   Voice: 0                      Voice: 1
 *   Instrument: Any               Instrument: Any
 *   Parameter: ADSR Envelope      Parameter: Volume
 * */

 /* TODO:
  * Command line arg to open STP file
  *
  * Instrument setting to enable echo
  *
  * song.settings to saved file.
  * song.settings in reset().
  * FIR presets in song.settings (Expanding Lists)
  *
  * Play instrument from pc keyboard (when not recording and when recording)
  * Instrument file format
  *
	* Muted track shadow
	* Retrigger Effect : It is SOOO ICONIC to SNES soundtracks
	* Note: when adding 3xy portamento command, make special 3FF mean legato
	*       Or maybe we need an SNES exclusive no KON effects, aside from
	*       the tracker envelopes...?? (no envelope KON retrigger)
	* Pan interpolation
	* Fine tune interpolation
	*
	* SUPER EXPERIMENTAL: Wavetable synthesizer that works by checking ENDX
	* at fast rate, and adjusting the loop addresses of samples after BRR
	* end blocks are detected.
	* Add KOFF keyboard button. Add KOFF characters design sdlfont.cpp
	* Add sample editor
	* add color to track headers that match STD track colors
	* Click and mute tracks from the track header
	* -> This will involve abstracting out the VoiceControl struct in
	*  shared/Voice_Control module. Move the common vars into static
	*  definitions. Take the debugger specific code into a
	*  debugger/DVoice_Control module. Create a tracker/TVoice_Control
	*  module. Make the tracker/Pattern.h TrackHeader rect a Clickable_Rect
	*  with a callback that takes the track number as argument. To solo when
	*  right clicked, see mute_solo_voice() for reference.
	*  -> includes darkening the entire track
	* add debugger window to snestracker
  * :: add FX to set/clear sample Loop bit (can get some cool sounds)
	* :: add FX to set/clear an inst envelope (vol, pan, pitch)
	* :: add noise FX support
	* noise envelope??
	* Add song-wide Echo support
	* Add fx echo support in some way
	* :: add FIR presets
	* Add  ability to mute / solo tracks from the track header.

	* add render_to_apu optimization that doesn't output an instrument byte
	* if there was a previous instrument byte in that pattern that was the
	* same exact instrument

  * Add TextEditRects to SpcExport window
  * Undo feature in PatternEditor
	* Envelope GUI element
	* Envelope storing / playback impl
	* Selections in PatternEditor
	* Instruments load/save.
	* Sample save
	* Specify sample loop point (from sample editor)
	*/

 /*
		Ideas
		~~~~~

		Global vol/pan/etc envelope banks, just like sample bank is global now.
		Envelope Load/Save/Zap buttons like for inst/samples.

		SMRPG changes the envelope settings during a KON, typically to migrate
		from an infinite sustain release to a sudden one that is slower than an
		actual KOFF.
		--> Have KOFF optionally (checkbox) trigger a "KOFF envelope"

		Panning envelope can be retriggered at KON or constant. (checkbox)

		Instrument Detune setting. (seperate from sample fine-tune)
*/

void SpcReport::report(Spc_Report::Type type, unsigned cmd, unsigned arg)
{
	//DEBUGLOG("SPC Tracker Report: Type: %d\n", type);
	switch (type)
	{
		case Spc_Report::TrackerCmd:
			//DEBUGLOG("SPC Tracker Report: Cmd: 0x%02x Arg: 0x%02x\n", cmd, arg);
			/* Ultimately, we'll be updating the PatternEditorPanel currow from
			 * here. We'll need a handle on it */
			switch (cmd)
			{
				case REPORT_TRACKER_INCROW:
				{
          //DEBUGLOG("REPORT::TRACKER::INCROW");
					SDL_Event uev;
					uev.type = SDL_USEREVENT;
					uev.user.code = UserEvents::report_tracker_incrow;
					SDL_PushEvent(&uev);
					break;
				}
				case REPORT_TRACKER_SETROW:
				{
          //DEBUGLOG("REPORT::TRACKER::SETROW");
					SDL_Event uev;
					uev.type = SDL_USEREVENT;
					uev.user.data1 = (void *)arg;
					uev.user.code = UserEvents::report_tracker_setrow;
					SDL_PushEvent(&uev);
					break;
				}
				case REPORT_TRACKER_SETPATTERN:
				{
					SDL_Event uev;
					uev.type = SDL_USEREVENT;
					uev.user.data1 = (void *)arg;
					uev.user.code = UserEvents::report_tracker_setpattern;
					SDL_PushEvent(&uev);
					break;
				}
				default:break;
			}
		break;

		default:
		break;
	}
}
