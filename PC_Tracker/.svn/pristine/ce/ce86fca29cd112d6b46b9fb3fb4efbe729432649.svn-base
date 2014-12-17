#ifndef __MISC_H__
#define __MISC_H__

/* Misc.h

*** Put miscellaneous Routines that may be used by several different files HERE

*/

#include "Globals.h"

void FillRectAlpha(SDL_Renderer *renderer, int x, int y, int w, int h, Uint8 r,Uint8 g,Uint8 b,Uint8 a);
void FillRectAlpha(SDL_Renderer *renderer, int x, int y, int w, int h, Uint32 rgba);
void FillRect(SDL_Renderer *renderer, int x, int y, int w, int h, Uint8 r,Uint8 g,Uint8 b);




/// Toggle fullScreen
int SDL_ToggleFS(SDL_Window*);

/// More english version of 
bool MouseCollidesWith(SDL_Rect *Crect);

/// The Universal Event Handler handles some events that should be handled in ANY event handler,
/// like Window Resize events for instance
void UniversalEventHandler(SDL_Event*);
//

#endif