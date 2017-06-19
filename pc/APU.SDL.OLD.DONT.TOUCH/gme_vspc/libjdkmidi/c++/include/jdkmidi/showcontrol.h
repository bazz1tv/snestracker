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
#ifndef JDKMIDI_SHOWCONTROL_H
#define JDKMIDI_SHOWCONTROL_H



#include "jdkmidi/sysex.h"

namespace jdkmidi
{
//
// Hacked defines that really should just go away:
//
  
// Simple macros to make Setter and Getter methods easier
// in an object.
//
// examples:
//
//	_ATTRIBUTE(long,Position)
//	
//	would generate a private 'long Position;'
//	a 'const long GetPosition() const;' method
//	and a 'void SetPosition( long a );' method
//
//	
//
//	_ATTRIBUTE_REF( long, Position )
//
//	would generate a private 'long Position;'
//	a 'const long &GetPosition() const;' method
//	and a 'void SetPosition( const long &a );' method
//
//
//
//	_ACCESS( double, DPosition, Position )
//
//	Is used to access an attribute with a different type.
//	It would generate just the methods:
//	'double GetDPosition() const { return Position; }
//	'void SetDPosition( double a ) { Position=a; }
//
//
//	_ACCESS_REF( double, DPosition, Position )
//
//	Is used to access an attribute with a different type.
//	It would generate just the methods:
//	'const double &GetDPosition() const { return Position; }
//	'void SetDPosition( const double &a ) { Position=a; }
//
  
#define	_ATTRIBUTE( TYPE, NAME )	\
public:					\
	const TYPE Get##NAME() const	\
		{ return NAME;	}	\
	void Set##NAME( TYPE a )	\
		{ NAME=a;	}	\
private:				\
	TYPE NAME
  
  
  
#define	_ATTRIBUTE_REF( TYPE, NAME )	\
public:					\
	const TYPE &Get##NAME() const	\
		{ return NAME;	}	\
	void Set##NAME( const TYPE & a )\
		{ NAME=a;	}	\
private:				\
	TYPE NAME
  
  
#define _ACCESS( TYPE, NAME1, NAME2 )	\
public:					\
	const TYPE Get##NAME1() const	\
		{ return NAME2;	}	\
	void Set##NAME1( TYPE a )	\
		{ NAME2=a; }	
  
  
#define _ACCESS_REF( TYPE, NAME1, NAME2 )\
public:					\
	const TYPE &Get##NAME1() const	\
		{ return NAME2;	}	\
	void Set##NAME1( const TYPE &a )\
		{ NAME2=a;	}	
  
  
  
#define	_PATTRIBUTE( TYPE, NAME )	\
protected:				\
	const TYPE *Get##NAME() const	\
		{ return NAME;	}	\
	void Set##NAME( TYPE *a )	\
		{ NAME=a;	}	\
private:				\
	TYPE NAME
  
  
#define	_PATTRIBUTE_REF( TYPE, NAME )	\
protected:				\
	const TYPE &Get##NAME() const	\
		{ return NAME;	}	\
	void Set##NAME( const TYPE & a )\
		{ NAME=a;	}	\
private:				\
	TYPE NAME
  
  
#define _PACCESS( TYPE, NAME1, NAME2 )	\
protected:				\
	const TYPE Get##NAME1() const	\
		{ return NAME2;	}	\
	void Set##NAME1( const TYPE a )	\
		{ NAME2=a;		}	
  
  
#define _PACCESS_REF( TYPE, NAME1, NAME2 )\
protected:				\
	const TYPE &Get##NAME1() const	\
		{ return NAME2;	}	\
	void Set##NAME1( const TYPE &a )\
		{ NAME2=a;	}	
  
  
  
  class MIDICue
    {
    public:
      MIDICue( const MIDICue &c )			: v1( c.v1 ), v2( c.v2), v3( c.v3 ), num_values( c.num_values )		{}
      MIDICue()					: v1( 0 ), num_values(1)			{}
      MIDICue( ulong v1_ )				: v1( v1_ ),num_values(1)			{}
      MIDICue( ulong v1_,ulong v2_ )			: v1( v1_ ),v2( v2_ ),num_values(2)		{}
      MIDICue( ulong v1_,ulong v2_, ulong v3_ )	: v1( v1_ ),v2( v2_ ),v3( v3_ ),num_values(3)	{}
      
      void	Clear()			{ v1=0; num_values=1;	}
      operator ulong () const		{ return v1;	}
      
