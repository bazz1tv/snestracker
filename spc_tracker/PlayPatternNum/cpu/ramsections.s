.include "cpu/memorymap.i"

.define RINGFIFO_SIZE 1024           ;  /* serial buffer in bytes (power 2)   */
.define RINGFIFO_MASK RINGFIFO_SIZE-1; /* buffer size mask                   */
.export RINGFIFO_MASK

.RAMSECTION "ZeroPage" BANK 0 SLOT 0
Joy1Raw     		DW              ; Holder of RAW joypad data from register
Joy2Raw     		DW

Joy1Press   		DW              ; Contains only pressed buttons (not held down)
Joy2Press   		DW

Joy1Held    		DW              ; Contains buttons that are Held
Joy2Held    		DW

Joy1Held2			DW
Joy1Held3			DW
Joy1Held4			DW

Joy1HeldCount		Dw
bg3_sc_location		DW
; for QuickSetup.s
FrameNum			DW

InputMode						db
SubMode							db
TimerNavRow						db
TimerOldVal						dw
PatternOldVal					db

; TimerOptions
TimerCursorPos						dw
TimerValueCursorPos					dw
TimerCountValCursorPos				dw
TimerCursorPos_RamLocation			dw
TimerValueCursorPos_RamLocation		dw
TimerCountValCursorPos_RamLocation	dw

SPCRam00buffer					dw
SPCRam02buffer					dw

; MIDI SECTION
MidiByte					db
RunningStatusBuffer				db
MidiData0	db
MidiData1	db
MidiStatus	db
LSR		db
ISR		db
FIFO		dsb 16
FIFCount	db
FIFPos		db
ThirdByteFlag	db
RINGFIFO_RDIDX				dw
RINGFIFO_WRIDX				dw

;MidiStatus	db
;MidiData0	db
;MidiData1	db
;MidiDataReadyToProcess	db
;MidiFifoCounter	db

; Strings
Cursor				DW        ;Holds 16-bit position on screen

TempPtr							dw
JoyWaitTimerCountValue			db
Joy1PressTemp					dw
temp							dw
temp1							dw
temp2							dw
temp3							dw
SPCCommandInitiated				db
SNESbPlaySong					db
VBlankPtr						dw

SNESTimerSelect					db		;; CAN PROLLY MAKE THIS DB NOW
SNESTimerValue					db
SNESTimerCountValue				dw
SNESTimerValueBG2CopyPos		dw
TimerCountValBG2CopyPos			dw

;Display Underline
Underline_DestinationAddress		dw
Underline_DestinationTileCount		dw		;; doesnt have to be word

spx_binary_loc		dw

.ends

.BASE $7e
.RAMSECTION "7ebankram"  BANK 0 SLOT $1f ; RAM 7e:2000-FFFF
	RINGFIFO_BUFFER				dsb RINGFIFO_SIZE
.ends

	
	
.BASE $7f
.RAMSECTION "7fbankram"  BANK $0 SLOT $20 ; RAM 7f:0000-ffff
	
	TextBuffer						dsb $400
	;bg2copy							dsb	$800
	
	
	
.ends
	
.BASE 0
