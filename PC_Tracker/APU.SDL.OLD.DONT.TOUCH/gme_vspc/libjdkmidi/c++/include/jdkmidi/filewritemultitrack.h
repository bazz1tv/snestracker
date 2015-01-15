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
#ifndef JDKMIDI_FILEWRITEMULTITRACK_H
#define JDKMIDI_FILEWRITEMULTITRACK_H

#include "jdkmidi/filewrite.h"
#include "jdkmidi/multitrack.h"

namespace jdkmidi
{
  
  class MIDIFileWriteMultiTrack
    {
    public:
      
      MIDIFileWriteMultiTrack(
        const MIDIMultiTrack *mlt_,
        MIDIFileWriteStream *strm_
        );
      
      virtual ~MIDIFileWriteMultiTrack();
      
      bool Write( int num_tracks, int division );

      bool Write( int num_tracks )
      {
        return Write( num_tracks, multitrack->GetClksPerBeat() );
      }
      bool Write()
      {
        return Write( multitrack->GetNumTracks(), multitrack->GetClksPerBeat() );
      }

    private:
      
      virtual bool PreWrite();
      virtual bool PostWrite();
      
      const MIDIMultiTrack *multitrack;
      MIDIFileWrite writer;
    };
  
}


#endif
