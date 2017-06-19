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
#ifndef _id666_h__
#define _id666_h__


typedef struct
{
	char title[33]; // 32 in file
	char game_title[33]; // 32 in file
	char name_of_dumper[17]; // 16 in file
	char comments[33]; // 32 in file

	//unsigned char date[12]; // 11 in file
	char seconds_til_fadeout[4]; // 3 in file
	//unsigned char length_of_fadeout[6]; // 5 in file, milliseconds
	//unsigned char artist[33]; // 32 in file
	//unsigned char chn_dis;
	//unsigned char emulator_used; // 0 unknown, 1 zsnes, 2 snes9x

} id666_tag;

/* returns true if a tag is found. tag is filled with tag
 * information.
 * 
 * returns false if a tag is not found. tag is filled
 * with empty strings and default values.
 */
int read_id666(FILE *fptr, id666_tag *tag);

#endif // _id666_h__

