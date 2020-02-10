#include "SDL.h"
#include "report.h"
#include "globals.h"

uint8_t *IAPURAM = NULL;
Voice_Control voice_control;

namespace mouse
{
  int x,y;
  char show=0;
}
