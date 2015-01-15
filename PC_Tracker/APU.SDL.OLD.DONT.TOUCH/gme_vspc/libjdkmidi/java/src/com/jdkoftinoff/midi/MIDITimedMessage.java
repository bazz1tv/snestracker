/*
 * MIDITimedMessage.java
 *
 * Created on 11 September 2006, 14:59 By Jeff Koftinoff <jeffk@jdkoftinoff.com>
 *
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
 *
 */

package com.jdkoftinoff.midi;

import java.util.Comparator;

/**
 * 
 */
public class MIDITimedMessage extends MIDIMessage
{

	/**
	 * 
	 */
	private static final long serialVersionUID = 4847287700730274873L;

	protected long time;

	public static class TimedMessageComparator implements
			Comparator<MIDITimedMessage>
	{
		public int compare(MIDITimedMessage m1, MIDITimedMessage m2)
		{

			boolean n1 = false;// m1.IsNoOp();
			boolean n2 = false;// m2.IsNoOp();

			// NOP's always are larger.

			if (n1 && n2)
				return 0; // same, do not care.

			if (n2)
				return 1; // m2 is larger

			if (n1)
				return -1; // m1 is larger

			if (m1.getTime() > m2.getTime())
				return -1; // m1 is larger

			if (m2.getTime() > m1.getTime())
				return 1;

			try
			{
				// if times are the same, a note off is always larger than a
				// note on,
				// for the same note number.

				if (m1.isNoteOn() && m2.isNoteOff()
						&& m1.getNote() == m2.getNote())
				{
					return 1; // m2 is bigger, put it later.
				}
				if (m1.isNoteOff() && m2.isNoteOn()
						&& m1.getNote() == m2.getNote())
				{
					return -1; // m1 is bigger, put it later.
				}
			} catch (MIDIError ex)
			{
				ex.printStackTrace();
			}

			return 0; // both are equal.
		}
	}

	public static TimedMessageComparator comparator = new TimedMessageComparator();

	/** Creates a new instance of MIDITimedMessage */
	public MIDITimedMessage()
	{
		super();
		time = 0;
	}

	public MIDITimedMessage(short time_, short status_, short byte1_,
			short byte2_)
	{
		super(status_, byte1_, byte2_);
		time = time_;
	}

	public MIDITimedMessage(int time_, MIDISystemExclusive sysex_)
	{
		super(sysex_);
		time = time_;
	}

	public MIDITimedMessage(int time_, MIDIMessage msg_)
	{

	}

	public void setTime(long t)
	{
		time = t;
	}

	public long getTime()
	{
		return time;
	}
}
