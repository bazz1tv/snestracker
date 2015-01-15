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
#ifndef JDKMIDI_SONG_H
#define JDKMIDI_SONG_H

#include "jdkmidi/multitrack.h"
#include "jdkmidi/sequencer.h"
#include "jdkmidi/filereadmultitrack.h"

namespace jdkmidi
{  
  class MIDISong
    {
    public:
      MIDISong( int max_tracks );
      virtual ~MIDISong();
      
      bool Load( const char *fname );
      
//	bool Save( const char *fname );	
      
      MIDIMultiTrack *GetMultiTrack()
        {
          return multitrack;
        }
      
      const MIDIMultiTrack *GetMultiTrack() const
        {
          return multitrack;
        }
      
      MIDISequencer *GetSeq()
        {
          return sequencer;
        }
      
      const MIDISequencer *GetSeq() const
        {
          return sequencer;
        }
      
    protected:
      
      MIDIMultiTrack track;	
      MIDISequencer seq;
      
      char title;
    };
}

#endif
