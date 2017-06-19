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

#include "jdkmidi/world.h"

#include "jdkmidi/showcontrolhandler.h"

namespace jdkmidi
{
  
  
  MIDISCHandle::MIDISCHandle()
  {
  }
  
  MIDISCHandle::~MIDISCHandle()
  {
  }
  
  
  bool MIDISCHandle::Dispatch( const MIDIShowControlPacket &p )
  {
    bool e;
    
    switch( p.GetCommand() )
    {
    case MIDI_SC_GO:
      if( p.GetHasQPath() )
        e=Go( p.GetQNumber(), p.GetQList(), p.GetQPath() );
      else if( p.GetHasQList() )
        e=Go( p.GetQNumber(), p.GetQList() );		
      else if( p.GetHasQNumber() )
        e=Go( p.GetQNumber() );
      else
        e=Go();
      break;
    case MIDI_SC_STOP:	
      if( p.GetHasQPath() )
        e=Stop( p.GetQNumber(), p.GetQList(), p.GetQPath() );
      else if( p.GetHasQList() )
        e=Stop( p.GetQNumber(), p.GetQList() );		
      else if( p.GetHasQNumber() )
        e=Stop( p.GetQNumber() );
      else
        e=Stop();
      break;	
    case MIDI_SC_RESUME:	
      if( p.GetHasQPath() )
        e=Resume( p.GetQNumber(), p.GetQList(), p.GetQPath() );
      else if( p.GetHasQList() )
        e=Resume( p.GetQNumber(), p.GetQList() );		
      else if( p.GetHasQNumber() )
        e=Resume( p.GetQNumber() );
      else
        e=Resume();
      break;	
    case MIDI_SC_LOAD:	
      if( p.GetHasQPath() )
        e=Load( p.GetQNumber(), p.GetQList(), p.GetQPath() );
      else if( p.GetHasQList() )
        e=Load( p.GetQNumber(), p.GetQList() );		
      else
        e=Load( p.GetQNumber() );
      break;	
    case MIDI_SC_GO_OFF:	
      if( p.GetHasQPath() )
        e=GoOff( p.GetQNumber(), p.GetQList(), p.GetQPath() );
      else if( p.GetHasQList() )
        e=GoOff( p.GetQNumber(), p.GetQList() );		
      else if( p.GetHasQNumber() )
        e=GoOff( p.GetQNumber() );
      else
        e=GoOff();
      break;		
    case MIDI_SC_GO_JAM:	
      if( p.GetHasQPath() )
        e=GoJam( p.GetQNumber(), p.GetQList(), p.GetQPath() );
      else if( p.GetHasQList() )
        e=GoJam( p.GetQNumber(), p.GetQList() );		
      else if( p.GetHasQNumber() )
        e=GoJam( p.GetQNumber() );
      else
        e=GoJam();
      break;		
    case	MIDI_SC_TIMED_GO:
      if( p.GetHasQPath() )
        e=TimedGo(
          p.GetHours(), p.GetMinutes(), p.GetSeconds(),
          p.GetFrames(), p.GetFractFrames(),
          p.GetQNumber(), p.GetQList(), p.GetQPath() 
          );
      else if( p.GetHasQList() )
        e=TimedGo( 				 
          p.GetHours(), p.GetMinutes(), p.GetSeconds(),
          p.GetFrames(), p.GetFractFrames(),
          p.GetQNumber(), p.GetQList() 
          );		
      else if( p.GetHasQNumber() )
        e=TimedGo(  				 
          p.GetHours(), p.GetMinutes(), p.GetSeconds(),
          p.GetFrames(), p.GetFractFrames(),
          p.GetQNumber() );
      else
        e=TimedGo( 				 
          p.GetHours(), p.GetMinutes(), p.GetSeconds(),
          p.GetFrames(), p.GetFractFrames()
          );
      break;		
    case	MIDI_SC_SET:
      
      if( !p.GetHasTime() )
      {
        e=Set( p.GetControlNum(), p.GetControlVal() );	
      }
      else
      {
        e=Set( 
          p.GetControlNum(), p.GetControlVal(), 
          p.GetHours(),  p.GetMinutes(), p.GetSeconds(),
          p.GetFrames(), p.GetFractFrames()
          );					
      }
      break;
    case	MIDI_SC_FIRE:
      e=Fire( (uchar)p.GetMacroNum() );
      break;
    case	MIDI_SC_ALL_OFF:
      e=AllOff();
      break;
    case	MIDI_SC_RESTORE: 
      e=Restore();
      break;	
    case	MIDI_SC_RESET:		
      e=Reset();
      break;	
    case	MIDI_SC_STANDBY_PLUS:
      if( p.GetHasQList() )
        e=StandbyPlus( p.GetQList() );
      else
        e=StandbyPlus();
      break;	
    case	MIDI_SC_STANDBY_MINUS:
      if( p.GetHasQList() )
        e=StandbyMinus( p.GetQList() );
      else
        e=StandbyMinus();
      break;		
    case	MIDI_SC_SEQUENCE_PLUS:	
      if( p.GetHasQList() )
        e=SequencePlus( p.GetQList() );
      else
        e=SequencePlus();
      break;		
    case	MIDI_SC_SEQUENCE_MINUS:	
      if( p.GetHasQList() )
        e=SequenceMinus( p.GetQList() );
      else
        e=SequenceMinus();
      break;		
    case	MIDI_SC_START_CLOCK:	
      if( p.GetHasQList() )
        e=StartClock( p.GetQList() );
      else
        e=StartClock();
      break;		
    case	MIDI_SC_STOP_CLOCK:	
      if( p.GetHasQList() )
        e=StopClock( p.GetQList() );
      else
        e=StopClock();
      break;			
    case	MIDI_SC_ZERO_CLOCK:
      if( p.GetHasQList() )
        e=ZeroClock( p.GetQList() );
      else
        e=ZeroClock();
      break;			
    case	MIDI_SC_MTC_CHASE_ON:	
      if( p.GetHasQList() )
        e=MTCChaseOn( p.GetQList() );
      else
        e=MTCChaseOn();
      break;			
    case	MIDI_SC_MTC_CHASE_OFF:		
      if( p.GetHasQList() )
        e=MTCChaseOff( p.GetQList() );
      else
        e=MTCChaseOff();
      break;				
    case	MIDI_SC_SET_CLOCK:
      if( p.GetHasQList() )
        e=SetClock( 			     	
          p.GetHours(),  p.GetMinutes(), p.GetSeconds(),
          p.GetFrames(), p.GetFractFrames(),
          p.GetQList()
          );
      else
        e=SetClock( 			     	
          p.GetHours(),  p.GetMinutes(), p.GetSeconds(),
          p.GetFrames(), p.GetFractFrames()
          );
      
    case	MIDI_SC_OPEN_Q_LIST:	
      e=OpenQList( p.GetQList() );
      break;					
    case	MIDI_SC_CLOSE_Q_LIST:	
      e=CloseQList( p.GetQList() );
      break;						
    case	MIDI_SC_OPEN_Q_PATH:
      e=OpenQPath( p.GetQPath() );
      break;						
    case	MIDI_SC_CLOSE_Q_PATH:	
      e=CloseQPath( p.GetQPath() );
      break;							
    default:
      e=false;
      
      break;
    }
    
    return e;
  }
  
  
  bool MIDISCHandle::Go()
  {
    bool e;
    e=false;
    
    return e;	
  }
  
