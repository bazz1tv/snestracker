Debugger
========

In addition to the tracker, there is a debugger program that allows you to hone in on SPC files in various ways. Here is a preview of the main pane, based on raph's vspcplay.
![debugger photo](./pics/std-mem.png)

Features
--------
- Memory Tab
  - Full view of RAM identifying read/write/execute points, including echo region and BRR sample/loop data regions.
  - Sample download
  - Real-time inspection and modification of SPC RAM and DSP registers
  - Global tempo and volume adjustments
  - Real-time channel activity indicators. pitch, volume, and gain.
  - Channel solo/mute
  - APU port interaction tool.
    - Interact with the SPC as if you are the SNES. This is great to find new sounds, or reverse engineer the SPC engine
- DSP Tab
  - Full access to all global and voice DSP registers in "break-out" format.
  - DIRectory tool - see what sample entries are active, download samples, change dir location!
  - STI / BRR Rip. Rip samples as raw BRR or SNES Tracker Instrument with ADSR info!
- Instrument Tab
  - Play any voice at any octave, with ability to customize ADSR. Can even play through MIDI keyboard
- SPC Export