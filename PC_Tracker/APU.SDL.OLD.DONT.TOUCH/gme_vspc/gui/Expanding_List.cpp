#include "gui/Expanding_List.h"
#include "globals.h"
//Expanding_List_Item::Expanding_List_Item(const char *str, 
//  int (*action)(void *)/*=NULL*/, void* data/*=NULL*/) : 
/*clickable_text(str, action, data)
{

}*/

void Expanding_List::do_thing(void *data/*=NULL*/)
{
  Context_Menu::do_thing(data);
  if (highlighted_item)
    currently_selected_item = highlighted_item;
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

bool Expanding_List::is_activated()
{
  return is_active;
}
void Expanding_List::activate()
{
  is_active = true;
}

void Expanding_List::deactivate()
{
  is_active=false;
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