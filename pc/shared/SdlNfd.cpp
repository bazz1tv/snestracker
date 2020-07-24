#include "SdlNfd.h"

nfdchar_t *SdlNfd::outPath = NULL;
SDL_RWops *SdlNfd::file = NULL;

bool SdlNfd::_active = false;
Uint32 SdlNfd::_drain = 0;
SDL_Window *SdlNfd::_sdlWindow = NULL;

int SdlNfd::init(SDL_Window *win)
{
  if (win == NULL)
    return -1; // invalid window ptr
  /*else if (win)
    return -2; // already loaded*/

  _sdlWindow = win;

  return 0;
}


nfdresult_t SdlNfd::get_file_handle(const char *rw, const char *filter_list/*=NULL*/)
{
  if (_sdlWindow == NULL)
  {
    fprintf(stderr, "You haven't called SdlNfd::init()! file %s:%d (function %s)\n",
            __FILE__, __LINE__, __func__);
    return NFD_ERROR;
  }

  _release(); // release any previous allocated material that was accidently left over

  char tmpbuf[200];
  outPath=NULL;
  nfdresult_t result;

  if (!strcmp(rw, "r") || !strcmp(rw, "rb"))
    result = NFD_OpenDialog( filter_list, NULL, &outPath );
  else if (!strcmp(rw, "w") || !strcmp(rw, "wb"))
    result = NFD_SaveDialog( filter_list, NULL, &outPath );
  else
  {
    fprintf(stderr, "Unsupported file mode '%s', ignoring...\n", rw);
    return NFD_ERROR;
  }

  /* bugfix #12
  Focusing the main window from the NFD call has caused the SDL event loop to react
  to certain NFD actions such as pressing enter to confirm the selected file also
  causing the tracker to begin playback. To remedy this, a 200ms bypassing of events
  is imposed after NFD has returned */
  _drain = SDL_GetTicks() + 200; // 200ms ignore events after nfd dialog
  _active = true;
  SDL_RaiseWindow(_sdlWindow);

  if ( result == NFD_OKAY )
  {
    file = SDL_RWFromFile(outPath, rw);

    if (file == NULL)
    {
      sprintf(tmpbuf, "Warning: Unable to open file %s!\n %s", outPath,
              SDL_GetError() );
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                     "Could not open FILE!",
                     tmpbuf,
                     NULL);
      if (outPath)
      {
        free(outPath);
        outPath = NULL;
      }
      return NFD_ERROR;
    }
    return result;
  }
  else if ( result == NFD_CANCEL )
  {
    if (outPath)
    {
      free(outPath);
      outPath = NULL;
    }

    puts("User pressed cancel.");
    return result;
  }
  else
  {
    printf("Error: %s, %s\n", outPath, NFD_GetError() );
    if (outPath)
    {
      free(outPath);
      outPath = NULL;
    }
    return NFD_ERROR;
  }
}

void SdlNfd::_release()
{
  if (outPath)
  {
    free(outPath);
    outPath = NULL;
  }
  if (file)
  {
    SDL_RWclose(file);
    file = NULL;
  }
}

bool SdlNfd::active(bool autorelease)
{
  if ( _active && (SDL_GetTicks() < _drain) ) // value won't wrap for 49 days ;)
    return true;

  // The wait is over. Auto-release materials and reset internal vars
  if (autorelease)
    _release();

  _drain = 0;
  return _active = false;
}