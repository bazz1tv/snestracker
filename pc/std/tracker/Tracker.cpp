#include "Tracker.h"
#include <getopt.h>
#include "utility.h"
#include "Screen.h"
#include "sdl_userevents.h"

#define L_FLAG 0
#define R_FLAG 1

std::unordered_set<DrawRenderer *> Tracker::prerenders, Tracker::postrenders;

Tracker::Tracker(int &argc, char **argv) :
bpm(120),
spd(6),
main_window(argc,argv, this)
{
  //::main_window = &main_window;
  //::menu_bar = &menu_bar;
  
  ::options_window = &options_window;
  ::spc_export_window = &spc_export_window;
  cur_exp = &main_window;

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

  int should_be_zero = SDL_GetCurrentDisplayMode(0, &monitor_display_mode);

    if(should_be_zero != 0)
      // In case of error...
      SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

    else
      // On success, print the current display mode.
      SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, 
        monitor_display_mode.w, monitor_display_mode.h, monitor_display_mode.refresh_rate);

  mousecursors = new MouseCursors;
  ::mousecursors = mousecursors;
  mousecursors->set_cursor(CURSOR_MPAINT_WHITE_HAND);

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
            sound_stop();
            break;
          case UserEvents::callback:
            void (*p) (void*) = ev.user.data1;
            p(ev.user.data2);
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
  if (bpm >= 300)
    bpm = 300;
  else bpm++;
}

void Tracker::dec_bpm()
{
  if (bpm <= 32)
    bpm = 32;
  else bpm--;
}

void Tracker::inc_spd()
{
  if (spd >= 31)
    spd = 31;
  else spd++;
}

void Tracker::dec_spd()
{
  if (spd <= 1)
    spd = 1;
  else spd--;
}

#define TIMER01_FREQS 0.000125
void Tracker::render_to_apu()
{
  /* Quick thoughts on Timer : We could add a checkmark to use the high
   * frequency timer. Also could have a mode where you specify ticks and
   * see the actual BPM */
  /* Convert BPM to ticks */
  // Ticks = 60 / ( BPM * 4 * Spd * freqS )
  double ticks = 60.0 / ( (double)bpm * 4.0 * (double)spd * TIMER01_FREQS );
  int ticksi = (int) floor(ticks + 0.5);

  /* Save ticks to SPC RAM. Wait, how do we know where to put that?? We
   * need a way to synchronize the spc program RAM and our C program
   * knowledge of those RAM locations. There a couple ways it could be
   * done
   *
   * 1. Invent a simple definition that exports to both C and SPCasm
   * 2. Depend on the spc program being built first, then import its
   * symbol table into a C format header file.
   *
   * Then there's the matter of embedding the SPC driver directly into the
   * PC program or not. It seems I should not, because that might provide
   * an opportunity to update the APU driver while not having to recompile
   * the tracker.
   *
   * There is also the notion of public facing RAM that pc tracker should
   * know about, and private driver RAM that it doesn't need to know
   * about. It would be nice if I could specify RAM sections as public and
   * then get a symbol file of all of those specific sections only. Of
   * course, if there are modifications to the public RAM table the pc
   * side program will need to be recompiled even if the driver is not
   * embedded, because it will no longer understand the RAM mapping. I
   * could load the mapping from outside, but it seems the mapping would
   * only change when a new feature needs to be implemented anyways.. */
}
