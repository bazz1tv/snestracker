#include "app.h"
#include "DEBUGLOG.h"
#include "gme/player/Music_Player.h"
#include "Tracker.h"
#include "Midi.h"
#include "File_System.h"
#include "App_Settings.h"
#include "Audio.h"

#include "mouse.h"
#include "gui/DialogBox.h"
#include "gui/Window.h"
#include "windows/Options_Window/Audio_Options.h"

static int create_midi(void *vapp)
{
  App *app = (App *)vapp;
  app->midi = new Midi;

  SDL_SemPost(app->sem);
  return 0;
}

App::App(int &argc, char **argv, int samplerate/*=44100*/)
{
  file_system = new File_System;
  audio = new Audio;
  player = new Music_Player;
  ::player = player;
  app_settings = new App_Settings(file_system);

///////////// OPEN AUDIO OUTPUT DEVICE ///////////////////////
  const char *outdev = app_settings->vars.audio_out_dev;
retry:
  blargg_err_t failed = player->init(samplerate, outdev);
  if (failed)
  {
    SDL_HideWindow(::render->sdlWindow);
    const char *buttonStr[3] = { "Retry", "Select Audio Device", "Quit" };
    char msg[512];
    snprintf(msg, 512 - 1,
      "There was a problem opening audio device \"%s\". "
      "Try selecting a different audio device or closing other audio apps first.",
      Audio::Devices::selected_audio_out_dev
    );
    int button = DialogBox::Custom("Couldn't Open Audio Device!",
      msg, 3, buttonStr, 0
    );

    if (button == 0)
    {
      DEBUGLOG("\tUser pushed RETRY\n");
      goto retry;
    }
    else if (button == 2)
    {
      DEBUGLOG("\tUser pushed QUIT\n");
      exit( EXIT_FAILURE );
    }
    else if (button == 1)
    {
      SDL_HideWindow(::render->sdlWindow);
      DEBUGLOG("\tUser pushed SELECT AUDIO DEVICE\n");
      /* Create an Audio_Options without a screen or renderer so we can customize the size
       * of the window according to the size of the context menu */
      Audio_Options ao(NULL, NULL, false);
      SDL_Rect r = {50, 25, 0, 0};
      // Generate coording and size data
      ao.preload( &r );
      const SDL_Rect *crect = &ao.context.menu->created_at;
      // Base the window size off that data
      int w, h;
      w = crect->x + crect->w + 50;
      h = crect->y + crect->h + 50;
      // create the window and assign it to the ao
      Window win(w, h, "Audio Devices");
      ao.screen = win.render.screen;
      ao.renderer = win.render.sdlRenderer;
      // Mark the currently selected item so we can track when it changes
      Context_Menu_Item *item = ao.context.menu->currently_selected_item;

      win.clear_screen();
      ao.one_time_draw();
      ao.draw();
      win.show();
      while (1)
      {
        ao.one_time_draw();
        ao.draw();
        win.update_screen();
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
          //DEBUGLOG("ev type = %d; ", ev.type);

          switch (ev.type)
          {
            case SDL_WINDOWEVENT:
                {
                  switch(ev.window.event)
                  {
                    // When the user clicks 'X' on the window
                    case SDL_WINDOWEVENT_CLOSE:
                      goto retry;
                    break;
                  }
                }
                break;
            case SDL_MOUSEMOTION:
              mouse::x = ev.motion.x;
              mouse::y = ev.motion.y;
            break;
              case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            /* Fix a weird bug where the 2nd time this window is opened,
            the first mouse-down registers as a mouse-up (tested only on Mac) */
              ev.type = SDL_MOUSEBUTTONDOWN;
              mouse::x = ev.button.x;
              mouse::y = ev.button.y;
            break;
            default:break;
          }
          ao.receive_event(ev);
          if (item != ao.context.menu->currently_selected_item)
          {
            item = ao.context.menu->currently_selected_item;
            outdev = item->clickable_text.str;
            /* Make sure to give the visual cue that the user selected their item */
            ao.one_time_draw();
            ao.draw();
            win.update_screen();
            SDL_PumpEvents(); /* Must call this so Windows target updates the screen highlighted row*/
            SDL_Delay(300);
            goto retry;
          }
        }
        SDL_Delay(20);
      }
    }
    else
    {
      DEBUGLOG("\tUNKNOWN BUTTON VALUE %d, forcing to RETRY\n", button);
      goto retry;
    }
  }
///////// END OPEN AUDIO OUTPUT DEVICE //////////
  tracker = new Tracker(argc, argv);
	::tracker = tracker;

  sem = SDL_CreateSemaphore(0);
  midithread = SDL_CreateThread(create_midi, "create_midi", this);
  SDL_DetachThread(midithread);
}

void App::run()
{
  tracker->run();

  // We're finished (quitting), now we may have to wait for the slow Midi thread
  // to finish up. (slow on Mac sometimes)
  int alpha = 255; // set this to 0 and uncomment below lines to do a fadeout
  bool thread_finished = false;

  DEBUGLOG("Waiting on Midi Thread (it could take several seconds)");
  while (!thread_finished || alpha != 255)
  {
    if (!thread_finished)
      thread_finished = !(SDL_SemWaitTimeout(sem, 15) == SDL_MUTEX_TIMEDOUT);
    /*SDL_SetRenderDrawBlendMode(::render->sdlRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, 0);
    SDL_RenderClear(::render->sdlRenderer);
    SDL_RenderCopy(::render->sdlRenderer, ::render->sdlTexture, NULL, NULL);

    SDL_SetRenderDrawColor(::render->sdlRenderer, 0, 0, 0, alpha);
    SDL_RenderFillRect(::render->sdlRenderer, NULL);

    SDL_RenderPresent(::render->sdlRenderer);*/
    // Fade out the screen
    DEBUGLOG(".");
    alpha += 4;
    if (alpha >= 255)
      alpha = 255;

    if (thread_finished)
      SDL_Delay(15);
  }
}

App::~App()
{
  DEBUGLOG("~App;");
  delete tracker;

  delete player;

  delete app_settings;
  delete midi;
  delete audio;

  delete file_system;
}

