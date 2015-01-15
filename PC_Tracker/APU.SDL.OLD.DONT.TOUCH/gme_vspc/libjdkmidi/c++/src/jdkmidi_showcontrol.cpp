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
#include "jdkmidi/showcontrol.h"

namespace jdkmidi
{
  
  
  MIDIShowControlPacket::MIDIShowControlPacket()	
    :
    QNumber(0),
    QList(0),
    QPath(0)
  {
    DeviceId=0;
    CommandFmt=0;
    Command=MIDI_SC_GO;
    HasTime=false;
    HasQNumber=false;
    HasQList=false;
    HasQPath=false;
    Hours=0;
    Minutes=0;
    Seconds=0;
    Frames=0;
    FractFrames=0;
    Val1=0;
    Val2=0;
  }
  
  void MIDIShowControlPacket::ClearVariableStuff()
  {
    QNumber.Clear();
    QList.Clear();
    QPath.Clear();
    
    Command=MIDI_SC_GO;
    HasTime=false;
    HasQNumber=false;
    HasQList=false;
    HasQPath=false;
    Hours=0;
    Minutes=0;
    Seconds=0;
    Frames=0;
    FractFrames=0;
    Val1=0;
    Val2=0;
  }
  
  
  bool	MIDIShowControlPacket::ParseEntireSysEx( const MIDISystemExclusive *e )
  {
    bool f=true;
    int pos = 0;
    
    if( e->GetData( pos++ )!=0xf0 )
      return false;
    if( e->GetData( pos++ )!=0x7f )
      return false;
    
    ClearVariableStuff();
    
    DeviceId=e->GetData( pos++ );
    
    if( e->GetData(pos++)!=0x02 )
      return false;
    
    CommandFmt = e->GetData( pos++ );
    Command = (MIDIShowCommand) e->GetData(pos++);
    
    switch( Command )
    {
    case MIDI_SC_GO:
    case	MIDI_SC_STOP:	
    case	MIDI_SC_RESUME:	
    case	MIDI_SC_LOAD:	
    case	MIDI_SC_GO_OFF:	
    case	MIDI_SC_GO_JAM:	
    {
      f&=Parse3Param( e, &pos );
    }
    break;
    case	MIDI_SC_TIMED_GO:
    {
      f&=ParseTime( e, &pos );
      f&=Parse3Param(e,&pos);
    }
    break;
    case	MIDI_SC_SET:
    {
      f&=ParseSet(e,&pos);	
    }
    break;
    case	MIDI_SC_FIRE:
    {
      f&=ParseFire(e,&pos);			
    }
    break;
    case	MIDI_SC_ALL_OFF:
    case	MIDI_SC_RESTORE:
    case	MIDI_SC_RESET:	
      // no additional params.	
      break;
      
    case	MIDI_SC_STANDBY_PLUS:
    case	MIDI_SC_STANDBY_MINUS:
    case	MIDI_SC_SEQUENCE_PLUS:	
    case	MIDI_SC_SEQUENCE_MINUS:	
    case	MIDI_SC_START_CLOCK:	
    case	MIDI_SC_STOP_CLOCK:	
    case	MIDI_SC_ZERO_CLOCK:	
    case	MIDI_SC_MTC_CHASE_ON:	
    case	MIDI_SC_MTC_CHASE_OFF:		
    {
      if( e->GetData(pos)!=0xf7 && e->GetData(pos)!=0  )
        f&=ParseQList( e,&pos );
    }
    break;
    case	MIDI_SC_SET_CLOCK:
    {
      f&=ParseTime(e,&pos);
      if( e->GetData(pos)!=0xf7 && e->GetData(pos)!=0  )			
        f&=ParseQList(e,&pos);
    }
    break;
    case	MIDI_SC_OPEN_Q_LIST:	
    case	MIDI_SC_CLOSE_Q_LIST:	
    {
      f&=ParseQList(e,&pos);	
    }
    break;
    case	MIDI_SC_OPEN_Q_PATH:
    case	MIDI_SC_CLOSE_Q_PATH:	
    {
      f&=ParseQPath(e,&pos );	
    }
    break;
    default:
      f=false;	// unrecognized command
      break;
    }
    
    if( e->GetLength() < pos )
    {
      return false;
    }
    
    return f;
  }
  
