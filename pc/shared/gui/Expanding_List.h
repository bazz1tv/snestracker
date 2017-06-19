#pragma once

#include "gui/Context_Menu.h"

/*struct Expanding_List_Item
{
  Expanding_List_Item(const char *str, int (*action)(void *)=NULL, void *data=NULL);
  Clickable_Text clickable_text;
};*/


struct Expanding_List : public Context_Menu
{
  enum Event
  {
    EVENT_INACTIVE=0,
    EVENT_ACTIVE,
    EVENT_MENU,
  };
  void do_thing(void *data=NULL);
  void preload(int &x, int &y, bool use_cache=false);
  void update_current_item(int index);

  Expanding_List(Context_Menu_Item *array,  bool isStatic=false, bool isActive=false);
  bool check_left_click_activate(const int &x, const int &y, const Uint8 &button=0, const SDL_Event *ev=NULL);
  void draw(SDL_Surface *screen);
  int receive_event(SDL_Event &ev);
  Context_Menu_Item *currently_selected_item=NULL;
  int currently_selected_item_index=0, highlighted_item_index=0;
  SDL_Rect single_item_rect;
  // INSTR ADSR vs. File context
  //bool is_static=false;
};