      const MIDICue &operator = ( const MIDICue &c )
        { v1=c.v1; v2=c.v2; v3=c.v3; num_values = c.num_values;	return *this;	}
      
      const MIDICue &operator = ( ulong v )
        { v1=v; num_values=1; return *this;	}
      
      const bool    operator == ( ulong v )
        { return v1==v;	}
      
      const MIDICue &operator == ( const MIDICue &c );
      const bool    operator != ( ulong v )
        { return v1!=v;	}
      const MIDICue &operator != ( const MIDICue &c );
      
      
      bool operator <= ( ulong v )
        { return v1<=v; }
      
      bool operator >= ( ulong v )
        { return v1>=v; }
      
      bool operator < ( ulong v )
        { return v1<v; }
      
      bool operator > ( ulong v )
        { return v1>v; }
      
      friend MIDICue operator - ( const MIDICue &c, const MIDICue &d );
      friend MIDICue operator - ( const MIDICue &c, ulong v );
      friend MIDICue operator - ( ulong v, const MIDICue &c );
      
      friend MIDICue operator + ( const MIDICue &c, const MIDICue &d );
      friend MIDICue operator + ( const MIDICue &c, ulong v );
      friend MIDICue operator + ( ulong v, const MIDICue &c );
      
      
      ulong	GetNumValues() const	{ return num_values;	 }
      ulong	GetV1()	const		{ return v1;	}
      ulong	GetV2()	const		{ return v2;	}
      ulong	GetV3()	const		{ return v3;	}
      
      void	SetNumValues( int a )	{ num_values=a;	}
      void	SetV1( ulong a )	{ v1=a;		}
      void	SetV2( ulong a )	{ v2=a;		}
      void	SetV3( ulong a )	{ v3=a;		}
    protected:
      ulong	v1,v2,v3;
      int	num_values;
    };
  
  inline MIDICue operator - ( const MIDICue &c, const MIDICue &d )
    {
      MIDICue result=c;
      
      result.v1 -= d.v1;
      result.v2 -= d.v2;
      result.v3 -= d.v3;
      
      return result;
    }
  
  inline MIDICue operator - ( const MIDICue &c, ulong v )
    {
      MIDICue result(c);
      
      result.v1 -= v;
      return result;
    }
  
  inline  MIDICue operator - ( ulong v, const MIDICue &c )
    {
      MIDICue result(v - c.v1 );
      
      return result;
    }
  
  inline  MIDICue operator + ( const MIDICue &c, const MIDICue &d )
    {
      MIDICue result=c;
      
      result.v1 += d.v1;
      result.v2 += d.v2;
      result.v3 += d.v3;
      
      return result;
      
    }
  
  inline  MIDICue operator + ( const MIDICue &c, ulong v )
    {
      MIDICue result(c);
      
      result.v1 += v;
      return result;
      
    }
  
  inline  MIDICue operator + ( ulong v, const MIDICue &c )
    {
      MIDICue result(v + c.v1 );
      
      return result;
    }
  
  
  
  
  enum MIDIShowCommand
    {
      MIDI_SC_GO 		= 0x01,
      MIDI_SC_STOP		= 0x02,
      MIDI_SC_RESUME		= 0x03,
      MIDI_SC_TIMED_GO	= 0x04,
      MIDI_SC_LOAD		= 0x05,
      MIDI_SC_SET		= 0x06,
      MIDI_SC_FIRE		= 0x07,
      MIDI_SC_ALL_OFF		= 0x08,
      MIDI_SC_RESTORE		= 0x09,
      MIDI_SC_RESET		= 0x0a,
      MIDI_SC_GO_OFF		= 0x0b,
      MIDI_SC_GO_JAM		= 0x10,
      MIDI_SC_STANDBY_PLUS	= 0x11,
      MIDI_SC_STANDBY_MINUS	= 0x12,
      MIDI_SC_SEQUENCE_PLUS	= 0x13,
      MIDI_SC_SEQUENCE_MINUS	= 0x14,
      MIDI_SC_START_CLOCK	= 0x15,
      MIDI_SC_STOP_CLOCK	= 0x16,
      MIDI_SC_ZERO_CLOCK	= 0x17,
      MIDI_SC_SET_CLOCK	= 0x18,
      MIDI_SC_MTC_CHASE_ON	= 0x19,
      MIDI_SC_MTC_CHASE_OFF	= 0x1a,
      MIDI_SC_OPEN_Q_LIST	= 0x1b,
      MIDI_SC_CLOSE_Q_LIST	= 0x1c,
      MIDI_SC_OPEN_Q_PATH	= 0x1d,
      MIDI_SC_CLOSE_Q_PATH	= 0x1e
    };
  
  
  class MIDIShowControlPacket
    {
    public:
      MIDIShowControlPacket();
      
