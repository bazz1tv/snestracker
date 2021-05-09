#pragma once

/* SDL2 NFD (NativeFileDialog) Wrapper

This solves a couple problems using NFD in SDL2 as well as takes care of resource
management. See sample code for more.

Copyright 2020 Michael Bazzinotti. All rights reserved
*/

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

  /* Call this function for either reading or writing to a user-specified file.
  It will:
   1. populate a blank path string (outPath)
   2. populate a SDL RWops handle (file)

   You can specify a filter list in the format "ext,ext2" which will limit the
   files displayed from the GUI menu

   After calling this function, check that it returned NFD_OKAY. Otherwise do not
   rely on the data.

   If the return code is NOT NFD_OKAY, do not interact with the data nor attempt to free it.

   Under good operating conditions (NFD_OKAY), the data will automatically be
   freed after the SDL event loop wait period (your check for active()), 500ms after
   the dialog has closed.
   If you need that data to persist, you will have to copy it manually to some
   other place.
   */
  nfdresult_t get_file_handle(const char *rw, const char *filter_list=NULL);

  // Prompts GUI for user to pick a directory, stores the string in dirPath.
  nfdresult_t get_directory();

  /* The following 2 vars are populated after a call to get_file_handle().
  Every call to get_file_handle() will cause a check and free() on subsequently
  reserved data. It's a good idea to use done() when you are finished */
  extern nfdchar_t *outPath;
  extern SDL_RWops *file;

  extern nfdchar_t *dirPath;

  /* call this from your SDL event handler, like:
  if (SdlNfd::active)
    continue;

  when true, ignores incoming events.
  This is to bypass a bug I noticed on Mac that if I confirmed a file entry from
  Cocoa menu with "Enter" key, my app would immediately process that key as an 
  in-app event, which was bad! This approach fixes that. */
  bool active(bool autorelease=true);

/// PRIVATE ///
  void _release();
  extern bool _active;
  extern Uint32 _drain;
  extern SDL_Window *_sdlWindow;

/* TODO: Add the support for multi-pathname reading */
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