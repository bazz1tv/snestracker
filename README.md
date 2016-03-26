SNES Tracker (ST)
============
Congratulations, you have stumbled upon a rare item. This is a program being written that is currently in its embryonic stages. It will become a tracker software for the Super Nintendo Entertainment System, AKA the Super Famicom.

A tracker is a program used to sequence music using audio samples. This particular tracker will take advantage of the hardware Digital Signal Processing (DSP) features of the SNES, including noise, echo, pitch modulation, ADSR, stereo panning, and inverse phasing (surround sound). It uses a SNES Audio Processing Unit (APU) emulator to produce cycle-accurate emulation, allowing you to create accurate SNES music, but in the comfort of your PC workstation.

The music that you create with SNES Tracker can be played on the real Super Nintendo, or an SNES emulator. 

SNES Tracker can produce compilations that can be played in a playlist on the real hardware, or as Music/SFX sets for your real video game!

This code has no real license at the moment, so please contact me if
you want to distribute it in any form.

**Main Website**: http://snestracker.club

SNES Tracker Debugger (STD)
===========================

``SNES Tracker Debugger`` is designed as a perfect SPC debugger. This program is also a work in progress, and may or may not be merged directly into SNES Tracker.

## Features

- Memory Tab
 - Full view of RAM identifying read/write/execute points, including echo region and BRR sample/loop data regions.
 - Sample donload / upload (todo)
 - Real-time inspection and modification of SPC RAM and DSP registers
 - Global tempo and volume adjustments
 - Real-time channel activity indicators, including pitch, volume, and gain.
 - Channel solo/mute
 - APU port interaction tool. Interact with the SPC as if you are the SNES. This is great to find new sounds, or reverse engineer the SPC engine
- DSP Tab
 - Full access to all global and voice DSP registers in "break-out" format.
 - DIRectory tool - see what sample entries have been used/unused, and change their values!
- Instrument Tab
 - Play any voice at any octave, with ability to customize ADSR

- SPC Export

## How to Compile

Currently, the following compilation strategies can be taken:

- compile native on OS X
- compile native on Linux
- cross-compile for Windows from Linux

In order to compile, certain library dependencies must be installed on your system first. It is preferred to install these libraries through your package manager if available.

### Dependencies

- [SDL2](https://www.libsdl.org/download-2.0.php) - 2.0.3-r200 - newer versions may be compatible
- [Boost](http://www.boost.org/users/history/ "Boost") 1.56.0-r1 - a newer version of Boost is likely compatible - need only compile the following: 
 - boost\_system-mt
 - boost\_filesystem-mt


### Linux-only deps

- gtk+ (2.0 or 3.0 will work)
- alsa-lib (rtmidi dep)

### Internal Dependencies

- libjdkmidi (packaged internally -- WIP getting build scripts to automate building this)
- rtmidi (packaged internally)
- NativeFileDialog (packaged internally, and modified for cross-platform compilation)
- 7zDec (packaged internally, and modified for cross-platform compilation)
- unrar (packaged internally, and modified for cross-platform compilation)

### Building
STD is currently found at the following path:
*PC\_Tracker/APU.SDL.OLD.DONT.TOUCH/gme\_vspc/std*

From that directory, the makefile can be found.

Currently, undocumented, the libjdkmidi must be properly manually compiled and the Makefile properly configured. This will be left undocumented until further notice, simply so I can get some sleep, and until the build system becomes more user friendly.

Recommendations
===============
Here are some recommended emulators:

- [mednafen](http://mednafen.fobby.net/ "Mednafen")
- [retroarch](http://www.libretro.com/) - a bit difficult to setup
- [zsnes](http://www.zsnes.com/)
- [snes9x](http://www.snes9x.com/)
- for debugging, I prefer my fork of [bsnes-classic](https://github.com/bazzinotti/bsnes-classic)

To play on the real Super Nintendo, you will need help of additional hardware, typically known as a flash cart:

- [SD2SNES](https://sd2snes.de)
- Super Everdrive (google it)
- [Tototek Superflash](http://www.tototek.com/store/index.php?main_page=product_info&cPath=1_8_11&products_id=39) (outdated)
