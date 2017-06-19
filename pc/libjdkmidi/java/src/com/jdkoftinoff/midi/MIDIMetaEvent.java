/*
 * https://clicker.jdkoftinoff.com/projects/trac/jdks/wiki/libjdkmidijava
 *
 *  libjdkmidijava Java Class Library for MIDI
 *
 *  Copyright (C) 2006  J.D. Koftinoff Software, Ltd.
 *  www.jdkoftinoff.com
 *  jeffk@jdkoftinoff.com
 *
 *  *** RELEASED UNDER THE GNU GENERAL PUBLIC LICENSE (GPL) September 11, 2006 ***
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

package com.jdkoftinoff.midi;

public abstract class MIDIMetaEvent
{
	private int meta_type;

	public MIDIMetaEvent()
	{
		super();
	}

	public MIDIMetaEvent(MIDIMetaEvent other)
	{
		meta_type = other.meta_type;
	}

	public MIDIMetaEvent(int meta_type_)
	{
		meta_type = meta_type_;
	}

	/**
	 * Clone the event
	 */
	public abstract MIDIMetaEvent clone();

	/**
	 * Create a meta event object based on the MF_* integer constants
	 */

	public static MIDIMetaEvent createFromMfType(int type) throws MIDIError
	{
		switch (type)
		{
		case MIDI.Meta.SEQUENCE_NUMBER:
		case MIDI.Meta.TEXT_EVENT:
		case MIDI.Meta.COPYRIGHT:
		case MIDI.Meta.TRACK_NAME:
		case MIDI.Meta.INSTRUMENT_NAME:
		case MIDI.Meta.LYRIC:
		case MIDI.Meta.MARKER:
		case MIDI.Meta.CUE_POINT:
		case MIDI.Meta.GENERIC_TEXT_8:
		case MIDI.Meta.GENERIC_TEXT_9:
		case MIDI.Meta.GENERIC_TEXT_A:
		case MIDI.Meta.GENERIC_TEXT_B:
		case MIDI.Meta.GENERIC_TEXT_C:
		case MIDI.Meta.GENERIC_TEXT_D:
		case MIDI.Meta.GENERIC_TEXT_E:
		case MIDI.Meta.GENERIC_TEXT_F:
			return new MIDIEventText(type);
		case MIDI.Meta.OUTPUT_CABLE:
			return new MIDIEventCable();
		case MIDI.Meta.TRACK_LOOP:
			return new MIDIEventTrackLoop();
		case MIDI.Meta.END_OF_TRACK:
			return new MIDIEventEndOfTrack();
		case MIDI.Meta.TEMPO:
			return new MIDIEventTempo();
		case MIDI.Meta.SMPTE:
			return new MIDIEventSMPTE();
		case MIDI.Meta.TIMESIG:
			return new MIDIEventTimeSignature();
		case MIDI.Meta.KEYSIG:
			return new MIDIEventKeySignature();
		case MIDI.Meta.SEQUENCER_SPECIFIC:
			return new MIDIEventSequencerSpecific();
		case MIDI.Meta.SYSEX:
			return new MIDISystemExclusive();
		}
		throw new MIDIError("Unknown Meta Event ID");
	}

}
