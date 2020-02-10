#include "SDL.h"
#include "globals.h"

bool quitting = false;
My_Nfd nfd;
Experience *cur_exp = NULL;
Voice_Control voice_control;
uint8_t *IAPURAM = NULL;

namespace mouse
{
  int x,y;
  char show=0;
}
