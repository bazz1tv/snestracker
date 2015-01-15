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

#include "jdkmidi/edittrack.h"

#ifndef DEBUG_MDETRACK
# define DEBUG_MDETRACK	0
#endif

#if DEBUG_MDETRACK
# undef DBG
# define DBG(a)	a
#endif

namespace jdkmidi
{
  
  
  MIDIEditTrackEventMatcher::MIDIEditTrackEventMatcher()
  {
  }
  
  MIDIEditTrackEventMatcher::~MIDIEditTrackEventMatcher()
  {
  }
  
  
  MIDIEditTrack::MIDIEditTrack( MIDITrack *track_ )
    :
    track( track_ )
  {
    ENTER( "MIDIEditTrack::MIDIEditTrack()" );
    
  }
  
  MIDIEditTrack::~MIDIEditTrack()
  {
    ENTER( "MIDIEditTrack::~MIDIEditTrack()" );
  }
  
  
  void 	MIDIEditTrack::Process(
    MIDIClockTime start_time,
    MIDIClockTime end_time,
    MIDIProcessor *process,
    MIDIEditTrackEventMatcher *match
    )
  {
    // TODO: Process
  }
  
  
  
//
// Truncate erases all events after a certain time. then
// adds appropriate note off's
//
  void	MIDIEditTrack::Truncate( MIDIClockTime start_time )
  {
    ENTER( "TO DO: MIDIEditTrack::Truncate()" );
    
    // TO DO:
  }
  
  
  
  
  
//
// this merge function merges two other tracks into this track.
// this is the faster form of merge
//
  void	MIDIEditTrack::Merge(
    MIDITrack *trk1,
    MIDITrack *trk2,
    MIDIEditTrackEventMatcher *match1,
    MIDIEditTrackEventMatcher *match2
    )
  {
    ENTER( "TO DO: MIDIEditTrack::Merge()" );
    
    // TO DO:
  }
  
  
  
  
  
  
//
// this erase function will erase all events from start to end time
// and can be jagged or not.
//
  void	MIDIEditTrack::Erase(
    MIDIClockTime start,
    MIDIClockTime end,
    bool jagged,
    MIDIEditTrackEventMatcher *match
    )
  {
    ENTER( "TO DO: MIDIEditTrack::Erase()" );
    
    // TO DO:
  }
  
  
  
  
  
//
// this delete function will delete all events like erase and then
// shift the events over
//
  void	MIDIEditTrack::Delete(
    MIDIClockTime start,
    MIDIClockTime end,
    bool jagged,
    MIDIEditTrackEventMatcher *match
    )
  {
    ENTER( "TO DO: MIDIEditTrack::Delete()" );
    
    // TO DO:
  }
  
  
  
  
//
// this insert function will insert 'length' clicks starting at
// the events at start time.
//
  void	MIDIEditTrack::Insert(
    MIDIClockTime start,
    MIDIClockTime length
    )
  {
    ENTER( "TO DO: MIDIEditTrack::Insert()" );
    
    // TO DO:
  }
  
  
  
  
//
// this shift function will shift all event times by an offset.
//
  void	MIDIEditTrack::Shift(
    signed long offset,
    MIDIEditTrackEventMatcher *match
    )
  {
    ENTER( "TO DO: MIDIEditTrack::Shift()" );
    
    // TO DO:
  }
  
  
  
}

#if 0

/*
 * Note: this code that follows is historic from 1992, before namespaces and bool existed in C++
 * The plan is to move the concepts and algorithms from this old code into the new form.
 * In this old form, the EMIDITrack class inherited from MIDITrack. In the new form it ought not to, 
 * as it breaks the 'is-a' rule.
 */

#include "world.h"
#pragma hdrstopd



#include <ETrack.hh>
#include <MMatrix.hh>

static int cmpmsgtime( const void *a, const void *b )
{
	register const TimedMIDIMessage *m1=(TimedMIDIMessage * )a;
	register const TimedMIDIMessage *m2=(TimedMIDIMessage * )b;


	//
	// Check if message is a NOP. if it is, then propegate them to
	// the end of track.
	//


	Boolean n1 = m1->IsNOP();
	Boolean	n2 = m2->IsNOP();

	if( n1 && n2 )
		return 0;
		
	if( n1 )
		return 1;
	if( n2 )
		return -1;


	//
	// Compare the event times
	//

	ulong t1,t2;

	t1=m1->GetTime();
	t2=m2->GetTime();

	if( t1<t2 )
		return -1;
	if( t1>t2 )
		return 1;

	// the times are the same. put note ons first.

	if( m1->GetStatus()==M_NOTE_ON && m1->byte2!=0 )
		return -1;
		
	if( m2->GetStatus()==M_NOTE_ON && m2->byte2!=0 )
		return 1;
			
	return 0;
}

