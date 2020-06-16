snestracker
===========

[![Build Status](https://dev.azure.com/bazzinotti/snestracker/_apis/build/status/bazzinotti.snestracker?branchName=master)](https://dev.azure.com/bazzinotti/snestracker/_build/latest?definitionId=1&branchName=master)

Congratulations, you have stumbled upon a rare item. Currently in its embryonic stages, snestracker aims to become a full-featured cross platform music production software for the Super Nintendo Entertainment System  (SNES), AKA the Super Famicom.

snestracker uses a custom SPC700 Audio Processing Unit (APU) emulator to produce cycle-accurate emulation, allowing you to create accurate SNES music in the comfort of your PC workstation.

snestracker will take advantage of the most important hardware Digital Signal Processing (DSP) features of the SNES, including noise, echo customization, pitch modulation, ADSR envelopes, stereo panning, and inverse phasing (surround sound).

The music that you create with SNES Tracker exports to various formats. It will export standalone music files in the form of a demo ROM, SPC, WAV. You will eventually be able to incorporate your new music and sound effects into your own custom programs / video games!

Here is an image of the current early prototype!

![snestracker early prototype preview](./pics/snestracker.png)

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
- Instrument Tab
  - Play any voice at any octave, with ability to customize ADSR. Can even play through MIDI keyboard
- SPC Export

Building
--------
See [Building.md](./Building.md)

Road Map
--------
Coming soon, [Roadmap.md](./Roadmap.md)

Community
=========

Feel free to join the new snestracker [Discord Server](https://discord.gg/2WXEJU9). You may also join via IRC. **Server**: irc.esper.net **Channel**:#snestracker

The Discord and IRC are bridged by snesbot :)

Contributing
============

Contributions from the community are valid and welcome! For those curious about getting involved, have a look at [Good First Issues](https://github.com/bazzinotti/snestracker/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22). If you see an issue that appeals to you, post a comment demonstrating your interest. You are also encouraged to join the Discord or IRC. Be interactive! Together we will learn, grow, and make snestracker the best it can be.

Supporting
==========

If you love what I'm doing with snestracker, please join me on my new [Patreon](http://patreon.bazz1.com). I also am selling the brand new [Bazz branded T-Shirt.](https://bazz1.com/shop/t-shirts/bazz-short-sleeve-unisex-t-shirt/) It looks awesome and is very comfy.

![T-Shirt Preview](https://bazz1.com/wp-content/uploads/2020/05/mockup-22a5493f-160x160.png)

Other Recommendations
=====================

Emulators
---------

- [mednafen](http://mednafen.fobby.net/ "Mednafen")
- [retroarch](http://www.libretro.com/)
- [zsnes](http://www.zsnes.com/) old but beloved
- [snes9x](http://www.snes9x.com/)
- for debugging, I prefer my fork of [bsnes-classic](https://github.com/bazzinotti/bsnes-classic).


Flash Carts
-----------
To play on the real Super Nintendo, you will need help of additional hardware, typically known as a flash cart:

- [SD2SNES](https://sd2snes.de)
- Super Everdrive
- [Tototek Superflash](http://www.tototek.com/store/index.php?main_page=product_info&cPath=1_8_11&products_id=39) (outdated)

![By Bazz](./pics/bazz.png)

Developed by Bazz, <bazz@bazz1.com>

Special thanks to Blargg.

Shoutout to byuu, Neviksti, mukunda (eKid), Oli (Vilxdryad) and the snesdev community!

