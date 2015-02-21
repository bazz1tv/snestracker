#include "gui/Window.h"

Window::Window(int width, int height, const char *title)
{
  SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN, &sdlWindow, &sdlRenderer);
  if (sdlWindow == NULL || sdlRenderer == NULL)
  {
    fprintf(stderr, "FCK\n");
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Warning!",
                       "Could not create Window!",
                       NULL);
    return;
  }

  windowID = SDL_GetWindowID(sdlWindow);

  //sdlWindow = *sdlWindow;
  //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");  // make the scaled rendering look smoother.
  SDL_RenderSetLogicalSize(sdlRenderer, width, height);

  screen = SDL_CreateRGBSurface(0, width, height, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
  sdlTexture = SDL_CreateTexture(sdlRenderer,
                                          SDL_PIXELFORMAT_RGB888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          width, height);

  if (screen == NULL || sdlTexture == NULL)
  {
    fprintf(stderr, "MAn we have Window::Window problems : I couldn't allocate a screen or Texture :\n");
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Warning!",
                       "Window::Window() - Could not allocate screen or Texture!",
                       NULL);
    return;
  }

  // needs to be more permanent eh!
  //char tmpbuf[100];
  //sprintf(tmpbuf, PROG_NAME_VERSION_STRING, APP_VERSION);
  
  //clear_screen();
  //SDL_RaiseWindow(sdlWindow);
  //clear_screen();
  //SDL_HideWindow(sdlWindow);
  SDL_SetWindowTitle(sdlWindow, title);
  //SDL_Delay(5000);
  //SDL_ShowWindow(sdlWindow);
}


Window::~Window()
{
  SDL_Log("~Window");
  if (screen)
    SDL_FreeSurface(screen);
  if (sdlTexture)
    SDL_DestroyTexture(sdlTexture);
  if (sdlRenderer)
    SDL_DestroyRenderer(sdlRenderer);
  if(sdlWindow)
    SDL_DestroyWindow(sdlWindow);
}

void Window::clear_screen()
{
  SDL_FillRect(screen, NULL, 0);
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

/*void Window::draw()
{
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}*/

void Window::show()
{
  SDL_ShowWindow(sdlWindow);
}

void Window::raise()
{
  SDL_RaiseWindow(sdlWindow);
}

void Window::hide()
{
  SDL_HideWindow(sdlWindow);
}

