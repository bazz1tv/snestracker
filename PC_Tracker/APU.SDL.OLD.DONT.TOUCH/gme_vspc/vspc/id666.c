/* hwapu - SPC music playback tools for real snes apu
 * Copyright (C) 2004-2005  Raphael Assenat <raph@raphnet.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <stdio.h>
#include <string.h>
#include "id666.h"

int read_id666(FILE *fptr, id666_tag *tag)
{
	long orig_pos = ftell(fptr);
	unsigned char istag=0;

	fseek(fptr, 0x23, SEEK_SET);
	fread(&istag, 1, 1, fptr);
	if (istag != 26)
	{
		printf("No tag\n");
		strcpy(tag->title, "");
		strcpy(tag->game_title, "");
		strcpy(tag->name_of_dumper, "");
		strcpy(tag->comments, "");
		strcpy(tag->seconds_til_fadeout, "150"); // 2.5 minutes
		return 0;
	}
	
	fseek(fptr, 0x2e, SEEK_SET);
	tag->title[32] = 0;
	fread(&tag->title, 32, 1, fptr);
	tag->game_title[32] = 0;
	fread(&tag->game_title, 32, 1, fptr);
	tag->name_of_dumper[16] = 0;
	fread(&tag->name_of_dumper, 16, 1, fptr);
	tag->comments[32] = 0;
	fread(&tag->comments, 32, 1, fptr);

	fseek(fptr, 0xa9, SEEK_SET);
	tag->seconds_til_fadeout[3] = 0;
	fread(&tag->seconds_til_fadeout, 3, 1, fptr);
	
	
	fseek(fptr, orig_pos, SEEK_SET);

	return 1;
}




