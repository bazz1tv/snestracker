#pragma once
#include <SDL.h>
#include "Organization.h"

struct File_System
{
  int init();
  ~File_System();
  
  bool is_loaded=false;
  bool is_writing_enabled=true;;
  char *data_path=NULL;
  char *pref_path=NULL;
};