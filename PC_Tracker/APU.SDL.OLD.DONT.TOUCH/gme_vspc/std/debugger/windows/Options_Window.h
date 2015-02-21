#pragma once

#include "gui/Window.h"
#include "gui/Tab.h"
#include "Experience.h"

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

  bool is_first_run=true;
  void run() {}
  void draw();
  int receive_event(SDL_Event &ev);

  struct Tabs
  {
    SDL_Rect rect;
    void preload(int x, int y, int h);
    Tabs() : audio(3,3, "Audio", switch_to_audio_tab, NULL, true)
    {

    }
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
    void draw () { audio.draw(); }
    static int switch_to_audio_tab(void *data);
  } tabs;
};