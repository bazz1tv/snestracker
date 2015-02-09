#pragma once

#include "gui/Clickable_Text.h"
#include "BaseD.h"
#include "gui/Expanding_List.h"
struct Menu_Bar
{
  //Menu_Bar();
  bool is_first_run=true;
  void draw(SDL_Surface *screen);
  
  //bool is_first_run=false;
  // QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK - MM - DM - INSTR"

           

  struct File_Context
  {
    // Take parent class here
    File_Context() : menu(menu_items, true)
    {
      
    }

    static int open_spc(void *data);
    static int quit(void *data) { BaseD::quitting = true; }

    Expanding_List menu;
    Context_Menu_Item menu_items[6] = 
    {
      {"file",        true,  NULL,  NULL},
      {"open SPC",    true,  open_spc,  NULL},
      {"close SPC",   true,  NULL,  NULL},
      {"write mask",  true,  NULL,  NULL},
      {"quit",        true,  quit,  NULL},
      {"",            false, NULL,  NULL}
    };
  };
  struct Track_Context
  {
    // Take parent class here
    Track_Context() : menu(menu_items, true)
    {
      //menu_items[1].clickable_text.data = &menu_items[1].clickable_text.str;
    }

    void draw(SDL_Surface *screen);

    static int toggle_pause (void *data);         
    static int restart_current_track (void *data);
    static int prev_track (void *data);          
    static int next_track (void *data); 
    static int test(void *data) { fprintf(stderr, "test"); } 

    Expanding_List menu;
    Context_Menu_Item menu_items[6] = 
    {
      {"track",       true,  test,                          NULL},
      {"pause",       true,  toggle_pause,           &menu_items[1].clickable_text},
      {"restart",     true,  restart_current_track,  NULL},
      {"prev",        true,  prev_track,             NULL},
      {"next",        true,  next_track,             NULL},
      {"",            false, NULL,                          NULL}
    };
  };
  struct Window_Context
  {
    Window_Context() : menu(menu_items, true)
    {
      
    }

    static int memory(void *data);
    static int DSP(void *data);
    static int Instrument(void *data);

    Expanding_List menu;
    Context_Menu_Item menu_items[5] = 
    {
      {"Window",      true,  NULL,  NULL},
      {"Memory",      true,  memory,  NULL},
      {"DSP",         true,  DSP,  NULL},
      {"Instrument",  true,  Instrument,  NULL},
      {"",            false, NULL,  NULL}
    };
  };

  struct Context_Menus
  {
    int x = 10, y = 10;
    Context_Menus() {}
    enum
    {
      EVENT_INACTIVE=0,
      EVENT_ACTIVE=1,
      EVENT_FILE,
      EVENT_TRACK,
      EVENT_WINDOW
    };

    bool check_left_click_activate(int &x, int &y, const Uint8 &button=0);
    int receive_event(SDL_Event &ev);
    void draw(SDL_Surface *screen);
    void preload(int x, int y);
    void update(Uint8 adsr, Uint8 adsr2);

    File_Context          file_context;
    Track_Context         track_context;
    Window_Context        window_context;
  } context_menus;
};