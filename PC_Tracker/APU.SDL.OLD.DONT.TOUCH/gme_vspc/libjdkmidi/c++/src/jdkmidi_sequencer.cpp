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
#include "jdkmidi/sequencer.h"

namespace jdkmidi
{
  
  static void FixQuotes( char *s_ )
  {
    unsigned char *s = (unsigned char *)s_;
    while( *s )
    {
      if( *s==0xd2 || *s==0xd3 )
      {
        *s='"';
      }
      else if( *s==0xd5 )
      {
        *s='\'';
      }
      else if( *s>=0x80 )
      {
        *s = ' ';
      }
      s++;
    }
  }
  
////////////////////////////////////////////////////////////////////////////
  
  MIDISequencerGUIEventNotifier::MIDISequencerGUIEventNotifier()
  {
  }
  
  MIDISequencerGUIEventNotifier::~MIDISequencerGUIEventNotifier()
  {
  }
  
  
  
////////////////////////////////////////////////////////////////////////////
  
  
  MIDISequencerGUIEventNotifierText::MIDISequencerGUIEventNotifierText(
    FILE *f_
    )
    :
    f(f_),
    en(true)
  {
    
  }
  
  MIDISequencerGUIEventNotifierText::~MIDISequencerGUIEventNotifierText()
  {
  }
  
  
  void MIDISequencerGUIEventNotifierText::Notify(
    const MIDISequencer *seq,
    MIDISequencerGUIEvent e
    )
  {
    if( en )
    {
      fprintf( f, "GUI EVENT: G=%d, SG=%d, ITEM=%d\n",
               e.GetEventGroup(),
               e.GetEventSubGroup(),
               e.GetEventItem()
        );
      
      if( e.GetEventGroup() == MIDISequencerGUIEvent::GROUP_TRANSPORT )
      {
        if(
          e.GetEventItem() == MIDISequencerGUIEvent::GROUP_TRANSPORT_BEAT
          )
        {
          fprintf( f, "MEAS %3d BEAT %3d\n",
                   seq->GetCurrentMeasure()+1,
                   seq->GetCurrentBeat()+1
            );
        }
      }
      else
        if( e.GetEventGroup() == MIDISequencerGUIEvent::GROUP_CONDUCTOR )
        {
          
          if(
            e.GetEventItem() == MIDISequencerGUIEvent::GROUP_CONDUCTOR_TIMESIG
            )
          {
            fprintf( f, "TIMESIG: %d/%d\n",
                     seq->GetTrackState(0)->timesig_numerator,
                     seq->GetTrackState(0)->timesig_denominator
              );
          }
          if(
            e.GetEventItem() == MIDISequencerGUIEvent::GROUP_CONDUCTOR_TEMPO
            )
          {
            fprintf( f, "TEMPO: %3.2f\n",
                     seq->GetTrackState(0)->tempobpm
              );
          }
        }
    }
  }
  
  bool MIDISequencerGUIEventNotifierText::GetEnable() const
  {
    return en;
  }
  
  void MIDISequencerGUIEventNotifierText::SetEnable( bool f )
  {
    en = f;
  }
  
/////////////////////////////////////////////////////////////////////////////
  
  MIDISequencerTrackNotifier::MIDISequencerTrackNotifier(
    MIDISequencer *seq_,
    int trk,
    MIDISequencerGUIEventNotifier *n
    )
    :
    seq( seq_ ),
    track_num( trk ),
    notifier( n )
  {
    
  }
  
  MIDISequencerTrackNotifier::~MIDISequencerTrackNotifier()
  {
    
  }
  
  void MIDISequencerTrackNotifier::Notify( int item )
  {
    if( notifier )
    {
      notifier->Notify(
        seq,
        MIDISequencerGUIEvent(
          MIDISequencerGUIEvent::GROUP_TRACK,
          track_num,
          item
          )
        );
    }
  }
  
  void MIDISequencerTrackNotifier::NotifyConductor( int item )
  {
    // only notify conductor if we are track #0
    if( notifier && track_num==0 )
    {
      notifier->Notify(
        seq,
        MIDISequencerGUIEvent(
          MIDISequencerGUIEvent::GROUP_CONDUCTOR,
          0,
          item
          )
        );
    }
  }
  
/////////////////
  
