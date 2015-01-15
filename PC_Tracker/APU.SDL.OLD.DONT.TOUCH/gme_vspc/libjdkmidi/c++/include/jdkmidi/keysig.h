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

#ifndef JDKMIDI_KEYSIG_H
#define JDKMIDI_KEYSIG_H

#include "jdkmidi/midi.h"

namespace jdkmidi
{
  enum MIDIAccidentalType
    {
      ACCFlat=0,
      ACCNatural,
      ACCSharp	
    };
  
  class	MIDIKeySignature
    {
    public:
      
      MIDIKeySignature();
      MIDIKeySignature( const MIDIKeySignature &k );
      
      void	Reset();
      
      
      bool	IsMajor()			{ return major;		}
      
      void	SetSharpFlats( int sf, bool maj=true )
        { sharp_flat=sf; major=maj; Reset();	}
      
      int	GetSharpFlats()			{ return sharp_flat;	}
      
      MIDIAccidentalType	GetNoteStatus( int white_note )
        { return state[white_note%7];		}
      
      
      bool	ConvertMIDINote( int in_note, int *out_note );
      
    protected:
      
      bool	ProcessWhiteNote( int in_note, int *out_note );
      bool	ProcessBlackNote( int in_note, int *out_note );
      
      MIDIAccidentalType	state[7];
      bool		use_sharps;
      int		sharp_flat;
      bool		major;
      
      static	int	sharp_list[7];
      static	int	flat_list[7];
      
      
    };
}

#endif


