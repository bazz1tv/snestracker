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

#ifndef JDKMIDI_TRACK_H
#define JDKMIDI_TRACK_H

#include "jdkmidi/midi.h"
#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"

namespace jdkmidi
{

  ///
  /// MIDITrackChunkSize is a constant which specifies how many events are in one MIDITrackChunk.
  ///

  const int MIDITrackChunkSize=512;
  

  ///
  /// A MIDITrack's events are allocated in these chunks in order to avoid memory fragmentation
  /// in embedded systems or other systems lacking an MMU. Every item within a MIDITrackChunk
  /// object is a MIDITimedBigMessage. To avoid unnecessary copied of big events, access
  /// to these events is only done via the GetEventAddress() method.
  ///

  class  MIDITrackChunk 	
    {      
    public:
      ///
      /// GetEventAddress() const returns the address of the MIDITimedBigMessage referred to by event_num
      /// @param event_num an integer specifying an event number in the range 0 to MIDITrackChunkSize
      /// @returns The const pointer to the requested event.
      ///
      const MIDITimedBigMessage * GetEventAddress( int event_num ) const;

      ///
      /// GetEventAddress()  returns the address of the MIDITimedBigMessage referred to by event_num
      /// @param event_num an integer specifying an event number in the range 0 to MIDITrackChunkSize
      /// @returns The non-const pointer to the requested event.
      ///

      MIDITimedBigMessage * GetEventAddress( int event_num );
      
    protected:
      
    private:
      MIDITimedBigMessage buf[MIDITrackChunkSize];
    }; 
  
  ///
  /// The MIDIChunksPerTrack constant specifies the maximum number of MIDITrackChunks that can be in one track.
  /// The value MIDIChunksPerTrack * MIDITrackChunkSize is the total number of events.
  /// This is a constant in order to avoid memory fragmentation in embedded systems or systems without an MMU
  ///

  const int MIDIChunksPerTrack=512;
  

  ///
  /// The MIDITrack class is a container that manages an array of MIDIChunk objects and provides an
  /// interface to the user that is useful for managing a list of MIDITimedBigMessages. It internally
  /// stores MIDITimedBigMessage objects. There is a fixed maximum number of events that it can store,
  /// which is defined by MIDIChunksPerTrack * MIDITrackChunkSize.
  ///

  class  MIDITrack 	
    {
    public:

      ///
      /// Construct a MIDITrack object with the specified number of events
      /// @param size The number of events, defaults to 0
      ///
      MIDITrack( int size=0 );

      ///
      /// Copy Constructor for a MIDITrack object
      /// @param t The reference to the MIDITrack object to copy
      ///      
      MIDITrack( const MIDITrack &t );
      
      ///
      /// The MIDITrack Destructor, frees all chunks and referenced MIDITimedBigMessage's
      ///
      ~MIDITrack();
      
      ///
      /// Clear() sets the number of active events in the track to 0. It does NOT
      /// free any events. See the Shrink() method.
      ///
      void	Clear();

      ///
      /// Shrink() frees any unused MIDITrackChunk objects and associated MIDITimedBigMessage events.
      ///
      void	Shrink();
      
      ///
      /// ClearAndMerge() allows you to merge the events in two separate tracks into a third track.
      /// @param src1 Pointer to first track
      /// @param src2 Pointer to second track
      /// ClearAndMerge() assumes all events in both tracks are already ordered by time.
      ///
      void	ClearAndMerge( const MIDITrack *src1, const MIDITrack *src2 );
      
//	bool	Insert( int start_event, int num_events );
//    bool 	Delete( int start_event, int num_events);
//    void 	Sort();
      
      bool	Expand( int increase_amount=(MIDITrackChunkSize) );
      
      MIDITimedBigMessage * GetEventAddress( int event_num );
      
      const MIDITimedBigMessage * GetEventAddress( int event_num ) const;
      
      const 	MIDITimedBigMessage *GetEvent( int event_num ) const;
      MIDITimedBigMessage *GetEvent( int event_num );
      bool	GetEvent( int event_num, MIDITimedBigMessage *msg ) const;
      
      bool	PutEvent( const MIDITimedBigMessage &msg );
      bool	PutEvent( const MIDITimedMessage &msg, MIDISystemExclusive *sysex );
      bool	SetEvent( int event_num, const MIDITimedBigMessage &msg );
      
      bool	MakeEventNoOp( int event_num );
      
      bool	FindEventNumber( MIDIClockTime time, int *event_num ) const;
      
      int	GetBufferSize()	const;
      int	GetNumEvents() const;
      
    private:
      
//	void 	QSort( int left, int right );
      
      MIDITrackChunk * chunk[MIDIChunksPerTrack];
      
      int buf_size;
      int num_events;
    }; 
  
}

#endif

