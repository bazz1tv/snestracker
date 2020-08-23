#include "Debugger.h"
#include <getopt.h>
#include "utility.h"
#include "Screen.h"
#include "sdl_userevents.h"

#include "SdlNfd.h"

#define L_FLAG 0
#define R_FLAG 1

Debugger::Debugger(int &argc, char **argv) :
main_window(argc,argv)
{
  BaseD::main_window = &main_window;
  BaseD::dsp_window = &dsp_window;
  BaseD::instr_window = &instr_window;
  BaseD::menu_bar = &menu_bar;
  ::options_window = &options_window;
  ::spc_export_window = &spc_export_window;
  exp = &main_window;

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

  int should_be_zero = SDL_GetCurrentDisplayMode(0, &monitor_display_mode);

    if(should_be_zero != 0)
      // In case of error...
      SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

    else
      // On success, print the current display mode.
      SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, 
        monitor_display_mode.w, monitor_display_mode.h, monitor_display_mode.refresh_rate);

  //}
}

Debugger::~Debugger()
{
  DEBUGLOG("~Debugger");
  // Belongs to BaseD
  if (rsn_spc_paths)
  {
    for (unsigned int i = 0; i < num_rsn_spc_paths; i++ )
    {
      SDL_free(rsn_spc_paths[i]);
    }

    SDL_free(rsn_spc_paths);
    rsn_spc_paths = NULL;
    num_rsn_spc_paths=0;
  }
}

/*static int fillbuff(void *p)
{
  sample_t sbuf[4096*2];
  int num = 512 * 2 * 2;
  SDL_AudioDeviceID id = ::audio->devices.id;

  SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

  while (!BaseD::quitting)
  {
    if (!::player->is_paused() && SDL_GetQueuedAudioSize(id) < 24000)
    {
      ::player->fill_buffer(::player, sbuf, num);
      SDL_QueueAudio(id, sbuf, num*2);
    }
    SDL_Delay(15);
  }

}*/

void Debugger::run()
{
  // gotta call this once to initialize important stuffz
  main_window.check_paths_and_reload();
  main_window.one_time_draw();

  exp->draw();
  SDL_ShowWindow(::render->sdlWindow);

  /*SDL_Thread *thread;
  thread = SDL_CreateThread(&fillbuff, "AudiobuffThread", this);
  SDL_DetachThread(thread);*/

  // exp is changed from BaseD
  while (!quitting)
  {
    exp->run();
    exp->draw();

    if (sub_window_experience)
    {
      sub_window_experience->run();
      sub_window_experience->draw();
    }

    mousecursors.draw_aux();
    SDL_RenderPresent(::render->sdlRenderer);
    
    handle_events();
    SDL_Delay(50);
  }

  sub_window_experience = NULL;

  if (!player->is_paused() && player->track_started)
  {
    player->fade_out(false);
    player->pause(1, false, false);
  }
}

void Debugger::handle_events()
{
  SDL_Event ev;

  while (SDL_PollEvent(&ev))
  {
    //DEBUGLOG("GOTEVENT 0x%x -", ev.type);

    /* ignore events immediately after NFD dialog (#12) */
    if (SdlNfd::active())
      continue;

    switch(ev.type)
    {
      case SDL_QUIT:
        BaseD::quitting = true;
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
            /*if (w > monitor_display_mode.w)
              w = monitor_display_mode.w;
            if (h > monitor_display_mode.h)
              h = monitor_display_mode.h;*/

            int wd, hd;
            //if (w > SCREEN_WIDTH)
            //{
            wd = w - ::render->w;
            //}
            //else
            //{
             // wd = SCREEN_WIDTH - height;
            //}

            hd = h - ::render->h;

            if (abs(wd) < abs(hd)) wd = hd;
            else if (abs(wd) > abs(hd)) hd = wd;

            ::render->w += wd;
            ::render->h += hd;

            DEBUGLOG("\twindow resize: w = %d, h = %d\n", ::render->w, ::render->h);
            //DEBUGLOG("%d - %d = %d\n", ::render->h, monitor_display_mode.h, ::render->h - monitor_display_mode.h);

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
          /*case SDL_WINDOWEVENT_LEAVE:
          {
            DEBUGLOG("Window %d Lost mouse focus\n", ev.window.windowID);
          }
          break;
          case SDL_WINDOWEVENT_ENTER:
          {
            DEBUGLOG("Window %d Gained mouse focus\n", ev.window.windowID);
          }
          break;*/
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
            //SDL_GetMouseState(&mouse::x, &mouse::y);
            sub_window_experience = NULL;
            Window *window = Window::getWindow(ev.window.windowID);
            if (window && window->oktoshow)
            {
              DEBUGLOG("Window_map \"%s\" gained experience. :D\n", window->title.c_str());
              sub_window_experience = (Experience *)window;
              //window_map[i]->raise();
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
                //SDL_RaiseWindow(::render->::render->sdlWindow);
              }
              else
              {
                sub_window_experience = NULL;
                DEBUGLOG("Window_map \"%s\" gained experience. :D\n", window->title.c_str());
                sub_window_experience = (Experience *)window;
                window->raise();
              }
              break;
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
              if (window)
              {
                window->hide();
                // maybe right here, instead of raising the main window, we should
                //  have a history of displayed windows.. and the last one should be raised.

                sub_window_experience = NULL;
                //SDL_RaiseWindow(::render->::render->sdlWindow);
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
        // Shows directory of dropped file
        /*SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_INFORMATION,
            "File dropped on window",
            dropped_filedir,
            ::render->sdlWindow
        );*/
        BaseD::nfd.free_pathset();
        check_paths_and_reload(&dropped_filedir, 1, true);
        SDL_free(dropped_filedir);    // Free dropped_filedir memory
        SDL_RaiseWindow(::render->sdlWindow);
      } break;
      case SDL_USEREVENT:
      {
        if (ev.user.code == UserEvents::sound_stop)
        {
          sound_stop();
        }
      } break;
      case SDL_MOUSEMOTION:
      {
        mouse::prescaled_x = ev.motion.x;
        mouse::prescaled_y = ev.motion.y;
        mouse::x = ev.motion.x;
        mouse::y = ev.motion.y;
      } break;
      /*case SDL_MOUSEBUTTONDOWN:
        DEBUGLOG("MOUSE BUTTON DOWN -");
      break;
      case SDL_MOUSEBUTTONUP:
        DEBUGLOG("MOUSE BUTTON UP -");
      break;*/

      default:break;
    }

    mousecursors.handle_event(ev);

    if (sub_window_experience)
    {
      sub_window_experience->receive_event(ev);
    }
    else exp->receive_event(ev);
  }
}




