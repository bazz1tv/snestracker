#include "SdlNfd.h"

bool SdlNfd::_active = false;
SDL_Window *SdlNfd::_sdlWindow = NULL;

#define PREREQ_CHECK \
  if (_sdlWindow == NULL) \
  { \
    fprintf(stderr, "You haven't called SdlNfd::init()! file %s:%d (function %s)\n", \
            __FILE__, __LINE__, __func__); \
    return NFD_ERROR; \
  }

int SdlNfd::init(SDL_Window *win)
{
  if (win == NULL)
    return -1; // invalid window ptr
  /*else if (win)
    return -2; // already loaded*/

  _sdlWindow = win;
}

nfdresult_t SdlNfd::get_file_read_handle(nfdchar_t **outPath, SDL_RWops **file, const char *filter_list/*=NULL*/)
{
  PREREQ_CHECK

  char tmpbuf[200];
  *outPath=NULL;
  nfdresult_t result = NFD_OpenDialog( filter_list, NULL, outPath );
  
  /* bugfix #12 */
  SDL_RaiseWindow(_sdlWindow);
  _active = true;

    if ( result == NFD_OKAY )
    {
      //puts("Success!");
      //puts(outPath);
      //SDL_RWops* SDL_RWFromFile(const char* file,
        //                const char* mode)
      *file = SDL_RWFromFile(*outPath, "rb");
      if (*file == NULL)
      {
        sprintf(tmpbuf, "Warning: Unable to open file!\n %s", SDL_GetError() );
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Could not open FILE!",
                       tmpbuf,
                       NULL);
        return NFD_ERROR;
      }
      return result;
    }
    else if ( result == NFD_CANCEL ) 
    {
      if (*outPath)
      {
        free(*outPath);
        *outPath = NULL;
      }
      puts("User pressed cancel.");
      return result;
    }
    else
    {
      if (*outPath)
      {
        free(*outPath);
        *outPath = NULL;
      }
      printf("Error: %s\n", NFD_GetError() );
      return NFD_ERROR;
    }
}

nfdresult_t SdlNfd::get_file_write_handle(nfdchar_t **outPath, SDL_RWops **file, const char *filter_list/*=NULL*/)
{
  PREREQ_CHECK

  char tmpbuf[200];
  *outPath=NULL;
  nfdresult_t result = NFD_SaveDialog( filter_list, NULL, outPath );

  /* bugfix #12 */
  _active = true;
  SDL_RaiseWindow(_sdlWindow);

  if ( result == NFD_OKAY )
  {
    *file = SDL_RWFromFile(*outPath, "wb");

    if (*file == NULL)
    {
      sprintf(tmpbuf, "Warning: Unable to open file!\n %s", SDL_GetError() );
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                     "Could not open FILE!",
                     tmpbuf,
                     NULL);
      return NFD_ERROR;
    }
    return result;
  }
  else if ( result == NFD_CANCEL )
  {
    if (*outPath)
    {
      free(*outPath);
      *outPath = NULL;
    }

    puts("User pressed cancel.");
    return result;
  }
  else
  {
    if (*outPath)
    {
      free(*outPath);
      *outPath = NULL;
    }
    printf("Error: %s\n", NFD_GetError() );
    return NFD_ERROR;
  }
}

void SdlNfd::done() { _active = false; }
bool SdlNfd::active() { return _active; }