  MIDISequencerTrackProcessor::MIDISequencerTrackProcessor() 
    :
    mute(false),
    solo(false),
    velocity_scale(100),
    rechannel(-1),
    transpose(0),
    extra_proc(0)
  {
  } 
  
  
  MIDISequencerTrackProcessor::~MIDISequencerTrackProcessor() 
  {
  } 
  
  
  void MIDISequencerTrackProcessor::Reset() 
  {
    mute=false;
    solo=false;
    velocity_scale=100;
    rechannel=-1;
    transpose=0;
  } 
  
  
  bool MIDISequencerTrackProcessor::Process( MIDITimedBigMessage *msg ) 
  {
    
    // are we muted?
    
    if( mute )
    {
      // yes, ignore event.
      return false;
    }
    
    // is the event a NoOp?
    
    if( msg->IsNoOp() )
    {
      // yes, ignore event.
      
      return false;
    }
    
    // pass the event to our extra_proc if we have one
    
    if( extra_proc && extra_proc->Process(msg)==false )
    {
      // extra_proc wanted to ignore this event
      return false;
    }
    
    // is it a normal MIDI channel message?
    if( msg->IsChannelMsg() )
    {
      // yes, are we to re-channel it?
      if( rechannel!=-1 )
      {
        msg->SetChannel( (unsigned char)rechannel );
      }
      
      // is it a note on message?
      if( msg->IsNoteOn() && msg->GetVelocity()>0 )
      {
        // yes, scale the velocity value as required
        
        int vel = (int)msg->GetVelocity();
        
        vel = vel*velocity_scale / 100;
        
        // make sure velocity is never less than 0
        
        if( vel<0 )
        {
          vel=0;
        }
        
        // rewrite the velocity
        
        msg->SetVelocity( (unsigned char)vel );
        
      }
      
      // is it a type of event that needs to be transposed?
      
      if( msg->IsNoteOn() || msg->IsNoteOff() || msg->IsPolyPressure() )
      {
        int new_note = ((int)msg->GetNote())+transpose;
        
        if( new_note>=0 && new_note<=127 )
        {
          // set new note number
          msg->SetNote( (unsigned char)new_note );
        }
        else
        {
          // otherwise delete this note - transposed value is out of range
          return false;
        }
        
      }
      
    }
    
    return true;
  } 
  
  
////////////////////////////////////////////////////////////////////////////
  
  MIDISequencerTrackState::MIDISequencerTrackState(
    MIDISequencer *seq_,
    int trk,
    MIDISequencerGUIEventNotifier *n
    ) 
    :
    MIDISequencerTrackNotifier( seq_, trk, n ),
    tempobpm( 120.0 ),
    volume(100),
    timesig_numerator(4),
    timesig_denominator(4),
    bender_value(0),
    got_good_track_name(false),
    notes_are_on(false),
    note_matrix()
  {
    *track_name = 0;
  } 
  
  
  MIDISequencerTrackState::~MIDISequencerTrackState() 
  {
  } 
  
  void MIDISequencerTrackState::GoToZero() 
  {
    tempobpm = 120.0;
    timesig_numerator=4;
    timesig_denominator=4;
    bender_value=0;
    note_matrix.Clear();
  } 
  
