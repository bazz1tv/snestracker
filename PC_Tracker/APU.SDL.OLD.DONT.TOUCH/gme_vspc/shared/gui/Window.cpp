#include "gui/Window.h"

Window::Window(int width, int height, const char *title)
{
  rect.w = width;
  rect.h = height;
  this->title = title;
  init();
}

void Window::init()
{
  this->destroy();
  SDL_CreateWindowAndRenderer(rect.w, rect.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN /*| SDL_WINDOW_BORDERLESS*/, &sdlWindow, &sdlRenderer);
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
  SDL_RenderSetLogicalSize(sdlRenderer, rect.w, rect.h);

  screen = SDL_CreateRGBSurface(0, rect.w, rect.h, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
  sdlTexture = SDL_CreateTexture(sdlRenderer,
                                          SDL_PIXELFORMAT_RGB888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          rect.w, rect.h);

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
  SDL_SetWindowTitle(sdlWindow, title.c_str());
  //SDL_Delay(5000);
  //SDL_ShowWindow(sdlWindow);
}

void Window::destroy()
{
  if (screen)
  {
    SDL_FreeSurface(screen);
    screen = NULL;
  }
  if (sdlTexture)
  {
    SDL_DestroyTexture(sdlTexture);
    sdlTexture = NULL;
  }
    
  if (sdlRenderer)
  {
    SDL_DestroyRenderer(sdlRenderer);
    sdlRenderer = NULL;
  }
    
  if(sdlWindow)
  {
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = NULL;
  }  
}

Window::~Window()
{
  SDL_Log("~Window");
  destroy();
}

void Window::clear_screen()
{
  SDL_FillRect(screen, NULL, 0);
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Window::update_screen()
{
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(sdlRenderer);
}

void Window::show()
{
  SDL_Log("Window::show()");
  SDL_ShowWindow(sdlWindow);
  SDL_RaiseWindow(sdlWindow);
  oktoshow = true;
  //init();
}

void Window::raise()
{
  SDL_Log("Window::raise()");
  SDL_RaiseWindow(sdlWindow);
}

void Window::hide()
{
  //this->destroy();
  //SDL_SetWindowGrab(sdlWindow, SDL_FALSE);
  SDL_HideWindow(sdlWindow);
  oktoshow = false;
  //SDL_SetWindowPosition(sdlWindow, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED);
}

