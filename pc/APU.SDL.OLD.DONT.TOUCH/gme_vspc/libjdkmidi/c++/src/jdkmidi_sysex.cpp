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
/*
**	Copyright 1986 to 1998 By J.D. Koftinoff Software, Ltd.
**
**	All rights reserved.
**
**	No one may duplicate this source code in any form for any reason
**	without the written permission given by J.D. Koftinoff Software, Ltd.
**
*/

#include "jdkmidi/world.h"

#include "jdkmidi/sysex.h"


#ifndef DEBUG_MDSYSEX
# define DEBUG_MDSYSEX	0
#endif

#if DEBUG_MDSYSEX
# undef DBG
# define DBG(a)	a
#endif

namespace jdkmidi
{
  
  
  MIDISystemExclusive::MIDISystemExclusive( int size_ )
  {
    ENTER( "MIDISystemExclusive::MIDISystemExclusive" );
    
    buf=new uchar[size_];
    
    if( buf )
      max_len=size_;
    else
      max_len=0;
    
    cur_len=0;
    chk_sum=0;
    deletable=true;
  }
  
  MIDISystemExclusive::MIDISystemExclusive( const MIDISystemExclusive &e )	
  {
    buf = new unsigned char [e.max_len];
    max_len = e.max_len;
    cur_len = e.cur_len;
    chk_sum = e.chk_sum;
    deletable = true;
    
    for( int i=0; i<cur_len; ++i )
    {
      buf[i] = e.buf[i];
    }
  } 
  
  MIDISystemExclusive::~MIDISystemExclusive()
  {
    ENTER( "MIDISystemExclusive::~MIDISystemExclusive" );
    
    if( deletable )
      delete [] buf;
  }
  
  
}