  bool MIDISCHandle::Go( const MIDICue & q_number )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Go( const MIDICue & q_number, const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Go( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Stop()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Stop( const MIDICue & q_number )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Stop( const MIDICue & q_number, const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Stop( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Resume()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Resume( const MIDICue & q_number )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Resume( const MIDICue & q_number, const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Resume( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::TimedGo( 
    uchar hr, uchar mn, uchar sc, uchar fr, uchar ff
    )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::TimedGo( 
    uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
    const MIDICue & q_number
    )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::TimedGo( 
    uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
    const MIDICue & q_number, const MIDICue & q_list
    )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  
  bool MIDISCHandle::TimedGo( 
    uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
    const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path 
    )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  
  bool MIDISCHandle::Load( const MIDICue & q_number )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Load( const MIDICue & q_number, const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Load( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Set( ulong ctrl_num, ulong ctrl_val )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  
  bool MIDISCHandle::Set( 
    ulong ctrl_num, 
    ulong ctrl_val,
    uchar hr, uchar mn, uchar sc, uchar fr, uchar ff 
    )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  
  bool MIDISCHandle::Fire( uchar macro_num )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::AllOff()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Restore()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::Reset()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoOff()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoOff( const MIDICue & q_number )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoOff( const MIDICue & q_number, const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoOff( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoJam()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoJam( const MIDICue & q_number )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoJam( const MIDICue & q_number, const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::GoJam( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StandbyPlus()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StandbyPlus( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StandbyMinus()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StandbyMinus( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::SequencePlus()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::SequencePlus( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::SequenceMinus()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::SequenceMinus( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StartClock()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StartClock( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StopClock()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::StopClock( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::ZeroClock()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::ZeroClock( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::SetClock( 
    uchar hr, uchar mn, uchar sc, uchar fr, uchar ff
    )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::SetClock( 
    uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
    const MIDICue & q_list
    )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::MTCChaseOn()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::MTCChaseOn( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::MTCChaseOff()
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::MTCChaseOff( const MIDICue & q_list )
  {
    bool e;
    e=false;
    
    return e;
  }
  
  
  bool MIDISCHandle::OpenQList( const MIDICue & q_list )
  {
    bool e;
    e=false;
    return e;
  }
  
  
  bool MIDISCHandle::CloseQList( const MIDICue & q_list )
  {
    bool e;
    e=false;
    return e;
  }
  
  
  bool MIDISCHandle::OpenQPath( const MIDICue & q_path )
  {
    bool e;
    e=false;
    return e;
  }
  
  
  bool MIDISCHandle::CloseQPath( const MIDICue & q_path )
  {
    bool e;
    e=false;
    return e;
  }
  
  
}