  bool	MIDIShowControlPacket::StoreToSysEx( MIDISystemExclusive *e ) const
  {
    bool f=true;
    
    e->Clear();
    e->PutEXC();
    e->PutByte( 0x7f );
    e->PutByte( DeviceId );
    e->PutByte( 0x02 );
    e->PutByte( CommandFmt );
    e->PutByte( Command );
    
    switch( Command )
    {
    case MIDI_SC_GO:
    case	MIDI_SC_STOP:	
    case	MIDI_SC_RESUME:	
    case	MIDI_SC_LOAD:	
    case	MIDI_SC_GO_OFF:	
    case	MIDI_SC_GO_JAM:	
    {
      f&=Store3Param( e );
    }
    break;
    case	MIDI_SC_TIMED_GO:
    {
      f&=StoreTime( e );
      f&=Store3Param(e);
    }
    break;
    case	MIDI_SC_SET:
    {
      f&=StoreSet(e);	
    }
    break;
    case	MIDI_SC_FIRE:
    {
      f&=StoreFire(e);			
    }
    break;
    case	MIDI_SC_ALL_OFF:
    case	MIDI_SC_RESTORE:
    case	MIDI_SC_RESET:	
      // no additional params.	
      break;
      
    case	MIDI_SC_STANDBY_PLUS:
    case	MIDI_SC_STANDBY_MINUS:
    case	MIDI_SC_SEQUENCE_PLUS:	
    case	MIDI_SC_SEQUENCE_MINUS:	
    case	MIDI_SC_START_CLOCK:	
    case	MIDI_SC_STOP_CLOCK:	
    case	MIDI_SC_ZERO_CLOCK:	
    case	MIDI_SC_MTC_CHASE_ON:	
    case	MIDI_SC_MTC_CHASE_OFF:		
    {
      if( HasQList )
        f&=StoreQList( e );
    }
    break;
    case	MIDI_SC_SET_CLOCK:
    {
      f&=StoreTime(e );
      if( HasQList )
        f&=StoreQList(e);
    }
    break;
    case	MIDI_SC_OPEN_Q_LIST:	
    case	MIDI_SC_CLOSE_Q_LIST:	
    {
      f&=StoreQList(e);	
    }
    break;
    case	MIDI_SC_OPEN_Q_PATH:
    case	MIDI_SC_CLOSE_Q_PATH:	
    {
      f&=StoreQPath(e );	
    }
    break;
    default:
      f=false;	// unrecognized command
      break;
    }
    
    e->PutEOX();
    
    if( e->IsFull() )
      return false;
    
    return f;
  }
  
  
  bool	MIDIShowControlPacket::StoreTime( MIDISystemExclusive *e ) const
  {
    if( HasTime )
    {
      e->PutByte( Hours );
      e->PutByte( Minutes );
      e->PutByte( Seconds );
      e->PutByte( Frames );
      e->PutByte( FractFrames );	
      return true;
    }
    else
    {
      return false;
    }
  }
  
  bool	MIDIShowControlPacket::ParseTime( const MIDISystemExclusive *e, int *pos)
  {
    
    Hours = e->GetData( (*pos)++ );
    Minutes = e->GetData( (*pos)++ );
    Seconds = e->GetData( (*pos)++ );
    Frames = e->GetData( (*pos)++ );
    FractFrames = e->GetData( (*pos)++ );	
    return true;
  }
  
  
  bool	MIDIShowControlPacket::Store3Param( MIDISystemExclusive *e ) const
  {
    bool f=true;
    
    if( HasQNumber )
    {
      f&=StoreAsciiNum( e, GetQNumber() );	
      if( HasQList )
      {
        e->PutByte( 0 );
        f&=StoreAsciiNum( e, GetQList() );
        
        if( HasQPath )
        {
          e->PutByte( 0 );
          f&=StoreAsciiNum( e, GetQPath() );
        }
        
      }
    }
    return f;
  }
  
