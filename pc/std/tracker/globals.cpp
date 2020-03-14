#include "SDL.h"
#include "globals.h"
#include "Tracker.h"

bool quitting = false;
My_Nfd nfd;
Experience *cur_exp = NULL;
Voice_Control voice_control;
uint8_t *IAPURAM = NULL;
track_info_t tag;
MouseCursors *mousecursors = NULL;

Tracker *tracker = NULL;
