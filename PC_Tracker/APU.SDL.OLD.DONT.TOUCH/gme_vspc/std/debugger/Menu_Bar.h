#pragma once

#include "gui/Clickable_Text.h"
#include "BaseD.h"
#include "gui/Expanding_List.h"
#include "Main_Window.h"
#include "Options_Window.h"
#include "gui/Spc_Export_Window.h"
struct Menu_Bar
{
  
  //Menu_Bar();
  bool is_first_run=true;
  void draw(SDL_Surface *screen);
  int receive_event(SDL_Event &ev);
  
  //bool is_first_run=false;
  // QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK - MM - DM - INSTR"

           

  struct File_Context
  {
    // Take parent class here
    File_Context() : menu(menu_items, true)
    {
      
    }

    static int open_spc(void *data);
    static int export_spc(void *data);
    static int export_wav(void *data);
    static int quit(void *data) { BaseD::quitting = true; return 0; }


    Expanding_List menu;
    Context_Menu_Item menu_items[6] = 
    {
      {"file",        true,  NULL,  NULL},
      {"open SPC",    true,  open_spc,  NULL},
      //{"close SPC",   true,  NULL,  NULL},
      //{"write mask",  true,  NULL,  NULL},
      {"export SPC", true, export_spc, NULL},
      {"export WAV", true, export_wav, NULL},
      {"quit",        true,  quit,  NULL},
      {"",            false, NULL,  NULL}
    };
  };
  struct Edit_Context
  {
    // Take parent class here
    Edit_Context() : menu(menu_items, true)
    {
      
    }

    static int open_options_window(void *data);
    static int open_options_window_in_thread(void *data);

    Expanding_List menu;
    Context_Menu_Item menu_items[4] = 
    {
      {"Edit",        true,  NULL,  NULL},
      {"------",      true,  NULL,  NULL},
      {"Options",     true,  open_options_window,  NULL},
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
    //static int test(void *data) { fprintf(stderr, "test"); return 0; } 
    static int create_profile(void *data);

    Expanding_List menu;
    Context_Menu_Item menu_items[8] = 
    {
      {"track",           true,   NULL,                   NULL},
      {"pause",           true,   toggle_pause,           &menu_items[1].clickable_text},
      {"restart",         true,   restart_current_track,  NULL},
      {"prev",            true,   prev_track,             NULL},
      {"next",            true,   next_track,             NULL},
      {"-------",         true,   NULL,                   NULL},
      {"Create Profile",  true,   create_profile,         NULL},
      {"",                false,  NULL,                   NULL}
    };
  };
  struct Window_Context
  {
    Window_Context() : menu(menu_items, true)
    {
      
    }

    
    static int restore_window_size(void *nada);

    Expanding_List menu;
    Context_Menu_Item menu_items[7] = 
    {
      {"Window",        true,  NULL,  NULL},
      {"Original Size", true,  restore_window_size, NULL},
      {"-------------", true, NULL, NULL},
      {"Memory",        true,  BaseD::switch_to_memory,  NULL},
      {"DSP",           true,  BaseD::switch_to_dsp,  NULL},
      {"Instrument",    true,  BaseD::switch_to_instrument,  NULL},
      {"",              false, NULL,  NULL}
    };
  };
  enum
    {
      EVENT_INACTIVE=0,
      EVENT_ACTIVE=1,
      EVENT_FILE,
      EVENT_TRACK,
      EVENT_WINDOW
    };

  struct Context_Menus
  {
    int x = 10, y = 10;
    int h = CHAR_HEIGHT;
    Context_Menus() {}

    bool is_anything_active();
    

    bool check_left_click_activate(int &x, int &y, const Uint8 &button=0, const SDL_Event *ev=NULL);
    int receive_event(SDL_Event &ev);
    void draw(SDL_Surface *screen);
    void preload(int x, int y);
    void update(Uint8 adsr, Uint8 adsr2);
    void deactivate_all() {
      file_context.menu.deactivate();
      edit_context.menu.deactivate();
      track_context.menu.deactivate();
      window_context.menu.deactivate();
    }

    File_Context          file_context;
    Edit_Context          edit_context;
    Track_Context         track_context;
    Window_Context        window_context;
  } context_menus;

  struct Tabs
  {
    SDL_Rect rect;
    void preload(int x, int y);
    Tabs() : mem(3,3, "Mem", BaseD::switch_to_memory, NULL, true),
      dsp(3,3, "DSP", BaseD::switch_to_dsp, NULL),
      instr(3,3, "INSTR", BaseD::switch_to_instrument, NULL)
    {

    }
    bool check_mouse_and_execute(int x, int y)
    {
      if (mem.check_mouse_and_execute(x,y)) 
      {
        mem.active = true;
        dsp.active = false;
        instr.active = false;
        return true;
      }
      if (dsp.check_mouse_and_execute(x,y)) 
      {
        mem.active = false;
        dsp.active = true;
        instr.active = false;
        return true;
      }
      if (instr.check_mouse_and_execute(x,y))
      {
        mem.active = false;
        dsp.active = false;
        instr.active = true;
        return true;
      }
      return false;
    }
    Tab mem, dsp, instr;
    void draw ();
    static int memory(void *data);
    static int DSP(void *data);
    static int Instrument(void *data);

    int logged_grand_mode=BaseD::GrandMode::MAIN;
  } tabs;

};