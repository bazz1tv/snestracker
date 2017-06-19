#include "gui/Context_Menu.h"
#include "Colors.h"
#include "sdlfont.h"

#include "globals.h"
#include "BaseD.h"




bool Context_Menu::is_activated()
{
  return is_active;
}

bool Context_Menu::toggle_activate()
{
  is_active = !is_active;
  if (!is_active) SDL_FillRect(BaseD::screen, &created_at, Colors::Interface::color[Colors::Interface::Type::bg]);
  return is_active;
}
void Context_Menu::activate()
{
  is_active = true;
}

void Context_Menu::deactivate()
{
  SDL_FillRect(BaseD::screen, &created_at, Colors::Interface::color[Colors::Interface::Type::bg]);
  is_active=false;
}

Context_Menu_Item::Context_Menu_Item(const char *str, bool is_visible, 
  int (*action)(void *)/*=NULL*/, void* data/*=NULL*/) : 
clickable_text(str, action, data), is_visible(is_visible)
{

}

bool Context_Menu::receive_event(SDL_Event &ev)
{
  if (!is_active)
    return false;

  switch (ev.type)
  {
    case SDL_KEYDOWN:
    {
      int scancode = ev.key.keysym.sym;
      switch (scancode)
      {
        case SDLK_RETURN:
        // act same as left mouse button click use a goto lol
          return do_thing();
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
          return do_thing();
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
  return false;

}

void Context_Menu::hover_highlight(bool yesno)
{
  should_highlight_hover = yesno;
}

void Context_Menu::highlight_currently_selected_item(bool yesno)
{
  should_highlight_currently_selected_item = yesno;
}

bool Context_Menu::do_thing(void *data/*=NULL*/)
{
  fprintf(stderr, "MEEP");
  Context_Menu_Item *p = highlighted_item;
  if (p != NULL)
  {
    currently_selected_item = p;
    if (p->clickable_text.action)
    {
      p->clickable_text.do_thing(data);
      is_active = false;
      return 1;
    }
  }
  is_active = false;
  return 0;
}
void Context_Menu::draw(SDL_Surface *screen)
{
  int i=0, drawn=0;
  //visible_items=0;
  //greatest_length=0;

  // draw background panel
  SDL_FillRect(screen, &created_at, Colors::Interface::color[Colors::Interface::Type::bg]);

  // find highlighted strip
    // this hsould go in its own function called from SDL_MOUSEMOTION event
  // any given item will be at created_at.x, created_at.y + 2 + (index*TILE_HEIGHT)

  // draw highlight'd strip
  Uint32 bg_color = Colors::Interface::color[Colors::Interface::Type::text_bg];
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
          if (should_highlight_hover)
          {
            SDL_Rect r = {created_at.x, created_at.y + drawn*(TILE_HEIGHT), created_at.w, TILE_HEIGHT};
            SDL_FillRect(screen, &r, Colors::magenta);
            bg_color = Colors::magenta;
          }
          highlighted_item = &items[i];
        }
      }
      if (should_highlight_currently_selected_item)
      {
        if (&items[i] == currently_selected_item )
        {
          SDL_Rect r = {created_at.x, created_at.y + drawn*(TILE_HEIGHT), created_at.w, TILE_HEIGHT};
          SDL_FillRect(screen, &r, Colors::magenta);
          bg_color = Colors::magenta;
        }
      }
      // draw this nigga
      sdlfont_drawString(screen, created_at.x+1, created_at.y + 1 + (drawn*TILE_HEIGHT), items[i].clickable_text.str.c_str(), Colors::white, bg_color, false);
      drawn++;
    }
    i++;
  }
}

Context_Menu::Context_Menu(Context_Menu_Item *array, bool isActive/*=false*/, bool isStatic/*=false*/) :
is_active(isActive), is_static(isStatic)
{
  items = array;
}
void Context_Menu::preload(int &x, int &y, bool use_cache)
{
  created_at.x = x;
  created_at.y = y;

  if (!use_cache)
  {
    int i=0;
    visible_items=0;
    greatest_length=0;
    while (items[i].clickable_text.str != "")
    {
      if (items[i].clickable_text.rect.w > greatest_length)
        greatest_length = items[i].clickable_text.rect.w;
      if (items[i].is_visible)
        visible_items++;
      i++;
    }
  }
  //if (is_static) visible_items--;
  created_at.w = greatest_length+TILE_WIDTH*3;
  created_at.h = visible_items*TILE_HEIGHT + TILE_HEIGHT/2;



  // readjust existing items?
  /*for ( auto &i : items ) 
  {
    std::cout << i.str << std::endl;
  }*/
}

void Context_Menu::push(char *str)
{
  //Clickable_Text item(str, created_at.x, created_at.y);
}
void Context_Menu::push(Clickable_Text &ref)
{
  //items.push_back(ref);
}