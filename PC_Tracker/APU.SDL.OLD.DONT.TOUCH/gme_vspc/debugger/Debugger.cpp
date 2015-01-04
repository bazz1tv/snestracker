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
  
}


void Debugger::run()
{
  main_window.reload();
  while (!quitting)
  {
    main_window.run();
    handle_events();
    main_window.draw();
  }
}

void Debugger::handle_events()
{
  SDL_Event ev;
  while (SDL_PollEvent(&ev))
  {
    main_window.receive_event(ev);
  }
}




