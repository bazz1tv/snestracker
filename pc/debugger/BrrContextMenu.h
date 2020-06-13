#pragma once
#include "gui/Context_Menu.h"

struct BrrContextMenu
{
	enum {
		SOLOSAMPLE=0,
		PLAYSAMPLE,
		RIPBRR,
		RIPSTI,
		SIZEOF_MENU
	};

	BrrContextMenu() : menu(menu_items)
	{
		/*menu_items[SOLOSAMPLE].clickable_text.data = base;
		menu_items[RIPBRR].clickable_text.data = base;
		menu_items[RIPBRRP].clickable_text.data = base;
		menu_items[RIPBRRI].clickable_text.data = base;
		menu_items[PLAYSAMPLE].clickable_text.data = base;*/
	}

	int receive_event(const SDL_Event &ev);

  void update( uint16_t brr_addr );

	Context_Menu menu;
	Context_Menu_Item menu_items[SIZEOF_MENU+1] = 
	{
		// Solo and play are null by default. Debugger Main_Memory_Area
		// construct will load it itself (let's escape this dirtiness!!!)
		{"Solo Sample", true, &solo_sample, NULL},
		{"Play Sample", true, &play_sample, NULL},
		/* How can the callback of this part, which will be common between
		 * "users" work across all of them? I've got it. When the user calls
		 * to display this context menu, have them overwrite the data field
		 * with some sort of pointer to a struct that defines all the
		 * necessary information for exporting any of BRR, BRI, STI */

		/* After much deliberation, I have decided that a BRI file is one
		 * that, along with the sample, also includes the relative-offset loop
		 * point (uint16_t) and an accompanying ADSR1/2 setting. When SNES
		 * Tracker imports a BRI, if it detects the ADSR, it should ask the
		 * user if they would like to create a new instrument with the ADSR
		 * setting, or just import the sample. Of course, even better is to
		 * prepackage the whole thing as a SNES Tracker Instrument (STI), in
		 * which case a new instrument will automatically be allocated, along
		 * with the other feature rich settings it can handle. In light of
		 * this, BRR+ may seem a waste but it is a way to more simply
		 * communicate BRR detail that is not only SNES Tracker data. */
		{"RIP BRR",true, &write_brr_to_file_callback, NULL},
		{"Rip STI",true, &write_sti_to_file_callback, NULL, false},
		{"",false, NULL,NULL}
	};

  static int solo_sample(void *data);
  static int play_sample(void *data);

	static int write_brr_to_file_callback(void *data);
	static int write_sti_to_file_callback(void *data);
};


uint8_t get_voice_srcn(uint8_t voice);
uint16_t get_voice_srcn_addr(uint8_t voice);
