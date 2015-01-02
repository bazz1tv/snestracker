#include "sdl_dblclick.h"

namespace dblclick
{ 
  SDL_Event trigger_event;
  // Holds event details at time-of-double-click, since a new USER_EVENT
  // is sent, the USER_EVENT itself can't hold the mouse events details..

  Uint8 numclicks=0;
  SDL_TimerID timerid;
  
  // Timer Callback.. Resets double click possibility
  Uint32 dblclicktimer(Uint32 interval, void *param)
  {
    SDL_Event event2;

    event2.type = SDL_USEREVENT;
    event2.user.code = UserEvents::mouse_reset;
    event2.user.data1 = 0;
    event2.user.data2 = 0;
    SDL_PushEvent(&event2);
    return 0;
  }

  void init()
  {
    SDL_Init(SDL_INIT_TIMER);
  }

  
  // You must call this from your Main SDL EVENT LOOP
  void check_event(SDL_Event *ev)
  {
    if (ev->type == SDL_MOUSEBUTTONDOWN)
    {
      //fprintf (stderr, "0, numclicks = %d\n", numclicks);
      if (ev->button.button == SDL_BUTTON_LEFT)
      {
        dblclick::numclicks++;
        if (dblclick::numclicks == 2)
        {
          SDL_RemoveTimer(dblclick::timerid);
          dblclick::numclicks = 0;
          
          trigger_event = *ev;
          SDL_Event event;
          event.type = SDL_USEREVENT;
          event.user.code = UserEvents::mouse_react;
          event.user.data1 = &trigger_event;
          event.user.data2 = 0;
          SDL_PushEvent(&event);
        }
        else 
        {
          dblclick::timerid = SDL_AddTimer(DBLCLICK_TIMEWINDOW, &dblclick::dblclicktimer, NULL);
        }
      }
    }
    else if (ev->type == SDL_USEREVENT)
    {
      if (ev->user.code == UserEvents::mouse_react)
      {
        // could react here or could replicate in main SDL_USEREVENT loop..
        // dblclick::trigger_event may be referenced here for details of the double-click event.
        // which is useful for things like mouse coordinates etc.
        // alternatively the trigger_event is passed as a void* pointer to ev->user.data1
      }
      else if (ev->user.code == UserEvents::mouse_reset)
      {
        dblclick::numclicks = 0;
      }
    }
  }

}