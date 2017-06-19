#include "Options_Window.h"
#include "Screen.h"

Options_Window::Tabs::Tabs(Options_Window &ref) : 
options_window(ref),
audio(3,3, "Audio", switch_to_audio_tab, &options_window, true)
{

}

int Options_Window::Tabs::switch_to_audio_tab(void *data)
{
  Options_Window *options_window = (Options_Window *)data;
  SDL_Log("switch_to_audio_tab");
  options_window->exp = &options_window->content_area.audio_options;
  options_window->content_area.clear();
  options_window->content_area.audio_options.one_time_draw();
  return 0;
}

Options_Window::Options_Window() : 
Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Options"),
tabs(*this),
content_area(*this)
{
  preload(10,10);

}

void Options_Window::show()
{
  exp->one_time_draw();
  draw();
  Window::show();

  //content_area.audio_options.screen = screen;
  //content_area.audio_options.renderer = sdlRenderer;
  
  //SDL_ShowWindow(sdlWindow);
}

void Options_Window::preload(int x, int y)
{
  tabs.preload(x,y);
  y += CHAR_HEIGHT*5;
  content_area.preload(x,y);
}

void Options_Window::run()
{
  if (exp) exp->run();
}

void Options_Window::draw()
{
  tabs.draw();
  if (exp) exp->draw();
  update_screen();
}

int Options_Window::receive_event(SDL_Event &ev)
{
  //SDL_Log("Options_Window::receive_event\n");
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
          hide();
        }
        break;
      }
    } break;

    default:break;
  }

  if (exp)
    exp->receive_event(ev);
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

Options_Window::Content_Area::Content_Area(Options_Window &ref) : 
options_window(ref),
audio_options(ref.screen, ref.sdlRenderer)
{

}

void Options_Window::Content_Area::preload(int x, int y)
{
  rect.x = x; rect.y = y;
  rect.w = options_window.rect.w - x;
  rect.h = options_window.rect.h - y;
  audio_options.preload(&rect);
}

void Options_Window::Content_Area::clear()
{
  SDL_FillRect(options_window.screen, &rect, Colors::Interface::color[Colors::Interface::bg]);
}
