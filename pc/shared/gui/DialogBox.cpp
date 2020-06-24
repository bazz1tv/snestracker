#include "DialogBox.h"
#include "DEBUGLOG.h"

#include <SDL.h>
DialogBox::ret_t DialogBox::SimpleYesNo(const char *title, const char *msg, bool defaultYes/*=true*/)
{
  SDL_MessageBoxButtonData buttons[] = {
      { /* .flags, .buttonid, .text */        0, NO, "no" },
      {                                       0, YES, "yes" },
//      { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
  };

  buttons[defaultYes].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
  const SDL_MessageBoxData messageboxdata = {
      SDL_MESSAGEBOX_INFORMATION, /* .flags */
      NULL, /* .window */
      title, /* .title */
      msg, /* .message */
      SDL_arraysize(buttons), /* .numbuttons */
      buttons, /* .buttons */
      NULL /* .colorScheme */
  };

  int buttonid;
tryagain:
  if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
      SDL_Log("error displaying message box");
    return ERROR;
  }
  if (buttonid == -1) {
    // Why / when would this happen? Let's just try showing it again..
    SDL_Log("no button selection (?!)");
    goto tryagain;
  }
  else if (buttonid == NO || buttonid == YES)
    return (ret_t) buttonid;
  else {
    // This should never happen
    SDL_Log("SUPER WEIRD BUG: Unknown button selection was %s. Forcing to YES\n", buttons[buttonid].text);
    // let's recover by assuming YES
    return YES;
  }
}

void DialogBox::SimpleOK(const char *title, const char *msg, Simple type/*=INFO*/)
{
  int rc = SDL_ShowSimpleMessageBox(type, title, msg, NULL);
  if (rc < 0)
  {
    DEBUGLOG("Message Box Error from SDL: %s\n", SDL_GetError());
  }
}