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
#ifndef JDKMIDI_QUEUE_H
#define JDKMIDI_QUEUE_H

#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"

namespace jdkmidi
{
  
  class MIDIQueue
    {
    public:
      MIDIQueue( int num_msgs );
      virtual ~MIDIQueue();
      
      void Clear();
      
      bool CanPut() const;
      
      bool CanGet() const;
      
      bool IsFull() const 
        {
          return !CanPut();
        } 
      
      
      void Put( const MIDITimedBigMessage &msg ) 
        {
          buf[next_in] = msg;
          
          next_in = (next_in+1)%bufsize;
        } 
      
      MIDITimedBigMessage Get() const 
        {
          return MIDITimedBigMessage( buf[next_out] );
        } 
      
      void Next() 
        {
          next_out = (next_out+1) % bufsize;
        } 
      
      const MIDITimedBigMessage *Peek() const 
        {
          return &buf[next_out];
        } 
      
    protected:
      MIDITimedBigMessage *buf;
      int bufsize;
      volatile int next_in;
      volatile int next_out;
    };
  
}

#endif
