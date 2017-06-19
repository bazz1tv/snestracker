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

#ifndef JDKMIDI_MULTITRACK_H
#define JDKMIDI_MULTITRACK_H

#include "jdkmidi/track.h"

namespace jdkmidi
{
  
  class MIDIMultiTrack;
  class MIDIMultiTrackIteratorState;
  class MIDIMultiTrackIterator;
  
  class MIDIMultiTrack 	
    {
    public:
      
      MIDIMultiTrack( int max_num_tracks_=64, bool deletable_=true );
      virtual	~MIDIMultiTrack();
      
      void SetTrack( int trk, MIDITrack *t );
      MIDITrack *GetTrack( int trk );
      const MIDITrack *GetTrack( int trk ) const;
      int	GetNumTracks() const								{ return num_tracks;	}
      
      
      void Clear();
      
      int GetClksPerBeat() const
        {
          return clks_per_beat;
        }
      
      void SetClksPerBeat( int c )
        {
          clks_per_beat = c;		
        }
      
    protected:
      
      MIDITrack **tracks;		
      const int num_tracks;
      bool	deletable;
      
      int 	clks_per_beat;	
      
    private:
      
    }; 
  
  class MIDIMultiTrackIteratorState  
    {
    public:	
      
      MIDIMultiTrackIteratorState( int num_tracks_=64);
      MIDIMultiTrackIteratorState( const MIDIMultiTrackIteratorState &m );	
      virtual ~MIDIMultiTrackIteratorState();
      
      const MIDIMultiTrackIteratorState & operator = ( const MIDIMultiTrackIteratorState &m );	
      
      int	GetNumTracks() const								{ return num_tracks;		}
      int	GetCurEventTrack() const							{ return cur_event_track;	}
      MIDIClockTime GetCurrentTime() const					{ return cur_time;			}
      
      void Reset();
      int FindTrackOfFirstEvent();
      
      MIDIClockTime cur_time;	
      int cur_event_track;
      int num_tracks;
      int *next_event_number;	
      MIDIClockTime *next_event_time;
    }; 
  
  class MIDIMultiTrackIterator  
    {
    public:	
      
      MIDIMultiTrackIterator( MIDIMultiTrack *mlt );
      virtual ~MIDIMultiTrackIterator();
      
      
      void GoToTime( MIDIClockTime time );		
      
      bool GetCurEventTime( MIDIClockTime *t) const;
      bool GetCurEvent( int *track, MIDITimedBigMessage **msg ) const;
      bool GoToNextEvent();
      
      bool GoToNextEventOnTrack( int track );
      
      const MIDIMultiTrackIteratorState &GetState() const 
        {
          return state;
        } 
      
      MIDIMultiTrackIteratorState &GetState() 
        {
          return state;
        } 
      
      void SetState( const MIDIMultiTrackIteratorState &s ) 
        {
          state = s;
        } 
      
      MIDIMultiTrack * GetMultiTrack()  				{ return multitrack;	}
      const MIDIMultiTrack * GetMultiTrack() const 	{ return multitrack; 	}	
      
    protected:
      
      MIDIMultiTrack *multitrack;	
      MIDIMultiTrackIteratorState state;
    }; 
  
}

#endif


