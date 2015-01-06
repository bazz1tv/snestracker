#pragma once

struct ClickableText 
{
  ClickableText(std::string str)
  {
    rect.w = strlen(str)*CHAR_WIDTH; // could add padding
    rect.h = CHAR_HEIGHT;
  }
  ClickableText(std::string str, int x, int y)
  {
    rect.w = strlen(str)*CHAR_WIDTH; // could add padding
    rect.h = CHAR_HEIGHT;
    rect.x = x;
    rect.y = y;
  }
  SDL_Rect rect;
  std::string str;
  // maybz function pointer?
  // must be static class function, or reg. global function, or somethin.
  // it can be in namespace
  int (*action)(void* data);
};