#include "Audio_Options.h"
#include "sdlfont.h"
#include "utility.h"
#include "sdl_dblclick.h"
#include "BaseD.h"

int Audio_Options::Context::change_audio_out_device(void *item)
{

  Context_Menu_Item *itemp = (Context_Menu_Item *)item;
  SDL_Log("change_audio_out_device, %s", itemp->clickable_text.str.c_str());
  BaseD::player->init(BaseD::player->sample_rate, itemp->clickable_text.str.c_str());
}

Audio_Options::Context::Context()
{
  int selected_index=0;
  int i;
  SDL_Log("Audio_Options::Context::Context");
  menu_items = new Context_Menu_Item[audio->devices.how_many+1];
  for (i=0; i < audio->devices.how_many; i++)
  {
    SDL_Log(audio->devices.device_strings[i]);
    //menu_items[i] = Context_Menu_Item(audio->devices.device_strings[i], true, NULL, NULL);
    menu_items[i].clickable_text.str = audio->devices.device_strings[i];
    menu_items[i].clickable_text.action = change_audio_out_device;
    menu_items[i].clickable_text.data = &menu_items[i];
    menu_items[i].is_visible=true;
    menu_items[i].clickable_text.init_width_height();
    if (!strcmp(audio->devices.selected_audio_out_dev,audio->devices.device_strings[i]))
      selected_index=i;
  }
  menu_items[i].clickable_text.str = "";
  menu_items[i].clickable_text.action = NULL;
  menu_items[i].clickable_text.data = NULL;
  menu_items[i].is_visible=false;
  menu = new Context_Menu(menu_items);
  menu->is_active = true;
  menu->should_highlight_hover = false;
  menu->should_highlight_currently_selected_item = true;
  menu->currently_selected_item = &menu->items[selected_index];
  menu->currently_selected_item_index = selected_index;
}

Audio_Options::Context::~Context()
{
  SDL_Log("Audio_Options::~Context()");
  delete menu;
  //SDL_free(menu_items);
  delete[] menu_items;
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
  Utility::draw_rect(screen, &context.menu->created_at, 1, Colors::white);
}

int Audio_Options::receive_event(SDL_Event &ev)
{
  //SDL_Log("Audio_Options::receieve_event");
  dblclick::check_event(&ev);

  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
    {
      SDL_Log("Audio_Options::receive_event");
      if (context.menu->receive_event(ev))
      {
        context.menu->is_active = true;
      }
    }
    break;

    case SDL_USEREVENT:
    {
      if (ev.user.code == UserEvents::mouse_react)
      {
        /*SDL_Log("DFDFDF");
        SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click
        SDL_Event mousebuttondown_event;
        mousebuttondown_event.type = SDL_MOUSEBUTTONDOWN;
        mousebuttondown_event.button.x = te->button.x;
        mousebuttondown_event.button.y = te->button.y;
        mousebuttondown_event.button.button = SDL_BUTTON_LEFT;
        context.menu->receive_event(mousebuttondown_event);*/
      }
    } break;

    default:break;
  }
}