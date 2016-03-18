/*
 *  libjdkmidi-2004 C++ Class Library for MIDI
 *
 *  Copyright (C) 2004  J.D. Koftinoff Software, Ltd.
 *  www.jdkoftinoff.com
 *  jeffk@jdkoftinoff.com
 *
 *  *** RELEASED UNDER THE GNU GENERAL PUBLIC LICENSE (GPL) April 27, 2004 ***
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/*
**	Copyright 1986 to 1998 By J.D. Koftinoff Software, Ltd.
**
**	All rights reserved.
**
**	No one may duplicate this source code in any form for any reason
**	without the written permission given by J.D. Koftinoff Software, Ltd.
**
*/


#include "jdkmidi/world.h"

#include "jdkmidi/midi.h"

namespace jdkmidi
{
  
  
  const signed char lut_msglen[16] =
  {
    0,0,0,0,0,0,0,0,
    3,	// 0x80=note off, 3 bytes
    3,	// 0x90=note on, 3 bytes
    3, 	// 0xa0=poly pressure, 3 bytes
    3,	// 0xb0=control change, 3 bytes
    2,	// 0xc0=program change, 2 bytes
    2,	// 0xd0=channel pressure, 2 bytes
    3,	// 0xe0=pitch bend, 3 bytes
    -1	// 0xf0=other things. may vary.
  };
  
  const signed char lut_sysmsglen[16] =
  {
    -1,	// 0xf0=sysex start. may vary
    2,	// 0xf1=MIDI Time Code. 2 bytes
    3,	// 0xf2=MIDI Song position. 3 bytes
    2,	// 0xf3=MIDI Song Select. 2 bytes.
    0,	// 0xf4=undefined
    0,	// 0xf5=undefined
    1,	// 0xf6=TUNE Request
    0,	// 0xf7=sysex end.
    1,	// 0xf8=timing clock. 1 byte
    1,	// 0xf9=proposed measure end?
    1,	// 0xfa=start. 1 byte
    1,	// 0xfb=continue. 1 byte
    1,	// 0xfc=stop. 1 byte
    0,	// 0xfd=undefined
    1,	// 0xfe=active sensing. 1 byte
    3	// 0xff= not reset, but a META-EVENT, which is always 3 bytes
  };
  
  
  const bool	lut_is_white[12] =
  {
//
//	C C#  D D#  E    F F# G G# A A# B
//
    1,0,  1,0,  1,   1,0, 1,0, 1,0, 1
  };
  
  
  
}
