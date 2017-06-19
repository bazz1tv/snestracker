WHERE SHIT IS
---------------------------------------------
The MIDI Diagnotics test is located in SNES-Midi-ByteTest3/Hello.asm AFAIK

The datasheets are located in datasheets/

and the spc_tracker modules are located in spc
---------------------------------------------

HOW SHIT WORKED
----------------
The original Midi cart used an Exar ST16C550CP UART with 16-byte FIFO. 
SNES would receive MIDI data via triggering of /IRQ interrupt, load the data from the
UART/FIFO and process MIDI commands. The problem encountered with this setup is that
the FIFO would often be overrun. So there's 2 solutions to this problem :

1) Process the MIDI data on a faster processor before sending to SNES
2) Increase the FIFO size

It would be nice to kill 2 birds with one stone. So let's see if we can use an ATmega micro processor in the place of the UART.

The only concern here is current draw. Do we know how much current the SNES can provide a cartridge? The guess is 5V@700mA are available to the cartridge. Let's try to keep that in mind somehow!! 