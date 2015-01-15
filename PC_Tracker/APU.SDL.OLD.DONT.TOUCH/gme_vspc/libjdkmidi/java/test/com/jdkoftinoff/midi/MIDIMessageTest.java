/*
 * MIDIMessageTest.java JUnit based test
 * 
 * Created on 10 September 2006, 17:20
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
public class MIDIMessageTest extends TestCase
{

    public MIDIMessageTest(String testName)
    {
        super(testName);
    }

    protected void setUp() throws Exception
    {
    }

    protected void tearDown() throws Exception
    {
    }

    /**
     * Test of SetNoteOn method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testSetNoteOn()
    {
        System.out.println("setNoteOn");

        short channel = 0;
        short note = 96;
        short velocity = 64;
        MIDIMessage instance = new MIDIMessage();

        instance.setNoteOn(channel, note, velocity);

        try
        {
            assertTrue(instance.isChannelMessage());
            assertTrue(instance.isNoteOn());
            assertFalse(instance.isNoteOff());
            assertEquals(instance.getNote(), note);
            assertEquals(instance.getVelocity(), velocity);
        }
        catch (MIDIError e)
        {
            e.printStackTrace();
            fail("MIDIError caught");
        }
    }

    /**
     * Test of SetNoteOff method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testSetNoteOff()
    {
        System.out.println("setNoteOff");

        short channel = 0;
        short note = 96;
        short velocity = 64;
        MIDIMessage instance = new MIDIMessage();

        instance.setNoteOff(channel, note, velocity);

        try
        {
            assertTrue(instance.isChannelMessage());
            assertTrue(instance.isNoteOff());
            assertFalse(instance.isNoteOn());
            assertEquals(instance.getNote(), note);
            assertEquals(instance.getVelocity(), velocity);
        }
        catch (MIDIError e)
        {
            e.printStackTrace();
            fail("MIDIError caught");
        }
    }

    /**
     * Test of GetChannel method, of class com.jdkoftinoff.midi.MIDIMessage.
     * 
     */
    public void testGetChannel()
    {
        System.out.println("getChannel");

        MIDIMessage instance = new MIDIMessage();

        try
        {
            int expResult = 0;
            int result = instance.getChannel();
            assertEquals(expResult, result);
        }
        catch (MIDIError e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            fail("MIDIError caught");
        }

    }

    /**
     * Test of GetNote method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testGetNote()
    {
        System.out.println("getNote");

        MIDIMessage instance = new MIDIMessage();

        try
        {
            int expResult = 0;
            int result = instance.getNote();
            assertEquals(expResult, result);
        }
        catch (MIDIError e)
        {
            e.printStackTrace();
            fail("MIDIError caught");
        }

    }

    /**
     * Test of GetVelocity method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testGetVelocity()
    {
        System.out.println("getVelocity");

        MIDIMessage instance = new MIDIMessage();

        try
        {
            int expResult = 0;
            int result = instance.getVelocity();
            assertEquals(expResult, result);
            // TODO review the generated test code and remove the default call
            // to fail.
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
     * Test of IsChannelMessage method, of class
     * com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testIsChannelMessage()
    {
        System.out.println("isChannelMessage");

        MIDIMessage instance = new MIDIMessage();

        boolean expResult = true;
        boolean result = instance.isChannelMessage();
        assertEquals(expResult, result);
        // TODO review the generated test code and remove the default call to
        // fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of IsNoteOn method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testIsNoteOn()
    {
        System.out.println("isNoteOn");

        MIDIMessage instance = new MIDIMessage();

        boolean expResult = true;
        boolean result = instance.isNoteOn();
        assertEquals(expResult, result);

        // TODO review the generated test code and remove the default call to
        // fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of IsNoteOff method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testIsNoteOff()
    {
        System.out.println("isNoteOff");

        MIDIMessage instance = new MIDIMessage();

        boolean expResult = true;
        boolean result = instance.isNoteOff();
        assertEquals(expResult, result);

        // TODO review the generated test code and remove the default call to
        // fail.
        fail("The test case is a prototype.");
    }

    public static Test suite()
    {
        TestSuite suite = new TestSuite(MIDIMessageTest.class);

        return suite;
    }

    /**
     * Test of setSysex method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testSetSysex()
    {
        System.out.println("setSysex");

        try
        { 
            MIDISystemExclusive sysex_ = null;
            MIDIMessage instance = new MIDIMessage();
            
            instance.setSysex(sysex_);

            // TODO review the generated test code and remove the default call
            // to fail.
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
     * Test of getSysex method, of class com.jdkoftinoff.midi.MIDIMessage.
     */
    public void testGetSysex()
    {
        System.out.println("getSysex");

        try
        {
            MIDIMessage instance = new MIDIMessage();

            MIDISystemExclusive expResult = null;
            MIDISystemExclusive result = instance.getSysex();
            assertEquals(expResult, result);

            // TODO review the generated test code and remove the default call to
            // fail.
            fail("The test case is a prototype.");
        }
        catch (MIDIError e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            fail("MIDIError caught");
        }
    }

}
