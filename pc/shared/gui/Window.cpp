#include "gui/Window.h"

Window *Window::map[NUM_WINDOWS+1];

Window::Window(int width, int height, const char *title) : render(width, height)
{
  rect.w = width;
  rect.h = height;
  // TODO: maybe have a buffer to ensure we don't lose the string contents?
  this->title = title;
  init();
}

void Window::init()
{
  this->destroy();
  SDL_CreateWindowAndRenderer(rect.w, rect.h, SDL_WINDOW_HIDDEN /*| SDL_WINDOW_BORDERLESS*/, &render.sdlWindow, &render.sdlRenderer);
  if (render.sdlWindow == NULL || render.sdlRenderer == NULL)
  {
    fprintf(stderr, "FCK\n");
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Warning!",
                       "Could not create Window!",
                       NULL);
    return;
  }

  render.windowID = SDL_GetWindowID(render.sdlWindow);

  //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");  // make the scaled rendering look smoother.
  //SDL_RenderSetLogicalSize(render.sdlRenderer, rect.w, rect.h);

  render.screen = SDL_CreateRGBSurface(0, rect.w, rect.h, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
  render.sdlTexture = SDL_CreateTexture(render.sdlRenderer,
                                          SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          rect.w, rect.h);

  if (render.screen == NULL || render.sdlTexture == NULL)
  {
    fprintf(stderr, "MAn we have Window::Window problems : I couldn't allocate a render.screen or Texture :\n");
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       "Warning!",
                       "Window::Window() - Could not allocate render.screen or Texture!",
                       NULL);
    return;
  }

  SDL_SetColorKey(render.screen, SDL_TRUE, 0);
  SDL_SetSurfaceBlendMode(render.screen, SDL_BLENDMODE_BLEND);

  SDL_SetTextureBlendMode(render.sdlTexture, SDL_BLENDMODE_BLEND);

  SDL_SetWindowTitle(render.sdlWindow, title.c_str());
}

void Window::destroy()
{
}

Window::~Window()
{
  SDL_Log("~Window");
  destroy();
}

void Window::clear_screen()
{
  render.clear_screen();
}

void Window::update_screen()
{
  render.sdl_draw();
}

void Window::show()
{
  SDL_Log("Window::show()");
  SDL_ShowWindow(render.sdlWindow);
  SDL_RaiseWindow(render.sdlWindow);
  oktoshow = true;
  //init();
}

void Window::raise()
{
  SDL_Log("Window::raise()");
  SDL_RaiseWindow(render.sdlWindow);
}

void Window::hide()
{
  //this->destroy();
  //SDL_SetWindowGrab(render.sdlWindow, SDL_FALSE);
  SDL_HideWindow(render.sdlWindow);
  oktoshow = false;
  //SDL_SetWindowPosition(render.sdlWindow, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED);
}

Window * Window::getWindow(Uint32 windowID)
{
  for (int i=0; i < NUM_WINDOWS; i++)
  {
    if (map[i]->render.windowID == windowID)
      return map[i];
  }

  return NULL;
}

Window * Window::getWindow(SDL_Window *sdlWindow)
{
  for (int i=0; i < NUM_WINDOWS; i++)
  {
    if (map[i]->render.sdlWindow == sdlWindow)
      return map[i];
  }

  return NULL;
}
