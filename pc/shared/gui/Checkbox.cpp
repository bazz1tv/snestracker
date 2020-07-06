#include "gui/Checkbox.h"
#include "utility.h"
#include "DEBUGLOG.h"
#include "shared/mouse.h" // for mouse coords
#include "shared/sdl_userevents.h"

Checkbox::Checkbox(bool *state/*=NULL*/, int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) :
    Clickable_Rect(action, data),
    state(state), almost_clicked(0)
{

}

void Checkbox::check_event(const SDL_Event &ev)
{
  //if (!enabled)
    //return;
  if (ev.type == SDL_MOUSEBUTTONDOWN &&
      (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &rect)) &&
      ev.button.button == SDL_BUTTON_LEFT)
  {
    DEBUGLOG("Checkbox mouseCheckboxdown; ");
    almost_clicked = 1;
  }
  else if (state && *state == ON)
  {
    if (!Utility::coord_is_in_rect(mouse::x, mouse::y, &rect))
    {
      almost_clicked = 0;
    }
    else if (ev.type == SDL_MOUSEBUTTONUP)
    {
      DEBUGLOG("Checkbox MouseCheckboxup; ");
      if (almost_clicked)
      {
        check_mouse_and_execute(mouse::x, mouse::y, &rect);
        if (state) *state = !*state;
        almost_clicked = 0;
      }
      else
      {
        DEBUGLOG("checkbox: mouse button released here but no mousebuttondown here first\n");
      }

    }
  }
}

void Checkbox::draw(SDL_Surface *screen)
{
  //DEBUGLOG("Checkbox::draw; ");
  // redudant calculating outer rect every frame :(
  inner = {rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4};
  Utility::set_render_color_rgba(::render->sdlRenderer, Colors::white);
  SDL_RenderDrawRect(::render->sdlRenderer, &rect);

  if (state && *state == ON)
  {
    SDL_FillRect(screen, &inner, Colors::gray);
  }
  else
  {
    SDL_FillRect(screen, &inner, Colors::transparent);
  }
}
