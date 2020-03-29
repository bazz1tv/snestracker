#include "BrrContextMenu.h"
#include "shared/mouse.h"

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



#include "gme/Spc_Dsp_Register_Map_Interface.h"
#include "gme/player/Music_Player.h"
#include "DEBUGLOG.h"

#include "Brr.h"
extern uint8_t *IAPURAM;
uint8_t get_voice_srcn(uint8_t voice)
{
	return player->spc_read_dsp(0x10 * voice + 4);
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
	uint8_t v = voice_addr >> 16;
	uint16_t addr = voice_addr & 0xffff;

	DEBUGLOG("v: %d, addr: %04x\n", v, addr);
}

