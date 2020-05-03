#include "Voice_Control.h"
#include "gme/player/Music_Player.h"

Uint8 Voice_Control::muted_toggle_protect = 0;
Uint8 Voice_Control::muted = 0;

int Voice_Control::mute_solo_voice(void *data)
{
	uintptr_t voicenum_plus_click = (uintptr_t)data;
	bool is_right_click = (voicenum_plus_click & 0x08);
	uintptr_t voicenum = voicenum_plus_click & 0x07;
	//fprintf(stderr, "Voice#: %d,", voicenum);
	if (is_right_click)
	{
		toggle_solo(voicenum+1);
		//fprintf(stderr, "solo\n");
	}
	else
	{
		toggle_mute(voicenum+1);
		//fprintf(stderr, "mute\n");
	}
	return 0;
}

void Voice_Control::mute(uint8_t i)
{
  muted = i;
}

void Voice_Control::solo(uint8_t i)
{
  muted = ~(1<<(i-1));
  ::player->mute_voices(muted);
}

void Voice_Control::solo_bits(uint8_t i)
{
  if (muted == (uint8_t)(~i))
  {
    muted = 0;
  }
  else muted = ~(i);
  ::player->mute_voices(muted);
}

void Voice_Control::toggle_solo(uint8_t i)
{
  if (muted == Uint8(~(1 << (i-1) ) ) )
  {
    muted = 0;
  }
  else muted = ~(1<<(i-1));
  ::player->mute_voices(muted);
}
void Voice_Control::toggle_mute(uint8_t m)
{
  assert (m>0 && m < 9 );
  muted ^= 1<<(m-1);
  ::player->mute_voices(muted);
}
void Voice_Control::mute_all()
{
  ::player->mute_voices(muted=0);
}
void Voice_Control::toggle_mute_all()
{
  if (!muted)
    muted = 0xff;
  else muted = 0;
  ::player->mute_voices(muted);
}
void Voice_Control::unmute_all()
{
  ::player->mute_voices(muted=0);
}

Uint8 Voice_Control::is_muted(int i)
{
  return muted & (1<<i);
}

