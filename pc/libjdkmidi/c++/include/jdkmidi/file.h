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


#ifndef JDKMIDI_FILE_H
#define JDKMIDI_FILE_H

#include "jdkmidi/midi.h"
#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"
#include "jdkmidi/tempo.h"


namespace jdkmidi
{
  
//
// The MIDIFile class contains definitions and utilities to deal with
// reading and writing midi files.
//
  
  
  const unsigned long _MThd=OSTYPE('M','T','h','d');
  const unsigned long  _MTrk=OSTYPE('M','T','r','k');
  
  class  MIDIFile 
    {
    public:
      
      MIDIFile();
      virtual	~MIDIFile();
      
      struct MIDIFileChunk
      {
        unsigned long id;
        unsigned long length;
      };
      
      struct MIDIFileHeader
      {
        short format;
        short ntrks;
        short division;
      };
      
      //
      // define all the different meta event message types.
      //
      
      enum
        {
          MF_SEQUENCE_NUMBER	=0,
          MF_TEXT_EVENT		=1,
          MF_COPYRIGHT		=2,
          MF_TRACK_NAME		=3,
          MF_INSTRUMENT_NAME	=4,
          MF_LYRIC		=5,
          MF_MARKER		=6,
          MF_CUE_POINT		=7,
          MF_GENERIC_TEXT_8	=8,
          MF_GENERIC_TEXT_9	=9,
          MF_GENERIC_TEXT_A	=0xA,
          MF_GENERIC_TEXT_B	=0xB,
          MF_GENERIC_TEXT_C	=0xC,
          MF_GENERIC_TEXT_D	=0xD,
          MF_GENERIC_TEXT_E	=0xE,
          MF_GENERIC_TEXT_F	=0xF,
          
          MF_OUTPUT_CABLE		=0x21,
          MF_TRACK_LOOP		=0x2E,
          MF_END_OF_TRACK		=0x2F,
          MF_TEMPO		=0x51,
          MF_SMPTE		=0x54,
          MF_TIMESIG		=0x58,
          MF_KEYSIG		=0x59,
          MF_SEQUENCER_SPECIFIC	=0x7F
        };
      
      
      //
      // ConvertTempoToFreq() returns the frequency of the required
      // tempo clock
      //
      
      static	unsigned long	ConvertTempoToFreq( 
        short division,
        MIDITempo &tempo 
        );
      
      //
      // Convert a four byte number to an unsigned long.
      //
      
      static	unsigned long   To32Bit( unsigned char	a, unsigned char b, unsigned char c, unsigned char d )	
        {
          return 	  ((unsigned long)a << 24)
            + ((unsigned long)b << 16)
            + ((unsigned long)c << 8 )
            + ((unsigned long)d << 0 );
        } 
      
      
      //
      // Convert a two byte number to an unsigned short
      //
      
      static	unsigned short  To16Bit( unsigned char a, unsigned char b )	
        {
          return 	  (unsigned short)(((unsigned short)a << 8)
                                           + ((unsigned short)b << 0));
        } 
      
      static	unsigned long	ReadVariableLengthNumber( unsigned char **in ); 
      
      static	unsigned char *	WriteVariableLengthNumber( unsigned long num, unsigned char *out );
      
    }; 
  
}

#endif

