#include "Debugger.h"
#include <getopt.h>
#include "utility.h"
#include "Screen.h"


#define L_FLAG 0
#define R_FLAG 1

Debugger::Debugger(int &argc, char **argv) : //, Music_Player *player,
  //SDL_Window *win, SDL_Renderer *renderer, SDL_Texture *text, SDL_Surface *screen) : 
//player(player), sdlWindow(win), sdlRenderer(renderer),sdlTexture(text),screen(screen),
//main_memory_area(screen,player),
//mouseover_hexdump_area(player,screen),
main_window(argc,argv)
{
  BaseD::main_window = &main_window;
  BaseD::dsp_window = &dsp_window;
  BaseD::instr_window = &instr_window;
  BaseD::menu_bar = &menu_bar;
  exp = &main_window;
}


void Debugger::run()
{
  //SDL_CaptureMouse(SDL_TRUE);
  // gotta call this once to initialize important stuffz
  main_window.check_paths_and_reload();
  main_window.one_time_draw();

  // exp is changed from BaseD
  while (!quitting)
  {
    exp->run();
    exp->draw();
    handle_events();
    //sdlfont_drawString(screen, 10,10, "back", Colors::red);

    //SDL_Delay( 1000 / 100 );
  }
  if (!player->is_paused() && player->track_started)
  {
    player->fade_out(false);
    player->pause(1, false, false);
  }
}

void Debugger::handle_events()
{
  SDL_Event ev;
  while (SDL_PollEvent(&ev))
  {
    if (ev.type == SDL_WINDOWEVENT)
    {
      if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        static int oldw=SCREEN_WIDTH,oldh=SCREEN_HEIGHT;
        int w = ev.window.data1;
        int h = ev.window.data2;
        int wd, hd;
        //if (w > SCREEN_WIDTH)
        //{
        wd = w - oldw;
        //}
        //else
        //{
         // wd = SCREEN_WIDTH - height;
        //}

        hd = h - oldh;

        if (abs(wd) < abs(hd)) wd = hd;
        else if (abs(wd) > abs(hd)) hd = wd;
        SDL_SetWindowSize(sdlWindow, oldw+wd, oldh+hd);
        oldw += wd;
        oldh += hd;
      }
    }
    else if (ev.type == SDL_DROPFILE)
    {
      char *dropped_filedir = ev.drop.file;
      // Shows directory of dropped file
      /*SDL_ShowSimpleMessageBox(
          SDL_MESSAGEBOX_INFORMATION,
          "File dropped on window",
          dropped_filedir,
          sdlWindow
      );*/
      BaseD::nfd.free_pathset();
      check_paths_and_reload(&dropped_filedir, 1, true);
      SDL_free(dropped_filedir);    // Free dropped_filedir memory
      SDL_RaiseWindow(sdlWindow);
    }
    exp->receive_event(ev);
  }
}




