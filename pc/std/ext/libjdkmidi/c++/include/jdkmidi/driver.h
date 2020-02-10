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
#ifndef JDKMIDI_DRIVER_H
#define JDKMIDI_DRIVER_H

#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"
#include "jdkmidi/matrix.h"
#include "jdkmidi/process.h"
#include "jdkmidi/queue.h"
#include "jdkmidi/tick.h"

namespace jdkmidi
{  
  class MIDIDriver : public MIDITick 
    {
    public:
      MIDIDriver(int queue_size );
      virtual ~MIDIDriver();
      
      virtual void Reset();
      
      // to get the midi in queue
      MIDIQueue * InputQueue() 
        {
          return &in_queue;
        } 
      
      const MIDIQueue * InputQueue() const 
        {
          return &in_queue;
        } 
      
      // to get the midi out queue
      MIDIQueue * OutputQueue() 
        {
          return &out_queue;
        } 
      
      const MIDIQueue * OutputQueue() const 
        {
          return &out_queue;
        } 
      
      
      //
      // returns true if the output queue is not full
      bool CanOutputMessage() const 
        {
          return out_queue.CanPut();
        } 
      
      
      // processes message with the OutProcessor and then
      // puts the message in the out_queue
      void OutputMessage( MIDITimedBigMessage &msg ) 
        {
          if( (out_proc && out_proc->Process( &msg )) || !out_proc )
          {
            out_matrix.Process( msg );
            out_queue.Put( msg );
          }
        } 
      
      void SetThruEnable( bool f ) 
        {
          thru_enable = f;
        } 
      
      bool GetThruEnable() const 
        {
          return thru_enable;
        } 
      
      // to set the midi processors used for thru, out, and in
      void SetThruProcessor( MIDIProcessor *proc ) 
        {
          thru_proc = proc;
        } 
      
      void SetOutProcessor( MIDIProcessor *proc ) 
        {
          out_proc = proc;
        } 
      
      void SetInProcessor( MIDIProcessor *proc ) 
        {
          in_proc = proc;
        } 
      
      void SetTickProc( MIDITick *tick ) 
        {
          tick_proc = tick;
        } 
      
      // to send all notes off on selected midi chanel
      void AllNotesOff(int chan);
      
      // to send all notes off on all midi channels
      void AllNotesOff();
      
      // call handle midi in when a parsed midi message
      // comes in to the system. Can be called by a callback function
      // or by your TimeTick() function.
      
      virtual bool HardwareMsgIn( MIDITimedBigMessage &msg );
      
      // HardwareMsgOut() must be overriden by a subclass - It must
      // take
      
      virtual bool HardwareMsgOut( const MIDITimedBigMessage &msg ) = 0;
      
      // the time tick procedure:
      // 	manages in/out/thru to hardware
      // inherited from MIDITick.
      //
      // if you need to poll midi in hardware,
      // you can override this method - Call MIDIDriver::TimeTick(t)
      // first, You may then poll the midi in
      // hardware, parse the bytes, form a message, and give the
      // resulting message to HandleMsgIn to process it and put it in
      // the in_queue.
      
      virtual void TimeTick( unsigned long sys_time );
      
      
    protected:
      
      
      // the in and out queues
      MIDIQueue in_queue;
      MIDIQueue out_queue;
      
      // the processors
      MIDIProcessor *in_proc;
      MIDIProcessor *out_proc;
      MIDIProcessor *thru_proc;
      
      bool thru_enable;
      
      // additional TimeTick procedure
      
      MIDITick *tick_proc;
      
      // to keep track of notes on going to MIDI out
      
      MIDIMatrix out_matrix;
    }; 
  
  
}

#endif
