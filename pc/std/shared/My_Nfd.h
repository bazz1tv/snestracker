#pragma once

#include <nfd.h>
#include "platform.h"

struct My_Nfd
{
  My_Nfd();
  ~My_Nfd();

  void free_pathset();

  nfdresult_t get_multifile_read_path(const char *filter_list=NULL);

  nfdchar_t *outPath=NULL;

  nfdpathset_t pathSet;
  bool pathSet_Allocated=false;
  nfdchar_t **paths=NULL;

  // this should probably not be here :P it should be encapsulated in a 
  // STD instance, but oh well
  char **rsn_spc_paths=NULL;
  size_t num_rsn_spc_paths=0;

  int numpaths=0;
};