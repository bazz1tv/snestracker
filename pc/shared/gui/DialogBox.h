#pragma once
#include "SDL.h"

/* TODO: Pressing Enter to confirm a dialog box causes that keypress to enter
the event loop and get processed as a valid input. In this case, it will cause
the tracker to start playing. Similar issue as experienced in SdlNfd. FIX IT! */

namespace DialogBox
{
  enum ret_t {
    ERROR=-1,
    YES=0,
    NO,
  };

  enum Simple {
    INFO = SDL_MESSAGEBOX_INFORMATION,
    WARNING = SDL_MESSAGEBOX_WARNING,
    ERR = SDL_MESSAGEBOX_ERROR
  };

  int Custom(const char *title, const char *msg, int numButtons,
                    const char **buttonText, int defaultButton=0, Simple type=INFO);
  ret_t SimpleYesNo(const char *title, const char *msg, bool defaultYes=true);

  void SimpleOK(const char *title, const char *msg, Simple type=INFO);
};