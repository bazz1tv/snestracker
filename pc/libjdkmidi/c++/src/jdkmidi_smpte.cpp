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
#include "jdkmidi/smpte.h"

#ifndef DEBUG_MDSMPTE
# define DEBUG_MDSMPTE	0
#endif

#if DEBUG_MDSMPTE
# undef DBG
# define DBG(a)	a
#else
# undef DBG
# define DBG(a)
#endif

namespace jdkmidi
{
  
  const uchar smpte_max_frames[] =
  {
    24, 25, 30, 30, 30, 30
  };
  
  const double smpte_smpte_rates[] = 
  {
    24.0,
    25.0,
    30.0/1.001,
    30.0/1.001,
    30.0,
    30.0
  };
  
  const double smpte_smpte_rates_long[] = 
  {
    2400,
    2500,
    3000/1.001,
    3000/1.001,
    3000,
    3000
  };
  
  
  const double smpte_sample_rates[] =
  {
    32000.0,
    44100.0/1.001,
    44100.0,
    48000.0/1.001,
    48000.0,
    48000.0*1.001
  };
  
  const long smpte_sample_rates_long[] =
  {
    320000,
    (long)(441000.0/1.001),
    441000,
    (long)(480000.0/1.001),
    480000,
    (long)(480000.0*1.001)
  };
  
  
  
  SMPTE::SMPTE(
    SMPTE_RATE smpte_rate_,
    SAMPLE_RATE sample_rate_
    )
    :
    smpte_rate( smpte_rate_ ),
    sample_rate( sample_rate_ ),
    sample_number( 0 ),
    hours(0),
    minutes(0),
    seconds(0),
    frames(0),
    sub_frames(0),
    sample_number_dirty(false)
  {
    
  }
  
  
  
  SMPTE::SMPTE(
    const SMPTE & s
    )
  {
    Copy(s);	
  }
  
  
  
  
  
  void	SMPTE::AddHours( char h )
  {
    AddSamples(  GetSampleRateLong()// samples per second times 10
                 * h *
                 (  60		// seconds per minute
                    * 60		// minutes per hour
                    / 10		// compensate for freq*10
                   )
      );
  }
  
  
  void	SMPTE::AddMinutes( char m )
  {
    AddSamples(
      GetSampleRateLong()
      * m *	// samples per second times 10
      ( 60 		// seconds per minute
        / 10 )	// compensate for freq*10
      );
  }
  
  
  void	SMPTE::AddSeconds( char s )
  {
    AddSamples(
      GetSampleRateLong() // samples per second times 10
      * s		 // number of seconds
      / 10		 // compensate for freq*10
      );
  }
  
  
  void	SMPTE::AddFrames( char f )
  {
    AddSamples(
      GetSampleRateLong() 	// samples per second times 10
      * f 			// number of frames
      * 10			// times 10
      / GetSMPTERateLong()	// divide by smpte rate (frames per second) times 100
      );
  }
  
  
  void	SMPTE::AddSubFrames( char sf )
  {
    AddSamples(
      GetSampleRateLong()	// samples per second times 10
      * sf			// number of sub frames
      / GetSMPTERateLong()	// divide by smpte rate (frames per second) times 100
      / 10			// divide by 10 to get hundredths of a frame
      );			
  }
  
  
  
