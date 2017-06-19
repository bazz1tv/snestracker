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


#include "jdkmidi/world.h"

#include "jdkmidi/keysig.h"


#ifndef DEBUG_MDKEYSIG
# define DEBUG_MDKEYSIG	0
#endif

#if DEBUG_MDKEYSIG
# undef DBG
# define DBG(a)	a
#endif

namespace jdkmidi
{
  
  
  int MIDIKeySignature::sharp_list[7] = { 3, 0, 4, 1, 5, 2, 6 };
  int MIDIKeySignature::flat_list[7] =  { 6, 2, 5, 1, 4, 0, 3 };
  
  
  MIDIKeySignature::MIDIKeySignature()
  {
    ENTER( "MIDIKeySignature::MIDIKeySignature()" );
    use_sharps=true;
    sharp_flat=0;
    major=true;
    Reset();
  }
  
  MIDIKeySignature::MIDIKeySignature( const MIDIKeySignature &k )
  {
    ENTER( "MIDIKeySignature::MIDIKeySignature()" );	
    use_sharps=k.use_sharps;
    sharp_flat=k.sharp_flat;
    major=k.major;
    
    Reset();
  }
  
  
//
// Reset() generates the sharp/flat list based on sharp_flat, major,
// and use_sharps.
//
  
  void MIDIKeySignature::Reset()
  {
    ENTER( "MIDIKeySignature::Reset()" );
    if( sharp_flat < -7 )
      sharp_flat=-7;
    if( sharp_flat >7 )
      sharp_flat=7;
    
    for( int note=0; note<7; ++note )
      state[note]=ACCNatural;
    
    if( sharp_flat==0 )
    {
      // Key of C has no sharps or flats.
      // and any accidentals will be sharp
      
      use_sharps=true;
    }
    else if( sharp_flat>0 )
    {
      //
      // this key has a number of sharps in it.
      //
      
      use_sharps=true;
      
      for( short i=0; i<sharp_flat; ++i )
      {
        state[sharp_list[i]]=ACCSharp;
      }
    }
    else if( sharp_flat<0 )
    {
      //
      // this key has flats in it.
      // -sharp_flat is how many flats.
      //
      
      int flats= -sharp_flat;
      
      use_sharps=false;
      
      for( int i=0; i<flats; ++i )
      {
        state[ flat_list[i] ]=ACCFlat;
      }
      
    }
    
  }
  
  
  
  
  bool	MIDIKeySignature::ProcessWhiteNote( int in_note, int *out_note )
  {
    ENTER( "MIDIKeySignature::ProcessWhiteNote()" );
    //
    // check to see if this white note is allowed in the current
    // state.
    //
    
    if( state[in_note]==ACCNatural )
    {
      //
      // yes it is allowed, return it.
      //
      
      *out_note=in_note;
      
      //
      // return false to signify that this note doesn't need
      // an accidental.
      //
      
      return false;
    }
    else
    {
      //
      // no it was not allowed. We must change our state.
      // to allow it. return it.
      //
      
      *out_note=in_note;
      
      //
      // change the desired note to a natural
      //
      
      state[in_note]=ACCNatural;
      
      //
      // return true because it needed an accidental
      //
      
      return true;
    }
  }
  
  
  bool MIDIKeySignature::ProcessBlackNote( int in_note, int *out_note )
  {
    ENTER( "MIDIKeySignature::ProcessBlackNote()" );	
    //
    // if this note is already sharped,
    // return the note unchanged and return false
    // because no accidental was required
    
    if( state[in_note]==ACCSharp )
    {
      *out_note=in_note;
      return false;
    }
    
    //
    // if the next note is flatted, then we could use it
    // instead.
    //
    
    if( state[in_note+1]==ACCFlat )
    {
      *out_note=in_note+1;
      return false;
    }
    
    //
    // Couldn't find a black note. we gotta make one.
    // make a sharp if use_sharps==1
    
    if( use_sharps )
    {
      //
      // make this white note a sharp.
      //
      
      state[in_note]=ACCSharp;
      *out_note=in_note;
      
      //
      // Accidental required. return true.
      //
      return true;
    }
    else
    {
      //
      // make the next white note a flat.
      //
      
      state[in_note+1]=ACCFlat;
      *out_note=in_note+1;
      
      //
      // Accidental required. Return true.
      //
      
      return true;
    }
  }
  
  
  
//
// ConvertMIDINote() takes a real MIDI note number and converts it to a
// white-key note number. it returns true if an Accidental is required.
//
  
  bool MIDIKeySignature::ConvertMIDINote(
    int in_note,
    int *out_note
    )
  {
    ENTER( "MIDIKeySignature::ConvertMIDINote()" );
    
    int octave=in_note/12;
    int midi_note=in_note%12;
    int actual_note=0;
    bool	changed=false;
    
    switch( midi_note )
    {
    case 0:		// C
      changed=ProcessWhiteNote(0,&actual_note);
      break;
    case 2:		// D
      changed=ProcessWhiteNote(1,&actual_note);
      break;
    case 4:		// E
      changed=ProcessWhiteNote(2,&actual_note);
      break;
    case 5:		// F
      changed=ProcessWhiteNote(3,&actual_note);
      break;
    case 7:		// G
      changed=ProcessWhiteNote(4,&actual_note);
      break;
    case 9:		// A
      changed=ProcessWhiteNote(5,&actual_note);
      break;
    case 11:	// B
      changed=ProcessWhiteNote(6,&actual_note);
      break;
      
    case 1:		// C#
      changed=ProcessBlackNote(0,&actual_note);
      break;
    case 3:		// D#
      changed=ProcessBlackNote(1,&actual_note);
      break;
    case 6:		// F#
      changed=ProcessBlackNote(3,&actual_note);
      break;
    case 8:		// G#
      changed=ProcessBlackNote(4,&actual_note);
      break;
    case 10:	// A#
      changed=ProcessBlackNote(5,&actual_note);
      break;
    };
    
    *out_note = (octave*7) + actual_note;
    return changed;
    
  }
  
}
