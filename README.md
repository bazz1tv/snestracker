SNES-Tracker
============
"Super Nintendo Entertainment System" Tracker. 

"Super Famicom" Tracker.

Main Website: http://snestracker.club

This code has no real license at the moment, so please contact me if
you want to distribute it in any form.

Majority of current work is on the SNES Tracker Debugger [STD] in the directory
PC_Tracker/APU.SDL.OLD.DONT.TOUCH/gme_vspc

SNES Tracker Debugger (STD)
===========================

The primary work being done is on this portion of the tool suite. Let's learn
how to compile this thing. Well, let's first establish the dependencies.

BOOST (only a couple things) - boost_system-mt, boost_filesystem-mt
libjdkmidi (packaged internally -- WIP getting build scripts to automate building this)
rtmidi (packaged internally) -- don't think it has to be built
NativeFileDialog (packaged internally)
7zDec (packaged internally)
unrar (packaged internally)

Linux-only deps
---------------
gtk+ 2.0 or 3.0 will work
alsa-lib