      void Put_Go()
        { Put_Simple0( MIDI_SC_GO ); 		}
      
      void Put_Go( const MIDICue & q_number )
        { Put_Simple1( MIDI_SC_GO, q_number ); 	}
      
      void Put_Go( const MIDICue & q_number, const MIDICue & q_list )
        { Put_Simple2( MIDI_SC_GO, q_number, q_list ); 	}
      
      void Put_Go( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
        { Put_Simple3( MIDI_SC_GO, q_number, q_list, q_path ); 	}
      
      void Put_Stop()
        { Put_Simple0( MIDI_SC_STOP ); 	}				
      
      void Put_Stop( const MIDICue & q_number )
        { Put_Simple1( MIDI_SC_STOP, q_number ); 	}				
      
      void Put_Stop( const MIDICue & q_number, const MIDICue & q_list )
        { Put_Simple2( MIDI_SC_STOP, q_number, q_list ); 	}								
      
      void Put_Stop( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
        { Put_Simple3( MIDI_SC_STOP, q_number, q_list, q_path ); 	}	
      
      
      void Put_Resume()
        { Put_Simple0( MIDI_SC_RESUME ); 	}		
      
      void Put_Resume( const MIDICue & q_number )
        { Put_Simple1( MIDI_SC_RESUME, q_number ); 	}						
      
      void Put_Resume( const MIDICue & q_number, const MIDICue & q_list )
        { Put_Simple2( MIDI_SC_RESUME, q_number, q_list ); 	}		
      
      void Put_Resume( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
        { Put_Simple3( MIDI_SC_RESUME, q_number, q_list, q_path ); 	}		
      
      
      void Put_TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff
        )
        {
          Put_Simple0(  MIDI_SC_TIMED_GO );
          SetHours( hr );
          SetMinutes( mn );
          SetSeconds( sc );
          SetFrames( fr );
          SetFractFrames( ff );
          SetHasTime( true );
        }
      
      void Put_TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_number
        )
        {
          Put_Simple1(  MIDI_SC_TIMED_GO, q_number );
          SetHours( hr );
          SetMinutes( mn );
          SetSeconds( sc );
          SetFrames( fr );
          SetFractFrames( ff );
          SetHasTime( true );
        }
      
      void Put_TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_number, const MIDICue & q_list
        )
        {
          Put_Simple2(  MIDI_SC_TIMED_GO, q_number, q_list );
          SetHours( hr );
          SetMinutes( mn );
          SetSeconds( sc );
          SetFrames( fr );
          SetFractFrames( ff );
          SetHasTime( true );
        }
      
      void Put_TimedGo( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path 
        )
        {
          Put_Simple3(  MIDI_SC_TIMED_GO, q_number, q_list, q_path );
          SetHours( hr );
          SetMinutes( mn );
          SetSeconds( sc );
          SetFrames( fr );
          SetFractFrames( ff );
          SetHasTime( true );
        }
      
      
      void Put_Load( const MIDICue & q_number )
        { Put_Simple1( MIDI_SC_LOAD, q_number ); 	}	
      
      void Put_Load( const MIDICue & q_number, const MIDICue & q_list )
        { Put_Simple2( MIDI_SC_LOAD, q_number, q_list ); 	}		
      
