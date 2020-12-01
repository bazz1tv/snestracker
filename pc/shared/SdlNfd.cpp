#include "SdlNfd.h"
#include "DEBUGLOG.h"

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
}

/* rw: "r" or "rb" read
       "w" or "wb" write
*/
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
  bool saving = false;          // track whether we're doing a file read or a file write

  if (!strcmp(rw, "r") || !strcmp(rw, "rb"))
  {
    saving = false;
    result = NFD_OpenDialog( filter_list, NULL, &outPath );
  }
  else if (!strcmp(rw, "w") || !strcmp(rw, "wb"))
  {
    saving = true;
    result = NFD_SaveDialog( filter_list, NULL, &outPath );
  }
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
    char *finalPath = outPath;
    // If we're saving a file, we need to check if the file extension was present in the outPath, and append if not.
    if (saving)
    {
      DEBUGLOG("in Saving\n");
      // Before we get the handle on that path, we're going to
      // (Get the first file extension in the filter list)
#     define MAX_EXT 10
      char extensionBuf[MAX_EXT]; // just trying to be generally safe here and allow an extension up to 9 characters long.
      // copy the first extension in the filter_list into the extensionBuf
      /* The will either stop at the first comma delimiter found, or it will stop when encountering the null char
       * You must manually finish off the string with a null char after the loop */

      /* OK, it IS possible that the filter list entries could have an extension longer than my general MAX_EXT defined above.
       * But, I am in control of the filter_lists, not the end user. So we're good! We're not going make a
       * 10 character file extension LUL */

      extensionBuf[0] = '.';
      // copy the first entry of the filter_list into extensionBuf
      {
        int i=0;
        for(; filter_list[i] != ',' && filter_list[i] != 0; i++)
          extensionBuf[i+1] = filter_list[i];
        extensionBuf[i+1] = 0;
        // so now extensionBuf has eg. ".sts\0"

        DEBUGLOG("extensionBuf: \"%s\"\n", extensionBuf);
      }

      // check if the file extension is in outPath.
      if (strstr(outPath, extensionBuf) == NULL)
      {
        DEBUGLOG("File extension is NOT present\n");
        finalPath = malloc (strlen(outPath) + 1 + strlen(extensionBuf) + 1); // the plus 1's are the '.' and the null char
        // append that extension to a new string
        strcpy(finalPath, outPath);
        strcat(finalPath, extensionBuf);
      }
      else
      {
        DEBUGLOG("EXTENSION was present\n");
      }
      // else, outPath already has the extension, and we don't need to do anything.
    }

    // IF NOT, append that file extension  to the path string

    //and then get a handle on THAT
    file = SDL_RWFromFile(finalPath, rw);

    if (file == NULL)
    {
      sprintf(tmpbuf, "Warning: Unable to open file %s!\n %s", outPath,
              SDL_GetError() );
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                     "Could not open FILE!",
                     tmpbuf,
                     NULL);

      // Free finalPath here
      if (finalPath != outPath) // only free if it was allocated
      {
        free(finalPath);
        finalPath = NULL;
      }

      if (outPath)
      {
        free(outPath);
        outPath = NULL;
      }
      return NFD_ERROR;
    }

    // Free finalPath here
    if (finalPath != outPath) // only free if it was allocated
    {
      free(finalPath);
      finalPath = NULL;
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