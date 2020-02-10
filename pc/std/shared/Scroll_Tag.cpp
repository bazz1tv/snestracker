#include "shared/Scroll_Tag.h"

void Scroll_Tag::compute(const char *str, Scroll_Tag *st, Uint32 extra_wait/*=1000*/)
{
  if (strlen(str) > Scroll_Tag::MAX_NO_SCROLL_LEN)
  {
    st->times_to_scroll = strlen(str) - Scroll_Tag::MAX_NO_SCROLL_LEN;
    st->cur_index=0;
    st->direction=+1;
    st->extra_wait=extra_wait;
    st->cur_ticks_compare = SDL_GetTicks();
    st->need_to_scroll=true;
    st->str = str;
  }
  else
  {
    st->need_to_scroll=false;
  }
}

void Scroll_Tag::scroll_draw()
{
  if (!need_to_scroll)
  { 
    return;
  }

  if (extra_wait)
  {
    if (!( (SDL_GetTicks() - cur_ticks_compare) >= extra_wait) )
      return;
    else extra_wait=0;
  }


  cur_index += direction;
  if (cur_index >= times_to_scroll)
  {
    extra_wait = 1000;
    cur_ticks_compare = SDL_GetTicks();
    direction = -direction;
  }
  else if (cur_index <= 0)
  {
    extra_wait = 1000; //ms
    cur_ticks_compare = SDL_GetTicks();
    direction = -direction;
  }
  sdlfont_drawString(::render->screen, rect.x, rect.y, &str[cur_index]);

}
