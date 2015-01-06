#pragma once
#include "gui/Clickable_Text.h"
#include <vector>

struct Context_Menu_Item
{
  Context_Menu_Item(const char *str, bool is_active, int (*action)(void *)=NULL, void *data=NULL);
  Clickable_Text clickable_text;
  bool is_visible=false;
};

struct Context_Menu
{
  SDL_Rect created_at;  // the mouse coordinate when context menu was created
  Context_Menu(Context_Menu_Item *array);
  void preload(int &x, int &y, bool use_cache=false);
  void push(Clickable_Text &ref);
  void push(char *str);
  void draw(SDL_Surface *screen);
  void do_thing(void *data=NULL);
  //std::vector<Clickable_Text> items;
  Context_Menu_Item *items=NULL;
  SDL_Rect rect;
  bool is_active=false;
  int greatest_length=0, visible_items=0;
  Context_Menu_Item *highlighted_item=NULL;
};