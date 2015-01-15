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

#include "jdkmidi/matrix.h"

#ifndef DEBUG_MDMATRIX
# define DEBUG_MDMATRIX	0
#endif

#if DEBUG_MDMATRIX
# undef DBG
# define DBG(a)	a
#endif


namespace jdkmidi
{
  
  
  MIDIMatrix::MIDIMatrix()
  {
    ENTER("MIDIMatrix::MIDIMatrix()");
    
    for( int channel=0; channel<16; channel++ )
    {
      channel_count[channel]=0;
      hold_pedal[channel]= false;
      for( unsigned char note=0; note<128; note++ )
        note_on_count[channel][note]=0;
    }
    
    total_count=0;
  }
  
  MIDIMatrix::~MIDIMatrix()
  {
    ENTER("MIDIMatrix::~MIDIMatrix()");
  }
  
  
  void 	MIDIMatrix::DecNoteCount( const MIDIMessage &, int channel, int note )
  {
    ENTER( "MIDIMatrix::DecNoteCount()" );
    
    if( note_on_count[channel][note]>0 )
    {
      --note_on_count[channel][note];
      --channel_count[channel];
      --total_count;
    }
  }
  
  void 	MIDIMatrix::IncNoteCount( const MIDIMessage &, int channel, int note )
  {
    ENTER( "MIDIMatrix::IncNoteCount()" );
    
    ++note_on_count[channel][note];
    ++channel_count[channel];
    ++total_count;
  }
  
  void	MIDIMatrix::OtherMessage( const MIDIMessage & )
  {
    ENTER( "MIDIMatrix::OtherMessage()" );
    
  }
  
  
  bool	MIDIMatrix::Process( const MIDIMessage &m )
  {
    ENTER( "MIDIMatrix::Process()" );
    
    bool status=false;
    
    if( m.IsChannelMsg() )
    {
      int channel=m.GetChannel();
      int note=m.GetNote();
      
      if( m.IsAllNotesOff() )
      {
        ClearChannel( channel );
        status=true;
      }
      else
        if( m.IsNoteOn() )
        {
          if( m.GetVelocity()!=0 )
            IncNoteCount( m, channel, note );
          else
            DecNoteCount( m, channel, note );
          status=true;
        }
        else
          if( m.IsNoteOff() )
          {
            DecNoteCount( m, channel, note );
            status=true;
          }
          else
            if( m.IsControlChange() && m.GetController()==C_DAMPER )
            {
              if( m.GetControllerValue() & 0x40 )
              {
                hold_pedal[channel]=true;
              }
              else
              {
                hold_pedal[channel]=false;
              }
            }
            else
              OtherMessage( m );
    }
    return status;
  }
  
  void	MIDIMatrix::Clear()
  {
    ENTER( "MIDIMatrix::Clear()" );
    
    for( int channel=0; channel<16; ++channel )
    {
      ClearChannel( channel );
    }
    total_count=0;
  }
  
  void	MIDIMatrix::ClearChannel( int channel )
  {
    ENTER( "MIDIMatrix::ClearChannel()" );
    
    for( int note=0; note<128; ++note )
    {
      total_count -= note_on_count[channel][note];
      note_on_count[channel][note]=0;
    }
    channel_count[channel]=0;
    hold_pedal[channel]=0;
  }
  
  
  
}
