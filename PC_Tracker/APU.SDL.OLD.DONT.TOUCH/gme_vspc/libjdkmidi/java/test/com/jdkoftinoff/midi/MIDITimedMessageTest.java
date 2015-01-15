/*
 * MIDITimedMessageTest.java
 * JUnit based test
 *
 *  libjdkmidijava Java Class Library for MIDI
 *
 *  https://clicker.jdkoftinoff.com/projects/trac/jdks/wiki/libjdkmidijava
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

import junit.framework.*;

/**
 *
 * @author jeffk
 */
public class MIDITimedMessageTest extends TestCase {
    
    public MIDITimedMessageTest(String testName) {
        super(testName);
    }

    protected void setUp() throws Exception {
    }

    protected void tearDown() throws Exception {
    }

    public static Test suite() {
        TestSuite suite = new TestSuite(MIDITimedMessageTest.class);
        
        return suite;
    }

    /**
     * Test of setTime method, of class com.jdkoftinoff.midi.MIDITimedMessage.
     */
    public void testSetTime() {
        System.out.println("setTime");
        
        long t = 0L;
        MIDITimedMessage instance = new MIDITimedMessage();
        
        instance.setTime(t);
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of getTime method, of class com.jdkoftinoff.midi.MIDITimedMessage.
     */
    public void testGetTime() {
        System.out.println("getTime");
        
        MIDITimedMessage instance = new MIDITimedMessage();
        
        long expResult = 0L;
        long result = instance.getTime();
        assertEquals(expResult, result);
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
}
