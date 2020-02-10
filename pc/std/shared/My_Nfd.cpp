#include "My_Nfd.h"
#include "BaseD.h"
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

  if (rsn_spc_paths)
  {
    for (int i = 0; i < num_rsn_spc_paths; i++ )
    {
      SDL_free(rsn_spc_paths[i]);
    }

    SDL_free(rsn_spc_paths);
    rsn_spc_paths = NULL;
    num_rsn_spc_paths=0;
  }
}
My_Nfd::~My_Nfd()
{
  
  free_pathset();
  

  if (rsn_spc_paths)
  {
    for (int i = 0; i < num_rsn_spc_paths; i++ )
    {
      SDL_free(rsn_spc_paths[i]);
    }

    SDL_free(rsn_spc_paths);
    rsn_spc_paths = NULL;
  }

  if (outPath)
  {
    free(outPath);
    outPath = NULL;
  }
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
    //puts(outPath);
    //SDL_RWops* SDL_RWFromFile(const char* file,
      //                const char* mode)
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
    //if (*outPath)
      //free(*outPath);
    puts("User pressed cancel.");
    return result;
  }
  else
  {
    //if (*outPath)
    //free(*outPath);
    printf("Error: %s\n", NFD_GetError() );
    return NFD_ERROR;
  }
}