  void MIDISequencerTrackState::Reset() 
  {
    tempobpm = 120.0;
    volume=100;
    notes_are_on=false;
    timesig_numerator=4;
    timesig_denominator=4;
    bender_value=0;
    *track_name=0;
    note_matrix.Clear();
    got_good_track_name=false;
  } 
  
  
  bool MIDISequencerTrackState::Process( MIDITimedBigMessage *msg ) 
  {
    // is the event a NoOp?
    
    if( msg->IsNoOp() )
    {
      // yes, ignore event.
      
      return false;
    }
    
    // is it a normal MIDI channel message?
    if( msg->IsChannelMsg() )
    {
      
      if( msg->GetType()==PITCH_BEND ) // is it a bender event?
      {
        // yes
        // remember the bender wheel value
        
        bender_value = msg->GetBenderValue();
      }
      else if( msg->IsControlChange() ) // is it a control change event?
      {
        // yes
        
        // is it a volume change event?
        
        if( msg->GetController()==C_MAIN_VOLUME )
        {
          // yes, store the current volume level
          volume = msg->GetControllerValue();
          
          Notify(
            MIDISequencerGUIEvent::GROUP_TRACK_VOLUME
            );
        }
      }
      else if( msg->IsProgramChange() ) // is it a program change event?
      {
        // yes
        
        // update the current program change value
        pg = msg->GetPGValue();
        
        Notify(
          MIDISequencerGUIEvent::GROUP_TRACK_PG
          );
      }
      
    }
    else
    {
      // event is not a channel message. is it a meta-event?
      if( msg->IsMetaEvent() )
      {
        // yes, is it a tempo event
        if( msg->IsTempo() )
        {
          // yes get the current tempo
          
          tempobpm = ((float)msg->GetTempo32())*(1.0f/32.0f);
          if(tempobpm<1 )
          {
            tempobpm=120.0;
          }
          
          NotifyConductor(
            MIDISequencerGUIEvent::GROUP_CONDUCTOR_TEMPO
            );
        }
        else // is it a time signature event?
          if( msg->GetMetaType()==META_TIMESIG )
          {
            // yes, extract the current numerator and denominator
            
            timesig_numerator = msg->GetTimeSigNumerator();
            timesig_denominator = msg->GetTimeSigDenominator();
            
            NotifyConductor(
              MIDISequencerGUIEvent::GROUP_CONDUCTOR_TIMESIG
              );
            
          }
          else // is it a track name event?
            if( ( msg->GetMetaType()==META_TRACK_NAME
                  || msg->GetMetaType()==META_INSTRUMENT_NAME
                  || (!got_good_track_name && msg->GetMetaType()==META_GENERIC_TEXT && msg->GetTime()==0)
                  )
                &&
                msg->GetSysEx() )
            {
              got_good_track_name = true;
              
              // yes, copy the track name
              int len = msg->GetSysEx()->GetLength();
              
              if( len>(int)sizeof(track_name)-1 )
                len=(int)sizeof(track_name)-1;
              
              memcpy(track_name, msg->GetSysEx()->GetBuf(), len );
              
              track_name[len]='\0';
              
              FixQuotes( track_name );
              
              Notify(
                MIDISequencerGUIEvent::GROUP_TRACK_NAME
                );
              
            }
        
      }
      
    }
    
    // pass the message to our note matrix to keep track of all notes on
    // on this track
    
    if( note_matrix.Process( *msg ) )
    {
      // did the "any notes on" status change?
      
      if(    (	notes_are_on && note_matrix.GetTotalCount()==0)
             || (!notes_are_on && note_matrix.GetTotalCount()>0 ) )
      {
        // yes, toggle our notes_are_on flag
        notes_are_on = !notes_are_on;
        
        // and notify the gui about the activity on this track
        Notify(
          MIDISequencerGUIEvent::GROUP_TRACK_NOTE
          );
      }
    }
    
    
    return true;
  } 
  
////////////////////////////////////////////////////////////////////////////
  
  MIDISequencerState::MIDISequencerState(
    MIDISequencer *s,
    MIDIMultiTrack *	m,
    MIDISequencerGUIEventNotifier *n
    )
    :
    notifier(n),
    multitrack(m),
    num_tracks( m->GetNumTracks() ),
    iterator( m ),
    cur_clock(0),
    cur_time_ms(0),
    cur_beat(0),
    cur_measure(0),
    next_beat_time(0)
  {
    
    for( int i=0; i<num_tracks; ++i )
    {
      track_state[i] = new MIDISequencerTrackState( s, i, notifier );
    }
  }
  
  MIDISequencerState::MIDISequencerState( const MIDISequencerState &s )
    :
    notifier(s.notifier),
    multitrack(s.multitrack),
    num_tracks( s.num_tracks ),
    iterator( s.iterator ),
    cur_clock( s.cur_clock),
    cur_time_ms( s.cur_time_ms),
    cur_beat( s.cur_beat),
    cur_measure( s.cur_measure),
    next_beat_time(s.next_beat_time)
  {
    
    for( int i=0; i<num_tracks; ++i )
    {
      track_state[i] = new MIDISequencerTrackState( *s.track_state[i] );
    }
    
  }
  
  
  MIDISequencerState::~MIDISequencerState()
  {
    for( int i=0; i<num_tracks; ++i )
    {
      delete track_state[i];
    }
  }
  
