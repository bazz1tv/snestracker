#include "gui/Spc_Export_Window.h"
#include "Screen.h"
#include "My_Nfd.h"
#include "File_System.h"
#include "globals.h"
#include "SdlNfd.h"
#include "mouse.h"

#define WIDTH 500
#define HEIGHT 125

Spc_Export_Window *spc_export_window = NULL;

Spc_Export_Window::Spc_Export_Window() :
Window(WIDTH, HEIGHT, "Export SPC"),
export_button("Export", Spc_Export_Window::save_file, (void *)this)
{
  
}

Spc_Export_Window::~Spc_Export_Window()
{
  destroy_state();
}

int Spc_Export_Window::init()
{
  // Prepare SPC memory area for export
  return 0;
}

void Spc_Export_Window::show()
{
  ::player->pause(1, false, false);
  Window::show();
  clear_screen();
  one_time_draw();

  SDL_StartTextInput();

  if (!state)
    state = (unsigned char *) SDL_malloc(sizeof(unsigned char) * Snes_Spc::spc_file_size);

  unsigned char* out = state;
  Snes_Spc::init_header(out);

  // Copy current SPC header info to this new one
  Spc_Emu::header_t header = ::player->spc_emu()->header();

  Spc_Emu::header_t *new_header = (Spc_Emu::header_t *) state;

  memcpy(new_header->song, &header.song, (size_t)((size_t)&new_header->emulator - (size_t)&new_header->song));
}

void Spc_Export_Window::one_time_draw()
{
  int x=10, y=10;
  char tmpbuf[267];
  sprintf(tmpbuf, "Game....: %s", ::tag.game);
  sdlfont_drawString(this->render.screen, x, y, tmpbuf);
  y+=CHAR_HEIGHT;
  sprintf(tmpbuf, "Title...: %s", ::tag.song);
  sdlfont_drawString(this->render.screen, x, y, tmpbuf);
  y+=CHAR_HEIGHT;
  sprintf(tmpbuf, "Length..: %ld", ::tag.length);
  sdlfont_drawString(this->render.screen, x, y, tmpbuf);
  y+=CHAR_HEIGHT;
  sprintf(tmpbuf, "Composer: %s", ::tag.author);
  sdlfont_drawString(this->render.screen, x, y, tmpbuf);
  y+=CHAR_HEIGHT*2;

  sprintf(tmpbuf, "Dumper..: %s", ::tag.dumper);
  sdlfont_drawString(this->render.screen, x, y, tmpbuf);
  y+=CHAR_HEIGHT;
  sprintf(tmpbuf, "Comment.: %s", ::tag.comment);
  sdlfont_drawString(this->render.screen, x, y, tmpbuf);
  y+=CHAR_HEIGHT*5;

  export_button.setup(WIDTH,y,true);
  draw();
}

void Spc_Export_Window::hide()
{
  SDL_StopTextInput();
  Window::hide();

  destroy_state();
}

void Spc_Export_Window::destroy_state()
{
  if (state)
  {
    SDL_free(state);
    state = NULL;
  }
}

int Spc_Export_Window::save_file(void *data)
{
  Spc_Export_Window *sew = (Spc_Export_Window *)data;

  nfdresult_t result = SdlNfd::get_file_handle("w", "spc");
  /* We don't actually need the file handle so release it */

  if (SdlNfd::file)
  {
    SDL_RWclose(SdlNfd::file);
    SdlNfd::file = NULL;
  }
  if ( result == NFD_OKAY )
  {
    fprintf(stderr, "Exporting SPC to: %s\n", SdlNfd::outPath);

    ::player->pause(1, false, false);
    //

    /* Begin writing to spc file */
    // copy header info
    unsigned char *out = sew->state;
    Spc_Emu::header_t *header = (Spc_Emu::header_t *)out;



    ::player->spc_emu()->can_has_apu()->save_spc(out);
    ::file_system->write_file( SdlNfd::outPath, sew->state, Snes_Spc::spc_file_size );
    // restore player state
    ::player->pause(1, false, false);
    sew->hide();
  }

  return result;
}

void Spc_Export_Window::run()
{
  //if (exp) exp->run();
}

void Spc_Export_Window::draw()
{
  export_button.draw(true, false, false, render.screen);
  update_screen();
}

int Spc_Export_Window::receive_event(SDL_Event &ev)
{
  //SDL_Log("Spc_Export_Window::receive_event\n");
  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
    {
      if (export_button.check_mouse_and_execute(mouse::prescaled_x, mouse::prescaled_y))
        return 0;
      //if (r) return r;
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
          close_window_ev.window.windowID = this->render.windowID;
          SDL_PushEvent(&close_window_ev);
        }
        break;
        //Handle backspace
        /*case SDLK_BACKSPACE:
          if (inputText.length() > 0 )
          {
            //lop off character
            inputText.pop_back();
            renderText = true;
          }
        break;
        //Handle copy
        case SDLK_c:
          if (SDL_GetModState() & KMOD_CTRL )
          {
            SDL_SetClipboardText( inputText.c_str() );
          }
        break;
        //Handle paste
        case SDLK_v:
          if (SDL_GetModState() & KMOD_CTRL )
          {
            inputText = SDL_GetClipboardText();
            renderText = true;
          }
        break;*/
      }
    } break;

    /*case SDL_TEXTINPUT:
    {
      //Not copy or pasting
      if( !( ( ev.text.text[ 0 ] == 'c' || ev.text.text[ 0 ] == 'C' ) && ( ev.text.text[ 0 ] == 'v' || ev.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
      {
        //Append character
        inputText += ev.text.text;
        renderText = true;
      }
    } break;*/
    default:break;
  }

  //if (exp)
    //exp->receive_event(ev);
  return 0;
}

