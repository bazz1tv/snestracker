#include "gui/Button.h"
#include "utility.h"
#include "DEBUGLOG.h"
#include "shared/mouse.h" // for mouse coords
#include "shared/sdl_userevents.h"

const unsigned int Button::holdtime = 200; //ms
const unsigned int Button::triggerfreq = 15; // ms
Uint32 Button::hold_tid=0; // timer ID

Button::Button(const char * str, int (*action)(void *data)/*=NULL*/,
  void *data/*=NULL*/, bool holdrepeat/*=false*/) :
    Clickable_Text(str, action, data),
    state(0), holdrepeat(holdrepeat)
{
   //outer = {rect.x + 2, rect.y + 4, rect.w, rect.h};
}

void Button::trigger_callback(void *b)
{
  Button *btn = (Button *)b;
  btn->check_mouse_and_execute(mouse::x, mouse::y, &btn->outer);
}

Uint32 Button::held_callback(Uint32 interval, void *b)
{
  //DEBUGLOG("held_callback; interval=%d, holdtime=%d, triggerfreq=%d; ", interval, holdtime, triggerfreq);
  if (interval == holdtime)
  {
    /* According to https://wiki.libsdl.org/SDL_AddTimer, an active timer's
    interval can be adjusted from the timer callback function's return value.
    This was confirmed on Linux and Windows 7, but on OSX 10.11 El Capitan,
    the timer appears to ignore the return value from the callback, and runs at
    a fixed interval - the one specified in the call to SDL_AddTimer().

    Since, in the case of the Button implementation, after the holdtime timer
    expires, the rate becomes fixed to triggerfreq, we can just remove the holdtime
    timer and then spawn a new timer with the triggerfreq interval set.

    However, as it stands, if a timer needs to be truly dynamically adjustable,
    it seems a bug report and fix is in order for Mac OSX El Capitan. This bug
    may apply to other OSX versions.

    UPDATE: this bug was only present in SDL 2.0.3, fixed likely by 2.0.4,
    and definitely fixed in the current 2.0.12.
    */
    //SDL_RemoveTimer(hold_tid);
    //hold_tid = SDL_AddTimer(triggerfreq, held_callback, b);
  }
  else if (interval == triggerfreq)
  {
    SDL_Event event;
    SDL_UserEvent userevent;

    /* In this example, our callback pushes a function
       into the queue, and causes our callback to be called again at the
       same interval: */

    userevent.type = SDL_USEREVENT;
    userevent.code = UserEvents::callback;
    userevent.data1 = (void *)&trigger_callback;
    userevent.data2 = b;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
  }
  else
  {
    DEBUGLOG("Oops, shouldn't be here!" FILE_LINE_FUNC_STR, FILE_LINE_FUNC_ARGS);
  }
  return triggerfreq;
}

void Button::check_event(const SDL_Event &ev)
{
  if (!enabled)
    return;
  if (state == 0 && ev.type == SDL_MOUSEBUTTONDOWN &&
      (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &outer)) &&
      ev.button.button == SDL_BUTTON_LEFT)
  {
    //DEBUGLOG("Button mousebuttondown; ");
    state = LEFTMOUSEBUTTON_HELD_IN_RECT;
    // start the timer
    if (holdrepeat)
    {
      //DEBUGLOG("holdrepeat; ");
      hold_tid = SDL_AddTimer(holdtime, held_callback, (void *)this);
    }
  }
  else if (state == LEFTMOUSEBUTTON_HELD_IN_RECT)
  {
    if (!Utility::coord_is_in_rect(mouse::x, mouse::y, &outer))
    {
      state = 0;
      // reset the timer
      if (holdrepeat)
        remove_hold_timer();
    }
    else if (ev.type == SDL_MOUSEBUTTONUP)
    {
      //DEBUGLOG("Button Mousebuttonup; ");
      check_mouse_and_execute(mouse::x, mouse::y, &outer);
      state = 0;

      if (holdrepeat)
        remove_hold_timer();
    }
  }
}

void Button::remove_hold_timer()
{
  /* ASSUMES holdrepeat flag check has already been done */
  if (hold_tid)
  {
    if (SDL_RemoveTimer(hold_tid) == SDL_FALSE)
    {
      DEBUGLOG("COULD NOT REMOVE TIMER IN BUTTON!\n");
    }
    hold_tid = 0;
  }
  else
    DEBUGLOG("Button::remove_hold_timer() called when no timer was active\n");
}

void Button::draw(SDL_Surface *screen, bool flipV/*=false*/)
{
  //DEBUGLOG("Button::draw; ");
  // redudant calculating outer rect every frame :(
  outer = {rect.x - 2, rect.y - 2, rect.w + 4, rect.h + 4};
  SDL_FillRect(screen, &outer,
      enabled ? Colors::Interface::color[Colors::Interface::button_bg + state] : Colors::nearblack);

  if (enabled)
    sdlfont_drawString(screen, state ? rect.x : rect.x + 1,
        state ? rect.y : rect.y + 2, str,
        Colors::transparent,
        Colors::Interface::color[Colors::Interface::button_bg + state],
        false, flipV);

  if (enabled)
    sdlfont_drawString(screen, state ? rect.x + 1 : rect.x,
      state ? rect.y + 2 : rect.y, str,
      Colors::Interface::color[Colors::Interface::button_fg + state],
      Colors::Interface::color[Colors::Interface::button_bg + state],
      false, flipV);
  else
    sdlfont_drawString(screen, rect.x,
      rect.y, str,
      Colors::gray,
      Colors::Interface::color[Colors::Interface::button_bg + state],
      false, flipV);
}
