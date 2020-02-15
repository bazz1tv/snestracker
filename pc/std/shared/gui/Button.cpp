#include "gui/Button.h"
#include "utility.h"
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
}

void Button::check_event(const SDL_Event &ev)
{
  if (ev.type == SDL_MOUSEBUTTONDOWN &&
      (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &rect)) &&
      ev.button.button == SDL_BUTTON_LEFT)
  {
    state = LEFTMOUSEBUTTON_HELD_IN_RECT;
  }
  else if (state == LEFTMOUSEBUTTON_HELD_IN_RECT)
  {
    if (ev.type == SDL_MOUSEMOTION &&
        (ev.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
        !Utility::coord_is_in_rect(ev.motion.x, ev.motion.y, &rect))
    {
      state = 0;
    }
    else if (ev.type == SDL_MOUSEBUTTONUP)
      check_mouse_and_execute(ev.button.x, ev.button.y);
  }
}

void Button::draw(SDL_Surface *screen)
{
  SDL_FillRect(screen, &rect,
      Colors::Interface::color[Colors::Interface::button_bg + state]);

  sdlfont_drawString(screen, rect.x + 2,
      state ? rect.y + 4 : rect.y + 2, str,
      Colors::Interface::color[Colors::Interface::button_fg + state],
      Colors::Interface::color[Colors::Interface::button_bg + state],
      false);
}
