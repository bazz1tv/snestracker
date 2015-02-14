J.D. Koftinoff Software, Ltd's C++ MIDI Library Source Code
web: www.jdkoftinoff.com
email: jeffk@jdkoftinoff.com

Released under the GPL (GNU PUBLIC LICENSE): Apr 27, 2004

For details of the GPL please see the file:
       COPYING


Please see the documentation in the subdirectory: 
       docs


This library was originally a collection of utility functions for MIDI written in C back in 1986 for the Atari ST computer. Since then it has evolved into a fairly powerful C++ MIDI Library including:

    * MIDI parsing
    * MIDI Show Control message creation and handling
    * Standard MIDI File type 0 and type 1 reading and writing
    * Timestamped MIDI message and System Exclusive encapsulation
    * Efficient MIDI Track objects for sequencing
    * MIDI Track objects for editing MIDI events
    * MIDI message process chains
    * Containers for multiple MIDI Tracks with iterators
    * MIDI Sequencer core for sequencing and triggering GUI events
    * MIDI Driver abstractions for I/O and sequencing
    * MIDI Driver implementation for Win32 for I/O and sequencing
    * Tempo calculations
    * MIDI Matrix to count note on's and off's and hold pedals to avoid stuck notes and all-notes-off problems when merging midi streams
    * SMPTE management and calculations 