  const MIDISequencerState & MIDISequencerState::operator = ( const MIDISequencerState & s )
  {
    if( num_tracks!=s.num_tracks )
    {
		{
			for( int i=0; i<num_tracks; ++i )
			{
				delete track_state[i];
			}
		}
      
      
       num_tracks = s.num_tracks;
      
	   {
			for( int i=0; i<num_tracks; ++i )
			{
				track_state[i] = new MIDISequencerTrackState( *s.track_state[i] );
			}
	   }
	 
    }
    
    
    iterator = s.iterator;
    cur_clock = s.cur_clock;
    cur_time_ms = s.cur_time_ms;
    cur_beat = s.cur_beat;
    cur_measure = s.cur_measure;
    next_beat_time = s.next_beat_time;
    
    return *this;
  }
  
  
////////////////////////////////////////////////////////////////////////////
  
  
  MIDISequencer::MIDISequencer(
    MIDIMultiTrack *m,
    MIDISequencerGUIEventNotifier *n
    ) 
    :
    solo_mode(false),
    tempo_scale(100),
    num_tracks( m->GetNumTracks() ),
    state( this, m,n ) // TODO: fix this hack
  {
    
    for( int i=0; i<num_tracks; ++i )
    {
      track_processors[i] = new MIDISequencerTrackProcessor;
    }
  } 
  
  
  MIDISequencer::~MIDISequencer() 
  {
    for(int i=0; i<num_tracks; ++i )
    {
      delete track_processors[i];
    }
  } 
  
  void MIDISequencer::ResetTrack( int trk )
  {
    state.track_state[trk]->Reset();
    track_processors[trk]->Reset();
  }
  
  void MIDISequencer::ResetAllTracks()
  {
    for( int i=0; i<num_tracks; ++i )
    {
      state.track_state[i]->Reset();
      track_processors[i]->Reset();
    }
  }
  
  MIDISequencerState *MIDISequencer::GetState()
  {
    return &state;
  }
  
  const MIDISequencerState *MIDISequencer::GetState() const
  {
    return &state;
  }
  
  void MIDISequencer::SetState( MIDISequencerState *s )
  {
    state = *s;
  }
  
  MIDIClockTime MIDISequencer::GetCurrentMIDIClockTime() const 
  {
    return state.cur_clock;
  } 
  
  double MIDISequencer::GetCurrentTimeInMs() const 
  {
    return state.cur_time_ms;
  } 
  
  int MIDISequencer::GetCurrentBeat() const 
  {
    return state.cur_beat;
  } 
  
  
  int MIDISequencer::GetCurrentMeasure() const 
  {
    return state.cur_measure;
  } 
  
  double MIDISequencer::GetCurrentTempoScale() const 
  {
    return ((double)tempo_scale)*0.01;
  } 
  
  double MIDISequencer::GetCurrentTempo() const 
  {
    return state.track_state[0]->tempobpm;
  } 
  
  MIDISequencerTrackState * MIDISequencer::GetTrackState( int trk ) 
  {
    return state.track_state[trk];
  } 
  
  const MIDISequencerTrackState * MIDISequencer::GetTrackState( int trk ) const 
  {
    return state.track_state[ trk ];
  } 
  
  MIDISequencerTrackProcessor * MIDISequencer::GetTrackProcessor( int trk ) 
  {
    return track_processors[trk];
  } 
  
  const MIDISequencerTrackProcessor * MIDISequencer::GetTrackProcessor( int trk ) const 
  {
    return track_processors[ trk ];
  } 
  
  bool MIDISequencer::GetSoloMode() const 
  {
    return solo_mode;
  } 
  
  void MIDISequencer::SetCurrentTempoScale( float scale ) 
  {
    tempo_scale = (int)(scale*100);
  } 
  
