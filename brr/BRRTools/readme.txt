=======================================
BRRtools by Bregalad
Special thanks to Kode54
Release 2.1 march 2012
=======================================

BRRtools are currently the most evolved tools to convert between standard RIFF .wav format and SNES's built-in BRR sound format.
They have many features never seen before in any other converter, is written in Java (run on all platforms), and are open source.
I heavily borrowed encoding algorithms from Kode54, which himself heavily borrowed code from some other ADPCM encoder.
This is freeware, feel free to redistribute/improve but DON'T CLAIM IT IS YOUR OWN WORK THANK YOU.

=== How do I use that ===
You have to get either JRE (Java Runtime Environment) or Eclipse (an IDE) to run BRRTools. They are both available on all modern computer
platforms, do a google search to find them.

BRR Tools currently comes into 3 parts :

=== 1) BRRDecoder ==

BRRDecoder decodes a .brr sound sample to a .wav file

Syntax :
java brr.BRRDecoder infile.brr outfile.wav -n1 -l0 -s32000
-n number of times to loop through the sample, default 1
-l loop start point (in BRR block units), default 0
-s output samplerate, default 32000

If you specify a loop count value >1 with the -n command, BRRDecoder will then tell you if the looping is stable, and if not it tries to detect when it gets stable.

=== 2) SPCDecoder ==

SPCDecoder is similar to BRRDecoder, but decode one or multiple BRR sound samples directly from a .spc file to .wav file(s)

Syntax :
java brr.SPCDecoder infile.spc outfile -n1 -f0 -l9 -s16000 -m1.0
-n number of times to loop through the sample(s), default 1
-f first sample # to decode (default : 0)
-l last sample # to decode (default : same as first)
-s output samplerate, default 16000
-m minimum length in secounds

The number of the sample and the ".wav" extention are appended at the end of the file.
In this example, the files created will be :
outfile_0.wav
outfile_1.wav
etc...

If you only specify the first sample number, SPCDecoder will only decode a single sample, but if you set both first and last samples, it will decode all samples between them.
As with BRRDecoder, info about sample's looping stability is written on the screen.

The minimum length functionlity forces all looped sample to loop so that their length is at least the specified number in seconds. This is useful as the length of samples in SPC files can vary greatly. If -n is also specified the longest of the two possible length is used.

=== BRREncoder ===

BRREncoder encodes a .wav file to a .brr native SNES sound sample.

Syntax :
java brr.BRREncoder infile.wav outfile.brr -l -w -f0123 -rc1.0 -sc32000
-l enable looping for BRR sample (default disabled)
If a number follows the -l flag, this is the loop point in samples. The data will be resampled the that the looped part of the sample is an integer # of BRR blocks.
-f enable filters for samples (default all enabled)
-w disable wrapping (encoded sample will be compatible with old/inaccurate emus/SPC players)
-r resample input stream, followed by resample ratio (0.0 to 4.0, not included)
-s automatically resample to get the specified samplerate (default 32kHz). (takes priority on -r)
Resampling interpolation type : -rn/-sn no interpolation, -rl/-sn linear, -rs/-ss sine, -rc/-sc cubic

Only .wav files of 8-bit PCM unsigned or 16-bit PCM signed are accepted. For any other sound format, use another program to convert your files to an accepted format, or even better modify the source so your format is supported.
Multi-channel input (stereo, surround or whatever) will automatically be converted to mono by averaging the channels before the encoding starts. This normally happens without problems but in some rare cases this could cause some destructive interferences and parts of the sound could "disappear".

You can select which filters are enabled or disabled. For example if you only want to use filter 1 and 2, you can do that by typing -f12.
If the first 16 samples aren't already all zeros, the encoder will add a zero block, which is necessary to initialise the decoder's history when decoding the sample.

The encoder allows you to resample your .wav before converting. This is useful so you don't need any external program to make the .wav file the desired length/sample rate.
There is currently 2 ways to do it :
- The -r command is here to resample the input by a specified ratio. For example if you use -rs1.5, the data is first resampled to be 1.5 times shorter. You can specify any real number between 0 and 4. The samplerate from the input file is ignored when you use this.
- The -s command uses the input file samplerate and automatically resamples the data so that it gets the specified output sample-rate.
If you use both -r and -s at the same time, -s will take priority.

If -l is used alone, it just set the loop bit in the encoded BRR sample.
However if -l is followed by a number, BRREncoder will slightly resample the data so that the distance between the specified loop point and the end of the sample is a multiple of 16, therefore an integer amount of BRR blocks.
It is safe to use -s or -r in combination with -l, just keep in mind the resampling is just slightly adjusted. The longer the distance is, the less the resampling is affected. The distance have to be greater then 16, or else BRREncoder will issue an error.
You can choose between several interoplation shemes, if you really don't know which one to use I'd say use sine.

=== Known bugs ===
- Cubic interpolation sometimes doesn't work like it's supposed and have some clicking noise in the compressed samples. If you encounter this problem, use another kind of interpolation.
- SPCdecoder will sometimes throw an exeption if you try to dump unused samples. This is normal as there is no easy way for SPCDecoder to know if a sample is used or not, and to ignore it if it's unused. Therefore it will read garbage in a random adress of the memory and this will not always be good.

=== Plans for future updates ===
- Any suggetions are welcome
- Could someone make a nice GUI for this program ? It would be amazing ! And presonally I have zero knownledge in GUI programing.

=== Contact ===
Contact me at jmasur at bluewin dot ch if you want to give ideas about how I can improve BRRTools, or even better, if you improved the program by yourself.

=== History ===
v1.0 november 2009
First public release

v2.0 march 2012
BRRDecoder :
- Decodes way faster than before.
- Changed the command line interface
- You can now specify any sample rate for the output

BRREncoder :
- Now supports multi-channel (stereo, surround, etc...) input (converted to mono before encoding)
- Is now more tolerant to various .wav files
- I removed the limit on file sizes so now it's possible to convert very long samples (such as entiere songs). Now nothing prevents you to stream audio on the SNES !
- Removed non-working gaussian interpolation (not interesting anyways)
- Can now resample to a specified sample rate (instead of just specifying the resample factor)
- The wrapping in the BRR encoding can be disabled.
- Can automatically resample input so that your loop is an integer multiple of BRR blocks.

SPCDecoder :
- Decodes way faster
- Changed the command line interface
- You can now specify any sample rate for the output
- You have to pick the begining of the filename for the output file(s)
- Samples without looping are threated as so

v2.1 march 2012
SPCDecoder :
- Fixes something in the doccumentation
- Added the "minimum length" functionality
