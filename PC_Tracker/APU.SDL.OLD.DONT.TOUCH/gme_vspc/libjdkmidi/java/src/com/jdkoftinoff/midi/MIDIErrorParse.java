/*
 * MIDIErrorParse.java
 *
 * Created on 12 September 2006, 17:36 By Jeff Koftinoff <jeffk@jdkoftinoff.com>
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

/**
 * The MIDIErrorParse class is an error that can be thrown by the various MIDI
 * Parser objects when they encounter invalid MIDI protocol bytes, or invalid
 * Standard MIDI File bytes
 */

public class MIDIErrorParse extends MIDIError
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 2583797129529817468L;

	/** Creates a new instance of MIDI ParseError */
	public MIDIErrorParse()
	{
		super();
	}

	public MIDIErrorParse(String reason)
	{
		super(reason);
	}
}
