#include "jdkmidi/world.h"
#include "jdkmidi/advancedsequencer.h"


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
  
  AdvancedSequencer::AdvancedSequencer()
    :
    thru_processor(2),
    thru_transposer(),
    thru_rechannelizer(),
    driver(256,stdout),
    tracks(17),
    notifier( stdout ),
    seq( &tracks, &notifier ),
    mgr( &driver, &notifier, &seq ),
    repeat_start_measure(0),
    repeat_end_measure(0),
    repeat_play_mode(false),
    num_warp_positions(0),
    file_loaded(false),
    chain_mode(false)
  {
  }
  
  AdvancedSequencer::~AdvancedSequencer()
  {
    Stop();
    CloseMIDI();
    
    for( int i=0; i<num_warp_positions; ++i )
    {
      delete warp_positions[i];
    }
  }
  
  
  
  bool AdvancedSequencer::OpenMIDI( int in_port, int out_port, int timer_resolution )
  {
#if 0
    CloseMIDI();
    
    if( !driver.StartTimer( timer_resolution ) )
    {
      return false;
    }
    
    if( in_port!=-1 )
    {
      driver.OpenMIDIInPort( in_port );
    }
    
    if( driver.OpenMIDIOutPort( out_port ) )
    {
      return true;
    }
    else
    {
      return false;
    }
#else
    return true;
#endif
  }
  
  
  void AdvancedSequencer::CloseMIDI()
  {
    Stop();
#if 0
    driver.StopTimer();
    driver.AllNotesOff();
    Sleep(100);
    driver.CloseMIDIInPort();
    driver.CloseMIDIOutPort();
#endif
  }
  
  
  void AdvancedSequencer::SetMIDIThruEnable( bool f )
  {
    driver.SetThruEnable( f );
  }
  
  bool AdvancedSequencer::GetMIDIThruEnable() const
  {
    return driver.GetThruEnable();
  }
  
  void AdvancedSequencer::SetMIDIThruChannel( int chan )
  {
    thru_rechannelizer.SetAllRechan( chan );
    driver.AllNotesOff();
    
  }
  
  
  int AdvancedSequencer::GetMIDIThruChannel() const
  {
    return thru_rechannelizer.GetRechanMap( 0 );
  }
  
  
  
  void AdvancedSequencer::SetMIDIThruTranspose( int val )
  {
    thru_transposer.SetAllTranspose( val );
    driver.AllNotesOff();
  }
  
  
  int AdvancedSequencer::GetMIDIThruTranspose() const
  {
    return thru_transposer.GetTransposeChannel( 0 );
  }
  
  
  
  bool AdvancedSequencer::Load( const char *fname )
  {
    char realname[1024];
    
    strcpy( realname, fname );
    int orignamelen = strlen(fname);
    
    chain_mode =false;
    if( orignamelen>0 )
    {
      if( realname[orignamelen-1]=='+' )
      {
        realname[orignamelen-1]=0;
        chain_mode = true;
      }
    }
    
    MIDIFileReadStreamFile mfreader_stream( realname );
    MIDIFileReadMultiTrack track_loader( &tracks );
    MIDIFileRead reader( &mfreader_stream, &track_loader );
    
    Stop();
    driver.AllNotesOff();
    
    tracks.Clear();
    seq.ResetAllTracks();
    
    if( reader.Parse() )
    {
      file_loaded=true;
      
      Reset();
      
      GoToMeasure( 0 );
      ExtractWarpPositions();
      
    }
    else
    {
      file_loaded=false;
    }
    return file_loaded;
  }
  
  
  void AdvancedSequencer::Reset()
  {
    Stop();
    driver.AllNotesOff();
    UnmuteAllTracks();
    UnSoloTrack();
    SetTempoScale( 1.00 );
    
    seq.ResetAllTracks();
    GoToMeasure( 0 );
    
  }
  
  void AdvancedSequencer::GoToTime( MIDIClockTime t )
  {
    if( mgr.IsSeqPlay() )
    {
      Stop();
      seq.GoToTime( t+1 );
      Play();
    }
    else
    {
      seq.GoToTime( t+1 );
    }
    
  }
  
  
  void AdvancedSequencer::GoToMeasure( int measure, int beat )
  {
    if( !file_loaded )
    {
      return;
    }
    // figure out which warp item we use
    // try warp to the last warp point BEFORE the
    // requested measure
    
    int warp_to_item = (measure-1) / MEASURES_PER_WARP;
    
    
    if( warp_to_item >= num_warp_positions )
      warp_to_item = num_warp_positions-1;
    
    if( warp_to_item<0 )
      warp_to_item=0;
    
    
    if( mgr.IsSeqPlay() )
    {
      Stop();
      
      if( warp_positions[warp_to_item] )
      {
        seq.SetState( warp_positions[warp_to_item] );
      }
      seq.GoToMeasure( measure, beat );
      Play();
    }
    else
    {
      if( warp_positions[warp_to_item] )
      {
        seq.SetState( warp_positions[warp_to_item] );
      }
      
      seq.GoToMeasure( measure, beat );
      for( int i=0; i<seq.GetNumTracks(); ++i )
      {
        seq.GetTrackState(i)->note_matrix.Clear();
      }
    }
  }
  
  
  void AdvancedSequencer::Play( int clock_offset )
  {
    if( !file_loaded )
    {
      return;
    }
    
    Stop();
    
    for( int i=0; i<seq.GetNumTracks(); ++i )
    {
      seq.GetTrackState(i)->note_matrix.Clear();
    }
    
    
    if( repeat_play_mode )
    {
      seq.GoToMeasure( repeat_start_measure );
    }
    
    MIDIClockTime cur_time = seq.GetCurrentMIDIClockTime();
    
    if( (long)cur_time>-clock_offset )
      cur_time+=clock_offset;
    
    seq.GoToTime( cur_time );
    
    mgr.SetSeqOffset( (unsigned long)seq.GetCurrentTimeInMs() );
    mgr.SetTimeOffset( 0 );
    
    mgr.SeqPlay();
  }
  
  
  
  void AdvancedSequencer::RepeatPlay( bool enable, int start_measure, int end_measure )
  {
    if( !file_loaded )
    {
      return;
    }
    
    if( start_measure < end_measure && start_measure>=0 )
    {
      repeat_play_mode = enable;
      repeat_start_measure = start_measure;
      repeat_end_measure = end_measure;
    }
    else
    {
      repeat_play_mode = false;
    }
    
    mgr.SetRepeatPlay(
      repeat_play_mode,
      repeat_start_measure,
      repeat_end_measure
      );
  }
  
  
  void AdvancedSequencer::Pause()
  {
    if( !file_loaded )
    {
      return;
    }
    Stop();
  }
  
  
  void AdvancedSequencer::Stop()
  {
    if( !file_loaded )
    {
      return;
    }
    if( !mgr.IsSeqStop() )
    {
      mgr.SeqStop();

      driver.AllNotesOff();
      for( int i=0; i<seq.GetNumTracks(); ++i )
      {
        seq.GetTrackState(i)->note_matrix.Clear();
      }
      
    }
  }
  
  
  
  void AdvancedSequencer::UnmuteAllTracks()
  {
    if( !file_loaded )
    {
      return;
    }
    for( int i=0; i<seq.GetNumTracks(); ++i )
    {
      if( seq.GetTrackProcessor(i)->mute )
      {
        seq.GetTrackState(i)->note_matrix.Clear();
        seq.GetTrackProcessor(i)->mute = false;
      }
    }
    
    driver.AllNotesOff();
  }
  
  void AdvancedSequencer::SoloTrack( int trk )
  {
    if( !file_loaded )
    {
      return;
    }
    if( trk==-1 )
    {
      seq.SetSoloMode(false);
      driver.AllNotesOff();
      
      for( int i=0; i<seq.GetNumTracks(); ++i )
      {
        seq.GetTrackState(i)->note_matrix.Clear();
      }
    }
    else
    {
      seq.SetSoloMode(true,trk);
      driver.AllNotesOff();
      
      for( int i=0; i<seq.GetNumTracks(); ++i )
      {
        seq.GetTrackState(i)->note_matrix.Clear();
      }
      
    }
  }
  
  
  void AdvancedSequencer::UnSoloTrack()
  {
    if( !file_loaded )
    {
      return;
    }
    seq.SetSoloMode( false );
    driver.AllNotesOff();
    for( int i=0; i<seq.GetNumTracks(); ++i )
    {
      seq.GetTrackState(i)->note_matrix.Clear();
    }
  }
  
  
  void AdvancedSequencer::SetTrackMute( int trk, bool f )
  {
    if( !file_loaded )
    {
      return;
    }
    seq.GetTrackProcessor(trk)->mute = f;
    driver.AllNotesOff();
  }
  
  
  
  void AdvancedSequencer::SetTempoScale( double scale )
  {
    if( !file_loaded )
    {
      return;
    }
    seq.SetCurrentTempoScale( static_cast<float>(scale) );
  }
  
  
  double AdvancedSequencer::GetTempoWithoutScale() const
  {
    return seq.GetCurrentTempo();
  }
  
  
  double AdvancedSequencer::GetTempoWithScale() const
  {
    return seq.GetCurrentTempo() * seq.GetCurrentTempoScale();
  }
  
  
  
  int AdvancedSequencer::GetMeasure() const
  {
    if( !file_loaded )
    {
      return 0;
    }
    
    return seq.GetCurrentMeasure();
  }
  
  
  int AdvancedSequencer::GetBeat() const
  {
    if( !file_loaded )
    {
      return 0;
    }
    
    return seq.GetCurrentBeat();
  }
  
  
  
  int AdvancedSequencer::GetTimeSigNumerator() const
  {
    if( !file_loaded )
    {
      return 4;
    }
    return seq.GetTrackState(0)->timesig_numerator;
  }
  
  
  int	AdvancedSequencer::GetTimeSigDenominator() const
  {
    if( !file_loaded )
    {
      return 4;
    }
    
    return seq.GetTrackState(0)->timesig_denominator;
  }
  
  
  
  int AdvancedSequencer::GetTrackNoteCount( int trk ) const
  {
    if( !file_loaded )
    {
      return 0;
    }
    
    if( mgr.IsSeqStop() )
    {
      return 0;
    }
    else
    {
      return seq.GetTrackState(trk)->note_matrix.GetTotalCount();
    }
  }
  
  
  const char *AdvancedSequencer::GetTrackName( int trk ) const
  {
    if( !file_loaded )
    {
      return "";
    }
    
    return seq.GetTrackState(trk)->track_name;
  }
  
  
  int AdvancedSequencer::GetTrackVolume( int trk ) const
  {
    if( !file_loaded )
    {
      return 100;
    }
    
    return seq.GetTrackState(trk)->volume;
  }
  
  
  
  void AdvancedSequencer::SetTrackVelocityScale( int trk, int scale )
  {
    if( !file_loaded )
    {
      return;
    }
    
    seq.GetTrackProcessor(trk)->velocity_scale = scale;
  }
  
  
  int AdvancedSequencer::GetTrackVelocityScale( int trk ) const
  {
    if( !file_loaded )
    {
      return 100;
    }
    
    return seq.GetTrackProcessor(trk)->velocity_scale;
  }
  
  
  
  void AdvancedSequencer::SetTrackRechannelize( int trk, int chan )
  {
    if( !file_loaded )
    {
      return;
    }
    
    seq.GetTrackProcessor(trk)->rechannel = chan;
    driver.AllNotesOff();
    seq.GetTrackState(trk)->note_matrix.Clear();
    
  }
  
  
  int AdvancedSequencer::GetTrackRechannelize( int trk ) const
  {
    if( !file_loaded )
    {
      return -1;
    }
    
    
    return seq.GetTrackProcessor(trk)->rechannel;
  }
  
  
  
  void AdvancedSequencer::SetTrackTranspose( int trk, int trans )
  {
    if( !file_loaded )
    {
      return;
    }
    
    bool was_playing=mgr.IsSeqPlay();
    
    if( mgr.IsSeqPlay() )
    {
      was_playing=true;
      mgr.SeqStop();
    }
    
    if( trk==-1 )
    {
      for( trk=0; trk<tracks.GetNumTracks(); ++trk )
      {
        seq.GetTrackProcessor(trk)->transpose = trans;
      }
    }
    else
    {
      seq.GetTrackProcessor(trk)->transpose = trans;
    }
    
    if( was_playing )
    {
#if 0
      driver.ResetMIDIOut();
      driver.AllNotesOff();
#endif
      seq.GetTrackState(trk)->note_matrix.Clear();
      
      mgr.SeqPlay();
    }
    
  }
  
  
  int AdvancedSequencer::GetTrackTranspose( int trk ) const
  {
    if( !file_loaded )
    {
      return 0;
    }
    
    return seq.GetTrackProcessor(trk)->transpose;
  }
  
  
  
  
  void AdvancedSequencer::ExtractMarkers( std::vector< std::string > *list )
  {
    if( !file_loaded )
    {
      list->clear();
      num_markers=0;
      return;
    }
    
    MIDITrack *t = tracks.GetTrack(0);
    
    list->clear();
    int cnt=0;
    
    int measure=0;
    int beat=0;
    
    int timesig_numerator=4;
    int timesig_denominator=4;
    
    MIDIClockTime last_beat_time=0;
    MIDIClockTime last_event_time=0;
    int clks_per_beat=tracks.GetClksPerBeat();
    
    for( int i=0; i<t->GetNumEvents(); ++i )
    {
      MIDITimedBigMessage *m = t->GetEventAddress(i);
      
      if( m )
      {
        // how many beats have gone by since the last event?
        
        long beats_gone_by = (m->GetTime()-last_beat_time)/clks_per_beat;
        
        if( beats_gone_by>0 )
        {
          // calculate what our new measure/beat is
          beat += beats_gone_by;
          // carry over beat overflow to measure
          measure += beat/timesig_numerator;
          beat = beat%timesig_numerator;
          last_beat_time += (clks_per_beat * beats_gone_by);
        }
        
        
        if( m->IsMetaEvent() && m->IsTimeSig() )
        {
          timesig_numerator = m->GetTimeSigNumerator();
          timesig_denominator = m->GetTimeSigDenominator();
          clks_per_beat=tracks.GetClksPerBeat() * 4 / timesig_denominator;
        }
        
        if( m->IsTextEvent() && m->GetSysEx() )
        {
          if( (m->GetMetaType() == META_GENERIC_TEXT )
              || m->GetMetaType() == META_MARKER_TEXT
              || m->GetMetaType() == META_CUE_TEXT )
          {
            char buf[256];
            char line[256];
            
            memcpy( buf, m->GetSysEx()->GetBuf(), m->GetSysEx()->GetLength() );
            buf[ m->GetSysEx()->GetLength() ] = '\0';
            FixQuotes( buf );
            
            sprintf( line, "%03d:%d        %s", measure+1, beat+1, buf );
            list->push_back( std::string(line) );
            marker_times[ cnt++ ] = m->GetTime();
          }
        }
        
        last_event_time = m->GetTime();
      }
    }
    num_markers = cnt;
  }
  
  
  int AdvancedSequencer::GetCurrentMarker() const
  {
    if( !file_loaded )
    {
      return -1;
    }
    
    // find marker with largest time that
    // is before cur_time
    
    MIDIClockTime cur_time = seq.GetCurrentMIDIClockTime();
    
    cur_time+=20;
    
    int last=-1;
    
    for( int i=0; i<num_markers; ++i )
    {
      if( marker_times[i] > cur_time )
      {
        break;
      }
      else
      {
        last=i;
      }
    }
    return last;
  }
  
  
  
  int AdvancedSequencer::FindFirstChannelOnTrack(int trk)
  {
    if( !file_loaded )
    {
      return -1;
    }
    
    int first_channel = -1;
    
    MIDITrack *t = tracks.GetTrack(trk);
    
    if( t )
    {
      // go through all events
      // until we find a channel message
      // and then return the channel number plus 1
      
      for( int i=0; i<t->GetNumEvents(); ++i )
      {
        MIDITimedBigMessage *m = t->GetEventAddress(i);
        
        if( m )
        {
          if( m->IsChannelMsg() )
          {
            first_channel = m->GetChannel() + 1;
            break;
          }
        }
      }
      
    }
    
    return first_channel;
  }
  
  
  void AdvancedSequencer::ExtractWarpPositions()
  {
    if( !file_loaded )
    {
      for( int i=0; i<num_warp_positions; ++i )
      {
        delete warp_positions[i];
      }
      num_warp_positions=0;
      return;
    }
    
    Stop();
    
    // delete all our current warp positions
    
    for( int i=0; i<num_warp_positions; ++i )
    {
      delete warp_positions[i];
    }
    num_warp_positions=0;
    
    
    
    while(num_warp_positions<MAX_WARP_POSITIONS)
    {
      if( !seq.GoToMeasure( num_warp_positions*MEASURES_PER_WARP, 0 ) )
      {
        break;
      }
      
      // save current sequencer state at this position
      warp_positions[num_warp_positions++] =
        new MIDISequencerState(
          *seq.GetState()
          );
    }
    
    seq.GoToMeasure( 0,0 );
  }
  
}
