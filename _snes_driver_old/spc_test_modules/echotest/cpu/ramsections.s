.include "cpu/memorymap.i"

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

; Strings
Cursor				DW        ;Holds 16-bit position on screen

TempPtr							dw
JoyWaitTimerCountValue			db
Joy1PressTemp					dw
temp							dw
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
	werp dw
	
.ends
.BASE $7f
.RAMSECTION "7fbankram"  BANK $0 SLOT $20 ; RAM 7f:0000-ffff
	
	TextBuffer						dsb $400
	bg2copy							dsb	$800
.ends
	
.BASE 0
