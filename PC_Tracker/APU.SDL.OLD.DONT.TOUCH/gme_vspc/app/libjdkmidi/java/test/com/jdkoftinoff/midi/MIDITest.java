/*
 * MIDITest.java JUnit based test
 * 
 * libjdkmidijava Java Class Library for MIDI
 * 
 * https://clicker.jdkoftinoff.com/projects/trac/jdks/wiki/libjdkmidijava
 * 
 * Copyright (C) 2006 J.D. Koftinoff Software, Ltd. www.jdkoftinoff.com
 * jeffk@jdkoftinoff.com
 * 
 * *** RELEASED UNDER THE GNU GENERAL PUBLIC LICENSE (GPL) September 11, 2006
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
 */

package com.jdkoftinoff.midi;

import junit.framework.*;

/**
 * 
 * @author jeffk
 */
public class MIDITest extends TestCase
{

    public MIDITest(String testName)
    {
        super(testName);
    }

    protected void setUp() throws Exception
    {
    }

    protected void tearDown() throws Exception
    {
    }

    public static Test suite()
    {
        TestSuite suite = new TestSuite(MIDITest.class);

        return suite;
    }

    /**
     * Test of getMessageLength method, of class com.jdkoftinoff.midi.MIDI.
     * \todo
     */
    public void testGetMessageLength()
    {
        System.out.println("getMessageLength");

        short status = 0;

        short expResult = 0;
        int result = MIDI.getMessageLength(status);
        assertEquals(expResult, result);

        // TODO review the generated test code and remove the default call to
        // fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of getSystemMessageLength method, of class
     * com.jdkoftinoff.midi.MIDI.
     * \todo
     */
    public void testGetSystemMessageLength()
    {
        System.out.println("getSystemMessageLength");

        assertEquals(2, MIDI.getSystemMessageLength(MIDI.Status.SONG_POSITION));
        assertEquals(2, MIDI.getSystemMessageLength(MIDI.Status.SONG_SELECT));
        assertEquals(1, MIDI.getSystemMessageLength(MIDI.Status.ACTIVE_SENSE));
        // TODO: More tests
    }

    /**
     * Test of isNoteWhite method, of class com.jdkoftinoff.midi.MIDI.
     * \todo
     */
    public void testIsNoteWhite()
    {
        System.out.println("isNoteWhite");

        short note = 0;

        boolean expResult = true;
        boolean result = MIDI.isNoteWhite(note);
        assertEquals(expResult, result);

        // TODO review the generated test code and remove the default call to
        // fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of isNoteBlack method, of class com.jdkoftinoff.midi.MIDI.
     * \todo
     */
    public void testIsNoteBlack()
    {
        System.out.println("isNoteBlack");

        short note = 0;

        boolean expResult = true;
        boolean result = MIDI.isNoteBlack(note);
        assertEquals(expResult, result);

        // TODO review the generated test code and remove the default call to
        // fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of getNoteOctave method, of class com.jdkoftinoff.midi.MIDI.
     * \todo
     */
    public void testGetNoteOctave()
    {
        System.out.println("getNoteOctave");

        short note = 0;

        short expResult = 0;
        int result = MIDI.getNoteOctave(note);
        assertEquals(expResult, result);

        // TODO review the generated test code and remove the default call to
        // fail.
        fail("The test case is a prototype.");
    }

}