void EMIDITrack::Sort()
{
	//
	// sort all the midi messages in the buffer by time.
	// do not sort the last event.
	//

	qsort( buffer, last_event, sizeof( *buffer ), cmpmsgtime  );

	//
	// TO DO: Clear away all NOP's at the end of the track.
	//

	//
	// ok, now find the maximum time value used in the track.
	// this would be the maximum of buffer[last_event-1] and
	// buffer[last_event].  Set the last event ( the data end event)
	// to the maximum time.
	//

	ulong t1=buffer[last_event-1].GetTime();
	ulong t2=buffer[last_event].GetTime();

	if( t1>t2 )
		buffer[last_event].SetTime( t1 );
}


void EMIDITrack::CopyEvent( MIDITrack *t, unsigned int ev )
{

	//
	// Get the event from the other track.
	//

	TimedMIDIMessage m;

	t->GetEvent( ev, m );

	//
	// is it a mark?
	//

	if( m.IsMark() )
	{
		//
		// Is it Sys-Ex?
		//
		//if( m.IsSysEx() )
		//{
			//
			// yes, make a new sys-ex buffer and copy the
			// sysex into it.
			//

			//SysEx *e = new SysEx;

			//
			// TO DO: Copy the SysEx
			//

		//}

		//
		// Is it a Text event?
		//

		//if( m.IsText() )
		//{
			//
			// yes,make a new MIDIText event and copy the
			// original into it.
			//

			//MIDIText *t = new MIDIText;

			//
			// TO DO: Copy the MIDIText
			//
		//}

		//
		// Is it a DataEnd event?
		//

		if( m.IsDataEnd() )
		{
			//
			// Yes, ignore it.
			//

			return;
		}

		//
		// All other marks are just copied over
		//

	}

	Put( m );
}


void EMIDITrack::Compress()
{
	unsigned int from_ev;
	unsigned int to_ev;
	unsigned int nop_count=0;

	//
	// find the first NOP
	//

	for( to_ev=0; to_ev<last_event; to_ev++ )
	{
		if( buffer[to_ev].IsNOP() )
			break;
	}

	//
	// if there is no NOP, then there is no way to compress
	//

	if( to_ev==last_event )
		return;


	//
	// go through the buffer and compress, including the last DATA END event
	//

	for( from_ev=to_ev+1; from_ev<=last_event; from_ev++ )
	{
		//
		// find the next non NOP event.
		//

		if( !buffer[from_ev].IsNOP() )
		{

			//
			// keep track of how many NOP's have been replaced
			//

			nop_count++;

			//
			// copy the found event to the NOP event
			//

			buffer[to_ev]=buffer[from_ev];

			//
			// put a NOP in the original event
			//

			buffer[from_ev].SetNOP();

			//
			// find the next NOP event.
			//

			for( to_ev++; to_ev<last_event; to_ev++ )
			{
				if( buffer[to_ev].IsNOP() )
					break;
			}

			//
			// if there is no more NOP's left,
			// then we are done.
			//

			if( to_ev>=last_event )
				break;

		}
	}

	//
	// Ok, everything is compacted; we gotta now change the
	// last_event to the new value
	//

	last_event-=nop_count;

}


void EMIDITrack::Truncate( ulong start_time )
{
	// find the first event at start_time or greater
	// or the data end event

	unsigned int ev;
	for( ev=0; ev<last_event; ev++ )
	{
		if( buffer[ev].IsDataEnd() )
			break;

		if( buffer[ev].GetTime() >= start_time )
			break;
	}

	//
	// now ev contains the event number that we truncate at
	//


	//
	// Search through the deleted events for Sys-Ex or MIDIText events
	// and free the memory for those as well.
	//

	last_event=ev;
	buffer[ev].SetDataEnd();
	buffer[ev].SetTime(start_time);

	FixNotes();
}


void EMIDITrack::FixNotes()
{
	MIDIMatrix *matrix=new MIDIMatrix;
	TimedMIDIMessage m;

	//
	// Go through the MIDIEvents, keeping track of all note on events.
	//


	for( unsigned int ev=0; ev<=last_event; ev++ )
	{
		if( buffer[ev].IsDataEnd() )
		{
			m.SetTime( buffer[ev].GetTime() );
			break;
		}
		matrix->Process( buffer[ev] );
	}


	//
	// now put a note off message for each note that was left on
	// at the end of the track. Do this for each channel.
	//

	for( uchar channel=0; channel<16; channel++ )
	{
		if( matrix->GetChannelCount(channel)>0 )
		{
			for( uchar note=0; note<128; note++ )
			{
				uchar num=matrix->GetNoteCount( channel,note);
				for( uchar c=0; c<num; c++ )
				{
					m.NoteOff( channel, note, 64 );
					Put( m );
				}
			}
		}
	}

	delete matrix;
}





