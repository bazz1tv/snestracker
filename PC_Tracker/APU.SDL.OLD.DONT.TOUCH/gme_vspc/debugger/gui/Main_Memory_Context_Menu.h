#pragma once
#include "gui/Context_Menu.h"
#include "BaseD.h"

struct Main_Memory_Context_Menu : public Context_Menu, public BaseD
{
  Main_Memory_Context_Menu();
  void push(Clickable_Text &ref);
  void preload(int &x, int &y, uint16_t &address);
};

