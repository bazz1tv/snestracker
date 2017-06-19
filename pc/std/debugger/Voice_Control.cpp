#include "Voice_Control.h"
#include "gme/player/Music_Player.h"

Voice_Control::Voice_Control()
{

}

void Voice_Control::checkmouse_mute(Uint16 &x,Uint16 &y)
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
    player->mute_voices(muted);
}
void Voice_Control::checkmouse_solo(Uint16 &x,Uint16 &y)
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
    player->mute_voices(muted);
}

void Voice_Control::mute(uint8_t i)
{
  muted = i;
}

void Voice_Control::solo(uint8_t i)
{
  muted = ~(1<<(i-1));
  player->mute_voices(muted);
}

void Voice_Control::solo_bits(uint8_t i)
{
  //fprintf(stderr, "muted = %02X, ~i = %02X", muted, ~i);
  if (muted == (uint8_t)(~i))
  {
    //fprintf(stderr, "deede");
    muted = 0;
  }
  else muted = ~(i);
  player->mute_voices(muted);
}

void Voice_Control::toggle_solo(uint8_t i)
{
  if (muted == Uint8(~(1 << (i-1) ) ) )
  {
    muted = 0;
  }
  else muted = ~(1<<(i-1));
  player->mute_voices(muted);
}
void Voice_Control::toggle_mute(uint8_t m)
{
  assert (m>0 && m < 9 );
  muted ^= 1<<(m-1);
  player->mute_voices(muted);
}
void Voice_Control::mute_all()
{
  player->mute_voices(muted=0);
}
void Voice_Control::toggle_mute_all()
{
  if (!muted)
    muted = 0xff;
  else muted = 0;
  //player->mute_voices(muted^=0xff);
  player->mute_voices(muted);
}
void Voice_Control::unmute_all()
{
  player->mute_voices(muted=0);
}
void Voice_Control::checkmouse(Uint16 &x, Uint16 &y, int b)
{
  muted_toggle_protect = 0;
  if (b == SDL_BUTTON_LEFT) checkmouse_mute(x,y);
  else if (b == SDL_BUTTON_RIGHT) checkmouse_solo(x,y);
}

Uint8 Voice_Control::is_muted(int i)
{
  return muted & (1<<i);
}

