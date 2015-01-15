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
#ifndef JDKMIDI_PROCESS_H
#define JDKMIDI_PROCESS_H

#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"

namespace jdkmidi
{
  class MIDIProcessor;
  class MIDIMultiProcessor;
  class MIDIProcessorTransposer;
  class MIDIProcessor;
  
  
  class MIDIProcessor 
    {
    public:
      MIDIProcessor();
      virtual ~MIDIProcessor();
      
      virtual bool Process( MIDITimedBigMessage *msg ) = 0;
    }; 
  
  class MIDIMultiProcessor : public MIDIProcessor 
    {
    public:
      MIDIMultiProcessor( int num_processors );
      virtual ~MIDIMultiProcessor();
      
      // MIDIProcessors given to a MIDIMultiProcessor are NOT owned
      // by MIDIMultiProcessor.
      
      void SetProcessor( int position, MIDIProcessor *proc )
        {
          processors[position]=proc;
        }   
      
      MIDIProcessor *GetProcessor( int position )
        {
          return processors[position];
        }			
      
      const MIDIProcessor *GetProcessor( int position ) const
        {
          return processors[position];
        }
      
      virtual bool Process( MIDITimedBigMessage *msg );
      
    private:
      MIDIProcessor **processors;
      int num_processors;
    }; 
  
  class MIDIProcessorTransposer : public MIDIProcessor 
    {
    public:
      MIDIProcessorTransposer();
      virtual ~MIDIProcessorTransposer();
      
      void SetTransposeChannel( int chan, int trans )
        {
          trans_amount[chan] = trans;
        }
      
      int GetTransposeChannel( int chan ) const
        {
          return trans_amount[chan];
        }
      
      void SetAllTranspose( int trans );
      
      virtual bool Process( MIDITimedBigMessage *msg );
    private:
      int trans_amount[16];
    }; 
  
  class MIDIProcessorRechannelizer : public MIDIProcessor 
    {
    public:
      MIDIProcessorRechannelizer();
      virtual ~MIDIProcessorRechannelizer();
      
      void SetRechanMap( int src_chan, int dest_chan )
        {
          rechan_map[src_chan] = dest_chan;
        }
      
      int GetRechanMap( int src_chan ) const
        {	
          return rechan_map[src_chan];
        }
      
      void SetAllRechan( int dest_chan );
      
      virtual bool Process( MIDITimedBigMessage *msg );
      
    private:
      
      int rechan_map[16];
    }; 
}

#endif
