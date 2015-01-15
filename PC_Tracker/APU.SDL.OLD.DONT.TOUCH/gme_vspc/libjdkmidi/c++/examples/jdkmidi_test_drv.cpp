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
#include "jdkmidi/track.h"
#include "jdkmidi/multitrack.h"
#include "jdkmidi/filereadmultitrack.h"
#include "jdkmidi/fileread.h"
#include "jdkmidi/fileshow.h"
#include "jdkmidi/sequencer.h"
#include "jdkmidi/manager.h"
#include "jdkmidi/driverdump.h"

using namespace jdkmidi;

void DumpTrackNames( MIDISequencer *seq ) 
{
  fprintf( stdout, "TEMPO = %f\n",
           seq->GetTrackState(0)->tempobpm
    );
  
  for( int i=0; i<seq->GetNumTracks(); ++i )
  {
    fprintf( stdout, "TRK #%2d : NAME = '%s'\n", 
             i,
             seq->GetTrackState(i)->track_name
      );
  }	
} 


void PlayDumpManager( MIDIManager *mgr ) 
{	
  MIDISequencer *seq = mgr->GetSeq();
  double pretend_clock_time = 0.0;
  
  seq->GoToTime( (unsigned long)pretend_clock_time );
  
  mgr->SeqPlay();
  
  // simulate a clock going forward with 10ms resolution for 1 minute
  
  for( pretend_clock_time=0.0; pretend_clock_time<60.0*1000.0; pretend_clock_time+=100 )
  {
    mgr->GetDriver()->TimeTick( (unsigned long)pretend_clock_time );	
  }
  
  mgr->SeqStop();
  mgr->GetDriver()->AllNotesOff();
  
} 


int main( int argc, char **argv )
{
  if( argc>1 )
  {
    MIDIFileReadStreamFile rs( argv[1] );
    MIDIMultiTrack tracks(64);
    MIDIFileReadMultiTrack track_loader( &tracks );
    MIDIFileRead reader( &rs, &track_loader );
    MIDISequencerGUIEventNotifierText gui( stdout );
    MIDISequencer seq( &tracks, &gui );
    MIDIDriverDump driver(128,stdout);
    MIDIManager mgr( &driver, &gui );
    
    reader.Parse();
    
    seq.GoToZero();
    mgr.SetSeq( &seq );
    
    DumpTrackNames( &seq );
    
    PlayDumpManager( &mgr );
  }
  
  return 0;
}
