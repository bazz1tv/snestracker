#include "Main_Memory_Area.h"
#include "gme/player/Music_Player.h"
#include "report.h"
#include "gme_m/Spc_Dsp_Register_Map_Interface.h"
#include <nfd.h>
#include "DEBUGLOG.h"
#include "SdlNfd.h"
#include "shared/Brr.h" // for brr header strings and file extensions
#include "shared/Voice_Control.h"

using namespace SdlNfd;


void Main_Memory_Area::spawnbrrcontextmenu(int x, int y)
{
	addr_when_user_right_clicked = mouse_addr;

	switch (check_brr(&addr_when_user_right_clicked))
	{
		case NOT_A_SAMPLE:
			::brrcontext.menu_items[BrrContextMenu::SOLOSAMPLE].is_visible = false;
			::brrcontext.menu_items[BrrContextMenu::PLAYSAMPLE].is_visible = false;
			::brrcontext.menu_items[BrrContextMenu::RIPBRR].is_visible = false;
			::brrcontext.menu_items[BrrContextMenu::RIPSTI].is_visible = false;
			::brrcontext.menu.is_active = false;
			break;

		case PLAIN_SAMPLE:
			::brrcontext.menu_items[BrrContextMenu::SOLOSAMPLE].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::PLAYSAMPLE].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::RIPBRR].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::RIPSTI].is_visible = true;
			break;
		case LOOP_SAMPLE:
			::brrcontext.menu_items[BrrContextMenu::SOLOSAMPLE].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::PLAYSAMPLE].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::RIPBRR].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::RIPSTI].is_visible = true;
			break;
		case CLICKED_ON_LOOP_ONLY:
			// notify user
			::brrcontext.menu_items[BrrContextMenu::SOLOSAMPLE].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::PLAYSAMPLE].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::RIPBRR].is_visible = true;
			::brrcontext.menu_items[BrrContextMenu::RIPSTI].is_visible = true;
			break;
	}

  ::brrcontext.update(brr_start);
	::brrcontext.menu.preload(x, y);
	::brrcontext.menu.activate();
}

static int write_loop_info_to_file(Main_Memory_Area *mma, SDL_RWops *file)
{
	Uint16 offset;
	SDL_RWwrite(file, &mma->is_looped_sample, 1, 1);
	SDL_RWwrite(file, &mma->is_loop_external, 1, 1);
	if (mma->is_looped_sample && ! mma->is_loop_external)
	{
		offset = mma->brr_loop_start - mma->brr_start;
		SDL_WriteLE16(file, offset);
	}
	else
	{
		offset = 0x0000;
		SDL_WriteLE16(file, offset);
	}
	return 0;
}

int write_brri_to_file(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data; 

	if (SdlNfd::get_file_handle("w", BRRI_FILE_EXTENSION) == NFD_OKAY)
	{
		if (outPath !=NULL)
			fprintf(stderr, "%s\n", outPath);

		SDL_RWwrite(file, BRRI_MAGIC_STR, strlen(BRRI_MAGIC_STR), 1);
		//
		write_loop_info_to_file(mma, file);
		// ADSR 1
		Uint8 tmpb = ::player->spc_read_dsp(mma->corresponding_voice*0x10 + dsp_reg::adsr1);
		SDL_RWwrite(file, &tmpb, 1, 1);
		// ADSR 2
		tmpb = ::player->spc_read_dsp(mma->corresponding_voice*0x10 + dsp_reg::adsr2);
		SDL_RWwrite(file, &tmpb, 1, 1);
		//BRR Sample
		SDL_RWwrite(file, &::IAPURAM[mma->brr_start], mma->brr_end - mma->brr_start + 1, 1);
		//
		if (mma->is_looped_sample && mma->is_loop_external) // write the external loop sample
			SDL_RWwrite(file, &::IAPURAM[mma->brr_loop_start], mma->brr_loop_end - mma->brr_loop_start + 1, 1);
	}
  return 0;
}