void	EMIDITrack::Merge( MIDITrack *trk1, MIDITrack *trk2 )
{
	Clear();

	unsigned int trk1_ev=0;
	unsigned int trk2_ev=0;
	unsigned int trk1_max=trk1->GetNumEvents();
	unsigned int trk2_max=trk2->GetNumEvents();

	//
	// loop until all events of both tracks are transferred
	//

	while( (trk1_ev < trk1_max) || (trk2_ev < trk2_max ) )
	{

		//
		// is track 1 completed and track 2 not complete?
		//

		if( (trk1_ev >= trk1_max) && (trk2_ev < trk2_max) )
		{
			//
			// yes, just copy over the events from track 2
			//

			CopyEvent( trk2, trk2_ev++ );

						//
			// continue the loop. no reason to
			// check the other if's.
			//
			continue;
		}

		//
		// is track 2 completed and track 1 not complete?
		//

		if( (trk2_ev >= trk2_max) && (trk1_ev < trk1_max ) )
		{
			//
			// yes, just copy over the events from track 1
			//

			CopyEvent( trk1, trk1_ev++ );


			//
			// continue the loop.
			//
			continue;

		}

		//
		// ok since we got here, both tracks are not completed yet.
		// we gotta compare times to see which event we copy over
		// first.
		//
		TimedMIDIMessage m1,m2;

		trk1->GetEvent( trk1_ev, m1 );
		trk2->GetEvent( trk2_ev, m2 );

		if( m1.GetTime() < m2.GetTime() )
		{
			//
			// track 1 event came first. copy it over.
			//
			CopyEvent( trk1, trk1_ev++ );
		}
		else
		{
			//
			// track 2 event came first. copy it over.
			//

			CopyEvent( trk2, trk2_ev++ );
		}
	}

}


void	EMIDITrack::Merge( MIDITrack *trk )
{
	for( unsigned int ev=0; ev<trk->GetNumEvents(); ev++ )
	{
		CopyEvent( trk, ev );
	}
	Sort();
	FixNotes();
}


