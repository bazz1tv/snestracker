/*
 * MIDISystemExclusiveTest.java
 * JUnit based test
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

import junit.framework.*;

/**
 *
 * @author jeffk
 */
public class MIDISystemExclusiveTest extends TestCase {
    
    public MIDISystemExclusiveTest(String testName) {
        super(testName);
    }
    
    protected void setUp() throws Exception {
    }
    
    protected void tearDown() throws Exception {
    }
    
    public static Test suite() {
        TestSuite suite = new TestSuite(MIDISystemExclusiveTest.class);
        
        return suite;
    }
    
    /**
     * Test of clear method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testClear() {
        System.out.println("clear");
        
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        instance.clear();
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of getData method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testGetData() {
        System.out.println("getData");
        
        int index = 0;
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        short expResult = 0;
        short result = 0;
        
        try
        {
            result = instance.getByte(index);
        }
        catch (MIDIErrorRange e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            fail("Exception");
        }
        assertEquals(expResult, result);
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of putByte method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testPutByte() {
        System.out.println("putByte");
        
        try
        {
            short val = 0;
            MIDISystemExclusive instance = new MIDISystemExclusive();
            
            instance.putByte(val);
            
            // TODO review the generated test code and remove the default call to fail.
            fail("The test case is a prototype.");
        }
        catch (MIDIError e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            fail("MIDIError caught");
        }
    }
    
    /**
     * Test of putSysByte method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testPutSysByte() {
        System.out.println("putSysByte");
        
        try
        {
            MIDISystemExclusive instance = new MIDISystemExclusive();
            
            instance.putSysByte(MIDI.Status.SYSEX_START);
            
            // TODO review the generated test code and remove the default call to fail.
            fail("The test case is a prototype.");
        }
        catch (MIDIError e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            fail("MIDIError caught");
        }
    }
    
    /**
     * Test of putEXC method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testPutEXC() {
        System.out.println("putEXC");
        
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        instance.putEXC();
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of putEOX method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testPutEOX() {
        System.out.println("putEOX");
        
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        instance.putEOX();
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of putChecksum method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testPutChecksum() {
        System.out.println("putChecksum");
        
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        instance.putChecksum();
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of getChecksum method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testGetChecksum() {
        System.out.println("getChecksum");
        
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        int expResult = 0;
        int result = instance.getChecksum();
        assertEquals(expResult, result);
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of clearChecksum method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testClearChecksum() {
        System.out.println("clearChecksum");
        
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        instance.clearChecksum();
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of putNibblizedByte1 method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testPutNibblizedByte1() {
        System.out.println("putNibblizedByte1");
        
        short b = 0;
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        try
        {
            instance.putNibblizedByteLE(b);
        }
        catch (MIDIErrorRange e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            fail("Exception");
        }
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of putNibblizedByte2 method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testPutNibblizedByte2() {
        System.out.println("putNibblizedByte2");
        
        short b = 0;
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        try
        {
            instance.putNibblizedByteBE(b);
        }
        catch (MIDIErrorRange e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            fail("Exception");
        }
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
    /**
     * Test of getLength method, of class com.jdkoftinoff.midi.MIDISystemExclusive.
     */
    public void testGetLength() {
        System.out.println("getLength");
        
        MIDISystemExclusive instance = new MIDISystemExclusive();
        
        int expResult = 0;
        int result = instance.getLength();
        assertEquals(expResult, result);
        
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
}
