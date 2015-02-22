#include "Options_Window.h"
#include "Screen.h"

Options_Window::Tabs::Tabs(Options_Window &ref) : 
options_window(ref),
audio(3,3, "Audio", switch_to_audio_tab, NULL, true)
{

}

int Options_Window::Tabs::switch_to_audio_tab(void *data)
{
  SDL_Log("switch_to_audio_tab");
  return 0;
}

Options_Window::Options_Window() : 
Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Options"),
audio_options(screen),
tabs(*this)
{
  preload(10,10);
}

void Options_Window::preload(int x, int y)
{
  tabs.preload(x,y);
  y += CHAR_HEIGHT*2;
  audio_options.preload(x,y);
}

void Options_Window::draw()
{
  if (is_first_run)
  {
    
    is_first_run=false;
  }

  tabs.draw();
  update_screen();
}

int Options_Window::receive_event(SDL_Event &ev)
{
  SDL_Log("Options_Window::receive_event\n");
  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
    {
      bool r = tabs.check_mouse_and_execute(ev.button.x, ev.button.y);
      if (r) return r;
    } break;

    case SDL_KEYDOWN:
    {
      switch (ev.key.keysym.sym)
      {
        case SDLK_ESCAPE:
        {
          SDL_Event close_window_ev;
          close_window_ev.type = SDL_WINDOWEVENT;
          close_window_ev.window.event = SDL_WINDOWEVENT_CLOSE;
          close_window_ev.window.windowID = windowID;
          SDL_PushEvent(&close_window_ev);
        }
        break;
      }
    } break;

    default:break;
  }
  return 0;
}

void Options_Window::Tabs::preload(int x, int y)
{
  // init Tabs
  audio.rect.x = x;
  audio.rect.y = y; // + h + CHAR_HEIGHT*2;
  //
  /*dsp.rect.x = mem.rect.x + mem.horiz_pixel_length() + CHAR_WIDTH;
  dsp.rect.y = mem.rect.y;
  //
  instr.rect.x = dsp.rect.x + dsp.horiz_pixel_length() + CHAR_WIDTH;
  instr.rect.y = mem.rect.y;

  rect = {mem.rect.x, mem.rect.y, instr.rect.x + instr.rect.w, CHAR_HEIGHT};*/
  rect = audio.rect;
}

