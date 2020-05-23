#pragma once

#include <nfd.h>
#include "SDL.h"

namespace SdlNfd
{
  /* init the SdlNfd provider with the SDL_Window.
  returns:
    0: success
    -1: failure (the SDL_Window was a nullptr)
  */
  int init(SDL_Window *win);
  nfdresult_t get_file_read_handle(nfdchar_t **outPath, SDL_RWops **file,
                                    const char *filter_list=NULL);

  nfdresult_t get_file_write_handle(nfdchar_t **outPath, SDL_RWops **file,
                                    const char *filter_list=NULL);
  void done(); // call this when you are done with a file
  bool active(); // call this from your SDL event handler

  extern bool _active;
  extern SDL_Window *_sdlWindow;

  /*namespace Multi
  {
    void free_pathset();

    nfdresult_t get_multifile_read_path(const char *filter_list=NULL);

    nfdpathset_t pathSet;
    bool pathSet_Allocated=false;
    nfdchar_t **paths=NULL;

    int numpaths=0;
  };*/
};