#include "gui/Expanding_List.h"
#include "globals.h"
#include "BaseD.h"
#include "utility.h"
//Expanding_List_Item::Expanding_List_Item(const char *str, 
//  int (*action)(void *)/*=NULL*/, void* data/*=NULL*/) : 
/*clickable_text(str, action, data)
{

}*/

void Expanding_List::update_current_item(int index)
{
  currently_selected_item = &items[index];
  currently_selected_item_index = index;
}

bool Expanding_List::check_left_click_activate(const int &x, const int &y)
{
  if (Utility::coord_is_in_rect(x, y, &single_item_rect))
  {
    activate();
  }
}

void Expanding_List::do_thing(void *data/*=NULL*/)
{
  Context_Menu::do_thing(data);
  if (highlighted_item)
  {
    fprintf(stderr, "DERP?!");
    currently_selected_item = highlighted_item;
    currently_selected_item_index = highlighted_item_index;
  }
}

int Expanding_List::receive_event(SDL_Event &ev)
{
  if (!is_active)
    return EVENT_INACTIVE;

  switch (ev.type)
  {
    case SDL_QUIT:
    if (!BaseD::g_cfg.nosound) {
      SDL_PauseAudio(1);
    }
    printf ("penis4\n");
    BaseD::quitting = true;
    break;

    case SDL_MOUSEMOTION:
    {
      mouse::x = ev.motion.x; mouse::y = ev.motion.y;
    } break;

    case SDL_KEYDOWN:
    {
      int scancode = ev.key.keysym.sym;
      switch (scancode)
      {
        case SDLK_RETURN:
        // act same as left mouse button click use a goto lol
          do_thing();
          return EVENT_MENU;
        break;

        case SDLK_ESCAPE:
          deactivate();
        break;
      }
    } break;

    case SDL_MOUSEBUTTONDOWN:
    {
      switch (ev.button.button)
      {
        case SDL_BUTTON_LEFT:
        {
          do_thing();
          return EVENT_MENU;
        }
        break;

        case SDL_BUTTON_RIGHT:
        break;

        default:break;
      }
    }
    break;
    default:break;
  }
  return EVENT_ACTIVE;
}

Expanding_List::Expanding_List(Context_Menu_Item *array, bool isActive/*=false*/) :
Context_Menu(array, isActive)
{
  highlighted_item = &items[0];
  currently_selected_item = &items[0];
}

void Expanding_List::preload(int &x, int &y, bool use_cache/*=false*/)
{
  Context_Menu::preload(x,y,use_cache);
  single_item_rect.x = created_at.x;
  single_item_rect.y = created_at.y;
  single_item_rect.w = created_at.w;
  single_item_rect.h = TILE_HEIGHT;
  created_at.y += TILE_HEIGHT;
  
}

void Expanding_List::draw(SDL_Surface *screen)
{
  int i=0, drawn=0;
  //visible_items=0;
  //greatest_length=0;

  // draw background panel
  SDL_FillRect(screen, &created_at, Colors::black);
  SDL_FillRect(screen, &single_item_rect, Colors::black);

  // find highlighted strip
    // this should go in its own function called from SDL_MOUSEMOTION event
  // any given item will be at created_at.x, created_at.y + 2 + (index*TILE_HEIGHT)

  // draw highlight'd strip

  if (is_active)
  {
    highlighted_item = NULL;
    while (items[i].clickable_text.str != "")
    {
      if (items[i].is_visible)
      {
        if (mouse::x >= created_at.x && mouse::x < (created_at.x+greatest_length))
        {
          //fprintf(stderr,"DERP1");
          if (mouse::y >= (created_at.y + drawn*(TILE_HEIGHT)) && mouse::y < (created_at.y + drawn*TILE_HEIGHT + TILE_HEIGHT))
          {
            //fprintf(stderr,"DERP2");
            // draw the highlighter
            SDL_Rect r = {created_at.x, created_at.y + (drawn)*(TILE_HEIGHT), created_at.w, TILE_HEIGHT};
            SDL_FillRect(screen, &r, Colors::magenta);
            highlighted_item = &items[i];
            highlighted_item_index = i;
          }
        }
        // draw this nigga
        //if (currently_selected_item != &items[i])
        //{
          sdlfont_drawString(screen, created_at.x+1, created_at.y + 1 + ((drawn)*TILE_HEIGHT), items[i].clickable_text.str.c_str(), Colors::white, false);
          drawn++;
        //}
      }
      i++;
    }
  }
  else //if (currently_selected_item != NULL)
  {
    sdlfont_drawString(screen, single_item_rect.x+1, single_item_rect.y + 1 + (0*TILE_HEIGHT), currently_selected_item->clickable_text.str.c_str(), Colors::white, false);
  }
  
}