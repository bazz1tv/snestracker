#include "Options_Window.h"
#include "Screen.h"

int Options_Window::Tabs::switch_to_audio_tab(void *data)
{
  return 0;
}

Options_Window::Options_Window() : 
Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Options")
{}

