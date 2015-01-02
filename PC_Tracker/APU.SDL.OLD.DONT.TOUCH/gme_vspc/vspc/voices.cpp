#include "voices.h"

namespace voices
{
  Uint8 was_keyed_on=0;
  Uint8 muted_toggle_protect=0; // for toggle protection during MouseMoition event
  Uint8 muted=0;

  void checkmouse_mute(Uint16 &x,Uint16 &y)
  {
    char changed=0;
    for (int i=0; i < 8; i++)
    {

      SDL_Rect *r1 = &screen_pos::voice0vol;
      SDL_Rect *r2 = &screen_pos::voice0pitch;
      
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
          voices::muted ^= (1<<i); // bit toggle
        }
      }
      else
      {
        muted_toggle_protect &= ~(1<<i);
      }
    }
    if (changed)
      player->mute_voices(voices::muted);
  }
  void checkmouse_solo(Uint16 &x,Uint16 &y)
  {
    char changed=0;
    for (int i=0; i < 8; i++)
    {
      if (x >= (screen_pos::voice0vol.x) && x <= (screen_pos::voice0vol.x+8+125) && // + 125 to cover the bar too
        y >= (screen_pos::voice0vol.y + (i*10) + 1) && y <= (screen_pos::voice0vol.y+9 + (i*10)) )
      {
          changed = 1;
          if (voices::muted == Uint8(~(1 << i)) )
          {
            muted = 0;
          }
          else voices::muted = ~(1<<i);
      }
      
    }
    if (changed)
      player->mute_voices(voices::muted);
  }
  void checkmouse(Uint16 &x, Uint16 &y, Uint8 &b)
  {
    voices::muted_toggle_protect = 0;
    if (b == SDL_BUTTON_LEFT) checkmouse_mute(x,y);
    else if (b == SDL_BUTTON_RIGHT) checkmouse_solo(x,y);
  }

  Uint8 is_muted(int i)
  {
    return voices::muted & (1<<i);
  }
  
}