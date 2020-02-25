#pragma once

#include <nfd.h>
#include "platform.h"

struct My_Nfd
{
  My_Nfd();
  ~My_Nfd();

  void free_pathset();

  nfdresult_t get_multifile_read_path(const char *filter_list=NULL);

  nfdpathset_t pathSet;
  bool pathSet_Allocated=false;
  nfdchar_t **paths=NULL;

  int numpaths=0;
};
