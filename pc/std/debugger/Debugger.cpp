#include "Debugger.h"
#include <getopt.h>
#include "utility.h"
#include "Screen.h"
#include "sdl_userevents.h"
#include "gui/Cursors.h"

#define L_FLAG 0
#define R_FLAG 1

Debugger::Debugger(int &argc, char **argv) :
main_window(argc,argv)
{
  BaseD::main_window = &main_window;
  BaseD::dsp_window = &dsp_window;
  BaseD::instr_window = &instr_window;
  BaseD::menu_bar = &menu_bar;
  BaseD::options_window = &options_window;
  BaseD::spc_export_window = &spc_export_window;
  BaseD::cursors = &cursors;
  exp = &main_window;

  //window_map.insert( std::pair<Uint32, Window*>())
  int i=0;
  window_map[i++] = &options_window;
  window_map[i++] = &spc_export_window;
  window_map[i] = NULL;

  
  // Get current display mode of all displays.
  //for(i = 0; i < SDL_GetNumVideoDisplays(); ++i){

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
}


void Debugger::run()
{
  //SDL_CaptureMouse(SDL_TRUE);
  // gotta call this once to initialize important stuffz
  main_window.check_paths_and_reload();
  main_window.one_time_draw();

  exp->draw();
  SDL_ShowWindow(::render->sdlWindow);

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
    
    handle_events();
    SDL_Delay( 1000 / 100 );
    
  }
  
  sub_window_experience = NULL;
  for (int i=0; i < NUM_WINDOWS; i++)
  {
    //window_map[i]->hide();
    // DO THIS because otherwise the hidden windows will blink
    // quickly on exit
    //window_map[i]->~Window();
  }

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
            for (int i=0; i < NUM_WINDOWS; i++)
            {
              if (ev.window.windowID == window_map[i]->windowID)
              {
                if (window_map[i]->oktoshow)
                {
                  DEBUGLOG("Window_map %d gained experience. :D\n", i);
                  sub_window_experience = (Experience *)window_map[i];
                  //window_map[i]->raise();
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
                  //SDL_RaiseWindow(::render->::render->sdlWindow);
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
    if (sub_window_experience)
    {
      sub_window_experience->receive_event(ev);
    }
    else exp->receive_event(ev);
  }
}