  void MIDISequencer::SetSoloMode( bool m, int trk ) 
  {
    int i;
    solo_mode = m;
    
    for( i=0; i<num_tracks; ++i )
    {
      if( i==trk )
      {
        track_processors[i]->solo = true;
      }
      else
      {
        track_processors[i]->solo = false;
      }
    }
    
  } 
  
  void MIDISequencer::GoToZero() 
  {
    // go to time zero
    
    for( int i=0; i<num_tracks; ++i )
    {
      state.track_state[i]->GoToZero();
    }
    
    state.iterator.GoToTime( 0 );
    
    state.cur_time_ms = 0.0;
    state.cur_clock = 0;
    
//	state.next_beat_time = state.multitrack->GetClksPerBeat();
    state.next_beat_time =
      state.multitrack->GetClksPerBeat()
      * 4 / (state.track_state[0]->timesig_denominator);
    
    // examine all the events at this specific time
    // and update the track states to reflect this time
    
    ScanEventsAtThisTime();
  } 
  
  bool MIDISequencer::GoToTime( MIDIClockTime time_clk ) 
  {
    // temporarily disable the gui notifier
    
    bool notifier_mode=false;
    if( state.notifier )
    {
      notifier_mode = state.notifier->GetEnable();
      state.notifier->SetEnable(false);
    }
    
    if( time_clk < state.cur_clock || time_clk==0 )
    {
      // start from zero if desired time is before where we are
      for( int i=0; i<state.num_tracks; ++i )
      {
        state.track_state[i]->GoToZero();
      }
      
      state.iterator.GoToTime( 0 );
      
      state.cur_time_ms = 0.0;
      state.cur_clock = 0;
//		state.next_beat_time = state.multitrack->GetClksPerBeat();
      state.next_beat_time =
        state.multitrack->GetClksPerBeat()
        * 4 / (state.track_state[0]->timesig_denominator);
      
      state.cur_beat = 0;
      state.cur_measure = 0;
      
    }
    
    MIDIClockTime t=0;
    int trk;
    MIDITimedBigMessage ev;
    
    while(
      GetNextEventTime( &t )
      && t<time_clk
      && GetNextEvent(&trk,&ev)
      )
    {
      ;
    }
    
    
    // examine all the events at this specific time
    // and update the track states to reflect this time
    
    ScanEventsAtThisTime();
    
    // re-enable the gui notifier if it was enabled previously
    if( state.notifier )
    {
      state.notifier->SetEnable( notifier_mode );
      
      // cause a full gui refresh now
      
      state.notifier->Notify( this, MIDISequencerGUIEvent::GROUP_ALL );
    }
    
    
    
    return true;
    
  } 
  
  bool MIDISequencer::GoToTimeMs( float time_ms ) 
  {
    // temporarily disable the gui notifier
    
    bool notifier_mode=false;
    if( state.notifier )
    {
      notifier_mode = state.notifier->GetEnable();
      state.notifier->SetEnable(false);
    }
    
    if( time_ms < state.cur_time_ms || time_ms==0.0 )
    {
      // start from zero if desired time is before where we are
      for( int i=0; i<state.num_tracks; ++i )
      {
        state.track_state[i]->GoToZero();
      }
      
      state.iterator.GoToTime( 0 );
      
      state.cur_time_ms = 0.0;
      state.cur_clock = 0;
//		state.next_beat_time = state.multitrack->GetClksPerBeat();
      state.next_beat_time =
        state.multitrack->GetClksPerBeat()
        * 4 / (state.track_state[0]->timesig_denominator);
      
      state.cur_beat = 0;
      state.cur_measure = 0;
    }
    
    float t=0;
    int trk;
    MIDITimedBigMessage ev;
    
    while(
      GetNextEventTimeMs( &t )
      && t<time_ms
      && GetNextEvent(&trk,&ev)
      )
    {
      ;
    }
    
    // examine all the events at this specific time
    // and update the track states to reflect this time
    
//	ScanEventsAtThisTime();
    
    // re-enable the gui notifier if it was enabled previously
    if( state.notifier )
    {
      state.notifier->SetEnable( notifier_mode );
      
      // cause a full gui refresh now
      
      state.notifier->Notify( this, MIDISequencerGUIEvent::GROUP_ALL );
    }
    
    
    return true;
    
  } 
  
