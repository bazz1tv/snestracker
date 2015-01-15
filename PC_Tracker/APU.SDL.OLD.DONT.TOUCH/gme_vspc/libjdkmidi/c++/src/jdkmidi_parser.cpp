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

#include "jdkmidi/parser.h"


#ifndef DEBUG_MDPARSER
# define DEBUG_MDPARSER	0
#endif

#if DEBUG_MDPARSER
# undef DBG
# define DBG(a)	a
#endif


namespace jdkmidi
{
  
  
  MIDIParser::MIDIParser( ushort max_sysex_size )
  {
    ENTER( "MIDIParser::MIDIParser" );
    sysex = new MIDISystemExclusive(max_sysex_size);
    state=FIND_STATUS;
  }
  
  MIDIParser::~MIDIParser()
  {
    ENTER( "MIDIParser::~MIDIParser" );
    
    delete sysex;
  }
  
  
  bool MIDIParser::Parse( uchar b, MIDIMessage *msg )
  {
    ENTER( "MIDIParser::Parse()" );
    
    //
    // No matter what state we are currently in we must deal
    // with bytes with the high bit set first.
    //
    
    if( b&0x80 )
    {
      //
      // check for system messages (>=0xf0)
      //
      
      uchar stat=(uchar)(b&0xf0);
      
      if( stat==0xf0 )
      {
        //
        // System messages get parsed by
        // ParseSystemByte()
        //
        
        return ParseSystemByte( b, msg );
      }
      else
      {
        //
        // Otherwise, this is a new status byte.
        //
        
        ParseStatusByte( b );
        return false;
      }
    }
    else
    {
      //
      // Try to parse the data byte
      //
      
      return ParseDataByte( b, msg );
    }
  }
  
  
  
  
  bool MIDIParser::ParseSystemByte( uchar b, MIDIMessage *msg )
  {
    ENTER( "MIDIParser::ParseSystemByte" );
    
    switch( b )
    {
    case RESET:
    {
      //
      // a reset byte always re-initializes our state	
      // machine.
      //
      
      state=FIND_STATUS;
      
      
      return false;
    }
    
    case SYSEX_START:
    {
      //
      // start receiving sys-ex data
      //
      
      state=SYSEX_DATA;
      
      //
      // Prepare sysex buffer.
      //
      
      sysex->Clear();
      sysex->PutEXC();
      
      return false;
    }
    
    case SYSEX_END:
    {
      //
      // We are finished receiving a sysex message.
      //
      
      
      //
      // If we were not in SYSEX_DATA mode, this
      // EOX means nothing.
      //
      
      if( state!=SYSEX_DATA )
      {
        
        return false;
      }
      
      //
      // reset the state machine
      //
      
      state=FIND_STATUS;
      
      
      //
      // finish up sysex buffer
      //
      
      sysex->PutEOX();
      
      //
      // return a MIDIMessage with status=SYSEX_START
      // so calling program can know to look at
      // the sysex buffer with GetSystemExclusive().
      //
      
      msg->SetStatus( SYSEX_START );
      
      
      return true;
    }
    
    case MTC:
    {
      //
      // Go into FIRST_OF_ONE_NORUN state.
      // this is required because MTC (F1) is not
      // allowed to be running status.
      //
      
      tmp_msg.SetStatus( MTC );
      
      state=FIRST_OF_ONE_NORUN;
      
      return false;
    }
    
    case SONG_POSITION:
    {
      //
      // This is a two data byte message, so go into
      // FIRST_OF_TWO state. 
      //
      
      state=FIRST_OF_TWO;
      
      tmp_msg.SetStatus( SONG_POSITION );
      
      return false;
    }
    
    case SONG_SELECT:
    {
      //
      // This is a one data byte message, so go into
      // the FIRST_OF_ONE state.
      //
      
      state=FIRST_OF_ONE;
      tmp_msg.SetStatus( SONG_SELECT );
      
      return false;
    }
    
    
    //
    // the one byte system messages.
    // these messages may interrupt any other message,
    // and therefore do not affect the current state or
    // running status.
    //
    
    case TUNE_REQUEST:
    case TIMING_CLOCK:
    case MEASURE_END:
    case START:
    case CONTINUE:
    case STOP:
    case ACTIVE_SENSE:
    {
      msg->SetStatus( b );
      return true;
    }
    
    default:
    {
      //
      // any other byte must be ignored.
      // It is either a communicatin error or
      // a new type of MIDI message.
      // go into FIND_STATUS state to ignore
      // any possible data bytes for this unknown message
      //
      
      state=FIND_STATUS;
      return false;
    }
    }
    
    
  }
  
  
  void MIDIParser::ParseStatusByte( uchar b )
  {
    ENTER( "MIDIParser::ParseStatusByte" );
    
    char len=GetMessageLength( b );
    
    if( len==2 )
    {
      state=FIRST_OF_ONE;
      tmp_msg.SetStatus( b );
    }
    else if( len==3 )
    {
      state=FIRST_OF_TWO;
      tmp_msg.SetStatus( b );
    }
    else
    {
      state=FIND_STATUS;
      tmp_msg.SetStatus( 0 );
    }
  }
  
  
  bool MIDIParser::ParseDataByte( uchar b, MIDIMessage *msg )
  {
    ENTER( "MIDIParser::ParseDataByte" );
    
    switch( state )
    {
    case FIND_STATUS:
    {
      //
      // just eat data bytes until we get a status byte
      //
      
      return false;
    }
    
    case FIRST_OF_ONE:
    {
      //
      // this is the only data byte of a message.
      // form the message and return it.
      //
      
      tmp_msg.SetByte1( b );
      *msg = tmp_msg;
      
      //
      // stay in this state for running status 
      //
      
      return true;
    }
    
    case FIRST_OF_TWO:
    {
      //
      // this is the first byte of a two byte message.
      // read it in. go to SECOND_OF_TWO state. do not 
      // return anything.
      //
      
      tmp_msg.SetByte1( b );
      state=SECOND_OF_TWO;
      return false;
    }
    
    case SECOND_OF_TWO:
    {
      //
      // this is the second byte of a two byte message.
      // read it in. form the message, and return in.
      // go back to FIRST_OF_TWO state to allow
      // running status.
      //
      
      tmp_msg.SetByte2( b );
      state=FIRST_OF_TWO;
      *msg=tmp_msg;
      
      return true;
    }
    
    case FIRST_OF_ONE_NORUN:
    {
      //
      // Single data byte system message, like MTC.
      // form the message, return it, and go to FIND_STATUS
      // state. Do not allow running status.
      //
      
      tmp_msg.SetByte1( b );
      state=FIND_STATUS;
      *msg=tmp_msg;
      
      return true;
    }
    
    case SYSEX_DATA:
    {
      //
      // store the byte into the sysex buffer. Stay
      // in this state. Only a status byte can
      // change our state.
      //
      
      sysex->PutByte( b );
      return false;
    }
    
    default:
    {
      //
      // UNKNOWN STATE! go into FIND_STATUS state
      //
      
      state=FIND_STATUS;
      return false;
    }
    
    }
  }
  
  
  
}
