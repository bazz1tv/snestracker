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
#include "jdkmidi/world.h"
#include "jdkmidi/driver.h"

namespace jdkmidi
{
  
  
  MIDIDriver::MIDIDriver( int queue_size ) 
    : 	
    in_queue(queue_size),
    out_queue(queue_size),
    in_proc(0),
    out_proc(0),
    thru_proc(0),
    thru_enable(false),
    tick_proc(0)
  {
    
  } 
  
  MIDIDriver::~MIDIDriver() 
  {
    
  } 
  
  void MIDIDriver::Reset()
  {
    in_queue.Clear();
    out_queue.Clear();
    out_matrix.Clear();
  }
  
  void MIDIDriver::AllNotesOff( int chan ) 
  {
    MIDITimedBigMessage msg;
    
    // send a note off for every note on in the out_matrix
    
    if( out_matrix.GetChannelCount(chan)>0 )
    {
      for( int note=0; note<128; ++note )
      {
        while( out_matrix.GetNoteCount(chan,note)>0 )
        {
          // make a note off with note on msg, velocity 0
          msg.SetNoteOn( (unsigned char)chan,
                         (unsigned char)note, 0 );
          
          OutputMessage( msg );
        }
      }
    }
    
    msg.SetControlChange(chan,C_DAMPER,0 );
    OutputMessage( msg );
    
    msg.SetAllNotesOff( (unsigned char)chan );
    OutputMessage( msg );
    
  } 
  
  void MIDIDriver::AllNotesOff() 
  {
    for( int i=0; i<16; ++i )
    {
      AllNotesOff(i);
    }
  } 
  
  bool MIDIDriver::HardwareMsgIn( MIDITimedBigMessage &msg ) 
  {
    // put input midi messages thru the in processor
    
    if( in_proc )
    {
      if( in_proc->Process( &msg )==false )
      {
        // message was deleted, so ignore it.
        return true;
      }
    }
    
    // stick input into in queue
    
    if( in_queue.CanPut() )
    {
      in_queue.Put( msg );
    }
    else
    {
      return false;
    }
    
    
    // now stick it through the THRU processor
    
    if( thru_proc )
    {
      if( thru_proc->Process( &msg )==false )
      {
        // message was deleted, so ignore it.
        return true;
      }
    }
    
    
    if( thru_enable )
    {
      // stick this message into the out queue so the tick procedure
      // will play it out asap
      
      if( out_queue.CanPut() )
      {
        out_queue.Put( msg );
      }
      else
      {
        return false;
      }
    }
    
    return true;
  } 
  
  void MIDIDriver::TimeTick( unsigned long sys_time ) 
  {
    // run the additional tick procedure if we need to
    if( tick_proc )
    {
      tick_proc->TimeTick( sys_time );
    }		
    
    // feed as many midi messages from out_queu to the hardware out port
    // as we can
    
    while( out_queue.CanGet() )
    {
      // use the Peek() function to avoid allocating memory for
      // a duplicate sysex
      
      if( HardwareMsgOut( *(out_queue.Peek() ) )==true )
      {
        // ok, got and sent a message - update our out_queue now
        out_queue.Next();
      }
      else
      {
        // cant send any more, stop now.
        break;
      }
      
    }
    
  } 
  
}