      void Put_Load( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
        { Put_Simple3( MIDI_SC_LOAD, q_number, q_list, q_path ); 	}		
      
      
      void Put_Set( ulong ctrl_num, ulong ctrl_val )
        { 
          Put_Simple0( MIDI_SC_SET ); 
          SetControlNum( ctrl_num );
          SetControlVal( ctrl_val );	
        }
      
      void Put_Set( 
        ulong ctrl_num, 
        ulong ctrl_val,
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff 
        )
        { 
          Put_Simple0( MIDI_SC_SET ); 
          SetControlNum( ctrl_num );
          SetControlVal( ctrl_val );
          SetHours( hr );
          SetMinutes( mn );
          SetSeconds( sc );
          SetFrames( fr );
          SetFractFrames( ff );
          SetHasTime( true );									
        }
      
      
      void Put_Fire( uchar macro_num )
        { Put_Simple0( MIDI_SC_ALL_OFF );  SetMacroNum(macro_num);	}			
      
      void Put_AllOff()
        { Put_Simple0( MIDI_SC_ALL_OFF ); 	}			
      
      void Put_Restore()
        { Put_Simple0( MIDI_SC_RESTORE ); 	}			
      
      void Put_Reset()
        { Put_Simple0( MIDI_SC_RESET ); 	}			
      
      void Put_GoOff()
        { Put_Simple0( MIDI_SC_GO_OFF ); 	}						
      
      void Put_GoOff( const MIDICue & q_number )
        { Put_Simple1( MIDI_SC_GO_OFF,q_number ); 	}										
      
      void Put_GoOff( const MIDICue & q_number, const MIDICue & q_list )
        { Put_Simple2( MIDI_SC_GO_OFF, q_number, q_list ); 	}					
      
      void Put_GoOff( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
        { Put_Simple3( MIDI_SC_GO_OFF, q_number, q_list, q_path ); 	}										
      
      void Put_GoJam()
        { Put_Simple0( MIDI_SC_GO_JAM ); 	}
      
      void Put_GoJam( const MIDICue & q_number )
        { Put_Simple1( MIDI_SC_GO_JAM, q_number ); 	}				
      
      void Put_GoJam( const MIDICue & q_number, const MIDICue & q_list )
        { Put_Simple2( MIDI_SC_GO_JAM, q_number, q_list ); 	}				
      
      void Put_GoJam( const MIDICue & q_number, const MIDICue & q_list, const MIDICue & q_path )
        { Put_Simple3( MIDI_SC_GO_JAM, q_number, q_list, q_path ); 	}				
      
      
      void Put_StandbyPlus()
        { Put_Simple0( MIDI_SC_STANDBY_PLUS ); 	}
      
      void Put_StandbyPlus( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_STANDBY_PLUS ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      void Put_StandbyMinus()
        { Put_Simple0( MIDI_SC_STANDBY_MINUS ); 	}	
      
      void Put_StandbyMinus( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_STANDBY_MINUS ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_SequencePlus()
        { Put_Simple0( MIDI_SC_SEQUENCE_PLUS ); 	}	
      
      void Put_SequencePlus( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_SEQUENCE_PLUS ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_SequenceMinus()
        { Put_Simple0( MIDI_SC_SEQUENCE_MINUS ); 	}	
      
      void Put_SequenceMinus( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_SEQUENCE_MINUS ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_StartClock()
        { Put_Simple0( MIDI_SC_START_CLOCK ); 	}	
      
      void Put_StartClock( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_START_CLOCK ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_StopClock()
        { Put_Simple0( MIDI_SC_STOP_CLOCK ); 	}	
      
      void Put_StopClock( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_STOP_CLOCK ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_ZeroClock()
        { Put_Simple0( MIDI_SC_ZERO_CLOCK ); 	}	
      
      void Put_ZeroClock( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_ZERO_CLOCK ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_SetClock( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff
        )
        {
          Put_Simple0( MIDI_SC_SET_CLOCK );
          SetHours( hr );
          SetMinutes( mn );
          SetSeconds( sc );
          SetFrames( fr );
          SetFractFrames( ff );
          SetHasTime( true );
        }
      
      void Put_SetClock( 
        uchar hr, uchar mn, uchar sc, uchar fr, uchar ff,
        const MIDICue & q_list
        )
        {
          Put_Simple0( MIDI_SC_SET_CLOCK );
          SetHours( hr );
          SetMinutes( mn );
          SetSeconds( sc );
          SetFrames( fr );
          SetFractFrames( ff );
          SetHasTime( true );
          SetQList( q_list );
          SetHasQList(true);
        }
      
      
      void Put_MTCChaseOn()
        { Put_Simple0( MIDI_SC_MTC_CHASE_ON ); 	}	
      
      void Put_MTCChaseOn( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_MTC_CHASE_ON ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      void Put_MTCChaseOff()
        { Put_Simple0( MIDI_SC_MTC_CHASE_OFF ); 	}	
      
      void Put_MTCChaseOff( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_MTC_CHASE_OFF ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_OpenQList( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_OPEN_Q_LIST ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_CloseQList( const MIDICue & q_list )
        { 
          Put_Simple0( MIDI_SC_CLOSE_Q_LIST ); 	
          SetQList(q_list);
          SetHasQList(true);
        }
      
      void Put_OpenQPath( const MIDICue & q_path )
        { 
          Put_Simple0( MIDI_SC_OPEN_Q_PATH ); 	
          SetQPath(q_path);
          SetHasQPath(true);
        }
      
      void Put_CloseQPath( const MIDICue & q_path )
        { 
          Put_Simple0( MIDI_SC_CLOSE_Q_PATH ); 	
          SetQPath(q_path);
          SetHasQPath(true);
        }
      
      bool	ParseEntireSysEx( const MIDISystemExclusive *e );
      bool	StoreToSysEx( MIDISystemExclusive *e ) const;
    protected:
      bool	StoreTime( MIDISystemExclusive *e ) const;
      bool	ParseTime( const MIDISystemExclusive *e, int *pos);
      bool	Store3Param( MIDISystemExclusive *e ) const;
      bool	Parse3Param( const MIDISystemExclusive *e, int *pos );
      bool	StoreSet( MIDISystemExclusive *e ) const;	
      bool	ParseSet( const MIDISystemExclusive *e,int *pos );
      bool	StoreFire( MIDISystemExclusive *e ) const;		
      bool	ParseFire( const MIDISystemExclusive *e, int *pos );
      bool	StoreQPath( MIDISystemExclusive *e ) const;			
      bool	ParseQPath( const MIDISystemExclusive *e, int *pos );
      bool	StoreQList( MIDISystemExclusive *e ) const;	
      bool	ParseQList( const MIDISystemExclusive *e, int *pos );
      bool	StoreAscii( MIDISystemExclusive *e, const char *str ) const;			
      
      bool	StoreAsciiNum( MIDISystemExclusive *e, const MIDICue & num ) const;
      bool	ParseAsciiNum( const MIDISystemExclusive *e, int *pos, MIDICue *num );
      bool	ParseAsciiNum( const MIDISystemExclusive *e, int *pos, ulong *num );
      
      void ClearVariableStuff();
      
      void Put_Simple0( MIDIShowCommand cmd )	
        {
          ClearVariableStuff();
          SetCommand( cmd );
        }
      
      
      void Put_Simple1( 
        MIDIShowCommand cmd, 
        const MIDICue & q_number 
        )
        {
          ClearVariableStuff();
          SetCommand( cmd );
          SetQNumber( q_number );
          SetHasQNumber( true );
        }
      
      
      void Put_Simple2( 
        MIDIShowCommand cmd, 
        const MIDICue & q_number, 
        const MIDICue & q_list 
        )
        {
          ClearVariableStuff();
          SetCommand( cmd );
          SetQNumber( q_number );
          SetHasQNumber( true );
          SetQList( q_list );
          SetHasQList( true );
        }
      
      
      void Put_Simple3( 
        MIDIShowCommand cmd, 
        const MIDICue & q_number, 
        const MIDICue & q_list, 
        const MIDICue & q_path 
        )
        {
          ClearVariableStuff();
          SetCommand( cmd );
          SetQNumber( q_number );
          SetHasQNumber(true);
          SetQList( q_list );
          SetHasQList(true);	
          SetQPath( q_path );
          SetHasQPath(true);	
        }
      
      
      _ATTRIBUTE( uchar, DeviceId	);
      _ATTRIBUTE( uchar, CommandFmt	);
      _ATTRIBUTE( MIDIShowCommand, Command );
      _ATTRIBUTE( bool, HasTime	);
      _ATTRIBUTE( bool, HasQNumber	);
      _ATTRIBUTE( bool, HasQList	);
      _ATTRIBUTE( bool, HasQPath	);
      _ATTRIBUTE( uchar, Hours      );
      _ATTRIBUTE( uchar, Minutes	);
      _ATTRIBUTE( uchar, Seconds	);
      _ATTRIBUTE( uchar, Frames		);
      _ATTRIBUTE( uchar, FractFrames	);
      
      _ATTRIBUTE_REF( MIDICue, QNumber	);
      _ATTRIBUTE_REF( MIDICue, QList	);
      _ATTRIBUTE_REF( MIDICue, QPath	);
      
      _ATTRIBUTE( ulong, Val1		);
      _ATTRIBUTE( ulong, Val2		);
      
      _ACCESS( ulong, MacroNum, Val1	);
      _ACCESS( ulong, ControlNum, Val1);
      _ACCESS( ulong, ControlVal, Val2);
    };

// unlearn the brain damage
#undef _ATTRIBUTE
#undef _ATTRIBUTE_REF
#undef _ACCESS
#undef _ACCESS_REF
#undef _PATTRIBUTE
#undef _PATTRIBUTE_REF
#undef _PACCESS
#undef _PACCESS_REF
  
}
    
#endif
