#pragma once

#include "shared/gui/Text.h"
#include "shared/gui/Button.h"
#include "shared/gui/Tab.h"
#include "shared/gui/Graph.h"
#include "shared/ADSR.h"
#include "shared/DrawRenderer.h"
#include "shared/gui/Checkbox.h"

struct Instrument_Panel;

struct AdsrPanel : public DrawRenderer
{
  // I hate doing this, but need a handle on tracker to add certain
  // SDL_Renderer pre drawing stuff. also need to inherit Experience
  AdsrPanel(Instrument_Panel *ip);
  void set_coords(int x, int y);
  int check_event(const SDL_Event &ev);
  void run();
  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);
  void draw_renderer(); // for the SDL_Renderer drawing stuff

  Text attack_text, decay_text, sustainlevel_text, sustainrelease_text;

  struct AdsrGraph : Graph
  {
    AdsrGraph()
    {
      points = (SDL_Point *) malloc(sizeof(SDL_Point) * 4);
      pcount = 4;
    }
    ~AdsrGraph()
    {
      free(points);
    }
  };

  AdsrGraph adsrgraph;
  ADSR::Context_Menus adsr_context_menus;
private:
  Instrument_Panel *instrpanel;
};


struct InstrumentEditor
{
  InstrumentEditor(Instrument_Panel *instrpanel);

  void set_coords(int x, int y);
  int handle_event(const SDL_Event &ev);
  /*void one_time_draw(SDL_Surface *screen=::render->screen);*/
  void draw(SDL_Surface *screen=::render->screen);

	void update_srcn();
	char srcn_cbuf[4];
	Text srcn_title, srcn_valtext;
	Button srcn_incbtn, srcn_decbtn;
	static int incsrcn(void *i);
	static int decsrcn(void *i);

  void update_vol();
  char vol_cbuf[4];
  Text vol_title, vol_valtext;
  Button vol_incbtn, vol_decbtn;
  static int incvol(void *i);
  static int decvol(void *i);

  void update_pan();
  char pan_cbuf[5];
  Text pan_title, pan_valtext;
  Button pan_incbtn, pan_decbtn;
  static int incpan(void *i);
  static int decpan(void *i);

  void update_finetune();
  char finetune_cbuf[5];
  Text finetune_title, finetune_valtext;
  Button finetune_incbtn, finetune_decbtn;
  static int incfinetune(void *i);
  static int decfinetune(void *i);

  // Audio Buffer Size
  struct EchoEnable
  {
    EchoEnable(InstrumentEditor *ie);
    void setCoords(int x, int y);
    void update();
    Text title;
    Checkbox checkbox;
    static int clicked(void *i);
  };
  EchoEnable echoEnable;

//////////// TABS /////////
  struct Tabs
  {
    Tabs(InstrumentEditor *ie);
    void set_coords(int x, int y);

    bool check_mouse_and_execute(int x, int y);

    void draw ();
    static int switchto_adsr(void *data);
    static int switchto_vol(void *data);
    static int switchto_pan(void *data);

    Tab adsr, vol, pan;
  } tabs;

///////////////// ADSR tab
  AdsrPanel adsrpanel;
////////////////////
private:
  Instrument_Panel *instrpanel;
};
