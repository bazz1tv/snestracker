#include "PanelCommon.h"
#include "mouse.h"

void panel_clear_all_rows(Text_Edit_Rect *ters, int num_rows, SDL_Surface *screen)
{
  SDL_Rect r = ters[0].rect;
  r.h += (ters[num_rows-1].rect.y - r.y) - 1;
  r.w = (ters[0].max_visible_chars * CHAR_WIDTH);
  SDL_FillRect(screen, &r, Colors::transparent);
  //fprintf(stderr, "x:%d y:%d w:%d h:%d\n", r.x, r.y, r.w, r.h);
}

void panel_clear_row(Text_Edit_Rect *ters, int row, SDL_Surface *screen)
{
  Text_Edit_Rect *ter = &ters[row];
  SDL_Rect r = ter->rect;
  r.w = (ter->max_visible_chars * CHAR_WIDTH);
  SDL_FillRect(screen, &r, Colors::transparent);
}

extern void conv_idx2ascii(int i, char **c);

int mousewheel_rows_event_handler(const SDL_Event &ev, int *rows_scrolled,
    int visible_rows, int total_rows, SDL_Rect *rect)
{
  switch (ev.type)
  {
    case SDL_MOUSEWHEEL:
      {
        if (Utility::coord_is_in_rect(mouse::x, mouse::y, rect))
        {
          /*int sign = (ev.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? 1 : -1;
            int val;
            if (ev.wheel.y > 0)
            val = 1 * sign;
            else if (ev.wheel.y < 0)
            val = -1 * sign;
            else // == 0
            val = 0;*/

          *rows_scrolled += (ev.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ?
            ev.wheel.y : -ev.wheel.y;
          //fprintf(stderr,"val = %d; ", val); //rows_scrolled);
          if (*rows_scrolled < 0)
            *rows_scrolled = 0;
          else if (*rows_scrolled >= (total_rows - visible_rows))
            *rows_scrolled = total_rows - visible_rows;

          return 1;
        }
      } break;
    default:break;
  }
  return 0;
}