int write_brrp_to_file(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data; 

	if (SdlNfd::get_file_handle("w", BRRP_FILE_EXTENSION) == NFD_OKAY)
	{
		if (outPath !=NULL)
			fprintf(stderr, "%s\n", outPath);

		SDL_RWwrite(file, BRRP_MAGIC_STR, strlen(BRRP_MAGIC_STR), 1);
		write_loop_info_to_file(mma, file);
		//BRR Sample
		SDL_RWwrite(file, &::IAPURAM[mma->brr_start], mma->brr_end - mma->brr_start + 1, 1);
		/* 1 Byte boolean "is_loop_external" : 0x00 or 0xff - no or yes
			 if no.: next and final byte is 16bit offset into previous sample where the loop point is.
			 if yes: Next BRR Sample is provided */
		//

		if (mma->is_looped_sample && mma->is_loop_external) // write the external loop sample
			SDL_RWwrite(file, &::IAPURAM[mma->brr_loop_start], mma->brr_loop_end - mma->brr_loop_start + 1, 1);
	}

  return 0;
}

int write_plain_brr_to_file(void *data)
{
  Main_Memory_Area *mma = (Main_Memory_Area*)data;

	if (SdlNfd::get_file_handle("w", BRR_FILE_EXTENSION) == NFD_OKAY)
	{
		if (outPath !=NULL)
			fprintf(stderr, "%s\n", outPath);
		SDL_RWwrite(file, &::IAPURAM[mma->brr_start], mma->brr_end - mma->brr_start + 1, 1);
	}
  return 0;
}

Main_Memory_Area::Main_Memory_Area(Mouse_Hexdump_Area *mouse_hexdump_area, uint16_t *dir) :
mouse_hexdump_area(mouse_hexdump_area), dir(dir)
{
}

void Main_Memory_Area::set_addr(int i)
{
  if (memcursor.is_active())
  {
    report::restore_color(addr_being_edited);
    memcursor.start_timer();
  }
  Memory::set_addr(i);
  // shared variables, but not all of them
  mouse_hexdump_area->update_editing_address();
}
void Main_Memory_Area::set_addr_from_cursor(int x, int y)
{
  x-= MEMORY_VIEW_X;
  y -= MEMORY_VIEW_Y;
  x /= 2;
  y /= 2;
  set_addr(y*256+x);
}

void Main_Memory_Area::lock(char l/*=1*/, int x/*=0*/, int y/*=0*/, uint8_t rx/*=0*/, uint8_t ry/*=0*/)
{
  
  locked = l;
  if (locked)
  {
     res_y = 0;
    if (mouse_hexdump_area->submode != Mouse_Hexdump_Area::EASY_EDIT)
    {
      if (rx || ry)
      {
        res_x = rx; res_y = ry;
      }
      else
      {
        address_remainder = address % 8;
        add_addr(-address_remainder);
        res_x = 0;
      }
    }

    cursor.start_timer();

    memcursor.start_timer();
    report::restore_color(addr_being_edited);
    addr_being_edited = address+(res_y*8)+res_x;
  }
  else
  {
    DEBUGLOG("unlocked form lcok");
    if (x && y)
    {
      if (  x >= MEMORY_VIEW_X && 
                  x < MEMORY_VIEW_X + 512 &&
                  y >= MEMORY_VIEW_Y &&
                  y < MEMORY_VIEW_Y + 512 )
      {
        set_addr_from_cursor(x, y);
      }
    }
    
    report::restore_color(addr_being_edited);
    //mode = MODE_NAV;
    cursor.stop_timer();
    memcursor.stop_timer();
  }
  
}
void Main_Memory_Area::toggle_lock(int x/*=0*/, int y/*=0*/)
{
  lock(!locked, x, y);
}

void Main_Memory_Area::unlock()
{
  DEBUGLOG("unlock");
  lock(0);
}

