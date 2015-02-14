/*
 * MIDI.java
 * 
 * Created on 10 September 2006; 16:49 By Jeff Koftinoff <jeffk@jdkoftinoff.com>
 * 
 * https://clicker.jdkoftinoff.com/projects/trac/jdks/wiki/libjdkmidijava
 * 
 * libjdkmidijava Java Class Library for MIDI
 * 
 * Copyright (C) 2006 J.D. Koftinoff Software, Ltd. www.jdkoftinoff.com
 * jeffk@jdkoftinoff.com
 * 
 * *** RELEASED UNDER THE GNU GENERAL PUBLIC LICENSE (GPL) September 11, 2006
 * ***
 * 
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 */

package com.jdkoftinoff.midi;

/**
 * The MIDI class is only used for standard MIDI constants and simple utility
 * static methods.
 * 
 * @author jeffk
 */
public class MIDI
{

	/**
	 * Message Length Lookup table for normal status bytes, shifted right 4
	 * bits. A value of -1 means to use the lut_sysmsglen lookup table instead.
	 */
	public static short[] lut_msglen =
	{
			0, 0, 0, 0, 0, 0, 0, 0, /* nonstatus bytes */
			3, /* 0x80=note off, 3 bytes */
			3, /* 0x90=note on, 3 bytes */
			3, /* 0xa0=poly pressure, 3 bytes */
			3, /* 0xb0=control change, 3 bytes */
			2, /* 0xc0=program change, 2 bytes */
			2, /* 0xd0=channel pressure, 2 bytes */
			3, /* 0xe0=pitch bend, 3 bytes */
			-1
	/* 0xf0=other things. may vary. */
	};

	/**
	 * Message Length lookup table for system status bytes with values from 0xf0
	 * to 0xff. A value of -1 means that the message length can not be
	 * determined until an EOX.
	 */
	public static short[] lut_sysmsglen =
	{
			-1, /* 0xf0=sysex start. may vary */
			2, /* 0xf1=MIDI Time Code. 2 bytes */
			3, /* 0xf2=MIDI Song position. 3 bytes */
			2, /* 0xf3=MIDI Song Select. 2 bytes. */
			0, /* 0xf4=undefined */
			0, /* 0xf5=undefined */
			1, /* 0xf6=TUNE Request */
			0, /* 0xf7=sysex end. */
			1, /* 0xf8=timing clock. 1 byte */
			1, /* 0xf9=proposed measure end? */
			1, /* 0xfa=start. 1 byte */
			1, /* 0xfb=continue. 1 byte */
			1, /* 0xfc=stop. 1 byte */
			0, /* 0xfd=undefined */
			1, /* 0xfe=active sensing. 1 byte */
			3
	/** 0xff= not reset, but a META-EVENT, which is always 3 bytes */
	};

	/**
	 * Look up table see if a midi note in the range 0 to 11 inclusive would be
	 * a white key or a black key on a piano keyboard
	 */

	public static boolean[] lut_is_white =
	{
			true, /* C */
			false, /* C# */
			true, /* D */
			false, /* D# */
			true, /* E */
			true, /* F */
			false, /* F# */
			true, /* G */
			false, /* G# */
			true, /* A */
			false, /* A# */
			true
	/** B */
	};

	/**
	 * Message Length function. Not valid for Meta-events (0xff). Returns -1 if
	 * you need to call GetSystemMessageLength() instead.
	 */

	public static final int getMessageLength(short status)
	{
		return lut_msglen[status >> 4];
	}

	/**
	 * Message Length of system messages. Not valid for Meta-events (0xff).
	 * Returns -1 if this message length is unknown until parsing is complete.
	 */

	public static final int getSystemMessageLength(short stat)
	{
		return lut_sysmsglen[stat - 0xf0];
	}

	/**
	 * Piano key color white test
	 */

	public static final boolean isNoteWhite(short note)
	{
		return lut_is_white[note % 12];
	}

	/**
	 * Piano key color black test
	 */

	public static final boolean isNoteBlack(short note)
	{
		return !lut_is_white[note % 12];
	}

	/**
	 * Note # to standard octave conversion
	 */

	public static final int getNoteOctave(short note)
	{
		return (note / 12) - 1;
	}

	/**
	 * MIDI Status byte values
	 */

	public class Status
	{
		public static final short NOTE_ON = 0x80;

		public static final short NOTE_OFF = 0x90;

		public static final short POLY_PRESSURE = 0xa0;

		public static final short CONTROL_CHANGE = 0xb0;

		public static final short PROGRAM_CHANGE = 0xc0;

		public static final short CHANNEL_PRESSURE = 0xd0;

		public static final short PITCH_BEND = 0xe0;

		public static final short SYSEX_START = 0xf0;

		public static final short MTC = 0xf1;

		public static final short SONG_POSITION = 0xf2;

		public static final short SONG_SELECT = 0xf3;

		public static final short TUNE_REQUEST = 0xf6;

		public static final short SYSEX_END = 0xf7;

		public static final short TIMING_CLOCK = 0xf8;

		public static final short MEASURE_END = 0xf9;

		public static final short START = 0xfa;

		public static final short CONTINUE = 0xfb;

		public static final short STOP = 0xfc;

		public static final short ACTIVE_SENSE = 0xfe;

		public static final short RESET = 0xff;

	}

