/* 

SDL Double Click functionality
author : bazz

How-to: 
@Init: 
call dblclick::init();

in event loop, for example: 
while (SDL_PollEvent(&ev))
{
  dblclick::check_event(&ev);
  switch (ev.type)
  {
    case SDL_QUIT:
    ..
    .
  }
}

*/


#pragma once
#include "SDL.h"

#define DBLCLICK_TIMEWINDOW 800

namespace dblclick
{ 
  SDL_Event trigger_event;
  Uint8 numclicks=0;
  SDL_TimerID timerid;
  enum Event { react, reset };
  Uint32 dblclicktimer(Uint32 interval, void *param)
  {
    SDL_Event event2;

    event2.type = SDL_USEREVENT;
    event2.user.code = reset;
    event2.user.data1 = 0;
    event2.user.data2 = 0;
    SDL_PushEvent(&event2);
    return 0;
  }

  void init()
  {
    SDL_Init(SDL_INIT_TIMER);
    timerid = SDL_AddTimer(DBLCLICK_TIMEWINDOW, &dblclicktimer, NULL);
  }

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
          //fprintf (stderr, "asd1, numclicks = %d\n", numclicks);
          SDL_RemoveTimer(dblclick::timerid);
          dblclick::numclicks = 0;
          
          trigger_event = *ev;
          SDL_Event event;
          event.type = SDL_USEREVENT;
          event.user.code = dblclick::react;
          event.user.data1 = &trigger_event;
          event.user.data2 = 0;
          SDL_PushEvent(&event);
          //fprintf (stderr, "r = %d\n", r);
        }
        else 
        {
          dblclick::timerid = SDL_AddTimer(800, &dblclick::dblclicktimer, NULL);
        }
      }
    }
    else if (ev->type == SDL_USEREVENT)
      {
      if (ev->user.code == dblclick::react)
      {
        // could react here or could replicate in main SDL_USEREVENT loop..
        // dblclick::trigger_event may be referenced here for details of the double-click event.
        // which is useful for things like mouse coordinates etc.
        // alternatively the trigger_event is passed as a void* pointer to ev->user.data1
      }
      else if (ev->user.code == dblclick::reset)
      {
        //fprintf(stderr, ".3.");
        dblclick::numclicks = 0;
      }
    }
  }

}