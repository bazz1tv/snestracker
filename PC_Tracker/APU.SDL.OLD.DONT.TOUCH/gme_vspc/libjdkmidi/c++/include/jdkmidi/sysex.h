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

#ifndef JDKMIDI_SYSEX_H
#define JDKMIDI_SYSEX_H

#include "jdkmidi/midi.h"

namespace jdkmidi
{
  
  class  MIDISystemExclusive	
    {
    public:
      MIDISystemExclusive( int size=384 );
      
      MIDISystemExclusive( const MIDISystemExclusive &e );
      
      
      MIDISystemExclusive(
        unsigned char *buf_,
        int max_len_,
        int cur_len_,
        bool deletable_
        )	
        {
          buf=buf_;
          max_len=max_len_;
          cur_len=cur_len_;
          chk_sum=0;
          deletable=deletable_;
        } 
      
      virtual	~MIDISystemExclusive(); 
      
      void	Clear()				{ cur_len=0; chk_sum=0;	}
      void	ClearChecksum()		{ chk_sum=0;			}
      
      void	PutSysByte( unsigned char b )	// does not add to chksum	
        {
          if( cur_len<max_len )
            buf[cur_len++]=b;
        } 
      
      void	PutByte( unsigned char b )	
        {
          PutSysByte(b);
          chk_sum+=b;
        } 
      
      void	PutEXC()	{ PutSysByte( SYSEX_START );	}  
      void	PutEOX()	{ PutSysByte( SYSEX_END );	}
      
      // low nibble first
      void	PutNibblizedByte( unsigned char b )	
        {
          PutByte( (unsigned char)(b&0xf) );
          PutByte( (unsigned char)(b>>4) );
        } 
      
      // high nibble first
      void	PutNibblizedByte2( unsigned char b )	
        {
          PutByte( (unsigned char)(b>>4) );
          PutByte( (unsigned char)(b&0xf) );
        } 
      
      void	PutChecksum()	
        { 
          PutByte( (unsigned char)(chk_sum&0x7f ));	
        } 
      
      unsigned char	GetChecksum() const	
        {
          return (unsigned char)(chk_sum&0x7f);		
        } 
      
      int		GetLength() const	
        { 
          return cur_len;	
        } 
      
      unsigned char	GetData( int i) const	
        { 
          return buf[i];	
        } 
      
      bool	IsFull() const	
        { 
          return cur_len>=max_len;	
        } 
      
      unsigned char *GetBuf() 
        {
          return buf;
        } 
      
      const unsigned char *GetBuf() const 
        {
          return buf;
        } 
      
    private:
      
      unsigned char *buf;
      int max_len;
      int cur_len;
      unsigned char  chk_sum;
      bool deletable;
    }; 
}

#endif


