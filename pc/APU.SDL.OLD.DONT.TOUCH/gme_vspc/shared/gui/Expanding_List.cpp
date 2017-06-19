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

bool Expanding_List::check_left_click_activate(const int &x, const int &y, const Uint8 &button, const SDL_Event *ev)
{
  SDL_Rect *r;
  r = &single_item_rect;  
  static Uint8 button_held=0;

  
  fprintf(stderr, "check_left_click_activate\n");
  if (Utility::coord_is_in_rect(x, y, r))
  {
    if (button == SDL_BUTTON_LEFT  && !button_held)
    {      
      if (ev) 
      {
        button_held = 1;
        fprintf(stderr, "button held");
      }
      return toggle_activate();
    }
    
    if (ev)
      if ( ev->type == SDL_MOUSEBUTTONUP) 
      {
        fprintf(stderr, "reset button\n");
        button_held = 0;
        //return is_active;
      }
      
    fprintf(stderr, "activate\n");
    activate();
    return true;
  }

  return false;
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
  SDL_FillRect(BaseD::screen, &created_at, Colors::Interface::color[Colors::Interface::Type::bg]);
  currently_selected_item = &items[0];
  highlighted_item = &items[0];
  highlighted_item_index=0;
  //SDL_FillRect(BaseD::screen, &single_item_rect, Colors::black);
}

int Expanding_List::receive_event(SDL_Event &ev)
{
  if (!is_active)
    return EVENT_INACTIVE;

  switch (ev.type)
  {
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
          return EVENT_ACTIVE;
        break;
      }
    } break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
      switch (ev.button.button)
      {
        case SDL_BUTTON_LEFT:
        {
          fprintf(stderr, "highlighted_item_index = %d\n", highlighted_item_index);
          if (highlighted_item_index == 0 && ev.type == SDL_MOUSEBUTTONUP) break;
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

Expanding_List::Expanding_List(Context_Menu_Item *array, bool isStatic, bool isActive/*=false*/) :
Context_Menu(array, isActive, isStatic)
{
  highlighted_item = &items[0];
  currently_selected_item = &items[0];
}

void Expanding_List::preload(int &x, int &y, bool use_cache/*=false*/)
{
  Context_Menu::preload(x,y,use_cache);
  single_item_rect.x = created_at.x;
  single_item_rect.y = created_at.y;
  if (is_static)
    single_item_rect.w = items[0].clickable_text.rect.w;
  else single_item_rect.w = created_at.w;
  
  single_item_rect.h = TILE_HEIGHT;
  if (!is_static) created_at.y += TILE_HEIGHT;
  
}

void Expanding_List::draw(SDL_Surface *screen)
{
  int i=0, drawn=0;
  if (is_static) i = 0;
  //visible_items=0;
  //greatest_length=0;

  // draw background panel
  if (is_active) SDL_FillRect(screen, &created_at, Colors::Interface::color[Colors::Interface::Type::bg]);  
  SDL_FillRect(screen, &single_item_rect, Colors::Interface::color[Colors::Interface::Type::bg]);

  // find highlighted strip
    // this should go in its own function called from SDL_MOUSEMOTION event
  // any given item will be at created_at.x, created_at.y + 2 + (index*TILE_HEIGHT)

  // draw highlight'd strip

  if (is_active)
  {
    if (is_static)
    {
      SDL_FillRect(screen, &single_item_rect, Colors::magenta);
    }
    //SDL_FillRect(screen, &created_at, Colors::black);
    //fprintf(stderr, "TTT");
    highlighted_item = NULL;
    while (items[i].clickable_text.str != "")
    {
      if (items[i].is_visible)
      {
        if (mouse::x >= created_at.x && mouse::x < (created_at.x+greatest_length))
        {
          //fprintf(stderr,"DERP1");
          if ( (items[i].clickable_text.str.c_str()[0] != '-') && ((is_static && i !=0) || !is_static) && mouse::y >= (created_at.y + drawn*(TILE_HEIGHT)) && mouse::y < (created_at.y + drawn*TILE_HEIGHT + TILE_HEIGHT))
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
          sdlfont_drawString(screen, created_at.x+1, created_at.y + 1 + ((drawn)*TILE_HEIGHT) /*+ (i > 0 ? TILE_HEIGHT:0)*/, 
            items[i].clickable_text.str.c_str(), Colors::Interface::color[Colors::Interface::Type::text_fg], 
            Colors::Interface::color[Colors::Interface::Type::text_bg], false);
          drawn++;
        //}
      }
      i++;
    }
  }
  else //if (currently_selected_item != NULL)
  {
    if (is_static)
    {
      sdlfont_drawString(screen, single_item_rect.x+1, single_item_rect.y + 1 + (0*TILE_HEIGHT), 
        items[0].clickable_text.str.c_str(), Colors::Interface::color[Colors::Interface::Type::text_fg], 
        Colors::Interface::color[Colors::Interface::Type::text_bg],false);
    }
    else
      sdlfont_drawString(screen, single_item_rect.x+1, single_item_rect.y + 1 + (0*TILE_HEIGHT), 
        currently_selected_item->clickable_text.str.c_str(), Colors::Interface::color[Colors::Interface::Type::text_fg],
        Colors::Interface::color[Colors::Interface::Type::text_bg], false);
  }
  
}