#include "gui/Spc_Export_Window.h"
#include "Screen.h"
#include "My_Nfd.h"
#include "BaseD.h"
#include "File_System_Context.h"

#define WIDTH 500
#define HEIGHT 125
// don't forget to use check_kon
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
}

void Spc_Export_Window::show()
{
  BaseD::player->pause(1, false, false);
  Window::show();
  clear_screen();
  one_time_draw();

  SDL_StartTextInput();
  //SDL_ShowWindow(sdlWindow);
  if (!state)
    state = (unsigned char *) SDL_malloc(sizeof(unsigned char) * Snes_Spc::spc_file_size);

  unsigned char* out = state;
  Snes_Spc::init_header(out);

  // Copy current SPC header info to this new one
  Spc_Emu::header_t header = BaseD::player->spc_emu()->header();

  Spc_Emu::header_t *new_header = (Spc_Emu::header_t *) state;

  memcpy(new_header->song, &header.song, (size_t)((size_t)&new_header->emulator - (size_t)&new_header->song));
}

void Spc_Export_Window::one_time_draw()
{
  int x=10, y=10;
  sprintf(BaseD::tmpbuf, "Game....: %s", BaseD::tag.game);
  sdlfont_drawString(this->screen, x, y, BaseD::tmpbuf); 
  y+=CHAR_HEIGHT;
  sprintf(BaseD::tmpbuf, "Title...: %s", BaseD::tag.song);
  sdlfont_drawString(this->screen, x, y, BaseD::tmpbuf); 
  y+=CHAR_HEIGHT;
  sprintf(BaseD::tmpbuf, "Length..: %ld", BaseD::tag.length);
  sdlfont_drawString(this->screen, x, y, BaseD::tmpbuf);
  y+=CHAR_HEIGHT;
  sprintf(BaseD::tmpbuf, "Composer: %s", BaseD::tag.author);
  sdlfont_drawString(this->screen, x, y, BaseD::tmpbuf); 
  y+=CHAR_HEIGHT*2;

  sprintf(BaseD::tmpbuf, "Dumper..: %s", BaseD::tag.dumper);
  sdlfont_drawString(this->screen, x, y, BaseD::tmpbuf); 
  y+=CHAR_HEIGHT;
  sprintf(BaseD::tmpbuf, "Comment.: %s", BaseD::tag.comment);
  sdlfont_drawString(this->screen, x, y, BaseD::tmpbuf); 
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
  nfdchar_t *outPath=NULL;


  nfdresult_t result = NFD_SaveDialog( "spc", NULL, &outPath );
  //SDL_RaiseWindow(BaseD::sdlWindow);
  if ( result == NFD_OKAY )
  {
    if (outPath !=NULL)
      fprintf(stderr, "%s\n", outPath);

    bool was_paused = BaseD::player->is_paused();
    BaseD::player->pause(1, false, false);
    //

    /* Begin writing to spc file */
    // copy header info
    unsigned char *out = sew->state;
    Spc_Emu::header_t *header = (Spc_Emu::header_t *)out;



    BaseD::player->spc_emu()->can_has_apu()->save_spc(out);
    File_System_Context::file_system->write_file( outPath, sew->state, Snes_Spc::spc_file_size );
    // restore player state
    BaseD::player->pause(was_paused, false, false);
    sew->hide();
  }
  else if ( result == NFD_CANCEL ) 
  {
    puts("User pressed cancel.");
  }
  else
  {
    printf("Error: %s\n", NFD_GetError() );
    result = NFD_ERROR;
  }
  
  if (outPath)
    SDL_free(outPath);
  return result;
}

void Spc_Export_Window::run()
{
  //if (exp) exp->run();
}

void Spc_Export_Window::draw()
{
  export_button.draw(true, false, false, screen);
  update_screen();
}

int Spc_Export_Window::receive_event(SDL_Event &ev)
{
  //SDL_Log("Spc_Export_Window::receive_event\n");
  switch (ev.type)
  {
    case SDL_MOUSEBUTTONDOWN:
    {
      if (export_button.check_mouse_and_execute(ev.button.x, ev.button.y))
        return;
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
          close_window_ev.window.windowID = this->windowID;
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

