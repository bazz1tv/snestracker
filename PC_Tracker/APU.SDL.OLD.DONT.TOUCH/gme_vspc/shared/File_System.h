#pragma once
#include <SDL.h>
#include "Organization.h"
#include <boost/filesystem.hpp>

#ifdef _WIN32
  #define RM_CMD "rmdir /s /q "
  #define MKDIR_CMD "mkdir "
  #define PATH_SEP '\\'
#else
  #define RM_CMD "rm -rf "
  #define MKDIR_CMD "mkdir -p "
  #define PATH_SEP '/'
#endif

// TEMPORARY LOCATION
/* If str is not NULL, prints it and exits program, otherwise returns */
void error( const char* str );

struct File_System
{
  File_System();
  int init();
  /* Writes data to file */
  void write_file( const char* path, void const* in, long size );
  ~File_System();
  
  bool is_loaded=false;
  bool is_writing_enabled=true;;
  char *data_path=NULL;
  char *data_path_quoted=NULL;
  char *pref_path=NULL;
  char *pref_path_quoted=NULL;
  char *tmp_path=NULL;
  char *tmp_path_quoted=NULL;
};