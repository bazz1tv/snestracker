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
#include "jdkmidi/process.h"

namespace jdkmidi
{
  
  
  MIDIProcessor::MIDIProcessor()
  {
  }
  
  MIDIProcessor::~MIDIProcessor()
  {
  }
  
  
  
  
  MIDIMultiProcessor::MIDIMultiProcessor( int num ) 
    :
    processors( new MIDIProcessor *[num] ),
    num_processors( num )
  {   
    for( int i=0; i<num_processors; ++i )
    {
      processors[i] = 0;
    }
  }
  
  MIDIMultiProcessor::~MIDIMultiProcessor()
  {
    delete [] processors;
  }
  
  
  
  bool MIDIMultiProcessor::Process( MIDITimedBigMessage *msg )
  {
    for( int i=0; i<num_processors; ++i )
    {
      if( processors[i] )
      {
        if( processors[i]->Process( msg )==false )
        {
          return false;
        }			
      }
      
    }	
    
    return true;
  }
  
  
  
  
  
  MIDIProcessorTransposer::MIDIProcessorTransposer()
  {
    for( int i=0; i<16; ++i )
    {
      trans_amount[i] = 0;
    }
    
  }
  
  MIDIProcessorTransposer::~MIDIProcessorTransposer()
  {
  }
  
	
  void MIDIProcessorTransposer::SetAllTranspose( int val )
  {
	  for( int chan=0; chan<16; ++chan )
    {
      trans_amount[chan]=val;
    }
  } 

  bool MIDIProcessorTransposer::Process( MIDITimedBigMessage *msg )
  {
    if( msg->IsChannelMsg() )
    {		
      if( msg->IsNoteOn() || msg->IsNoteOff() || msg->IsPolyPressure() )
      {
        int trans = trans_amount[ msg->GetChannel() ];
        
        int new_note = ((int)msg->GetNote())+trans;
        
        if( trans>127 || trans<0 )
        {
          // delete event if out of range
          return false;
        }			
        else
        {
          // set new note number
          msg->SetNote( (unsigned char)new_note );
        }
        
      }
    }
    
    return true;
  }
  
  
  
  
  
  MIDIProcessorRechannelizer::MIDIProcessorRechannelizer()
  {
    for( int i=0; i<16; ++i )
    {
      rechan_map[i] = i;
    }
    
  }
  
  MIDIProcessorRechannelizer::~MIDIProcessorRechannelizer()
  {
  }
  
  
  void MIDIProcessorRechannelizer::SetAllRechan( int dest_chan )
  {
    for( int i=0; i<16; ++i )
    {
      rechan_map[i] = dest_chan;
    }
  }
  
  bool MIDIProcessorRechannelizer::Process( MIDITimedBigMessage *msg )
  {
    if( msg->IsChannelMsg() )
    {		
      int new_chan = rechan_map[ msg->GetChannel() ];
      
      if( new_chan==-1 )
      {
        // this channel is to be deleted! return false
        return false;
      }
      
      msg->SetChannel( (unsigned char)new_chan );
    }
    
    return true;
  }
  
  
  
  
  
}
