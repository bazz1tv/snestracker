#pragma once
#include <SDL.h>
#include "Organization.h"
#include <boost/filesystem.hpp>

struct File_System
{
  File_System();
  int init();
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