#pragma once

#include <nfd.h>
#include "SDL.h"

/* This is just meant to maintain a set of paths loaded via a multipath NFD call */
struct My_Nfd
{
  //My_Nfd(SDL_Window *win);
  ~My_Nfd();

  void free_pathset();

  nfdresult_t get_multifile_read_path(const char *filter_list=NULL);

  
  nfdchar_t **paths=NULL;

  int numpaths=0;
private:
  nfdpathset_t pathSet;
  bool pathSet_Allocated = false;
  //SDL_Window *sdlWindow = NULL;
};