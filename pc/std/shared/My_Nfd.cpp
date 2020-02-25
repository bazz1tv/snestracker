#include "My_Nfd.h"
#include "SDL.h"
My_Nfd::My_Nfd()
{

}

void My_Nfd::free_pathset()
{
  if ( pathSet_Allocated )
  {
    free (paths);
    NFD_PathSet_Free(&pathSet);
    pathSet_Allocated = false;
    numpaths=0;
  }
}

My_Nfd::~My_Nfd()
{
  free_pathset();
}

nfdresult_t My_Nfd::get_multifile_read_path(const char *filter_list/*=NULL*/)
{
  nfdpathset_t tpathSet;
  nfdresult_t result = NFD_OpenDialogMultiple( filter_list, NULL, &tpathSet );
  //SDL_RaiseWindow(BaseD::sdlWindow);
  if ( result == NFD_OKAY )
  {
    puts("DERP!");
    free_pathset();

    this->pathSet = tpathSet;
    paths = (nfdchar_t **) malloc(sizeof(nfdchar_t *) * (NFD_PathSet_GetCount(&pathSet)+1));
    size_t i;
    for ( i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i )
    {
        paths[i] = NFD_PathSet_GetPath(&pathSet, i);
        printf("Path %li: %s\n", i, paths[i] );
    }
    numpaths=i;
    paths[i] = NULL;
    pathSet_Allocated = true;
    return result;
  }
  else if ( result == NFD_CANCEL ) 
  {
    puts("User pressed cancel.");
    return result;
  }
  else
  {
    printf("Error: %s\n", NFD_GetError() );
    return NFD_ERROR;
  }
}