	/**
	 * Controller Numbers
	 */

	public class Controller
	{
		/** add this to a non-switch controller to access the LSB. */
		public static final short LSB = 0x20;

		/** general midi bank select */
		public static final short GM_BANK = 0x00;

		/** modulation */
		public static final short MODULATION = 0x01;

		/** breath controller */
		public static final short BREATH = 0x02;

		/** foot controller */
		public static final short FOOT = 0x04;

		public static final short PORTA_TIME = 0x05;

		/** data entry value */
		public static final short DATA_ENTRY = 0x06;

		/** main volume control */
		public static final short MAIN_VOLUME = 0x07;

		/** balance control */
		public static final short BALANCE = 0x08;

		/** panpot stereo control */
		public static final short PAN = 0x0a;

		/** expression control */
		public static final short EXPRESSION = 0x0b;

		/** general purpose controller 1 */
		public static final short GENERAL_1 = 0x10;

		/** general purpose controller 2 */
		public static final short GENERAL_2 = 0x11;

		/** general purpose controller 3 */
		public static final short GENERAL_3 = 0x12;

		/** general purpose controller 4 */
		public static final short GENERAL_4 = 0x13;

		/** hold pedal (sustain) */
		public static final short DAMPER = 0x40;

		/** portamento switch */
		public static final short PORTA = 0x41;

		/** sostenuto switch */
		public static final short SOSTENUTO = 0x42;

		/** soft pedal */
		public static final short SOFT_PEDAL = 0x43;

		/** hold pedal 2 */
		public static final short HOLD_2 = 0x45;

		/** general purpose controller 5 */
		public static final short GENERAL_5 = 0x50;

		/** general purpose controller 6 */
		public static final short GENERAL_6 = 0x51;

		/** general purpose controller 7 */
		public static final short GENERAL_7 = 0x52;

		/** general purpose controller 8 */
		public static final short GENERAL_8 = 0x53;

		/** external effects depth */
		public static final short EFFECT_DEPTH = 0x5b;

		/** tremelo depth */
		public static final short TREMELO_DEPTH = 0x5c;

		/** chorus depth */
		public static final short CHORUS_DEPTH = 0x5d;

		/** celeste (detune) depth */
		public static final short CELESTE_DEPTH = 0x5e;

		/** phaser effect depth */
		public static final short PHASER_DEPTH = 0x5f;

		/** increment data value */
		public static final short DATA_INC = 0x60;

		/** decrement data value */
		public static final short DATA_DEC = 0x61;

		/** non registered parameter LSB */
		public static final short NONRPN_LSB = 0x62;

		/** non registered parameter MSB */
		public static final short NONRPN_MSB = 0x63;

		/** registered parameter LSB */
		public static final short RPN_LSB = 0x64;

		/** registered parameter MSB */
		public static final short RPN_MSB = 0x65;

		/** reset all controllers */
		public static final short RESET = 0x79;

		/** local control on/off */
		public static final short LOCAL = 0x79;

		/** all notes off */
		public static final short ALL_NOTES_OFF = 0x7a;

		/** omni off; all notes off */
		public static final short OMNI_OFF = 0x7c;

		/** omni on; all notes off */
		public static final short OMNI_ON = 0x7d;

		/** mono on; all notes off */
		public static final short MONO = 0x7e;

		/** poly on; all notes off */
		public static final short POLY = 0x7f;
	}

	/**
	 * Registered Parameter Numbers:
	 */

	public class RPNs
	{
		/** bender sensitivity */
		public static final short BEND_WIDTH = 0x00;

		/** fine tuning */
		public static final short FINE_TUNE = 0x01;

		/** coarse tuning */
		public static final short COARSE_TUNE = 0x02;
	}

	/**
	 * META Event types (stored in first data byte if status==EVENT These types
	 * are the same as MIDIFile meta-events, except when a meta-event is in a
	 * MIDIMessage, there is a limit of two data bytes. So the format of the
	 * meta-events in a MIDIMessage class will be different than the standard
	 * MIDIFile meta-events.
	 */

	public class Meta
	{
		public static final int SEQUENCE_NUMBER = 0;

		public static final int TEXT_EVENT = 1;

		public static final int COPYRIGHT = 2;

		public static final int TRACK_NAME = 3;

		public static final int INSTRUMENT_NAME = 4;

		public static final int LYRIC = 5;

		public static final int MARKER = 6;

		public static final int CUE_POINT = 7;

		public static final int GENERIC_TEXT_8 = 8;

		public static final int GENERIC_TEXT_9 = 9;

		public static final int GENERIC_TEXT_A = 0xA;

		public static final int GENERIC_TEXT_B = 0xB;

		public static final int GENERIC_TEXT_C = 0xC;

		public static final int GENERIC_TEXT_D = 0xD;

		public static final int GENERIC_TEXT_E = 0xE;

		public static final int GENERIC_TEXT_F = 0xF;

		public static final int OUTPUT_CABLE = 0x21;

		public static final int TRACK_LOOP = 0x2E;

		public static final int END_OF_TRACK = 0x2F;

		public static final int TEMPO = 0x51;

		public static final int SMPTE = 0x54;

		public static final int TIMESIG = 0x58;

		public static final int KEYSIG = 0x59;

		public static final int SEQUENCER_SPECIFIC = 0x7F;

		public static final int SYSEX = 0x100;

	}

}