  bool MIDISequencer::GoToMeasure( int measure, int beat ) 
  {
    // temporarily disable the gui notifier
    
    bool notifier_mode=false;
    if( state.notifier )
    {
      notifier_mode = state.notifier->GetEnable();
      state.notifier->SetEnable(false);
    }
    
    if( measure < state.cur_measure || measure==0 )
    {
      for( int i=0; i<state.num_tracks; ++i )
      {
        state.track_state[i]->GoToZero();
      }
      
      state.iterator.GoToTime( 0 );
      
      state.cur_time_ms = 0.0;
      state.cur_clock = 0;
      state.cur_beat = 0;
      state.cur_measure = 0;
//		state.next_beat_time = state.multitrack->GetClksPerBeat();
      state.next_beat_time =
        state.multitrack->GetClksPerBeat()
        * 4 / (state.track_state[0]->timesig_denominator);
      
    }
    
    MIDIClockTime t=0;
    int trk;
    MIDITimedBigMessage ev;
    
    // iterate thru all the events until cur-measure and cur_beat are
    // where we want them.
    
    while(
      GetNextEventTime( &t )
      && GetNextEvent(&trk,&ev)
      && state.cur_measure<=measure
      )
    {
      if( state.cur_measure==measure && state.cur_beat>=beat )
      {
        break;
      }
    }
    
    
    // examine all the events at this specific time
    // and update the track states to reflect this time
    
    
    ScanEventsAtThisTime();
    
    
    
    // re-enable the gui notifier if it was enabled previously
    if( state.notifier )
    {
      state.notifier->SetEnable( notifier_mode );
      
      // cause a full gui refresh now
      
      state.notifier->Notify( this, MIDISequencerGUIEvent::GROUP_ALL );
    }
    
    // return true if we actually found the measure requested
    
    return state.cur_measure == measure && state.cur_beat == beat;
    
  } 
  
  
  
  bool MIDISequencer::GetNextEventTimeMs( float *t ) 
  {
    MIDIClockTime ct;
    bool f = GetNextEventTime( &ct );
    
    if( f )
    {
      // calculate delta time from last event time
      
      double delta_clocks = (double)(ct - state.cur_clock);
      
      // calculate tempo in milliseconds per clock
      
      double clocks_per_sec = ((state.track_state[0]->tempobpm *
                                (((double)tempo_scale)*0.01)
                                * (1.0f/60.0f)) * state.multitrack->GetClksPerBeat());
      
      if( clocks_per_sec>0 )
      {
        float ms_per_clock = 1000.0f / (float)clocks_per_sec;
        
        // calculate delta time in milliseconds
        
        float delta_ms = float(delta_clocks * ms_per_clock);
        
        // return it added with the current time in ms.
        
        *t = delta_ms + state.cur_time_ms;
      }
      else
      {
        f=false;
      }
    }
    
    return f;
  } 
  
  bool MIDISequencer::GetNextEventTime( MIDIClockTime *t ) 
  {
    // ask the iterator for the current event time
    bool f = state.iterator.GetCurEventTime(t);
    
    if( f )
    {
      // if we have an event in the future, check to see if it is
      // further in time than the next beat marker
      
      if( (*t) >= state.next_beat_time )
      {
        // ok, the next event is a beat - return the next beat time
        
        *t = state.next_beat_time;
      }
    }
    
    return f;
  } 
  
