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
#include "jdkmidi/midi.h"
#include "jdkmidi/msg.h"
#include "jdkmidi/sysex.h"
#include "jdkmidi/parser.h"

using namespace jdkmidi;


void PrintSysEx( FILE *f, MIDISystemExclusive *ex )
{
  int l = ex->GetLength();
  
  fprintf( f, "Sysex Len=%d", l );
  
  for( int i=0; i<l; ++i )
  {
    if( ((i)%20) == 0 )
    {
      fprintf( f, "\n" );
    }		
    fprintf( f, "%02x ", (int)ex->GetData( i ) );		
  }
  fprintf( f, "\n" );
  fflush(f);
}


void PrintMsg( FILE *f, MIDIMessage *m )
{
  int l = m->GetLength();
  
  fprintf( f, "Msg : " );
  
  if( l==1 )
  {
    fprintf( f, " %02x \t\t=", m->GetStatus() );
  }
  else if( l==2 )
  {
    fprintf( f, " %02x %02x \t\t=", m->GetStatus(), m->GetByte1() );	
  }
  else if( l==3 )
  {
    fprintf( f, " %02x %02x %02x \t=", m->GetStatus(), m->GetByte1(), m->GetByte2() );
  }
  
  char buf[129];
  
  m->MsgToText( buf );
  
  fprintf( f, "%s\n", buf );
  fflush(f);
}


int main( int argc, char ** argv )
{
  fprintf( stdout, "mdparse:\n" );
  
  MIDIParser p(32*1024);
  MIDIMessage m;
  FILE *f = stdin;
  
  while( !feof(f) )
  {
    int c = fgetc(f);	
    
    if( c==EOF )
      break;
    
    if( p.Parse( (uchar)c, &m ) )
    {			
      if( m.IsSysEx() )
      {
        PrintSysEx( stdout, p.GetSystemExclusive() );
      }			
      else
      {
        PrintMsg( stdout, &m );
      }					
    }			
  }
  
  return 0;
}