int Main_Memory_Area::check_brr(uint16_t *address)
{
	is_looped_sample = false;
	is_loop_external = false;
	bool no_lower_loop_found=false;

	assert (address != 0);
	Uint8 lowest_srcn_index=0x0;
	Uint8 lowest_loop_index=0x0;

	// get closest SRCN address before clicked address
	uint16_t lowest_closest_srcn_address=0xffff, lowest_offset=0xffff;
	uint16_t lowest_closest_brrloopstart_address_from_click=0xffff;
	uint16_t lowest_offset2=0xffff;
	for (int i=0; i < MAX_SRCN_ENTRIES; i++)
	{
		Spc_Report::Src *src = &Spc_Report::src[i];
		if (src->brr_start == 0xffff) continue;

		if (src->brr_start < *address)
		{
			uint16_t offset = *address - src->brr_start;
			if (offset < lowest_offset)
			{
				lowest_closest_srcn_address=src->brr_start;

				lowest_srcn_index=i;
				lowest_offset = offset;
			}
		}
		if (src->brr_loop_start < *address)
		{
			uint16_t offset = *address - src->brr_loop_start;
			if (offset < lowest_offset2)
			{
				lowest_loop_index = i;
				lowest_closest_brrloopstart_address_from_click = src->brr_loop_start;
				lowest_offset2 = offset;
			}
		}

	}

	// quit out of we clicked before any SAMPLE.. i mean that's an idiot click right there...
	if (lowest_closest_srcn_address == 0xffff)
	{
		fprintf(stderr, "no appropriate SRCN was found :(\n");
		return NOT_A_SAMPLE;
	}

	// solo the voices that are using this sample RIGHT NOW
	// if no voice is using this sample, just keep them all going..
	for (int x=0; x < MAX_VOICES; x++)
	{
		if (get_voice_srcn_addr(x) == lowest_closest_srcn_address)
		{
			corresponding_voice = x;
		}
	}


	// get closest LOOP Address before clicked address
	if (lowest_closest_brrloopstart_address_from_click == 0xffff)
	{
		no_lower_loop_found = true;
		fprintf(stderr, "no appropriate LOOP point was found :(\n");
	}

	// Find closest BRR end block after closest SRCN
	uint16_t lowest_closest_brrend_address_from_srcn = 0xffff;

	uint16_t p = lowest_closest_srcn_address;
	while(1)
	{
		if (::IAPURAM[p] & 1)
		{
			if (::IAPURAM[p] & 2)
				is_looped_sample=true;
			p+=8;
			break;
		}
		p+=9;
	}
	lowest_closest_brrend_address_from_srcn = p;

	if (lowest_closest_brrend_address_from_srcn == 0xffff)
	{
		fprintf(stderr, "no appropriate BRR_END after SRCN was found :(\n");
	}

	// get loop end
	uint16_t lowest_closest_brrend_address_from_loop = 0xffff;

	p = Spc_Report::src[lowest_loop_index].brr_loop_start;
	while(1)
	{
		if (::IAPURAM[p] & 1)
		{
			p+=8;
			break;
		}
		p+=9;
	}

	lowest_closest_brrend_address_from_loop = p;
	Spc_Report::src[lowest_loop_index].brr_loop_end = p;

	if (lowest_closest_brrend_address_from_loop == 0xffff)
	{
		fprintf(stderr, "no appropriate Loop BRR_END was found :(\n");
	}

	if (!no_lower_loop_found && lowest_closest_brrloopstart_address_from_click > lowest_closest_brrend_address_from_srcn)
	{
		fprintf(stderr,"You clicked on an external Loop sample\n");
		brr_start = Spc_Report::src[lowest_loop_index].brr_start;
		brr_end = Spc_Report::src[lowest_loop_index].brr_end;
		brr_loop_start = Spc_Report::src[lowest_loop_index].brr_loop_start;
		brr_loop_end = Spc_Report::src[lowest_loop_index].brr_loop_end;
		return CLICKED_ON_LOOP_ONLY;
	}
	else
	{
		Spc_Report::src[lowest_srcn_index].brr_end = lowest_closest_brrend_address_from_srcn;
		fprintf(stderr, "BRR @ 0x%04X-0x%04X; ",
				lowest_closest_srcn_address,
				lowest_closest_brrend_address_from_srcn);
		brr_start = lowest_closest_srcn_address;
		brr_end = lowest_closest_brrend_address_from_srcn; //inclusive
		if (is_looped_sample == true)
		{
			brr_loop_start = Spc_Report::src[lowest_srcn_index].brr_loop_start;
			brr_loop_end = Spc_Report::src[lowest_srcn_index].brr_loop_end;

			if (brr_loop_start > brr_end)
				is_loop_external = true;
			else is_loop_external = false;

			return LOOP_SAMPLE;
		}
		else
		{
			brr_loop_start=0xffff;
			brr_loop_end = 0xffff;
			return PLAIN_SAMPLE;
		}
	}

	fprintf(stderr, "lowest_closest_srcn_address = %04X\n", lowest_closest_srcn_address);
	fprintf(stderr, "lowest_closest_brrend_address_from_srcn = %04X\n", lowest_closest_brrend_address_from_srcn);
	fprintf(stderr, "lowest_closest_brrloopstart_address_from_click = %04X\n", lowest_closest_brrloopstart_address_from_click);
}
