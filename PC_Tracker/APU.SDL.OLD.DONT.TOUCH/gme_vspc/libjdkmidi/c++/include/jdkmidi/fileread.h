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

#ifndef JDKMIDI_FILEREAD_H
#define JDKMIDI_FILEREAD_H

#include "jdkmidi/midi.h"
#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"
#include "jdkmidi/file.h"

namespace jdkmidi
{
  class MIDIFileReadStream;
  class MIDIFileReadStreamFile;
  class MIDIFileEvents;
  class MIDIFileRead;
  
  
  class MIDIFileReadStream	
    {
    public:
      MIDIFileReadStream()
        {
        }
      
      virtual ~MIDIFileReadStream()
        {
        }
      
      virtual int ReadChar() = 0;
    }; 
  
  class MIDIFileReadStreamFile : public MIDIFileReadStream	
    {
    public:	
      explicit MIDIFileReadStreamFile( const char *fname )	
        {
          f=fopen( fname, "rb" );
        } 
      
      explicit MIDIFileReadStreamFile( FILE *f_ ) : f(f_)	
        {
        } 
      
      virtual ~MIDIFileReadStreamFile()	
        {
          if( f )
          {	
            fclose(f);
          }
          
        } 
      
      
      virtual int ReadChar()	
        {
          int r=-1;
          
          if( f && !feof(f) && !ferror(f) )
          {	
            r=fgetc(f);
          }
          
          return r;
        } 
      
      
    private:
      FILE *f;
    }; 
  
  class MIDIFileEvents : protected MIDIFile	
    {
    public:
      MIDIFileEvents()
        {
        }
      
      virtual ~MIDIFileEvents()
        {
        }
      
      
//
// The possible events in a MIDI Files
//
      
      virtual void    mf_system_mode( const MIDITimedMessage &msg );
      virtual void    mf_note_on( const MIDITimedMessage &msg );
      virtual void    mf_note_off( const  MIDITimedMessage &msg );
      virtual void    mf_poly_after( const MIDITimedMessage &msg );
      virtual void    mf_bender( const MIDITimedMessage &msg );
      virtual void    mf_program( const MIDITimedMessage &msg );
      virtual void    mf_chan_after( const MIDITimedMessage &msg );
      virtual void    mf_control( const MIDITimedMessage &msg );
      virtual void    mf_sysex( MIDIClockTime time, const MIDISystemExclusive &ex );
      
      virtual void    mf_arbitrary( MIDIClockTime time, int len, unsigned char *data );
      virtual void    mf_metamisc( MIDIClockTime time, int, int, unsigned char * 	);
      virtual void    mf_seqnum( MIDIClockTime time, int );
      virtual void    mf_smpte( MIDIClockTime time, int, int, int, int, int );
      virtual void    mf_timesig( MIDIClockTime time, int, int, int, int );
      virtual void    mf_tempo( MIDIClockTime time, unsigned long tempo );
      virtual void    mf_keysig(MIDIClockTime time, int, int );
      virtual void    mf_sqspecific( MIDIClockTime time, int, unsigned char * );
      virtual void    mf_text( MIDIClockTime time, int, int, unsigned char * );
      virtual void    mf_eot( MIDIClockTime time );
      
//
// the following methods are to be overridden for your specific purpose
//
      
      virtual void    mf_error( char * );
      
      virtual void    mf_starttrack( int trk );
      virtual void    mf_endtrack( int trk );
      virtual void    mf_header( int, int, int );
      
//
// Higher level dispatch functions
// 
      virtual	void	UpdateTime( MIDIClockTime delta_time );
      virtual	void    MetaEvent( MIDIClockTime time, int type, int len, unsigned char *buf );
      virtual	void    ChanMessage( const MIDITimedMessage &msg );
      
    }; 
  
  class MIDIFileRead : protected MIDIFile	
    {
    public:
      
      MIDIFileRead(
        MIDIFileReadStream *input_stream_,
        MIDIFileEvents *event_handler_,
        unsigned long max_msg_len=8192 
        );
      virtual         ~MIDIFileRead();
      
      virtual bool    Parse();
      
      int		GetFormat()			{ return header_format;		}
      int		GetNumberTracks()   { return header_ntrks;		}
      int		GetDivision()		{ return header_division;	}
      
    protected:
      
      virtual	int  ReadHeader();
      
      virtual void    mf_error( char * );
      
    protected:
      int  no_merge;
      MIDIClockTime   cur_time;
      int  skip_init;
      unsigned long   to_be_read;
      int  	cur_track;
      int  	abort_parse;
      
      unsigned char   *the_msg;
      int		max_msg_len;
      int   	msg_index;
      
    private:
      unsigned long   ReadVariableNum();
      unsigned long   Read32Bit();
      int  	Read16Bit();
      
      void    ReadTrack();
      
      void    MsgAdd( int );
      void    MsgInit();
      
      int  	EGetC();
      
      int  	ReadMT( unsigned long, int );
      void    BadByte( int );
      
      void	FormChanMessage( unsigned char st, unsigned char b1, unsigned char b2 );
      
      
      int		header_format;
      int		header_ntrks;
      int		header_division;
      
      MIDIFileReadStream *input_stream;
      MIDIFileEvents *event_handler;
    }; 
}

#endif

