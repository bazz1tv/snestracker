#pragma once

#include "gui/Window.h"
#include "gui/Tab.h"
#include "Experience.h"
#include "Audio_Options.h"
/*
   _______    _________
  | Audio |  | Keybord | 
   -------    ---------
 --------------------------------------------------
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|                                                  |
 --------------------------------------------------
*/

/*
   ___..__    _________
  | Audio |  | Keybord | 
   -------    ---------
 --------------------------------------------------
|                                                  
|  Output Device (CM)                              
|  -----------------                              
| | Built-in Output |                              
| | Soundflower     |                              
|  -----------------                               
|                                                  
|                                                  
 --------------------------------------------------
*/

struct Options_Window : public Window
{

  Options_Window();

  
  Experience *exp=&content_area.audio_options;

  bool is_first_run=true;
  void show();
  void run();
  void draw();
  int receive_event(SDL_Event &ev);
  void preload(int x,int y);

  void clear_content_area();

  struct Tabs
  {
    //Tabs() : options_window(ref) {}
    SDL_Rect rect;
    void preload(int x, int y);
    Tabs(Options_Window &ref);

    Options_Window &options_window;
    bool check_mouse_and_execute(int x, int y)
    {
      if (audio.check_mouse_and_execute(x,y)) 
      {
        audio.active = true;
        //dsp.active = false;
        //instr.active = false;
        return true;
      }
      return false;
    }
    Tab audio;
    void draw () { audio.draw(options_window.screen); }
    static int switch_to_audio_tab(void *data);
  } tabs;

  struct Content_Area
  {
    Content_Area(Options_Window &ref);
    void preload(int x, int y);
    SDL_Rect rect;
    void clear();
    Options_Window &options_window;
    Audio_Options audio_options;
    
  } content_area;
};