#include "Debugger_Base.h"
/*Music_Player *Debugger_Base::player;
SDL_Surface *Debugger_Base::screen;
uint8_t *Debugger_Base::IAPURAM;*/

int Debugger_Base::grand_mode=GrandMode::MAIN;
//int Debugger_Base::submode=0;
Debugger_Base::Cfg Debugger_Base::g_cfg;// = { 0,0,0,0,0,0,DEFAULT_SONGTIME,0,0,0,0,0,NULL };

int Debugger_Base::g_paused = 0;
uint8_t * Debugger_Base::IAPURAM=NULL;
bool Debugger_Base::quitting=false;


Experience * Debugger_Base::exp=NULL;
Main_Window * Debugger_Base::main_window=NULL;
Dsp_Window * Debugger_Base::dsp_window=NULL;

void Debugger_Base::change_grand_mode(int mode)
{
  grand_mode = mode;
  if (mode == GrandMode::MAIN)
  {
    if (main_window)
      exp = (Experience*)main_window;
    else 
    {
      fprintf(stderr, "NO MAIN WINDOW!?!\n");
      exit(2);
    }
  }
  else if (mode == GrandMode::DSP_MAP)
  {
    if (dsp_window)
      exp = (Experience*)dsp_window;
    else 
    {
      fprintf(stderr, "NO DSPWINDOW!?!\n");
      exit(2);
    }
  }
}