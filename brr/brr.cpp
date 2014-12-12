#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static signed short clamp_16( int n )
{
	if ( ( signed short ) n != n )
		n = ( signed short ) ( 0x7FFF - ( n >> 24 ) );
	return n;
}

/* This is mostly adapted from SoX adpcm.c. Recently changed to take advantage of sample wrapping behavior,
   and also to convert deltas to double before squaring them, since anything over 46340 will cause the 32-bit
   multiply to wrap around and screw up the running error.

   SoX - Sound eXchange: http://sox.sourceforge.net/ */
static double
AdpcmMashS (int curc,		/* current channel */
			int ch,		/* total channels */
			signed short *v,		/* values to use as starting 2 */
			signed short *vl,		/* if set, factor final two samples against these and add to error */
			int filter,	/* lin predictor coeffs */
			signed short *ibuff,	/* ibuff[] is interleaved input samples */
			int iopow,		/* input/output step, REQUIRE 16 <= *st <= 0x7fff */
			unsigned char *obuff	/* output buffer[blockAlign], or NULL for no output  */
			)
{
	signed short *ip, *itop;
	unsigned char *op;
	int ox = curc;		/*  */
	int d, v0, v1, step;
	double d2;			/* long long is okay also, speed abt the same */

	ip = ibuff + curc;		/* point ip to 1st input sample for this channel */
	itop = ibuff + (ch << 4) + curc;
	v0 = v[0];
	v1 = v[1];
	d2 = 0;

	step = 1 << iopow;

	op = obuff;			/* output pointer (or NULL) */
	for (; ip < itop; ip += ch)
    {
		int vlin, d, da, dp, c;

		/* make linear prediction for next sample */
		/*      vlin = (v0 * iCoef[0] + v1 * iCoef[1]) >> 8; */
		switch ( filter )
		{
		case 0:
			vlin = 0;
			break;
		
		case 1:
			vlin = v0 >> 1;
			vlin += (-v0) >> 5;
			break;
		
		case 2:
			vlin = v0;
			vlin += ( -( v0 + ( v0 >> 1 ) ) ) >> 5;
			vlin -= v1 >> 1;
			vlin += v1 >> 5;
			break;
		
		case 3:
			vlin = v0;
			vlin += ( -( v0 + ( v0 << 2 ) + ( v0 << 3 ) ) ) >> 7;
			vlin -= v1 >> 1;
			vlin += ( v1 + ( v1 >> 1 ) ) >> 4;
			break;
		}
		d = ( *ip >> 1 ) - vlin;		/* difference between linear prediction and current sample */
		da = abs( d );
		if ( da > 16384 && da < 32768 )
		{
			/* Take advantage of wrapping */
			d = d - 32768 * ( d >> 24 );
		}
		dp = d + (step << 2) + (step >> 2);
		c = 0;
		if (dp > 0)
		{
			if (step > 1)
				c = dp / ( step / 2 );
			else
				c = dp * 2;
			if (c > 15)
				c = 15;
		}
		c -= 8;
		dp = ( c << iopow ) >> 1;		/* quantized estimate of samp - vlin */
		/* edge case, if caller even wants to use it */
		if ( iopow > 12 )
			dp = ( dp >> 14 ) & ~0x7FF;
		c &= 0x0f;		/* mask to 4 bits */

		v1 = v0;			/* shift history */
		v0 = ( signed short ) ( clamp_16( vlin + dp ) * 2 );

		d = *ip - v0;
		d2 += (double)d * d;		/* update square-error */

		if (op)
		{			/* if we want output, put it in proper place */
			op[ox >> 1] |= c << ( 4 - ( 4 * ( ox & 1 ) ) );
			ox += ch;
		}
    }
	if (vl)
    {
		d = v0 - vl[0];
		d2 += (double)d * d;
		d = v1 - vl[1];
		d2 += (double)d * d;
		d2 /= 18.;
    }
	else
		d2 /= 16.;			/* be sure it's non-negative */
	if (op)
    {
		/* when generating real output, we want to return these */
		v[0] = v0;
		v[1] = v1;
    }
	return sqrt (d2);
}

void
AdpcmBlockMashI (int ch,	/* number of channels */
				 signed short *ip,	/* ip[] is input samples */
				 unsigned char *obuff,	/* output buffer */
				 signed short *v,	/* input/output last samples */
				 signed short *vl)
{
	int s, smin;
	int k, kmin;
	int c;
	double dmin;
	
	/*static int count = 0;
	bool display = false;
	
	if ( count++ == 2137 ) display = true;*/ /* debug code to monitor bad blocks; in this case, one which had large error deltas */

	for (s = ch; s < ch * 9; s++)
		obuff[s] = 0;

	for (c = 0; c < ch; c++)
    {
		dmin = 0.;
		kmin = 0;
		smin = 0;
		for (s = 0; s < 13; s++)
		{
			for (k = 0; k < 4; k++)
			{
				double d;
				d =
					AdpcmMashS (c, ch, &v[c << 1], vl ? &vl[c << 1] : 0, k,
					ip, s, NULL);

				/*if ( display )
				{
					fprintf( stderr, "%02u, %u: %f\n", s, k, (float)d );
				}*/

				if ((!s && !k) || d < dmin)
				{
					kmin = k;
					dmin = d;
					smin = s;
				}
			}
		}
		obuff[c] = (smin << 4) | (kmin << 2);

		AdpcmMashS (c, ch, &v[c << 1], 0, kmin, ip, smin, obuff + ch);
    }
}

