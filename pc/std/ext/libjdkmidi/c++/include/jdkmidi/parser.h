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

#ifndef JDKMIDI_PARSER_H
#define JDKMIDI_PARSER_H

#include "jdkmidi/midi.h"
#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"

namespace jdkmidi
{
  
  class  MIDIParser
    {
    public:
      MIDIParser( ushort max_sysex_size=384 );
      virtual		~MIDIParser();
      
      void		Clear()
        {
          state=FIND_STATUS;
        }
      
      virtual	bool	Parse( uchar b, MIDIMessage *msg );
      
      MIDISystemExclusive *GetSystemExclusive() const 	{ return sysex;	}
      
    protected:
      
      //
      // The states used for parsing messages.
      //
      
      enum State
        {
          FIND_STATUS,		// ignore data bytes
          FIRST_OF_ONE,		// read first data byte of a one data byte msg
          FIRST_OF_TWO,		// read first data byte of two data byte msg
          SECOND_OF_TWO,		// read second data byte of two data byte msg
          FIRST_OF_ONE_NORUN,	// read one byte message, do not allow 
          // running status (for MTC)
          SYSEX_DATA		// read sysex data byte
        };
      
      MIDIMessage tmp_msg;
      MIDISystemExclusive *sysex;	
      State 	state;
      
      bool	ParseSystemByte( uchar b, MIDIMessage *msg );
      bool	ParseDataByte( uchar b, MIDIMessage *msg );
      void	ParseStatusByte( uchar b );
    };
  
  
}

#endif