  void	SMPTE::SampleToTime()
  {
    //
    // make a temporary copy of the sample number
    //
    
    ulong tmp_sample=sample_number;
    
    
    //
    // keep track of the actual rates in use in doubles.
    //
    
    double the_smpte_rate = smpte_smpte_rates[ smpte_rate ];
    double the_sample_rate = smpte_sample_rates[ sample_rate ];
    
    //
    // keep track of the maximum frame number for this smpte format.
    //
    
    uchar max_frame = smpte_max_frames[ smpte_rate ];
    
    //
    // Calculate the number of samples per frame.
    //
    
    double samples_per_frame=smpte_sample_rates[ sample_rate ] / smpte_smpte_rates[ smpte_rate ];
    
    //
    // if the smpte rate is a drop frame type, calculate the number
    // of frames that must be dropped.
    //
    
    if( smpte_rate==SMPTE_RATE_30DF || smpte_rate==SMPTE_RATE_2997DF )
    {
      //
      // Calculate number of minutes that have gone by
      //
      
      // short num_minutes = (short)((double)tmp_sample/(smpte_sample_rates[sample_rate]))/60;
      int num_minutes = tmp_sample/(48000*60);
      
      DBG( printf( "num_minutes=%d\n", (int)num_minutes ) );
      
      //
      // Calculate the number of tens of minutes that have gone by, including minute 00
      //
      
      int ten_minutes = num_minutes/10;
      
      DBG( printf( "ten_minutes=%d\n", (int)ten_minutes) );
      
      //
      // Calculate the number of frames that are dropped by this
      // time.
      //
      
      int drops=(num_minutes - ten_minutes)*2;
      
      DBG( printf( "drops=%d\n", (int)drops ) );
      
      //
      // Offset the tmp_sample number by this amount of frames.
      //
      
      DBG( printf( "tmp_sample before drops=%ld\n", (long)tmp_sample ) );
      
      tmp_sample+=(ulong)(drops*samples_per_frame);
      
      DBG( printf( "tmp_sample after drops=%ld\n", (long)tmp_sample ) );
    }
    
    //
    // Calculate the time in sub frames, frames, seconds, minutes, hours
    //
    
    ulong rounded_sub_frames= (ulong)((tmp_sample*the_smpte_rate * 100)/the_sample_rate +.5);
    
    DBG( printf( "rounded_sub_frames = %ld\n", rounded_sub_frames ) );
    
    sub_frames = (uchar) ((rounded_sub_frames)%100);
    frames 	   = (uchar) ((rounded_sub_frames/100) % max_frame);
    seconds	   = (uchar) ((rounded_sub_frames/(100L*max_frame)) % 60 );
    minutes	   = (uchar) ((rounded_sub_frames/(100L*60L*max_frame)) % 60 );
    hours	   = (uchar) ((rounded_sub_frames/(100L*60L*24L*max_frame)) % 24 );
    
  }
  
  
  void	SMPTE::TimeToSample()
  {
    
    //
    // keep track of the actual rates in use in doubles.
    //
    
    double the_smpte_rate = smpte_smpte_rates[ smpte_rate ];
    double the_sample_rate = smpte_sample_rates[ sample_rate ];
    
    //
    // optimize a coupla similiar double divides by calculating it once.
    //
    
    double samples_per_frame=the_sample_rate / the_smpte_rate;
    
    //
    // calculate the sample number
    //
    
    double tmp_sample = (double) (
      (	(hours	 	* the_sample_rate * (60 * 60) )
        +	(minutes 	* the_sample_rate * 60 )
        + 	(seconds 	* the_sample_rate )
        + 	(frames  	* samples_per_frame )
        +	(sub_frames 	* samples_per_frame * (1.0/100.0) ) +.5)
      );
    
    //
    // Now compensate for Drop Frame mode if we are in drop frame mode.
    //
    
    if( smpte_rate == SMPTE_RATE_30DF || smpte_rate==SMPTE_RATE_2997DF )
    {
      //
      // Calculate number of minutes that have gone by
      //
      
      int num_minutes = (int)((double)tmp_sample/(smpte_sample_rates[sample_rate]*60));
      
      DBG( printf( "num_minutes=%d\n", (int)num_minutes ) );
      
      //
      // Calculate the number of tens of minutes that have gone by, including minute 00
      //
      
      int ten_minutes = num_minutes/10;
      
      DBG( printf( "ten_minutes=%d\n", (int)ten_minutes) );
      
      //
      // Calculate the number of frames that are dropped by this
      // time.
      //
      
      int drops=(num_minutes - ten_minutes)*2;
      
      DBG( printf( "drops=%d\n", (int)drops ) );
      
      //
      // Offset the tmp_sample number by this amount of frames.
      //
      
      DBG( printf( "tmp_sample before drops=%ld\n", (long)tmp_sample ) );
      
      tmp_sample-=drops*samples_per_frame;
      
      DBG( printf( "tmp_sample after drops=%ld\n", (long)tmp_sample ) );
      
    }
    
    //
    // save the calculated sample number in self.
    //
    
    sample_number=(ulong)tmp_sample;
  }
  
  
  
  void	SMPTE::Copy( const SMPTE & s )
  {
    smpte_rate=s.smpte_rate;
    sample_rate=s.sample_rate;
    sample_number=s.sample_number;
    hours=s.hours;
    minutes=s.minutes;
    seconds=s.seconds;
    frames=s.frames;
    sub_frames=s.sub_frames;
    sample_number_dirty=s.sample_number_dirty;
  }
  
  
  int	SMPTE::Compare( SMPTE & s )
  {
    ulong a = GetSampleNumber();
    ulong b = s.GetSampleNumber();
    
    if( a<b )
      return -1;
    if( a>b )
      return 1;
    
    return 0;
  }
  
  
  void	SMPTE::Add( SMPTE & s )
  {
    ulong a=GetSampleNumber();
    ulong b=s.GetSampleNumber();
    
    SetSampleNumber( a+b );
  }
  
  void	SMPTE::Subtract( SMPTE & s )
  {
    ulong a=GetSampleNumber();
    ulong b=s.GetSampleNumber();
    
    SetSampleNumber( a-b );
  }
  
  
  
  
  
}
