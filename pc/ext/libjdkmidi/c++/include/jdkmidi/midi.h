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


#ifndef JDKMIDI_MIDI_H
#define JDKMIDI_MIDI_H

#include "jdkmidi/world.h"

namespace jdkmidi
{
  
  typedef unsigned long MIDIClockTime;
  
  ///
  /// MIDI Status bytes
  ///
  
  enum
    {
      NOTE_OFF	=0x80, ///< Note off message with velocity
      NOTE_ON		=0x90, ///< Note on message with velocity or Note off if velocity is 0
      POLY_PRESSURE	=0xa0,  ///< Polyphonic key pressure/aftertouch with note and pressure
      CONTROL_CHANGE	=0xb0, ///< Control change message with controller number and 7 bit value
      PROGRAM_CHANGE	=0xc0, ///< Program change message with 7 bit program number
      CHANNEL_PRESSURE=0xd0, ///< Channel pressure/aftertouch with pressure
      PITCH_BEND	=0xe0, ///< Channel bender with 14 bit bender value
      SYSEX_START	=0xf0, ///< Start of a MIDI System-Exclusive message
      MTC		=0xf1, ///< Start of a two byte MIDI Time Code message
      SONG_POSITION	=0xf2, ///< Start of a three byte MIDI Song Position message
      SONG_SELECT	=0xf3, ///< Start of a two byte MIDI Song Select message
      TUNE_REQUEST	=0xf6,  ///< Single byte tune request message
      SYSEX_END	=0xf7,  ///< End of a MIDI System-Exclusive message
      RESET		=0xff,	///< 0xff is a reset byte on the serial line. We never used as reset in a MIDIMessage object,
      META_EVENT	=0xff	///< 0xff means a meta event in our internal processing.
    };
  
  
  
  ///
  /// MIDI Real Time Messages
  ///
  
  enum
    {
      TIMING_CLOCK	=0xf8,  ///< 24 timing clocks per beat 
      MEASURE_END	=0xf9,	///< Measure end byte
      START		=0xfa,  ///< Sequence start message
      CONTINUE	=0xfb, ///< Sequence continue message
      STOP		=0xfc, ///< Sequence stop message
      ACTIVE_SENSE	=0xfe	///< Active sense message
    };
  
  
  ///
  /// Controller Numbers
  ///
  
  enum
    {
      C_LSB		=0x20,	///< add this to a non-switch controller to access the LSB.
      
      C_GM_BANK	=0x00,	///< general midi bank select
      C_MODULATION	=0x01,	///< modulation
      C_BREATH	=0x02,	///< breath controller
      C_FOOT		=0x04,	///< foot controller
      C_PORTA_TIME	=0x05,	///< portamento time
      C_DATA_ENTRY	=0x06,	///< data entry value
      C_MAIN_VOLUME	=0x07,	///< main volume control
      C_BALANCE	=0x08,	///< balance control
      C_PAN		=0x0a,	///< panpot stereo control
      C_EXPRESSION	=0x0b,	///< expression control
      C_GENERAL_1	=0x10,	///< general purpose controller 1
      C_GENERAL_2	=0x11,	///< general purpose controller 2
      C_GENERAL_3	=0x12,	///< general purpose controller 3
      C_GENERAL_4	=0x13,	///< general purpose controller 4
      
      C_DAMPER	=0x40,	///< hold pedal (sustain)
      C_PORTA		=0x41,	///< portamento switch
      C_SOSTENUTO	=0x42,	///< sostenuto switch
      C_SOFT_PEDAL	=0x43,	///< soft pedal
      C_HOLD_2	=0x45,	///< hold pedal 2
      
      C_GENERAL_5	=0x50,	///< general purpose controller 5
      C_GENERAL_6	=0x51,	///< general purpose controller 6
      C_GENERAL_7	=0x52,	///< general purpose controller 7
      C_GENERAL_8	=0x53,	///< general purpose controller 8
      
      C_EFFECT_DEPTH	=0x5b,	///< external effects depth
      C_TREMELO_DEPTH	=0x5c,	///< tremelo depth
      C_CHORUS_DEPTH	=0x5d,	///< chorus depth
      C_CELESTE_DEPTH	=0x5e,	///< celeste (detune) depth
      C_PHASER_DEPTH	=0x5f,	///< phaser effect depth
      
