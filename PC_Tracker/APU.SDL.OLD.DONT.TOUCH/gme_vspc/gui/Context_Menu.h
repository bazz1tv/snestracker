#pragma once
#include "gui/Clickable_Text.h"
struct ContextMenu
{
  struct
  {
    int x,y;
  } created;  // the mouse coordinate when context menu was created
  ContextMenu();
  preload(int &x, int &y);
  void push(Clickable_Text &ref);
  vector<Clickable_Text> items;
};