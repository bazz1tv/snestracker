/*
 * MIDISystemExclusive.java
 *
 * Created on 11 September 2006, 13:58 By Jeff Koftinoff <jeffk@jdkoftinoff.com>
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
 */

package com.jdkoftinoff.midi;

import java.io.Serializable;

/**
 * 
 */
public class MIDISystemExclusive extends MIDIMetaEvent implements Serializable
{

	private static final long serialVersionUID = 7907376115507540412L;

	private short sysex_bytes[];

	private int cur_len;

	private int max_len;

	protected short chk_sum;

	/** Creates a new instance of MIDISystemExclusive */
	public MIDISystemExclusive()
	{
		sysex_bytes = null;
		chk_sum = 0;
		cur_len = 0;
		max_len = 0;
	}

	/**
	 * Creates a new instance of MIDISystemExclusive with the specified maximum
	 * size
	 * 
	 * @param initial_size
	 */
	public MIDISystemExclusive(int initial_size)
	{
		chk_sum = 0;
		cur_len = 0;
		max_len = initial_size;
		sysex_bytes = new short[max_len];
	}

	/**
	 * Creates a copy of the specified other MIDISystemExclusive object
	 * 
	 * @param other
	 */
	public MIDISystemExclusive(MIDISystemExclusive other)
	{
		super(other);
		chk_sum = 0;
		cur_len = 0;
		max_len = other.max_len;
		if (max_len > 0 && other.sysex_bytes != null)
		{
			sysex_bytes = new short[max_len];

			for (int i = 0; i < cur_len; ++i)
			{
				sysex_bytes[i] = other.sysex_bytes[i];
			}
		} else
		{
			sysex_bytes = null;
		}
	}

	public MIDISystemExclusive clone()
	{
		return new MIDISystemExclusive(this);
	}

	/**
	 * clear the contents, re-allocate to default size of 256 bytes.
	 * 
	 */
	public void clear()
	{
		chk_sum = 0;
		cur_len = 0;
		max_len = 256;
		sysex_bytes = new short[max_len];
	}

	/**
	 * Get the current length
	 * 
	 * @return the current length of the contents
	 */
	public int getLength()
	{
		return cur_len;
	}

	/**
	 * Get the data byte at the specific index
	 * 
	 * @param index
	 * @return value at byte
	 * @throws MIDIErrorRange
	 */
	public short getByte(int index) throws MIDIErrorRange
	{
		if (index > cur_len || index < 0)
		{
			throw new MIDIErrorRange("System Exclusive offset out of range");
		}
		return sysex_bytes[index];
	}

	/**
	 * Append the specified value to the buffer, automatically extending the
	 * buffer size if needed, with no validation of the contents.
	 * 
	 * @param val
	 */

	private void appendRawByte(short val)
	{
		if (cur_len >= max_len || sysex_bytes == null)
		{
			int new_max_len = cur_len + 256;
			short new_buffer[] = new short[new_max_len];

			if (sysex_bytes != null && cur_len > 0)
			{
				for (int i = 0; i < cur_len; ++i)
				{
					new_buffer[i] = sysex_bytes[i];
				}
			}
		}

		sysex_bytes[cur_len++] = val;
	}

	/**
	 * Append the specified data byte to the sysex, and update the checksum
	 * 
	 * @param val
	 * @throws MIDIError
	 */
	public void putByte(short val) throws MIDIError
	{
		if (val < 0 || val > 0x7f)
			throw new MIDIErrorRange("System Exclusive data byte out of range");

		appendRawByte(val);

		chk_sum = (short) ((chk_sum + val) & (short) 0x7f);
	}

	/**
	 * Append the specified system byte (high bit set) to the sysex
	 * 
	 * @param val
	 * @throws MIDIError
	 */
	public void putSysByte(short val) throws MIDIError
	{
		if (val < 0x80 || val > 0xff)
			throw new MIDIErrorRange(
					"System Exclusive system byte out of range");
		appendRawByte(val);
	}

	/**
	 * Append a sysex start byte
	 * 
	 */
	public void putEXC()
	{
		appendRawByte(MIDI.Status.SYSEX_START);
	}

	/**
	 * Append a sysex end byte
	 * 
	 */
	public void putEOX()
	{
		appendRawByte(MIDI.Status.SYSEX_END);
	}

	/**
	 * Append the calculated checksum value
	 * 
	 */
	public void putChecksum()
	{
		appendRawByte((short) (chk_sum & 0x7f));
	}

	/**
	 * get the current checksum
	 * 
	 * @return the checksum
	 */
	public int getChecksum()
	{
		return chk_sum & 0x7f;
	}

	/**
	 * Artificially clear the current checksum
	 */

	public void clearChecksum()
	{
		chk_sum = 0;
	}

	/**
	 * Append the 8 bit value in little-endian form (low nibble first)
	 * 
	 * @param b
	 * @throws MIDIErrorRange
	 */
	// low nibble first
	public void putNibblizedByteLE(short b) throws MIDIErrorRange
	{
		if (b < 0 || b >= 256)
			throw new MIDIErrorRange("Nibblized 8 bit value out of range.");

		appendRawByte((short) (b & 0xf));
		appendRawByte((short) (b >> 4));
	}

	/**
	 * Read the 8 bit value at pos in little-endian form (low nibble first)
	 * 
	 * @param pos
	 * @return 8 bit value
	 */
	public int getNibbilizedByteLE(short pos)
	{
		return (sysex_bytes[pos] & 0xf) + ((sysex_bytes[pos + 1] & 0x0f) << 4);
	}

	/**
	 * Append the 8 bit value in big-endian form (high nibble first)
	 * 
	 * @param b
	 * @throws MIDIErrorRange
	 */
	// low nibble first
	public void putNibblizedByteBE(short b) throws MIDIErrorRange
	{
		if (b < 0 || b >= 256)
			throw new MIDIErrorRange("Nibblized 8 bit value out of range.");

		appendRawByte((short) (b >> 4));
		appendRawByte((short) (b & 0xf));
	}

	/**
	 * Read the 8 bit value at pos in big-endian form (high nibble first)
	 * 
	 * @param pos
	 * @return 8 bit value
	 */
	public int getNibbilizedByteBE(short pos)
	{
		return ((sysex_bytes[pos] & 0xf) << 4)
				+ ((sysex_bytes[pos + 1] & 0x0f));
	}

}