      C_DATA_INC	=0x60,	///< increment data value
      C_DATA_DEC	=0x61,	///< decrement data value
      
      C_NONRPN_LSB	=0x62,	///< non registered parameter LSB
      C_NONRPN_MSB	=0x63,	///< non registered parameter MSB
      
      C_RPN_LSB	=0x64,	///< registered parameter LSB
      C_RPN_MSB	=0x65,	///< registered parameter MSB
      
      
      
      C_RESET		=0x79,	///< reset all controllers
      
      C_LOCAL		=0x79,	///< local control on/off
      C_ALL_NOTES_OFF	=0x7a,	///< all notes off
      C_OMNI_OFF	=0x7c,	///< omni off, all notes off
      C_OMNI_ON	=0x7d,	///< omni on, all notes off
      C_MONO		=0x7e,	///< mono on, all notes off
      C_POLY		=0x7f	///< poly on, all notes off
    };
  
  
  ///
  /// Registered Parameter Numbers:
  ///
  
  enum
    {
      RPN_BEND_WIDTH	=0x00,	///< bender sensitivity
      RPN_FINE_TUNE	=0x01,	///< fine tuning
      RPN_COARSE_TUNE =0x02	///< coarse tuning
    };
  
  
  
  ///
  /// META Event types (stored in first data byte if status==META_EVENT)
  /// These types are the same as MIDIFile meta-events,
  /// except when a meta-event is in a MIDIMessage, there is a limit
  /// of two data bytes. So the format of the meta-events in a
  /// MIDIMessage class will be different than the standard MIDIFile
  /// meta-events.
  ///
  
  enum
    {
      META_SEQUENCE_NUMBER	=0x00,	///< value=16 bits. type 2 files
      
      META_GENERIC_TEXT	=0x01,	///< value=16 bits, text item #
      META_COPYRIGHT		=0x02,	///< value=17 bits, text item #
      META_INSTRUMENT_NAME	=0x03,
      META_TRACK_NAME		=0x04,
      META_LYRIC_TEXT		=0x05,
      META_MARKER_TEXT	=0x06,
      META_CUE_TEXT		=0x07,      
     
      META_OUTPUT_CABLE	=0x21,
      META_TRACK_LOOP		=0x2E,
      META_DATA_END		=0x2f,			
      META_END_OF_TRACK	=0x2F,
      META_TEMPO		=0x51,	///< value=16 bits, tempo(bpm)*256
      META_SMPTE		=0x54,	///< what for?
      META_TIMESIG		=0x58,  ///< value=num, denom
      META_KEYSIG		=0x59,  ///< value=# of sharps/flats, major/minor
      META_BEAT_MARKER =0x7e,
      META_SEQUENCER_SPECIFIC	=0x7F,
      META_NO_OPERATION	=0x7f
      
    };

  extern	const	signed 	char	lut_msglen[16];
  extern 	const	signed	char	lut_sysmsglen[16];
  extern	const	bool	lut_is_white[12];	
  
  
  ///
  /// Message Length function. Not valid for Meta-events (0xff). Returns -1 if you need to call
  /// GetSystemMessageLength() instead.
  ///
  
  inline	signed char	GetMessageLength( unsigned char stat )
    { 
      return lut_msglen[stat>>4];	
    }

  ///
  /// Message Length of system messages. Not valid for Meta-events (0xff). Returns -1 if this message
  /// length is unknown until parsing is complete.
  ///
  
  inline	signed char	GetSystemMessageLength( unsigned char stat )
    { 
      return lut_sysmsglen[stat-0xf0];	
    }
  
  
  ///
  /// Piano key color white test
  ///
  
  inline	bool	IsNoteWhite( unsigned char note )
    { 
      return lut_is_white[ note%12 ];	
    }

  ///
  /// Piano key color black test
  ///
  
  inline	bool	IsNoteBlack( unsigned char note ) 
    { 
      return !lut_is_white[ note%12 ];	
    }
  
  
  ///
  /// Note # to standard octave conversion
  ///
  
  inline	int	GetNoteOctave( unsigned char note )
    { return (note/12)-1;	}
  
  
  
}




#endif