void	EMIDITrack::Erase( ulong start, ulong end, Boolean jagged )
{

	//
	// This Erase method should work fine, erasing all events
	// between two times, and will keep track of all note on/off
	// events so that there will be no hung notes.
	//
	// the edit may be jagged or sharp.  if it is jagged,
	// any notes left on before start time and shut off during the
	// erase time will not have their gate times changed.
	// if jagged==FALSE, then the note off's will be moved to
	// 'start' time.
	//
	// also, any note off messages that are AFTER 'end' time
	// that were turned on during the erase time will be erased
	// as well.
	//
	// One thing, though; if a note was turned on before 'start'
	// time and shut off after 'end' time, it will not be affected
	// by this erase method no matter what jagged is set to.
	//
	// Dealing with this situation would require that all
	// events be sorted afterwards.
	//



	//
	// Make sure we got valid values for end.
	//

	if( end<=start)
		return;


	Boolean changed=FALSE;  	// set to TRUE if anything is erased.


	//
	// start up the MIDIMatrix objects.
	// we need a MIDIMatrix to keep track of the
	// note ON's going into the area we are deleting
	// as well as a MIDIMatrix to keep track of the
	// note ON's that we have deleted that have
	// note OFF's after 'end' time.
	//

	MIDIMatrix *before_matrix = new MIDIMatrix;
	MIDIMatrix *during_matrix = new MIDIMatrix;


	//
	// go through all the events and 'NOP' just the ones
	// in that time period.
	//

	for( unsigned int ev=0; ev<last_event; ev++ )
	{
		ulong time = buffer[ev].GetTime();

		//
		// if the event is before our erase region,
		// then give it to the before_matrix object
		//

		if( time<start )
		{
			before_matrix->Process( buffer[ev] );
			continue;
		}


		if( time>=start && time<end )
		{
			//
			// see if this event should be ignored or
			// erased.
			//
			TimedMIDIMessage m=buffer[ev];

			if( ! FilterEvent( m ) )
				continue;



			//
			// give this event to our during_matrix object
			// to keep track of note on's
			//

			during_matrix->Process( buffer[ev] );


			if( buffer[ev].IsMark() )
			{
				//
				// Is this event a Sys-Ex event?
				//

				if( m.IsSysEx() )
				{
					//
					// yep. we delete it as well as the
					// sys-ex message itself.
					//
					unsigned int s=m.GetSysEx();

					exclusives.Delete(s);
				}

				//
				// is this event a MIDIText event?
				//

				if( m.IsText() )
				{
					//
					// yep. delete it and the MIDIText
					// event itself.
					//
					unsigned int t=m.GetText();
					text.Delete(t);
				}

				//
				// all other sequencer marks just get deleted.
				//

				changed=TRUE;
				buffer[ev].SetNOP();

			}
			else
			{
				//
				// ok, we gotta make sure that if this is a
				// note off event, we do not 'NOP' it if the
				// corresponding note on event was before
				// the start time.
				//

				if( (buffer[ev].GetStatus()==M_NOTE_ON &&
					buffer[ev].GetVelocity()==0 )
					|| buffer[ev].GetStatus()==M_NOTE_OFF )
				{

					//
					// ok, it was a note off event.
					// see if it needs to be deleted.
					//

					uchar channel = buffer[ev].GetChannel();
					uchar note = buffer[ev].GetNote();

					if( before_matrix->GetNoteCount(channel,note)!=0 )
					{
						//
						// do not delete the note off.
						// we need it. first process it
						// so we know that note is now off.
						//

						before_matrix->Process( buffer[ev] );

						//
						// if a non-jagged edit was requested,
						// then we shall move this note off to
						// the 'start' time.
						//

						if( !jagged )
						{
							buffer[ev].SetTime( start );
						}

					}
					else
					{
						changed=TRUE;
						buffer[ev].SetNOP();
					}
				}
				else
				{
					//
					// ok, it wasn't a note off message.
					// give it to the during_matrix object.
					//

					during_matrix->Process( buffer[ev] );

					//
					// and now delete it.
					//

					buffer[ev].SetNOP();
					changed=TRUE;
				}
			}
		}

		//
		// if the time of the event>end, and the event is
		// a note off event which was turned on in the
		// during_matrix object, then 'NOP' it. The note on
		// was deleted.
		//

		if( time>=end )
		{
			//
			// make sure the message is not a mark.
			//

			if( !buffer[ev].IsMark() )
			{
				//
				// is it a note off message?
				//

				if( buffer[ev].GetStatus()==M_NOTE_OFF
				   || (buffer[ev].GetStatus()==M_NOTE_ON &&
				    buffer[ev].GetVelocity()==0 ) )
				{
					//
					// ok, it is a note off message,
					// was the corresponding note on
					// deleted?
					//

					uchar channel=buffer[ev].GetChannel();
					uchar note=buffer[ev].GetNote();

					if( during_matrix->GetNoteCount( channel, note ) )
					{
						//
						// yes, it was, so we gotta NOP this event.
						// give it to during_matrix so it knows
						// that the note is off now.

						during_matrix->Process( buffer[ev] );
						buffer[ev].SetNOP();
						changed=TRUE;
					}
				}
			}
		}

	}

	//
	// If there were any events NOP'd
	// then we gotta compress them out of the way.
	//

	if( changed )
		Compress();


	//
	// kill our helpful MIDIMatrix's
	//

	delete before_matrix;
	delete during_matrix;

}

void    EMIDITrack::Delete( ulong start, ulong end, Boolean jagged )
{
	//
	// Delete is just like Erase, except afterwards
	// we move all the event times after 'end' from end to start.
	//

	//
	// Make sure we got valid values for end.
	//

	if( end<=start)
		return;

	//
	// Erase the events
	//

	Erase( start, end, jagged );


	//
	// calculate the amount that we have to subtract from
	// each event time after 'end' time.
	//

	ulong diff= end-start;


	//
	// go through all events (including the last event, data end)
	//

	for( unsigned int ev=0; ev<=last_event; ev++ )
	{
		//
		// get the event's time.
		//

		ulong time=buffer[ev].GetTime();

		//
		// is it after the end time?
		//

		if( time>=end )
		{
			//
			// yes, change the event time to what it is
			// supposed to be.
			//

			buffer[ev].SetTime( time-diff );
		}
	}
}

void	EMIDITrack::Insert( ulong start, ulong end )
{
	//
	// make sure end is after start.
	//

	if( end<start )
		return;

	//
	// calculate the amount of time we gotta add to the event times
	//

	ulong diff=end-start;

	//
	// go through all the events, including the last DATA END event.
	//

	for( unsigned int ev=0; ev<=last_event; ev++ )
	{

		//
		// Get the event's time.
		//

		ulong time=buffer[ev].GetTime();

		//
		// is it after 'start' time?
		//

		if( time>=start )
		{
			//
			// move the event forward in time by 'diff' clicks.
			//

			buffer[ev].SetTime( time+diff );
		}
	}
}

#endif
