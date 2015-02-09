#pragma once

#include <nfd.h>

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
  int numpaths=0;
};