#include "Tracker.h"
#include <getopt.h>
#include "utility.h"
#include "Screen.h"
#include "sdl_userevents.h"
#include "kbd.h"
#include "apuram.h"

#define L_FLAG 0
#define R_FLAG 1

std::unordered_set<DrawRenderer *> Tracker::prerenders, Tracker::postrenders;

Tracker::Tracker(int &argc, char **argv) :
bpm(DEFAULT_BPM),
spd(DEFAULT_SPD),
main_window(argc,argv, this),
spcreport(this)
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
  SDL_SetWindowPosition(::options_window->sdlWindow, x, y);

  int i=0;
  window_map[i++] = ::options_window;
  window_map[i++] = ::spc_export_window;
  window_map[i] = NULL;
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

  update_fps(30);
}

Tracker::~Tracker()
{
  DEBUGLOG("~Tracker");
  delete mousecursors;
  mousecursors = NULL;
}

void Tracker::update_fps(int fps)
{
  this->fps = fps;
  // calc from framerate. could put this into dynamic function later
  frame_tick_duration = 1000 / fps; // how many ms per frame
}

void Tracker::run()
{
  // gotta call this once to initialize important stuffz
  main_window.one_time_draw();

  cur_exp->draw();
  SDL_ShowWindow(::render->sdlWindow);

  // exp is changed from BaseD
  while (!::quitting)
  {
    frame_tick_timeout = SDL_GetTicks() + frame_tick_duration;

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

    // If we finished the frame early, sleep until the next frame start
    Uint32 curticks = SDL_GetTicks();
    if (!SDL_TICKS_PASSED(curticks, frame_tick_timeout))
    {
      SDL_Delay(frame_tick_timeout - curticks);
      //DEBUGLOG("duration: %d, remaining: %d\n", frame_tick_duration,
                 //frame_tick_timeout - curticks);
    }
    //else DEBUGLOG("no time to wait :(\n");
  }

  sub_window_experience = NULL;

  if (!player->is_paused() && player->track_started)
  {
    player->fade_out(false);
    player->pause(1, false, false);
  }
}

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
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), frame_tick_timeout)
          && SDL_PollEvent(&ev))
  {
    switch(ev.type)
    {
      case SDL_QUIT:
        ::quitting = true;
        break;
      case SDL_WINDOWEVENT:
      {
        switch(ev.window.event)
        {
          case SDL_WINDOWEVENT_RESIZED:
          {
            break;
            int w = ev.window.data1;
            int h = ev.window.data2;

            DEBUGLOG("window resize: w = %d, h = %d\n", w, h);

            int wd, hd;
            wd = w - ::render->w;
            hd = h - ::render->h;

            if (abs(wd) < abs(hd)) wd = hd;
            else if (abs(wd) > abs(hd)) hd = wd;

            ::render->w += wd;
            ::render->h += hd;

            DEBUGLOG("\twindow resize: w = %d, h = %d\n", ::render->w, ::render->h);

            if (::render->w > monitor_display_mode.w)
            {
              int tmp_wd = ::render->w - monitor_display_mode.w;
              ::render->w -= tmp_wd;
              ::render->h -= tmp_wd;
              DEBUGLOG("\t\twindow resize: tmp_wd = %d, w = %d, h = %d\n", tmp_wd, ::render->w, ::render->h);
            }
            if (::render->h > monitor_display_mode.h)
            {
              int tmp_hd = ::render->h - monitor_display_mode.h;
              ::render->w -= tmp_hd;
              ::render->h -= tmp_hd;
              DEBUGLOG("\t\twindow resize: tmp_hd = %d, w = %d, h = %d\n", tmp_hd, ::render->w, ::render->h);

            }

            SDL_SetWindowSize(::render->sdlWindow, ::render->w, ::render->h);
            DEBUGLOG("\t\t\twindow resize: w = %d, h = %d\n", ::render->w, ::render->h);
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
            for (int i=0; i < NUM_WINDOWS; i++)
            {
              if (ev.window.windowID == window_map[i]->windowID)
              {
                if (window_map[i]->oktoshow)
                {
                  DEBUGLOG("Window_map %d gained experience. :D\n", i);
                  sub_window_experience = (Experience *)window_map[i];
                }
                break;
              }
            }
          }
          break;
          case SDL_WINDOWEVENT_SHOWN:
          {
            SDL_Log("Window %d shown", ev.window.windowID);
            for (int i=0; i < NUM_WINDOWS; i++)
            {
              if (ev.window.windowID == window_map[i]->windowID)
              {
                if (!window_map[i]->oktoshow)
                {
                  window_map[i]->hide();
                  /* maybe right here, instead of raising the main window, we should
                    have a history of displayed windows.. and the last one should be raised.
                  */
                  sub_window_experience = NULL;
                }
                else
                {
                  sub_window_experience = NULL;
                  DEBUGLOG("Window_map %d gained experience. :D\n", i);
                  sub_window_experience = (Experience *)window_map[i];
                  window_map[i]->raise();
                }
                break;
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
              for (int i=0; i < NUM_WINDOWS; i++)
              {
                if (ev.window.windowID == window_map[i]->windowID)
                {
                  window_map[i]->hide();
                  // maybe right here, instead of raising the main window, we should
                  //  have a history of displayed windows.. and the last one should be raised.

                  sub_window_experience = NULL;
                  //SDL_RaiseWindow(::render->::render->sdlWindow);
                  break;
                }
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
						::player->post_fadeout();
						break;
          case UserEvents::callback:
					{
            void (*p) (void*) = ev.user.data1;
            p(ev.user.data2);
					}
          break;
					case UserEvents::report_tracker_incrow:
						main_window.pateditpanel.inc_currow();
					break;
					case UserEvents::report_tracker_setrow:
						main_window.pateditpanel.set_currow((int)ev.user.data1);
					break;
					case UserEvents::report_tracker_setpattern:
						main_window.patseqpanel.set_currow((int)ev.user.data1);
					break;
        }
      } break;
      case SDL_MOUSEMOTION:
      {
        mouse::x = ev.motion.x;
        mouse::y = ev.motion.y;
      } break;
      case SDL_MOUSEBUTTONDOWN:
      {
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
          case SDLK_LEFT:
            if ((mod & KMOD_SHIFT) && (mod & KMOD_CTRL))
              mousecursors->prev();
          break;
          case SDLK_RIGHT:
            if ((mod & KMOD_SHIFT) && (mod & KMOD_CTRL))
              mousecursors->next();
          break;
					case SDLK_RETURN:
          {
            bool repeat_pattern = false;
						if (Text_Edit_Rect::cur_editing_ter != NULL)
							break;

            if (MODONLY(mod, KMOD_SHIFT))
              repeat_pattern = true;

						playback = !playback;
						if (playback)
							render_to_apu(repeat_pattern);
						else
							::player->fade_out(true);
          }
					break;
					case SDLK_BACKQUOTE:
						player->spc_emu()->write_port(1, SPCCMD_PLAYSONG);
					break;
        }
      } break;
      default:break;
    }

    mousecursors->handle_event(ev);

    if (menu_bar.receive_event(ev))
      continue;
 
    if (sub_window_experience)
    {
      sub_window_experience->receive_event(ev);
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

void Tracker::inc_bpm()
{
  if (bpm >= MAX_BPM)
    bpm = MAX_BPM;
  else bpm++;
}

void Tracker::dec_bpm()
{
  if (bpm <= MIN_BPM)
    bpm = MIN_BPM;
  else bpm--;
}

void Tracker::inc_spd()
{
  if (spd >= MAX_SPD)
    spd = MAX_SPD;
  else spd++;
}

void Tracker::dec_spd()
{
  if (spd <= MIN_SPD)
    spd = MIN_SPD;
  else spd--;
}

void Tracker::inc_patlen()
{
	uint8_t *len = &patseq.patterns[
	    patseq.sequence[ main_window.patseqpanel.currow ] ].p.len;

	if (*len < MAX_PATTERN_LEN)
		*len += 1;
}

void Tracker::dec_patlen()
{
	uint8_t *len = &patseq.patterns[
	    patseq.sequence[ main_window.patseqpanel.currow ] ].p.len;

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
	}
}

// SNES APU timer 0 and 1 frequency rate in seconds
#define TIMER01_FREQS 0.000125

void Tracker::render_to_apu(bool repeat_pattern/*=false*/)
{
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
  /* Convert BPM to ticks */
  // Ticks = 60 / ( BPM * 4 * Spd * freqS )
  double ticks = 60.0 / ( (double)bpm * 4.0 * (double)spd * TIMER01_FREQS );
  int ticksi = (int) floor(ticks + 0.5);
	if (ticksi == 256)
		ticksi = 0; // max timer setting is 0
	else if (ticksi > 256)
	{
		DEBUGLOG("Ticks value too high\n");
		ticksi = 255;
	}
	// Forcing using timer0 for ticks, but that's fine for now (or forever)
	apuram->ticks = ticksi;
	apuram->spd = spd;
	/* END BPM AND SPD */

	uint16_t freeram_i = SPCDRIVER_CODESTART + SPCDRIVER_CODESIZE;

	/* Another strategy would be to position the DIR at the base of the
	 * offset rather than push it up further. Would need to check how many
	 * DIR entries are needed if there's room or not */
	uint16_t dir_i, dspdir_i;
	dir_i = freeram_i + ((freeram_i % 0x100) ? (0x100 - (freeram_i % 0x100)) : 0);
	dspdir_i = dir_i / 0x100;

	uint16_t numsamples = 0;
	uint8_t highest_sample = 0;
	for (int i=0; i < NUM_SAMPLES; i++)
	{
		if (samples[i].brr == NULL)
			continue;
		numsamples++;
		highest_sample = i;
	}

	uint16_t numinstr = 0;
	uint8_t highest_instr = 0;
	for (int i=0; i < NUM_INSTR; i++)
	{
		Instrument *instr = &instruments[i];
		if (instr->used == 0)
			continue;

		numinstr++;
		highest_instr = i;
	}

	uint16_t instrtable_i = dir_i + ( (highest_sample + 1) * 0x4);
	//                             {applied size of DIR}  {INSTR TABLE SIZE}
	uint16_t sampletable_i = dir_i + ( (highest_sample + 1) * 0x4) + ( (highest_instr + 1) * 0x7);

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
	for (int i=0; i < NUM_SAMPLES; i++)
	{
		if (samples[i].brr == NULL)
		{
			if (i <= highest_sample)
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
		*(dir+1) = cursample_i + samples[i].rel_loop;
		// has a sample. this instrument is valid for export. However, perhaps
		// an even better exporter would also check if this instrument has
		// been used in any (in)active pattern data.
		size_t s=0;
		for (; s < samples[i].brrsize; s++)
		{
			uint8_t *bytes = (uint8_t *)samples[i].brr;
			::IAPURAM[cursample_i + s] = bytes[s];
		}
		cursample_i += s;
	}

	for (int i=0; i < NUM_INSTR; i++)
	{
		Instrument *instr = &instruments[i];
		if (instr->used == 0)
		{
			if (i <= highest_instr)
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
		::IAPURAM[cursample_i++] = instr->pan;
		::IAPURAM[cursample_i++] = instr->srcn;
		::IAPURAM[cursample_i++] = instr->adsr.adsr1;
		::IAPURAM[cursample_i++] = instr->adsr.adsr2;
		::IAPURAM[cursample_i++] = instr->semitone_offset;
		::IAPURAM[cursample_i++] = instr->finetune;
	}
	apuram->dspdir_i = dspdir_i;
	::player->spc_write_dsp(dsp_reg::dir, dspdir_i);
	// INSTRUMENTS END

	// PATTERNS
	// First calculate the number of used patterns in the song. This is not
	// sequence length, but the number of unique patterns. With that length
	// calculated, we can allocate the amount of RAM necessary for the
	// PatternLUT (detailed below comments).
	uint8_t num_usedpatterns = 0;
	uint16_t highest_pattern = 0;
	uint16_t patternlut_i = cursample_i, patternlut_size;

	apuram->patterntable_ptr = patternlut_i;

	for (int p=0; p < MAX_PATTERNS; p++)
	{
		PatternMeta *pm = &patseq.patterns[p];
		if (pm->used == 0)
			continue;
		num_usedpatterns++;
		highest_pattern = p;
	}
	patternlut_size = (highest_pattern + 1) * 2; // WORD sized address table
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
	for (int p=0; p < MAX_PATTERNS; p++)
	{
		PatternMeta *pm = &patseq.patterns[p];
		if (pm->used == 0)
		{
			if (p <= highest_pattern)
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
						// however, the "actual" instrument is 0-based
						::IAPURAM[pat_i++] = pr->instr - 1;
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
	for (int i=0; i < patseq.num_entries; i++)
		::IAPURAM[patseq_i++] = patseq.sequence[i];
	::IAPURAM[patseq_i++] = 0xff; // mark end of sequence
	// going to check in apu driver for a negative number to mark end

  // set flag whether to repeat the pattern
  apuram->extflags |= repeat_pattern << EXTFLAGS_REPEATPATTERN;
	// PATTERN SEQUENCER END
	
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
}

/* Song File Format
~~~~~~~~~~~~~~~~~~~
All integers spanning more than 1 byte are stored little-endian unless
otherwise noted.

"STSONG"   -- file magic header string. not null terminated
Song Title -- null terminated string
bpm/spd    -- 16-bit: (BPM << 6) | Spd
                (0-300)   (1-32?)
              ----BPM---- ==SPD==
              0 0000 0000 0000 00

#samples   -- 1 byte

What follows is sequential sample data of the follow format:
index      -- 1 byte
name       -- null-terminated string
TODO: Add rel_loop, finetune settings here
brrsize    -- size in bytes of following brr sample
brrsample

#instruments -- 1 byte
What follows are sequential instrument data of the following nature:
index      -- 1 byte
Name       -- null terminated string
vol        -- 1 byte
pan        -- "
srcn       -- "
adsr1      -- "
adsr2      -- "
semitone_offset -- "
finetune   -- "

#Patterns  -- 1 byte
What follows are sequential pattern data of the following format:
index      -- 1 byte
len        -- 1 byte: number of rows
Compressed Pattern Data for 8 tracks

Pattern Sequencer Entries -- an arbitrary number of byte sized pattern indices
that will make up the pattern sequencer. End is signaled by EOF
*/

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




/* WARNING: no bounds checking!? */
static void read_str_from_file(SDL_RWops *file, char *str_ptr)
{
	do {
		SDL_RWread(file, str_ptr, 1, 1);
	} while (*(str_ptr++) != 0);
}

void Tracker::reset()
{
	// Reset Important GUI Elements
	if (Text_Edit_Rect::cur_editing_ter)
		Text_Edit_Rect::stop_editing(Text_Edit_Rect::cur_editing_ter);

	// Reset Panel currows
	main_window.patseqpanel.set_currow(0);
	main_window.instrpanel.set_currow(0);
	main_window.samplepanel.currow = 0;
	main_window.samplepanel.rows_scrolled = 0;

	// Reset Song Title
	main_window.song_title_str[0] = 0;

	// Reset BPM/SPD
	bpm = DEFAULT_BPM;
	spd = DEFAULT_SPD;

	// Reset all Samples (and free memory!)
	for (int i=0; i < NUM_SAMPLES; i++)
	{
		Sample *s = &samples[i];
		if (s->brr)
			free(s->brr);
		*s = Sample();
	}

	// Reset all instruments
	for (int i=0; i < NUM_INSTR; i++)
	{
		Instrument *instr = &instruments[i];
		*instr = Instrument();
	}

	// Reset all patterns
	memset(patseq.patterns, 0, sizeof(patseq.patterns));
	for (int i=0; i < MAX_PATTERNS; i++)
	{
		patseq.patterns[i].p.len = DEFAULT_PATTERN_LEN;
	}
	// Reset Pat Sequencer
	// NOTE: This is done from the caller. new_file will properly load for
	// pattern 00, while load_file will.. load from file :)

	// Reset Other GUI elements

	/* HACKS */
	/* Since the BPM and SPD widgets do not constantly poll (they normally
	 * only update graphically when manually altered via +/- buttons, we
	 * need to manually update it */
	main_window.bsawidget.updatebpm();
	main_window.bsawidget.updatespd();
}

/* TODO: Add sanitization where necessary */
int Tracker::read_from_file(SDL_RWops *file)
{
	uint8_t buf[512];
	size_t rc;

	reset();

	rc = SDL_RWread(file, buf, sizeof("STSONG") - 1, 1);
	if (rc == 0)
	{
		DEBUGLOG("Could not read from file: %s\n", SDL_GetError());
		return -1;
	}
	buf[sizeof("STSONG") - 1] = 0;
	if (strcmp(buf, "STSONG") != 0)
	{
		DEBUGLOG("STSONG Magic not found in File header!\n");
	}

	// Read Song title until NULL is read in
	// corner case: no title == 0
	read_str_from_file(file, main_window.song_title_str);

	uint16_t bpmspd;
	rc = SDL_RWread(file, &bpmspd, 2, 1);
	if (rc == 0)
	{
		DEBUGLOG("Could not read from file: %s\n", SDL_GetError());
		return -1;
	}
	// Check for valid BPM/SPD
	bpm = bpmspd >> 6;
	if (bpm < MIN_BPM || bpm > MAX_BPM)
	{
		DEBUGLOG("Invalid BPM: %d. Setting to default %d\n", bpm, DEFAULT_BPM);
		bpm = DEFAULT_BPM;
	}
	spd = (uint8_t)(bpmspd & 0b111111);
	if (spd < MIN_SPD || spd > MAX_SPD)
	{
		DEBUGLOG("Invalid SPD: %d. Setting to default %d\n", spd, DEFAULT_SPD);
		bpm = DEFAULT_BPM;
	}

	// number of samples
	uint8_t numsamples;
	SDL_RWread(file, &numsamples, 1, 1);
	for (int i=0; i < numsamples; i++)
	{
		uint8_t idx;
		SDL_RWread(file, &idx, 1, 1);

		Sample *s = &samples[idx];
		read_str_from_file(file, s->name);

		SDL_RWread(file, &samples[idx].rel_loop, 2, 1);

		uint16_t brrsize;
		SDL_RWread(file, &brrsize, 2, 1);
		Brr *brr = (Brr *) malloc(brrsize);

		Sint64 nb_read_total = 0, nb_read = 1;
		char* buf = (char *)brr;
		while (nb_read_total < brrsize && nb_read != 0) {
			nb_read = SDL_RWread(file, buf, 1, (brrsize - nb_read_total));
			nb_read_total += nb_read;
			buf += nb_read;
		}

		if (nb_read_total != brrsize)
		{
			DEBUGLOG("Error Reading Sample %d!?\n", i);
			free(brr);
			return -1;
		}

		if (s->brr != NULL)
			free(s->brr);

		s->brr = brr;
		s->brrsize = brrsize;
	}

	uint8_t numinstr = 0;
	SDL_RWread(file, &numinstr, 1, 1);
	for (int i=0; i < numinstr; i++)
	{
		uint8_t idx;
		SDL_RWread(file, &idx, 1, 1);
		Instrument *instr = &instruments[idx];
		/* TODO: store/read the instr # */

		read_str_from_file(file, instr->name);

		/* TODO: Store/Read the instr used. Better yet, calculate it after the
		 * patterns have been loaded */

		// Time to load instrument info
		SDL_RWread(file, &instr->vol, 1, 1);
		SDL_RWread(file, &instr->pan, 1, 1);
		SDL_RWread(file, &instr->srcn, 1, 1);
		SDL_RWread(file, &instr->adsr.adsr1, 1, 1);
		SDL_RWread(file, &instr->adsr.adsr2, 1, 1);
		SDL_RWread(file, &instr->semitone_offset, 1, 1);
		SDL_RWread(file, &instr->finetune, 1, 1);
	}
	
	// PATTERNS
	//
	uint8_t numpatterns = 0;

	SDL_RWread(file, &numpatterns, 1, 1);
	for (int p=0; p < numpatterns; p++)
	{
		uint8_t idx;
		SDL_RWread(file, &idx, 1, 1);
		PatternMeta *pm = &patseq.patterns[idx];

		Pattern *pattern = &pm->p;
		SDL_RWread(file, &pattern->len, 1, 1);
		for (int t=0; t < MAX_TRACKS; t++)
		{
			uint8_t rlecounter = 0;
			uint8_t a;

			for (int tr=0; tr < pattern->len; tr++)
			{
				PatternRow *pr = &pattern->trackrows[t][tr];
				if (rlecounter)
					if (--rlecounter >= 0)
					{
						/* with the proper reset() written, we won't need to
						 * rewrite 0 values here */
						/*pr->note = NOTE_NONE;
						pr->instr = 0;
						pr->vol = 0;
						pr->fx = 0;
						pr->fxparam = 0;*/
						continue;
					}

				SDL_RWread(file, &a, 1, 1);
				if (a <= 0x7f) // positive
				{
					pr->note = a;
					SDL_RWread(file, &pr->instr, 1, 1);
					instruments[pr->instr - 1].used++; // update instrument metadata
					SDL_RWread(file, &pr->vol, 1, 1);
					SDL_RWread(file, &pr->fx, 1, 1);
					SDL_RWread(file, &pr->fxparam, 1, 1);
				}
				else
				{
					/* TODO: since reset() written, we won't need to
					 * rewrite 0 values here ( the else statements will be removed )*/
					if ( a & ( 1 << CBIT_NOTE ) )
						SDL_RWread(file, &pr->note, 1, 1);
					//else pr->note = NOTE_NONE;

					if ( a & ( 1 << CBIT_INSTR ) )
					{
						SDL_RWread(file, &pr->instr, 1, 1);
						instruments[pr->instr - 1].used++;
					}
					//else pr->instr = 0;

					if ( a & ( 1 << CBIT_VOL ) )
						SDL_RWread(file, &pr->vol, 1, 1);
					//else pr->vol = 0;

					if ( a & ( 1 << CBIT_FX ) )
						SDL_RWread(file, &pr->fx, 1, 1);
					//else pr->fx = 0;

					if ( a & ( 1 << CBIT_FXPARAM ) )
						SDL_RWread(file, &pr->fxparam, 1, 1);
					//else pr->fxparam = 0;

					if ( a & ( 1 << CBIT_RLE_ONLY1 ) )
					{
						rlecounter = ( a & ( 1 << CBIT_RLE ) ) ? 2 : 1;
					}
					else if ( a & ( 1 << CBIT_RLE ) )
						SDL_RWread(file, &rlecounter, 1, 1);
				}
			}
		}
	}
	// PAttern Sequencer
	patseq.num_entries = 0; // WARNING: could be dangerous
	for (int i=0; 1; i++)
	{
		rc = SDL_RWread(file, &patseq.sequence[i], 1, 1);
		if (rc == 0) // EOF
			break;

		patseq.patterns[patseq.sequence[i]].used++;
		patseq.num_entries++;
	}

	/* HACKS */
	/* Since the BPM and SPD widgets do not constantly poll (they normally
	 * only update graphically when manually altered via +/- buttons, we
	 * need to manually update it */
	main_window.bsawidget.updatebpm();
	main_window.bsawidget.updatespd();

	return 0;
}

void Tracker::save_to_file(SDL_RWops *file)
{
	SDL_RWwrite(file, "STSONG", sizeof("STSONG") - 1, 1);
	// write song title
	const char *songtitle = main_window.song_title.str;
	size_t len = strlen(songtitle);
	SDL_RWwrite(file, songtitle, len + 1, 1); // also write null byte

	uint16_t bpmspd = ((uint16_t)bpm << 6) | spd;
	SDL_RWwrite(file, &bpmspd, 2, 1);

	// number of samples
	uint8_t numsamples = 0;
	for (int i=0; i < NUM_SAMPLES; i++)
	{
		if (samples[i].brr == NULL)
			continue;
		numsamples++;
	}
	SDL_RWwrite(file, &numsamples, 1, 1);
	// for each sample, write number of bytes as uint16_t followed by the
	// bytes
	for (uint16_t i=0; i < NUM_SAMPLES; i++)
	{
		if (samples[i].brr == NULL)
			continue;

		uint16_t size = samples[i].brrsize;
		SDL_RWwrite(file, &i, 1, 1); // write sample index (only 1 byte)
		SDL_RWwrite(file, samples[i].name, strlen(samples[i].name) + 1, 1);
		SDL_RWwrite(file, &samples[i].rel_loop, 2, 1);
		SDL_RWwrite(file, &size, 2, 1);
		SDL_RWwrite(file, samples[i].brr, size, 1);
	}

	uint8_t numinstr = 0;
	for (uint16_t i=0; i < NUM_INSTR; i++)
	{
		Instrument *instr = &instruments[i];
		if (instr->used == 0)
			continue;

		numinstr++;
	}
	SDL_RWwrite(file, &numinstr, 1, 1);

	for (uint16_t i=0; i < NUM_INSTR; i++)
	{
		Instrument *instr = &instruments[i];
		if (instr->used == 0)
			continue;

		SDL_RWwrite(file, &i, 1, 1); // write instr index (only 1 byte)
		SDL_RWwrite(file, instr->name, strlen(instr->name) + 1, 1);
		// Time to load instrument info
		SDL_RWwrite(file, &instr->vol, 1, 1);
		SDL_RWwrite(file, &instr->pan, 1, 1);
		SDL_RWwrite(file, &instr->srcn, 1, 1);
		SDL_RWwrite(file, &instr->adsr.adsr1, 1, 1);
		SDL_RWwrite(file, &instr->adsr.adsr2, 1, 1);
		SDL_RWwrite(file, &instr->semitone_offset, 1, 1);
		SDL_RWwrite(file, &instr->finetune, 1, 1);
	}
	// INSTRUMENTS END

	// PATTERNS
	// First calculate the number of used patterns in the song. This is not
	// sequence length, but the number of unique patterns. With that length
	// calculated, we can allocate the amount of RAM necessary for the
	// PatternLUT (detailed below comments).
	uint8_t num_usedpatterns = 0;

	/* NOTE: Unlike the render_to_apu, which only exports patterns that are
	 * present in the sequencer, we will check here for ANY patterns that
	 * are not fully blank, whether or not they are present in the sequencer
	 * */
	for (uint8_t p=0; p < MAX_PATTERNS; p++)
	{
		PatternMeta *pm = &patseq.patterns[p];
		// NOTE: This logic chunk is duplicated in the next for loop. Wish I
		// would take time to optimize it BUT shouldn't prematurely do this.
		// So many more important factors to the tracker that need coding!
		if (pm->used == 0)
		{
			bool hasdata = false;
			// not in sequencer, but is there data?
			Pattern *pmp = &pm->p;
			for (int t=0; t < MAX_TRACKS; t++)
				for (int tr=0; tr < pmp->len; tr++)
				{
					PatternRow *pr = &pmp->trackrows[t][tr];
					if (pr->note || pr->instr || pr->vol || pr->fx || pr->fxparam)
					{
						hasdata = true;
						break;
					}
				}
			if (!hasdata)
				continue;
		}
		num_usedpatterns++;
	}
	SDL_RWwrite(file, &num_usedpatterns, 1, 1);
	for (uint8_t p=0; p < MAX_PATTERNS; p++)
	{
		PatternMeta *pm = &patseq.patterns[p];
		if (pm->used == 0)
		{
			bool hasdata = false;
			// not in sequencer, but is there data?
			Pattern *pmp = &pm->p;
			for (int t=0; t < MAX_TRACKS; t++)
				for (int tr=0; tr < pmp->len; tr++)
				{
					PatternRow *pr = &pmp->trackrows[t][tr];
					if (pr->note || pr->instr || pr->vol || pr->fx || pr->fxparam)
					{
						hasdata = true;
						break;
					}
				}
			if (!hasdata)
				continue;
		}

		SDL_RWwrite(file, &p, 1, 1); // write pattern index

		/*pm->used only says whether this pattern is in the pattern sequencer
		 * or not. It does not check whether the pattern has any data or not.
		 * That could be done the long-processing way by checking all rows
		 * manually for any note, instr, vol, fx, or fxparam entries -- or a
		 * quick processing map could be used, an array where each bit
		 * represents any of the above fields being present on a row (8 rows
		 * represented in a byte. it would quicken the check. TODO: For now,
		 * patterns that are not present in the sequencer will BE LOST!!! In
		 * the spirit of rapid prototyping, I don't care. Fix it later */

		Pattern *pattern = &pm->p;

		SDL_RWwrite(file, &pattern->len, 1, 1); // TODO write code that handles len of 0 == 256

		for (int t=0; t < MAX_TRACKS; t++)
		{
			for (int tr=0; tr < pattern->len; tr++)
			{
				int ttrr;
				PatternRow *pr = &pattern->trackrows[t][tr];
				uint8_t cbyte = 0, rlebyte;
				/* Lookahead: how many empty rows from this one until the next
				 * filled row? If there's only 1 empty row, use RLE_ONLY1 */
//#define PATROW_EMPTY(pr) ( pr->note == 0 && pr->instr == 0 && pr->vol == 0 && pr->fx == 0 && pr->fxparam == 0 )
				for (ttrr=tr+1; ttrr < pattern->len; ttrr++)
				{
					PatternRow *row = &pattern->trackrows[t][ttrr];
					if ( (!(PATROW_EMPTY(row))) || ttrr == (pattern->len - 1))
					{
						// we found a filled row or we made it to the end of pattern
						ttrr -= ( (PATROW_EMPTY(row)) ? 0 : 1);
						int num_empty = ttrr - tr; /* HACK: the +1 is actually to compensate for the way
																					apu driver code is handled, it's just to help the loop portion of the code stay clean
																					on APU side. but as an lsr value it should be disregarded. The actual
																					APU portion of code is anything using an rlecounter(s) (ReadPTRacks, QuickReadPTrack)*/
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

				if ( tr == 0 && (
							pr->note == NOTE_NONE &&
							pr->instr == 0 &&
							pr->vol == 0 && pr->fx == 0 && pr->fxparam == 0) )
				{
					cbyte |= 1<<CBIT;
				}
				// Only if every element is filled are we NOT going to use a
				// special compression byte. so let's check if every element is
				// filled first.
				else if (! (
							pr->note != NOTE_NONE &&
							pr->instr != 0 &&
							pr->vol != 0 && pr->fx != 0 && pr->fxparam != 0) )
				{
					cbyte |=
						(pr->note ? ( 1<<CBIT_NOTE ) : 0) |
						(pr->instr ? ( 1<<CBIT_INSTR ) : 0) |
						(pr->vol ? ( 1<<CBIT_VOL ) : 0) |
						(pr->fx ? ( 1<<CBIT_FX ) : 0) |
						(pr->fxparam ? ( 1<<CBIT_FXPARAM ) : 0);
				}

				if (cbyte)
				{
					cbyte |= ( 1<<CBIT );
					SDL_RWwrite(file, &cbyte, 1, 1);
				}
				/* we should now write the actual byte for any data that is
				 * present */
				if (pr->note)
					SDL_RWwrite(file, &pr->note, 1, 1);
				if (pr->instr)
					SDL_RWwrite(file, &pr->instr, 1, 1);
				if (pr->vol)
					SDL_RWwrite(file, &pr->vol, 1, 1);
				if (pr->fx)
					SDL_RWwrite(file, &pr->fx, 1, 1);
				if (pr->fxparam)
					SDL_RWwrite(file, &pr->fxparam, 1, 1);
				if ( (cbyte & (( 1<<CBIT_RLE ) | ( 1<<CBIT_RLE_ONLY1 )) ) == ( 1<<CBIT_RLE ) )
					SDL_RWwrite(file, &rlebyte, 1, 1);

				tr = ttrr; // skip over empty rows
			}
		}
	}
	// PATTERNS END

	// PATTERN SEQUENCER START
	for (int i=0; i < patseq.num_entries; i++)
		SDL_RWwrite(file, &patseq.sequence[i], 1, 1);
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
 /* TODO:
	* Rip BRR from the DIR table
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

SpcReport::SpcReport(Tracker *tracker) : tracker(tracker)
{
}

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
					SDL_Event uev;
					uev.type = SDL_USEREVENT;
					uev.user.code = UserEvents::report_tracker_incrow;
					SDL_PushEvent(&uev);
					break;
				}
				case REPORT_TRACKER_SETROW:
				{
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
