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
#ifndef JDKMIDI_DRIVERWIN32_H
#define JDKMIDI_DRIVERWIN32_H

#include "jdkmidi/driver.h"
#include "jdkmidi/sequencer.h"

#ifdef WIN32
#include "windows.h"
#include "mmsystem.h"

namespace jdkmidi
{
  
  class MIDISequencerGUIEventNotifierWin32 :
    public MIDISequencerGUIEventNotifier
    {
    public:
      MIDISequencerGUIEventNotifierWin32(
        HWND w,
        DWORD wmmsg,
        WPARAM wparam_value_=0
        );
      
      virtual ~MIDISequencerGUIEventNotifierWin32();
      
      virtual void Notify( const MIDISequencer *seq, MIDISequencerGUIEvent e );
      virtual bool GetEnable() const;
      virtual void SetEnable( bool f );
      
    private:
      
      HWND dest_window;
      DWORD window_msg;
      WPARAM wparam_value;
      bool en;
    };
  
  
  
  class MIDIDriverWin32 : public MIDIDriver
    {
    public:
      MIDIDriverWin32( int queue_size);
      virtual ~MIDIDriverWin32();
      
      void ResetMIDIOut();
      
      bool StartTimer( int resolution_ms );
      bool OpenMIDIInPort( int id );
      bool OpenMIDIOutPort( int id );
      
      void StopTimer();
      void CloseMIDIInPort();
      void CloseMIDIOutPort();
      
      
      bool HardwareMsgOut( const MIDITimedBigMessage &msg );
      
    protected:
      
      static void CALLBACK win32_timer(
        UINT wTimerID,
        UINT msg,
        DWORD dwUser,
        DWORD dw1,
        DWORD dw2
        );
      
      static void CALLBACK win32_midi_in(
        HMIDIIN hMidiIn,
        UINT wMsg,
        DWORD dwInstance,
        DWORD dwParam1,
        DWORD dwParam2
        );
      
      HMIDIIN in_handle;
      HMIDIOUT out_handle;
      int timer_id;
      int timer_res;
      
      bool in_open;
      bool out_open;
      bool timer_open;
    }; 
  
  
}
#endif

#endif
