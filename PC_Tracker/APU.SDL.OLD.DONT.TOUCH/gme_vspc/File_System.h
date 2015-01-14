#pragma once
#include <SDL.h>
#include "Organization.h"

namespace File_System
{
  int init();
  
  extern bool is_loaded;
  extern char *data_path;
  extern char *pref_path;
}