  bool MIDISequencer::GetNextEvent( int *tracknum, MIDITimedBigMessage *msg ) 
  {
    MIDIClockTime t;
    
    
    // ask the iterator for the current event time
    if( state.iterator.GetCurEventTime(&t) )
    {
      // move current time forward one event
      
      MIDIClockTime new_clock;
      float new_time_ms=0.0f;
      
      GetNextEventTime( &new_clock );
      GetNextEventTimeMs( &new_time_ms );
      
      // must set cur_clock AFTER GetnextEventTimeMs() is called
      // since GetNextEventTimeMs() uses cur_clock to calculate
      
      state.cur_clock = new_clock;
      state.cur_time_ms = new_time_ms;
      
      
      // is the next beat marker before this event?
      
      if( state.next_beat_time<=t )
      {
        // yes, this is a beat event now.
        
        // say this event came on track 0, the conductor track
        *tracknum = 0;
        
        // put current info into beat marker message
        beat_marker_msg.SetBeatMarker();
        beat_marker_msg.SetTime( state.next_beat_time );
        *msg = beat_marker_msg;
        
        // update our beat count
        
        int new_beat = state.cur_beat+1;
        int new_measure = state.cur_measure;
        
        // do we need to update the measure number?
        
        if( new_beat>=state.track_state[0]->timesig_numerator )
        {
          // yup
          
          new_beat=0;
          ++new_measure;
        }
        
        // update our next beat time
        
        
        // denom=4  (16) ---> 4/16 midi file beats per symbolic beat
        // denom=3  (8)  ---> 4/8 midi file beats per symbolic beat
        // denom=2  (4)  ---> 4/4 midi file beat per symbolic beat
        // denom=1  (2)  ---> 4/2 midi file beats per symbolic beat
        // denom=0  (1)  ---> 4/1 midi file beats per symbolic beat
        
        state.next_beat_time +=
          state.multitrack->GetClksPerBeat()
          * 4 / (state.track_state[0]->timesig_denominator);
        
        state.cur_beat = new_beat;
        state.cur_measure = new_measure;
        
        // now notify the GUI that the beat number changed
        state.notifier->Notify(
          this,
          MIDISequencerGUIEvent(
            MIDISequencerGUIEvent::GROUP_TRANSPORT,
            0,
            MIDISequencerGUIEvent::GROUP_TRANSPORT_BEAT
            )
          );
        
        // if the new beat number is 0 then the measure changed too
        if( state.cur_beat==0 )
        {
          state.notifier->Notify(
            this,
            MIDISequencerGUIEvent(
              MIDISequencerGUIEvent::GROUP_TRANSPORT,
              0,
              MIDISequencerGUIEvent::GROUP_TRANSPORT_MEASURE
              )
            );
        }
        
        // give the beat marker event to the conductor track to process
        state.track_state[*tracknum]->Process(msg);
        
        
        return true;
      }
      else	// this event comes before the next beat
      {
        MIDITimedBigMessage *msg_ptr;
        
        if( state.iterator.GetCurEvent( tracknum, &msg_ptr ) )
        {
          int trk=*tracknum;
          
          // copy the event so Process can modify it
          
          *msg = *msg_ptr;
          
          bool allow_msg=true;
          
          // are we in solo mode?
          
          if( solo_mode )
          {
            // yes, only allow this message thru if
            // the track is either track 0
            // or it is explicitly solod.
            
            if( trk==0 || track_processors[trk]->solo )
            {
              allow_msg=true;
            }
            else
            {
              allow_msg=false;
            }
            
          }
          
          
          if( !(allow_msg
                && track_processors[trk]->Process(msg)
                && state.track_state[trk]->Process(msg))
            )
          {
            // the message is not allowed to come out!
            // erase it
            msg->SetNoOp();
          }
          
          // go to the next event on the multitrack
          state.iterator.GoToNextEvent();
          
          return true;
          
        }
      }
    }
    
    return false;
  } 
  
  void MIDISequencer::ScanEventsAtThisTime() 
  {
    // save the current iterator state
    
    MIDIMultiTrackIteratorState istate( state.iterator.GetState() );
    int prev_measure = state.cur_measure;
    int prev_beat = state.cur_beat;
    
    
    // process all messages up to and including this time only
    
    MIDIClockTime orig_clock = state.cur_clock;
    double orig_time_ms = state.cur_time_ms;
    
    
    MIDIClockTime t=0;
    int trk;
    MIDITimedBigMessage ev;
    
    while(
      GetNextEventTime( &t )
      && t==orig_clock
      && GetNextEvent(&trk,&ev)
      )
    {
      ;
    }
    
    // restore the iterator state
    state.iterator.SetState( istate );
    
    // and current time
    state.cur_clock = orig_clock;
    state.cur_time_ms = float(orig_time_ms);
    
    state.cur_measure=prev_measure;
    state.cur_beat = prev_beat;
    
  } 
  
  
  
  
}