  bool	MIDIShowControlPacket::Parse3Param( 
    const MIDISystemExclusive *e,
    int *pos 
    )
  {
    bool f=true;
    MIDICue v;
    
    if( e->GetData( *pos) !=0xf7 )
    {
      //
      // Read the Q number
      //
      
      f=ParseAsciiNum( e, pos, &v );
      
      if( f )
      {
        HasQNumber=true;
        SetQNumber( v );
        
        if( *pos < e->GetLength()  )
        {
          //
          // read the q list
          //
          
          f=ParseAsciiNum( e, pos, &v );
          
          if( f )
          {
            HasQList=true;
            SetQList( v );
            
            if( *pos < e->GetLength() )
            {
              //
              // read the q path
              //
              
              f=ParseAsciiNum( e, pos, &v );
              
              if( f )
              {
                HasQPath=true;
                SetQPath( v );
                
              }
              
            }
          }
          
        }
      }
    }
    
    return f;
  }
  
  
  bool	MIDIShowControlPacket::StoreSet( MIDISystemExclusive *e ) const
  {
    bool f=true;
    e->PutByte( (uchar)(GetControlNum() & 0x7f) );
    e->PutByte( (uchar)((GetControlNum()>>7) & 0x7f) );
    
    e->PutByte( (uchar)(GetControlVal() & 0x7f) );
    e->PutByte( (uchar)((GetControlVal()>>7) & 0x7f) );
    if( HasTime )
    {
      f=StoreTime( e );
    }
    return f;
  }
  
  bool	MIDIShowControlPacket::ParseSet( const MIDISystemExclusive *e,int *pos )
  {
    ulong v;
    
    v=e->GetData( (*pos)++ );
    v+=(e->GetData( (*pos)++ )<<7);
    SetControlNum( v );
    
    v=e->GetData( (*pos)++ );
    v+=(e->GetData( (*pos)++ )<<7);
    SetControlVal(  v );
    
    if( e->GetData(*pos)!=0xf7 )
    {
      return ParseTime( e, pos );	
    }
    else
    {
      return true;	
    }
    
  }
  
  bool	MIDIShowControlPacket::StoreFire( MIDISystemExclusive *e ) const
  {
    e->PutByte( (uchar)(GetMacroNum()) );
    return true;
  }
  
  bool	MIDIShowControlPacket::ParseFire( const MIDISystemExclusive *e, int *pos )
  {
    int v;
    
    v=e->GetData( (*pos)++ );
    SetMacroNum( v );
    
    return true;
  }
  
  bool	MIDIShowControlPacket::StoreQPath( MIDISystemExclusive *e ) const
  {
    if( HasQPath )
    {
      return StoreAsciiNum( e, GetQPath() );
    }
    else
    {
      return false;	
    }	
  }
  
  bool	MIDIShowControlPacket::ParseQPath( const MIDISystemExclusive *e, int *pos )
  {
    bool f=true;
    MIDICue v;
    
    if( e->GetData( *pos) !=0xf7 )
    {
      //
      // Read the Q Path
      //
      
      f=ParseAsciiNum( e, pos, &v );
      
      SetQPath( v );
      HasQPath=true;
      
      return f;
    }
    else
    {
      return false;	
    }
    
  }
  
  bool	MIDIShowControlPacket::StoreQList( MIDISystemExclusive *e ) const
  {
    if( HasQList )
    {
      return StoreAsciiNum( e, GetQList() );
    }
    else
    {
      return false;	
    }	
  }
  
