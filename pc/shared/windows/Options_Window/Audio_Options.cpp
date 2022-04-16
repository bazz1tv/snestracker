#include "Audio_Options.h"
#include "sdlfont.h"
#include "utility.h"
#include "sdl_dblclick.h"
#include "gme/player/Music_Player.h"

int Audio_Options::Context::change_audio_out_device(void *item)
{
  Audio_Options *ao = (Audio_Options *) item;
  Context_Menu_Item *itemp = (Context_Menu_Item *) ao->context.menu->currently_selected_item;
  SDL_Log("change_audio_out_device, %s", itemp->clickable_text.str);
  if (ao->openDeviceonClick)
    ::player->init(::player->sample_rate, itemp->clickable_text.str);
  return 0;
}

Audio_Options::Context::Context(Audio_Options *ao) : ao(ao)
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
    menu_items[i].clickable_text.data = ao;
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
  menu->linespace = 4;
  menu->hpadding = CHAR_WIDTH;
}

Audio_Options::Context::~Context()
{
  SDL_Log("Audio_Options::~Context()");
  delete menu;
  //SDL_free(menu_items);
  delete[] menu_items;
}

Audio_Options::Audio_Options(SDL_Surface *screen, SDL_Renderer *renderer, bool openDeviceonClick/*=true*/) :
screen(screen),
renderer(renderer),
context(this),
openDeviceonClick(openDeviceonClick)
{

}

Audio_Options::BufferSize::BufferSize() :
  cbuf("  2048"),
  title("Buffer Size"),
  valtext(cbuf),
  decbtn("<", dec, this, false),
  incbtn(">", inc, this, false)
{}

void Audio_Options::BufferSize::update()
{
  sprintf(cbuf, "%6d", ::player->sample_frame_size);
}

void Audio_Options::BufferSize::setCoords(int x, int y)
{
  title.rect.x = x;
  title.rect.y = y;
  valtext.rect.x = x + ( (strlen(title.str) + 1) * CHAR_WIDTH );
  valtext.rect.y = y;
  decbtn.rect.x  = valtext.rect.x + ( (strlen(valtext.str) + 1) * CHAR_WIDTH );
  decbtn.rect.y = y;
  incbtn.rect.x = decbtn.rect.x + CHAR_WIDTH + 5;
  incbtn.rect.y = y;
}

int Audio_Options::BufferSize::inc(void *b)
{
  BufferSize *bs = (BufferSize *) b;
  if (::player->sample_frame_size < AUDIO_BUFFER_MAX)
  {
    ::player->sample_frame_size *= 2;
    ::player->init(::player->sample_rate, Audio::Devices::selected_audio_out_dev);
  }
  return 0;
}

int Audio_Options::BufferSize::dec(void *b)
{
  BufferSize *bs = (BufferSize *) b;
  if (::player->sample_frame_size > AUDIO_BUFFER_MIN)
  {
    ::player->sample_frame_size /= 2;
    ::player->init(::player->sample_rate, Audio::Devices::selected_audio_out_dev);
  }
  return 0;
}

void Audio_Options::preload(SDL_Rect *rect)
{
  this->rect = *rect;
  context.menu->preload(rect->x, rect->y);

  bufferSize.setCoords(rect->x, rect->y + context.menu->created_at.h + (3*CHAR_WIDTH));
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

  bufferSize.update();
  bufferSize.title.draw(screen);
  bufferSize.valtext.draw(screen);
  bufferSize.incbtn.draw(screen);
  bufferSize.decbtn.draw(screen);
}
void Audio_Options::one_time_draw()
{
  int x=rect.x, y=rect.y;
  sdlfont_drawString(screen, x, y, "Output Device");
  y+=CHAR_HEIGHT;
  context.menu->preload(x,y);
  SDL_Rect r = context.menu->created_at;
  r.x -=1;
  r.y -= 1;
  r.w += 1;
  r.h += 1;
  Utility::DrawRect(&r, 1, Colors::white, screen);
}

int Audio_Options::receive_event(SDL_Event &ev)
{
  //SDL_Log("Audio_Options::receieve_event");
  bufferSize.decbtn.check_event(ev);
  bufferSize.incbtn.check_event(ev);

  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
    {
      SDL_Log("Audio_Options::receive_event");
      context.menu->is_active = true;
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
  return 0;
}
