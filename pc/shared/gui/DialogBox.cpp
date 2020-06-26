#include "DialogBox.h"
#include "DEBUGLOG.h"

#include <SDL.h>
DialogBox::ret_t DialogBox::SimpleYesNo(const char *title, const char *msg, bool defaultYes/*=true*/)
{
  const char *yesnoStr[2] = {"Yes", "No"};
  return (ret_t) Custom(title, msg, 2, yesnoStr, defaultYes ? 0 : 1);
}

/* Returns the index of the button that was pressed */
int DialogBox::Custom(const char *title, const char *msg, int numButtons,
                         const char **buttonText, int defaultButton/*=0*/)
{
  int rc;
  SDL_MessageBoxButtonData *buttons = (SDL_MessageBoxButtonData *) malloc(sizeof(SDL_MessageBoxButtonData) * numButtons);

  for (int i=0; i < numButtons; i++)
  {
    buttons[i] = { 0, i, buttonText[i] };
  }

  buttons[defaultButton].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;


  const SDL_MessageBoxData messageboxdata = {
      SDL_MESSAGEBOX_INFORMATION, /* .flags */
      NULL, /* .window */
      title, /* .title */
      msg, /* .message */
      numButtons, /* .numbuttons */
      buttons, /* .buttons */
      NULL /* .colorScheme */
  };

  int buttonid;
tryagain:
  if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
      SDL_Log("error displaying message box");
    rc = (int) ERROR;
  }
  else if (buttonid == -1) {
    // Why / when would this happen? Let's just try showing it again..
    SDL_Log("no button selection (?!)");
    goto tryagain;
  }
  else if (buttonid >= 0 && buttonid < numButtons)
    rc = (int) buttonid;
  else {
    // This should never happen
    SDL_Log("SUPER WEIRD BUG: Unknown button selection id was %d\n", buttonid);
    rc = (int) ERROR;
  }

  free(buttons);
  return rc;
}

void DialogBox::SimpleOK(const char *title, const char *msg, Simple type/*=INFO*/)
{
  int rc = SDL_ShowSimpleMessageBox(type, title, msg, NULL);
  if (rc < 0)
  {
    DEBUGLOG("Message Box Error from SDL: %s\n", SDL_GetError());
  }
}