  bool	MIDIShowControlPacket::ParseQList( const MIDISystemExclusive *e, int *pos )
  {
    bool f=true;
    MIDICue v;
    
    if( e->GetData( *pos) !=0xf7 )
    {
      //
      // Read the Q list
      //
      
      f=ParseAsciiNum( e, pos, &v );
      
      SetQList( v );
      HasQList=true;
      
      return f;
    }
    else
    {
      return false;	
    }	
  }
  
  bool	MIDIShowControlPacket::StoreAscii( MIDISystemExclusive *e, const char *str ) const
  {
    while( *str )
    {
      e->PutByte( *str++ );	
    }
    return true;
  }
  
  
  bool	MIDIShowControlPacket::StoreAsciiNum( 
    MIDISystemExclusive *e, 
    const MIDICue & num 
    ) const
  {
    char buf[32];
    bool f=false;
    
    *buf='\0';
    
    switch( num.GetNumValues() )
    {
    default:
    case 0:
      break;
    case 1:
      sprintf( buf, "%ld", num.GetV1() );
      break;
    case 2:
      sprintf( buf, "%ld.%ld", num.GetV1(), num.GetV2() );
      break;
    case 3:
      sprintf( buf, "%ld.%ld.%ld", num.GetV1(), num.GetV2(), num.GetV3() );
      break;
    }
    
    f=StoreAscii( e, buf );
    return f;
  }
  
  bool	MIDIShowControlPacket::ParseAsciiNum( 
    const MIDISystemExclusive *e,	
    int *pos, 
    MIDICue *num )
  {
    ulong v;
    uchar c;
    bool f;
    
    // check if there is a field
    
    c= e->GetData( *pos );
    
    if( c==0xf7 )
    {
      // no field
      
      return false;
    }
    
    f=ParseAsciiNum( e, pos, &v );
    
    if( f )
    {
      num->SetNumValues(1);
      num->SetV1( v );
    }
    
    //
    // Is there another field?
    //
    
    c= e->GetData( *pos );
    
    if( c=='.' )
    {
      //
      // yes read it
      //
      
      (*pos)++;
      
      f=ParseAsciiNum( e,pos,&v );
      
      if( f )
      {
        num->SetNumValues(2);
        num->SetV2( v );
      }
      
      //
      // Is there another field?
      //
      
      c= e->GetData( *pos );
      
      if( c=='.' )
      {
        //
        // yes read it
        //
        
        (*pos)++;
        
        f=ParseAsciiNum( e,pos,&v );
        
        if( f )
        {
          num->SetNumValues(2);
          num->SetV3( v );
        }
        
        //
        // Is there more fields?
        //
        
        c= e->GetData( *pos );
        
        if( c=='.' )
        {
          //
          // Yes, skip them
          //
          
          while( *pos < e->GetLength() )
          {
            c=e->GetData( (*pos) );
            
            if( c==0xf7 || c==0x00 )
            {
              break;
            }
            (*pos)++;
          }
        }
      }
    }
    
    while( ++(*pos) < e->GetLength() )
    {
      if( e->GetData( *pos )!=0 )
        break;
    }
    
    return f;
  }
  
  
  bool	MIDIShowControlPacket::ParseAsciiNum( 
    const MIDISystemExclusive *e, 
    int *pos, 
    ulong *num 
    )
  {
    bool f=true;
    ulong v=0;
    uchar c=0;
    
    //
    // Read ascii decimal digits until '.' or 0x00 or 0xf7
    //
    
    while(*pos<e->GetLength())
    {
      c=e->GetData( (*pos) );
      
      if( c>='0' && c<='9' )
      {
        //
        // If the character is ascii number then shift our current value and 
        // add the digit
        //
        
        v*=10;
        v+=c-'0';
      }
      else if( c==0x00 || c==0xf7 || c=='.' )
      {
        //
        // If the character is 0 or EOX or . then this field is over
        //
        
        *num=v;
        
        return f;
      }
      (*pos)++;
    }
    
    *num=v;
    return false;
  }
  
  
  
}
