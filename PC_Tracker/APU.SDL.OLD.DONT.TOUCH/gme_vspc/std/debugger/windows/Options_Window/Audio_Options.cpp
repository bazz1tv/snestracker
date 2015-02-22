#include "Audio_Options.h"
#include "sdlfont.h"
#include "utility.h"

Audio_Options::Context::Context()
{
  int selected_index=0;
  SDL_Log("Audio_Options::Context::Context");
  menu_items = (Context_Menu_Item *) SDL_malloc(sizeof(Context_Menu_Item) * (audio->devices.how_many+1));
  for (int i=0; i < audio->devices.how_many; i++)
  {
    SDL_Log(audio->devices.device_strings[i]);
    menu_items[i] = Context_Menu_Item(audio->devices.device_strings[i], true, NULL, NULL);
    if (audio->devices.selected_audio_out_dev == audio->devices.device_strings[i])
      selected_index=i;
  }
  menu = new Context_Menu(menu_items);
  menu->is_active = true;
  menu->should_highlight_hover = false;
  menu->should_highlight_currently_selected_item = true;
  menu->currently_selected_item = &menu->items[selected_index];
  menu->currently_selected_item_index = selected_index;
}

Audio_Options::Context::~Context()
{
  SDL_Log("Audio_Options::Context::~Context()");
  delete menu;
  SDL_free(menu_items);
}

Audio_Options::Audio_Options(SDL_Surface *screen, SDL_Renderer *renderer) : 
screen(screen),
renderer(renderer)
{

}

void Audio_Options::preload(SDL_Rect *rect)
{
  this->rect = *rect;
  // not using w or h yet
  //SDL_Rect r = {rect.x,rect.y,20*CHAR_WIDTH, 8*CHAR_HEIGHT};
}

void Audio_Options::run()
{
  //SDL_Log("Audio_Options::run");
}

void Audio_Options::draw()
{
  if (is_first_run)
  {

  }

  is_first_run=false;
  context.menu->draw(screen);
  //SDL_Log("Audio_Options::draw");
}
void Audio_Options::one_time_draw()
{
  int x=rect.x, y=rect.y;
  sdlfont_drawString(screen, x, y, "Output Device");
  y+=CHAR_HEIGHT;
  context.menu->preload(x,y);
  //Utility::draw_rect(screen, &r, 1, Colors::white);
}

int Audio_Options::receive_event(SDL_Event &ev)
{
  //SDL_Log("Audio_Options::receieve_event");
}