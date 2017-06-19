#pragma once
#include "SDL.h"

#include <string>
#include <nfd.h>

namespace Utility
{
  const char* rstrstr(const char* haystack, const char* needle);
  Uint8 scancode_to_hex(int &scancode);
  bool coord_is_in_rect(int x, int y, SDL_Rect *r);
  int hexchar_to_int(char scancode);
  nfdresult_t get_file_write_handle(nfdchar_t **outPath, SDL_RWops **file, const char *filter_list=NULL);
  nfdresult_t get_file_read_handle(nfdchar_t **outPath, SDL_RWops **file, const char *filter_list=NULL);
  nfdresult_t get_file_read_path(nfdchar_t **outPath, const char *filter_list=NULL);
  SDL_bool file_is_valid( std::string filename );
  void clearsstream(std::stringstream &s);
  void draw_rect(SDL_Surface * dst, const SDL_Rect * rect, int depth, Uint32 color);

}