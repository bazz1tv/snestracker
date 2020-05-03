#include "BrrContextMenu.h"
#include "shared/mouse.h"
#include "gme_m/Spc_Dsp_Register_Map_Interface.h" // MAX_VOICES

int BrrContextMenu::receive_event(const SDL_Event &ev)
{
	if (menu.is_active)
	{
		switch (ev.type)
		{
			case SDL_MOUSEMOTION:
				{
					mouse::x = ev.motion.x; mouse::y = ev.motion.y;
				} break;

			case SDL_KEYDOWN:
				{
					int scancode = ev.key.keysym.sym;
					switch (scancode)
					{
						case SDLK_RETURN:
							// act same as left mouse button click use a goto lol
							menu.do_thing();
							break;

						case SDLK_ESCAPE:
							menu.is_active = false;
							break;
					}
				} break;

			case SDL_MOUSEBUTTONDOWN:
				{
					switch (ev.button.button)
					{
						case SDL_BUTTON_LEFT:
							{
								menu.do_thing();
							}
							break;

						case SDL_BUTTON_RIGHT:
							break;

						default:break;
					}
				}
				break;
			default:break;
		}
		return 1;
	}
	return 0;
}

#include "gme/player/Music_Player.h"
void BrrContextMenu::update( uint16_t brr_addr )
{
  // update the addresses of those BRR fields
  for (int i = 0; i < SIZEOF_MENU; i++)
    menu_items[i].clickable_text.data = (void *) brr_addr;

  // If this DIR entry is actively loaded to a voice, we can enable the
  // BRRI download
  menu_items[SOLOSAMPLE].is_visible = false;
  menu_items[PLAYSAMPLE].is_visible = false;
  menu_items[RIPSTI].is_visible = false;

  uint8_t solo = 0; // also update the solo bits to correspond to this frame

  for (int v=0; v < MAX_VOICES; v++)
    if (get_voice_srcn_addr(v) == brr_addr)
    {
      solo |= 1 << v;
      uint8_t adsr1 = player->spc_read_dsp(v * 0x10 + dsp_reg::adsr1);
      uint8_t adsr2 = player->spc_read_dsp(v * 0x10 + dsp_reg::adsr2);

      /* The solo sample implementation is not good. Let's just disable it for now */
      /*menu_items[SOLOSAMPLE].is_visible = true;
      menu_items[SOLOSAMPLE].clickable_text.data = (void *) ( (solo << 16) | brr_addr );
      menu_items[PLAYSAMPLE].is_visible = true;
      menu_items[PLAYSAMPLE].clickable_text.data = (void *) ( (solo << 16) | brr_addr );*/
      menu_items[RIPSTI].is_visible = true;
      menu_items[RIPSTI].clickable_text.data = (void *) ( (adsr2 << 24) | (adsr1 << 16) | brr_addr );
    }
}

#include "DEBUGLOG.h"

#include "Brr.h"
extern uint8_t *IAPURAM;
uint8_t get_voice_srcn(uint8_t voice)
{
	return player->spc_read_dsp(0x10 * voice + dsp_reg::srcn);
}

uint16_t get_voice_srcn_addr(uint8_t voice)
{
	uint16_t dir = player->spc_read_dsp(dsp_reg::dir) * 0x100;
	return *(uint16_t*) &IAPURAM[dir + ( get_voice_srcn(voice) * 4)];
}

int BrrContextMenu::write_brr_to_file_callback(void *data)
{
	uintptr_t brr_addr = (uintptr_t) data;
	write_brr_to_file( (Brr *) &::IAPURAM[brr_addr]);
}

int BrrContextMenu::write_sti_to_file_callback(void *data)
{
	uintptr_t voice_addr = (uintptr_t) data;
	uint8_t adsr1 = voice_addr >> 16;
  uint8_t adsr2 = voice_addr >> 24;
	uint16_t addr = voice_addr & 0xffff;

	DEBUGLOG("adsr1: $%02x, adsr2: $%02x, addr: %04x\n", adsr1, adsr2, addr);
}

#include "BaseD.h"
int BrrContextMenu::play_sample(void *data)
{
  uintptr_t solo = (uintptr_t) data;
  if (solo)
  {
    uint8_t bits = solo;
    uint8_t i = 0xFF; // -1
    for (; bits && i < MAX_VOICES; i++)
      bits >>= 1;
    BaseD::play_sample(i);
  }
  return 0;
}

int BrrContextMenu::solo_sample(void *data)
{
  uintptr_t solo = (uintptr_t) data;
  if (solo)
    ::voice_control.solo_bits(solo);
  return 0;
}
