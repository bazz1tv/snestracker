#pragma once
#include "gui/Window.h"
#include "Experience.h"
#include "sdlfont.h"
#include "BaseD.h"
#include "Clickable_Text.h"

struct Spc_Export_Window : public Window
{

  Spc_Export_Window();
  ~Spc_Export_Window();

  int init();
  void show();
  void hide();
  void one_time_draw();

  Clickable_Text export_button;

  unsigned char *state = NULL;

  //Clickable_Text text[6];
  enum {
    GAME=0, TITLE, COMPOSER, DUMPER, COMMENT, LENGTH
  };

  bool is_first_run=true;
  void run();
  void draw();
  int receive_event(SDL_Event &ev);
  void preload(int x,int y);

  static int save_file(void *data);

  void clear_content_area();

private:
  void destroy_state();
};