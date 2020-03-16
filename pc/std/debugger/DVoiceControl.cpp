#include "DVoiceControl.h"
#include "Screen.h"
#include "gme/player/Music_Player.h"

void DVoiceControl::checkmouse_mute(Uint16 &x,Uint16 &y)
{
  char changed=0;
  SDL_Rect *r1 = &Screen::voice0vol;
  SDL_Rect *r2 = &Screen::voice0pitch;
  for (int i=0; i < 8; i++)
  { 
    // if we click on the number of the voice, whether in the Pitch or Volume section
    if ( (x >= (r1->x) && x <= (r1->x + r1->w) &&
      y >= (r1->y + (i*r1->h)) && y <= (r1->y+((i*r1->h))+r1->h-1) )  ||
      (
        (x >= (r2->x) && x <= (r2->x+r2->w)) &&
          (y >= (r2->y + (i*r2->h)) && y <= (r2->y+(i*r2->h))+r2->h-1 )
      ) )
    {
      // if the voice isnt being toggle protected
      if (!(muted_toggle_protect & (1 << i)) )
      {
        muted_toggle_protect |= 1<<i;
        changed = 1;
        muted ^= (1<<i); // bit toggle
      }
    }
    else
    {
      muted_toggle_protect &= ~(1<<i);
    }
  }
  if (changed)
    ::player->mute_voices(muted);
}

void DVoiceControl::checkmouse_solo(Uint16 &x,Uint16 &y)
{
  char changed=0;
  SDL_Rect *r1 = &Screen::voice0vol;
  SDL_Rect *r2 = &Screen::voice0pitch;
  for (int i=0; i < 8; i++)
  {
    if ( (x >= (r1->x) && x <= (r1->x + r1->w) &&
      y >= (r1->y + (i*r1->h)) && y <= (r1->y+((i*r1->h))+r1->h-1) )  ||
      (
        (x >= (r2->x) && x <= (r2->x+r2->w)) &&
          (y >= (r2->y + (i*r2->h)) && y <= (r2->y+(i*r2->h))+r2->h-1 )
      ) )
    {
        changed = 1;
        if (muted == Uint8(~(1 << i)) )
        {
          muted = 0;
        }
        else muted = ~(1<<i);
    }
    
  }
  if (changed)
    ::player->mute_voices(muted);
}

void DVoiceControl::checkmouse(Uint16 &x, Uint16 &y, int b)
{
  muted_toggle_protect = 0;
  if (b == SDL_BUTTON_LEFT) checkmouse_mute(x,y);
  else if (b == SDL_BUTTON_RIGHT) checkmouse_solo(x,y);
}
