#include "Debugger_Base.h"
/*Music_Player *Debugger_Base::player;
SDL_Surface *Debugger_Base::screen;
uint8_t *Debugger_Base::IAPURAM;*/

int Debugger_Base::mode=0;
int Debugger_Base::submode=0;
Debugger_Base::Cfg Debugger_Base::g_cfg;// = { 0,0,0,0,0,0,DEFAULT_SONGTIME,0,0,0,0,0,NULL };

int Debugger_Base::g_paused = 0;
uint8_t * Debugger_Base::IAPURAM=NULL;
//Music_Player * Debugger_Base::player=NULL;