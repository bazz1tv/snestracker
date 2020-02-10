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

#include "jdkmidi/filereadmultitrack.h"

namespace jdkmidi
{
  
  
  MIDIFileReadMultiTrack::MIDIFileReadMultiTrack( MIDIMultiTrack *mlttrk )
    : multitrack( mlttrk ), cur_track(-1)
  {
    
  }
  
  MIDIFileReadMultiTrack::~MIDIFileReadMultiTrack()
  {
    
  }
  
  void    MIDIFileReadMultiTrack::mf_error( char * )	
  {
  }	
  
  
  
  void    MIDIFileReadMultiTrack::mf_starttrack( int trk )	
  {
    cur_track = trk;
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_endtrack( int trk )	
  {
    cur_track = -1;
  }	
  
  void 	MIDIFileReadMultiTrack::AddEventToMultiTrack( 
    const MIDITimedMessage &msg,
    MIDISystemExclusive *sysex,
    int dest_track
    )	
  {
    if( dest_track!=-1 && dest_track<multitrack->GetNumTracks() )
    {
      MIDITrack *t = multitrack->GetTrack( dest_track );
      
      if( t )
      {
        t->PutEvent( msg, sysex );
      }		
    }	
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_header( 
    int the_format_, 
    int ntrks_, 
    int division_ 
    )	
  {
    the_format = the_format_;
    num_tracks = ntrks_;
    division = division_;
    
    multitrack->SetClksPerBeat( division );
    
  }	
  
  
  void    MIDIFileReadMultiTrack::ChanMessage( const MIDITimedMessage &msg )	
  {
    if( the_format == 0 || cur_track==0 )
    {
      // split format 0 files into separate tracks, one for each channel,
      // keep track 0 for tempo and meta-events
      
      AddEventToMultiTrack( msg, 0, msg.GetChannel()+1 );
    }
    else
    {
      AddEventToMultiTrack( msg, 0, cur_track );
    }	
  }	
  
  
  
  
  void    MIDIFileReadMultiTrack::mf_sysex( MIDIClockTime time, const MIDISystemExclusive &ex )	
  {
    MIDITimedMessage msg;
    
    msg.SetSysEx();
    msg.SetTime(time);
    
    MIDISystemExclusive *sysex = new MIDISystemExclusive( ex );
    
    AddEventToMultiTrack( msg, sysex, cur_track );
  }	
  
  
  
  void    MIDIFileReadMultiTrack::mf_arbitrary( MIDIClockTime time, int len, unsigned char *data )	
  {
    // ignore arbitrary byte strings
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_metamisc( MIDIClockTime time, int, int, unsigned char * 	)	
  {
    // ignore miscellaneous meta events
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_seqnum( MIDIClockTime time, int )	
  {
    // ignore sequence number events
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_smpte( MIDIClockTime time, int, int, int, int, int )	
  {
    // ignore smpte events
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_timesig( 
    MIDIClockTime time, 
    int num,
    int denom_power, 
    int clks_per_metro,
    int notated_32nd_per_quarter
    )	
  {
    MIDITimedMessage msg;
    
    int denom= 1<<denom_power;
    
    msg.SetTimeSig( (unsigned char)num, (unsigned char)denom );
    msg.SetTime( time );
    
    MIDISystemExclusive *sysex = new MIDISystemExclusive( 4 );
    
    sysex->PutByte( (unsigned char)num );
    sysex->PutByte( (unsigned char)denom_power );
    sysex->PutByte( (unsigned char)clks_per_metro );
    sysex->PutByte( (unsigned char)notated_32nd_per_quarter );
    
    AddEventToMultiTrack( msg, sysex, cur_track );
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_tempo( MIDIClockTime time, unsigned long tempo )	
  {
    unsigned long tempo_bpm_times_32;
    
    if( tempo==0 )
      tempo=1;
    
    // tempo is in microseconds per beat
    
    // calculate beats per second by
    
    float beats_per_second = static_cast<float>( 1e6 / (double)tempo );// 1 million microseconds per second
    
    float beats_per_minute = beats_per_second * 60;
    
    tempo_bpm_times_32 = static_cast<unsigned long>(beats_per_minute * 32.0);
    
    MIDITimedMessage msg;
    
    msg.SetTempo32( static_cast<unsigned short>(tempo_bpm_times_32) );
    msg.SetTime( time );
    
    AddEventToMultiTrack( msg, 0, cur_track );
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_keysig(MIDIClockTime time, int c, int v )	
  {
    MIDITimedMessage msg;
    
    msg.SetKeySig( (unsigned char)c, (unsigned char)v );
    msg.SetTime( time );
    
    AddEventToMultiTrack( msg, 0, cur_track );    
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_sqspecific( MIDIClockTime time, int, unsigned char * )	
  {
    // ignore any sequencer specific messages
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_text( MIDIClockTime time, int type, int len, unsigned char *s )	
  {
    MIDITimedMessage msg;
    
    msg.SetStatus( META_EVENT );
    msg.SetMetaType( (uchar)type ); // remember - MF_*_TEXT* id codes match META_*_TEXT codes
    msg.SetTime( time );
    
    MIDISystemExclusive *sysex = new MIDISystemExclusive(len);
    
    for( int i=0; i<len; ++i )
    {
      sysex->PutSysByte( s[i] );
    }
    
    AddEventToMultiTrack( msg, sysex, cur_track );		
  }	
  
  
  void    MIDIFileReadMultiTrack::mf_eot( MIDIClockTime time )	
  {
    MIDITimedMessage msg;
    
    msg.SetStatus( META_EVENT );
    msg.SetMetaType( META_END_OF_TRACK );
    msg.SetTime( time );
    
    AddEventToMultiTrack( msg, 0, cur_track );	
  }	
  
  
  
}
