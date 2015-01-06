#include "gui/Context_Menu.h"
#include "Colors.h"
#include "sdlfont.h"

#include "globals.h"

Context_Menu_Item::Context_Menu_Item(const char *str, bool is_visible, int (*action)(void *)/*=NULL*/, void* data/*=NULL*/) : 
clickable_text(str, action, data), is_visible(is_visible)
{

}

void Context_Menu::do_thing(void *data/*=NULL*/)
{
  Context_Menu_Item *p = highlighted_item;
  if (p != NULL)
  {
    if (p->clickable_text.action)
    {
      p->clickable_text.do_thing();
    }
  }
  is_active = false;
}
void Context_Menu::draw(SDL_Surface *screen)
{
  int i=0, drawn=0;
  //visible_items=0;
  //greatest_length=0;

  // draw background panel
  SDL_FillRect(screen, &created_at, Colors::black);

  // find highlighted strip
    // this hsould go in its own function called from SDL_MOUSEMOTION event
  // any given item will be at created_at.x, created_at.y + 2 + (index*TILE_HEIGHT)

  // draw highlight'd strip

  highlighted_item = NULL;
  while (items[i].clickable_text.str != "")
  {
    if (items[i].is_visible)
    {
      if (mouse::x >= created_at.x && mouse::x < (created_at.x+greatest_length))
      {
        //fprintf(stderr,"DERP1");
        if (mouse::y >= (created_at.y + drawn*(TILE_HEIGHT)) && mouse::y < (created_at.y + drawn*TILE_HEIGHT + items[i].clickable_text.rect.h))
        {
          //fprintf(stderr,"DERP2");
          // draw the highlighter
          SDL_Rect r = {created_at.x, created_at.y + drawn*(TILE_HEIGHT), created_at.w, TILE_HEIGHT};
          SDL_FillRect(screen, &r, Colors::magenta);
          highlighted_item = &items[i];
        }
      }
      // draw this nigga
      sdlfont_drawString(screen, created_at.x+1, created_at.y + 1 + (drawn*TILE_HEIGHT), items[i].clickable_text.str.c_str(), Colors::white, false);
      drawn++;
    }
    i++;
  }
}

Context_Menu::Context_Menu(Context_Menu_Item *array)
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
  created_at.w = greatest_length+2;
  created_at.h = visible_items*TILE_HEIGHT + 2;



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