/* This is mostly lifted from Spc_Dsp.cpp in blargg's Game_Music_Emu library,
   which is available at http://www.slack.net/~ant/libs/audio.html#Game_Music_Emu */
int brrdec( unsigned char *ibuff, signed short *obuff, signed short *v )
{
	int block_remain = 16;
	int decoded = 0;
	int addr = 1;
	int block_header = *ibuff;

	while ( block_remain )
	{
		if ( block_remain == 9 && ( ibuff [9] & 3 ) == 1 && ( block_header & 3 ) != 3 )
			return decoded;
	
		int delta = ibuff [addr];
		if ( block_remain & 1 )
		{
			delta <<= 4; // use lower nybble
			addr++;
		}
		
		// Use sign-extended upper nybble
		delta = (signed char) (delta) >> 4;
		
		// For invalid ranges (D,E,F): if the nybble is negative,
		// the result is F000.  If positive, 0000. Nothing else
		// like previous range, etc seems to have any effect.  If
		// range is valid, do the shift normally.  Note these are
		// both shifted right once to do the filters properly, but 
		// the output will be shifted back again at the end.
		int shift = block_header >> 4;
		delta = (delta << shift) >> 1;
		if ( shift > 0x0C )
			delta = (delta >> 14) & ~0x7FF;
		
		// One, two and three point IIR filters
		int smp1 = v[0];
		int smp2 = v[1];
		switch ( (block_header >> 2) & 3 )
		{
			case 0:
				break;
			
			case 1:
				delta += smp1 >> 1;
				delta += (-smp1) >> 5;
				break;
			
			case 2:
				delta += smp1;
				delta += (-(smp1 + (smp1 >> 1))) >> 5;
				delta -= smp2 >> 1;
				delta += smp2 >> 5;
				break;
			
			case 3:
				delta += smp1;
				delta += (-(smp1 + (smp1 << 2) + (smp1 << 3))) >> 7;
				delta -= smp2 >> 1;
				delta += (smp2 + (smp2 >> 1)) >> 4;
				break;
		}
		
		v[1] = smp1;
		v[0] = (signed short) ( clamp_16( delta ) * 2 ); // sign-extend
		
		obuff [decoded++] = v[0];
		
		block_remain--;
	}
	
	return decoded;
}

/* #define DECODE */

/* This function should be sufficient for encoding raw one-shot samples.

   Looping samples will be more fun to deal with. The sample should be
   padded at the start so the length is a multiple of 16 samples, and
   then the loop size should be a multiple of 16 samples as well.

   The encoder history samples ( v[], in this case ) should be saved
   when the encoder reaches the loop point, then passed to the encoder
   when encoding the last block.

   signed short v [2];
   signed short vl [2];
   
   while ( position < end )
   {
     ( read a block of samples )
     AdpcmBlockMashI( 1, pcm_buf, brr_buf, (short*) &v, ( end - position < 16 ) ? 0 : (short*) &vl );
     if ( position == loop_start ) vl = v;
   } */

int main( int argc, const char * const * argv )
{
	if ( argc == 3 )
	{
		FILE * f, * g;
		
		f = fopen( argv[1], "rb" );
		g = fopen( argv[2], "wb" );
		
		if ( !f || !g )
		{
			fputs( "Error opening files.\n", stderr );
			return 1;
		}
		
		signed short v [2] = {0};
		signed short pcm_buf [16];
		unsigned char brr_buf [10];
		int read = 16;
		
		/* One-shot samples need to be padded with at least 9 samples before the end bit is set */
		while ( !feof( f ) && !ferror( g ) )
		{
#ifdef DECODE
			memset( brr_buf, 0, sizeof( brr_buf ) );
			read = fread( brr_buf, sizeof( *brr_buf ), 10, f );
			if ( read < 10 )
			{
				if ( brr_buf [0] & 1 ) break;
				brr_buf [9] = 1;
			}
			else fseek( f, -1, SEEK_CUR );
			read = brrdec( brr_buf, pcm_buf, (short*) &v );
			fwrite( pcm_buf, sizeof( *pcm_buf ), read, g );
#else
			memset( pcm_buf, 0, sizeof( pcm_buf ) );
			read = 16 - fread( pcm_buf, sizeof( *pcm_buf ), 16, f );
			AdpcmBlockMashI( 1, pcm_buf, brr_buf, (short*) &v, 0 );
			fwrite( brr_buf, sizeof( *brr_buf ), 9, g );
#endif
		}
#ifndef DECODE
		if ( read < 9 )
		{
			memset( brr_buf, 0, sizeof( brr_buf ) );
			fwrite( brr_buf, sizeof( *brr_buf ), 9, g );
		}
		brr_buf [0] = 1;
		memset( brr_buf + 1, 0, sizeof( brr_buf ) - sizeof( *brr_buf ) );
		fwrite( brr_buf, sizeof( *brr_buf ), 9, g );
#endif
		
		fclose( g );
		fclose( f );
	}
	
	return 0;
}
