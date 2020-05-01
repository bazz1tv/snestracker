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
  if (interval == holdtime)
  {
    // nothing needs to be done ?
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
  if (ev.type == SDL_MOUSEBUTTONDOWN &&
      (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &outer)) &&
      ev.button.button == SDL_BUTTON_LEFT)
  {
    //DEBUGLOG("Button mousebuttondown; ");
    state = LEFTMOUSEBUTTON_HELD_IN_RECT;
    // start the timer
    if (holdrepeat)
      hold_tid = SDL_AddTimer(holdtime, held_callback, (void *)this);
  }
  else if (state == LEFTMOUSEBUTTON_HELD_IN_RECT)
  {
    if (ev.type == SDL_MOUSEMOTION &&
        (ev.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)))
    {
      if (!Utility::coord_is_in_rect(ev.motion.x, ev.motion.y, &outer))
      {
        state = 0;
        // reset the timer
        if (holdrepeat)
          remove_hold_timer();
      }
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
    SDL_RemoveTimer(hold_tid);
    hold_tid = 0;
  }
  else
    DEBUGLOG("Button::remove_hold_timer() called when no timer was active\n");
}

void Button::draw(SDL_Surface *screen)
{
  //DEBUGLOG("Button::draw; ");
  // redudant calculating outer rect every frame :(
  outer = {rect.x - 2, rect.y - 2, rect.w + 4, rect.h + 4};
  SDL_FillRect(screen, &outer,
      Colors::Interface::color[Colors::Interface::button_bg + state]);

  sdlfont_drawString(screen, state ? rect.x : rect.x + 1,
      state ? rect.y : rect.y + 2, str,
      Colors::transparent,
      Colors::Interface::color[Colors::Interface::button_bg + state],
      false);

  sdlfont_drawString(screen, state ? rect.x + 1 : rect.x,
      state ? rect.y + 2 : rect.y, str,
      Colors::Interface::color[Colors::Interface::button_fg + state],
      Colors::Interface::color[Colors::Interface::button_bg + state],
      false);
}
