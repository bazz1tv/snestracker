#include "gui/Checkbox.h"
#include "utility.h"
#include "DEBUGLOG.h"
#include "shared/mouse.h" // for mouse coords
#include "shared/sdl_userevents.h"

Checkbox::Checkbox(bool *state/*=NULL*/, int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) :
    Clickable_Rect(action, data),
    state(state), almost_clicked(0)
{
  rect.w = 9;
  rect.h = 9;
}

void Checkbox::check_event(const SDL_Event &ev)
{
  if (!enabled)
    return;
  if (ev.type == SDL_MOUSEBUTTONDOWN &&
      (Utility::coord_is_in_rect(ev.button.x, ev.button.y, &rect)) &&
      ev.button.button == SDL_BUTTON_LEFT)
  {
    //DEBUGLOG("Checkbox mouseCheckboxdown; ");
    //DEBUGLOG("Checkbox MouseCheckboxup; ");
    //if (almost_clicked)
    //{
      //DEBUGLOG("Checkbox check/execute\n");
      if (state != NULL)
      {
        //DEBUGLOG("WE'rE HERE\n");
        *state = !*state;
        //DEBUGLOG("state is %d\n", *state);
      }
      check_mouse_and_execute(mouse::x, mouse::y, &rect);
  }
}

void Checkbox::draw(SDL_Surface *screen)
{
  //DEBUGLOG("Checkbox::draw; ");
  // redudant calculating outer rect every frame :(
  SDL_Rect outer = {rect.x - 1, rect.y - 1, rect.w + (1*2), rect.h + (1*2)};
  SDL_Rect inner = {rect.x + 2, rect.y + 2, rect.w - (2*2), rect.h - (2*2)};

  Uint32 border[2] = {Colors::gray, Colors::white}, fill[2] = {Colors::transparent, Colors::yellow};
  //inner = {rect.x + 4, rect.y + 4, rect.w - 8, rect.h - 8};
  //Utility::set_render_color_rgba(::render->sdlRenderer, Colors::white);
  //SDL_RenderDrawRect(::render->sdlRenderer, &rect);
  SDL_FillRect(screen, &outer, border[enabled]);
  SDL_FillRect(screen, &rect, Colors::transparent);

  if (state != NULL && *state == true)
  {
    //DEBUGLOG("ON");
    SDL_FillRect(screen, &inner, fill[enabled]);
  }
  else if (state != NULL && *state == false)
  {
    //DEBUGLOG("OFF");
    SDL_FillRect(screen, &inner, Colors::nearblack);
  }
}
