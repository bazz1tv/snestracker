#include "gui/Button.h"
#include "utility.h"
#include "DEBUGLOG.h"
Button::Button(const char * str, int (*action)(void *data)/*=NULL*/,
  void *data/*=NULL*/) :
    Clickable_Text(str, action, data)
{
  /*
  bg_color[0] = Colors::Interface::color[Colors::Interface::button_bg];
  bg_color[1] = Colors::Interface::color[Colors::Interface::button_pressed_bg];
  fg_color[0] = Colors::Interface::color[Colors::Interface::button_fg];
  fg_color[1] = Colors::Interface::color[Colors::Interface::button_pressed_fg];
  */
  //outer = {rect.x + 2, rect.y + 4, rect.w, rect.h};
}

void Button::check_event(const SDL_Event &ev)
{
  if (ev.type == SDL_MOUSEBUTTONDOWN &&
      (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &outer)) &&
      ev.button.button == SDL_BUTTON_LEFT)
  {
    //DEBUGLOG("Button mousebuttondown; ");
    state = LEFTMOUSEBUTTON_HELD_IN_RECT;
  }
  else if (state == LEFTMOUSEBUTTON_HELD_IN_RECT)
  {
    if (ev.type == SDL_MOUSEMOTION &&
        (ev.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
        !Utility::coord_is_in_rect(ev.motion.x, ev.motion.y, &outer))
    {
      state = 0;
    }
    else if (ev.type == SDL_MOUSEBUTTONUP)
    {
      //DEBUGLOG("Button Mousebuttonup; ");
      check_mouse_and_execute(ev.button.x, ev.button.y, &outer);
      state = 0;
    }
  }
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
