#pragma once
#include <SDL.h>
#include "Organization.h"
#include <boost/filesystem.hpp>

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