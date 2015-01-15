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

#include "jdkmidi/world.h"

#ifdef WIN32
#include "jdkmidi/driverwin32.h"

namespace jdkmidi
{
  
  
  MIDISequencerGUIEventNotifierWin32::MIDISequencerGUIEventNotifierWin32(
    HWND w,
    DWORD msg,
    WPARAM wparam_value_
    )
    :
    dest_window(w),
    window_msg(msg),
    wparam_value( wparam_value_ ),
    en(true)
  {
    
  }
  
  MIDISequencerGUIEventNotifierWin32::~MIDISequencerGUIEventNotifierWin32()
  {
  }
  
  
  void MIDISequencerGUIEventNotifierWin32::Notify(
    const MIDISequencer *seq,
    MIDISequencerGUIEvent e
    )
  {
    if( en )
    {
      PostMessage(
        dest_window,
        window_msg,
        wparam_value,
        (unsigned long)e
        );
    }
  }
  
  bool MIDISequencerGUIEventNotifierWin32::GetEnable() const
  {
    return en;
  }
  
  void MIDISequencerGUIEventNotifierWin32::SetEnable( bool f )
  {
    en = f;
  }
  
  
  
  MIDIDriverWin32::MIDIDriverWin32( int queue_size)
    :
    MIDIDriver( queue_size ),
    in_handle(0),
    out_handle(0),
    in_open(false),
    out_open(false),
    timer_open(false)
  {
  }
  
  MIDIDriverWin32::~MIDIDriverWin32()
  {
    StopTimer();
    CloseMIDIInPort();
    CloseMIDIOutPort();
  }
  
  void MIDIDriverWin32::ResetMIDIOut()
  {
    if( out_open )
    {
      midiOutReset(out_handle);
    }
  }
  
  bool MIDIDriverWin32::StartTimer( int res )
  {
    if( !timer_open )
    {
      TIMECAPS tc;
      
      if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
      {
        return false;
      }
      
      timer_res = res;
      
      if( timer_res < (int)tc.wPeriodMin )
        timer_res = (int)tc.wPeriodMin;
      
      if( timer_res > (int)tc.wPeriodMax )
        timer_res = (int)tc.wPeriodMax;
      
      
      timeBeginPeriod(timer_res);
      
      timer_id = timeSetEvent(
        res,
        res,
        win32_timer,
        (DWORD)this,
        TIME_PERIODIC
        );
      
      if( timer_id )
      {
        timer_open=true;
      }
    }
    return true;
  }
  
  void MIDIDriverWin32::StopTimer()
  {
    if( timer_open )
    {
      timeKillEvent( timer_id );
      timeEndPeriod( timer_res );
      timer_open = false;
    }
  }
  
  bool MIDIDriverWin32::OpenMIDIInPort( int id )
  {
    if( !in_open )
    {
      if( midiInOpen(
            &in_handle,
            id,
            (DWORD)win32_midi_in,
            (DWORD)this,
            CALLBACK_FUNCTION )!=0
        )
      {
        return false;
      }
      
      midiInStart( in_handle );
      
      in_open=true;
    }
    return true;
  }
  
  bool MIDIDriverWin32::OpenMIDIOutPort( int id )
  {
    if( !out_open )
    {
      int e = midiOutOpen(
        &out_handle,
        id,
        0,
        0,
        CALLBACK_NULL
        );
      
      if( e!=0 )
      {
        return false;
      }
      out_open=true;
    }
    return true;
    
  }
  
  void MIDIDriverWin32::CloseMIDIInPort()
  {
    if( in_open )
    {
      midiInStop( in_handle );
      midiInClose( in_handle );
      in_open = false;
    }
  }
  
  void MIDIDriverWin32::CloseMIDIOutPort()
  {
    if( out_open )
    {
      midiOutClose( out_handle );
      out_open=false;
      Reset();
    }
  }
  
  bool MIDIDriverWin32::HardwareMsgOut( const MIDITimedBigMessage &msg )
  {
    if( out_open )
    {
      // dont send sysex or meta-events
      
      if( msg.GetStatus()<0xff && msg.GetStatus()!=0xf0 )
      {
        DWORD winmsg;
        
        
        winmsg =
          (((DWORD)msg.GetStatus()&0xff)<<0)
          |  	(((DWORD)msg.GetByte1()&0xff)<<8)
          |	(((DWORD)msg.GetByte2()&0xff)<<16);
        
        
        if( midiOutShortMsg( out_handle, winmsg )!=0 )
        {
          return false;
        }
      }
      
      return true;
    }
    return false;
  }
  
  void CALLBACK MIDIDriverWin32::win32_timer(
    UINT wTimerID,
    UINT msg,
    DWORD dwUser,
    DWORD dw1,
    DWORD dw2
    )
  {
    MIDIDriverWin32 *self = (MIDIDriverWin32 *)dwUser;
    
    self->TimeTick( timeGetTime() );
  }
  
  void CALLBACK MIDIDriverWin32::win32_midi_in(
    HMIDIIN hMidiIn,
    UINT wMsg,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2
    )
  {
    MIDIDriverWin32 *self = (MIDIDriverWin32 *)dwInstance;
    
    if( wMsg==MIM_DATA )
    {
      MIDITimedBigMessage msg;
      
      msg.SetStatus( (unsigned char)(dwParam1&0xff) );
      msg.SetByte1( (unsigned char)((dwParam1>>8)&0xff) );
      msg.SetByte2( (unsigned char)((dwParam1>>16)&0xff) );
      
      msg.SetTime( timeGetTime() );
      
      self->HardwareMsgIn( msg );
    }
  }
  
}  
#endif

