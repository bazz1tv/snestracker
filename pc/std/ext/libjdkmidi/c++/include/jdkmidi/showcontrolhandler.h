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

#ifndef JDKMIDI_SHOWCONTROLHANDLER_H
#define JDKMIDI_SHOWCONTROLHANDLER_H



#include "jdkmidi/showcontrol.h"


namespace jdkmidi
{
  
  class MIDISCHandle
    {
    public:
      MIDISCHandle();
      virtual ~MIDISCHandle();
      
      virtual bool Dispatch( const MIDIShowControlPacket &p );
      
      virtual bool Go();
      virtual bool Go( const MIDICue & q_number );
      virtual bool Go( const MIDICue & q_number, const MIDICue & q_list );
      virtual bool Go( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path );
      virtual bool Stop();
      virtual bool Stop( const MIDICue & q_number );
      virtual bool Stop( const MIDICue & q_number, const MIDICue & q_list );
      virtual bool Stop( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path );
      virtual bool Resume();
      virtual bool Resume( const MIDICue & q_number );
      virtual bool Resume( const MIDICue & q_number, const MIDICue & q_list );
      virtual bool Resume( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path );
      virtual bool TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff
        );
      virtual bool TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_number
        );
      virtual bool TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_number, const MIDICue & q_list
        );
      
      virtual bool TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path 
        );
      
      virtual bool Load( const MIDICue & q_number );
      virtual bool Load( const MIDICue & q_number, const MIDICue & q_list );
      virtual bool Load( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path );
      virtual bool Set( ulong ctrl_num, ulong ctrl_val );
      
      virtual bool Set( 
        ulong ctrl_num, 
        ulong ctrl_val,
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff 
        );
      
      virtual bool Fire( uchar macro_num );
      virtual bool AllOff();
      virtual bool Restore();
      virtual bool Reset();
      virtual bool GoOff();
      virtual bool GoOff( const MIDICue & q_number );
      virtual bool GoOff( const MIDICue & q_number, const MIDICue & q_list );
      virtual bool GoOff( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path );
      virtual bool GoJam();
      virtual bool GoJam( const MIDICue & q_number );
      virtual bool GoJam( const MIDICue & q_number, const MIDICue & q_list );
      virtual bool GoJam( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path );
      virtual bool StandbyPlus();
      virtual bool StandbyPlus( const MIDICue & q_list );
      virtual bool StandbyMinus();
      virtual bool StandbyMinus( const MIDICue & q_list );
      virtual bool SequencePlus();
      virtual bool SequencePlus( const MIDICue & q_list );
      virtual bool SequenceMinus();
      virtual bool SequenceMinus( const MIDICue & q_list );
      virtual bool StartClock();
      virtual bool StartClock( const MIDICue & q_list );
      virtual bool StopClock();
      virtual bool StopClock( const MIDICue & q_list );
      virtual bool ZeroClock();
      virtual bool ZeroClock( const MIDICue & q_list );
      virtual bool SetClock( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff
        );
      virtual bool SetClock( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_list
        );
      virtual bool MTCChaseOn();
      virtual bool MTCChaseOn( const MIDICue & q_list );
      virtual bool MTCChaseOff();
      virtual bool MTCChaseOff( const MIDICue & q_list );
      virtual bool OpenQList( const MIDICue & q_list );
      virtual bool CloseQList( const MIDICue & q_list );
      virtual bool OpenQPath( const MIDICue & q_path );
      virtual bool CloseQPath( const MIDICue & q_path );
      
    };
  
}

#endif
