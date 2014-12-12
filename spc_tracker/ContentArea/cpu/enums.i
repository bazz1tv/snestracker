;.include "cpu/memorymap.i"




.ENUM $00 EXPORT
Joy1Raw     DW              ; Holder of RAW joypad data from register
Joy2Raw     DW

Joy1Press   DW              ; Contains only pressed buttons (not held down)
Joy2Press   DW

Joy1Held    DW              ; Contains buttons that are Held
Joy2Held    DW

Joy1Held2	DW
Joy1Held3	DW
Joy1Held4	DW

Joy1HeldCount	Dw

; for QuickSetup.s
FrameNum		DW
bg3_sc_location	DW

; Strings
Cursor			DW         ;Holds 16-bit position on screen

; Main
spx_binary_loc	dw

; Plotter
xpos		dw
ypos		dw
x_tile		dw
x_remainder dw
y_tile 		dw
y_remainder dw
old_vram_pos dw
vram_plot_position dw
x_pixel		dw
x_tilesize	dw
y_tilesize	dw

; TimerOptions
TimerCursorPos				dw
TimerValueCursorPos			dw
TimerCountValCursorPos		dw
TimerCursorPos_RamLocation	dw
TimerValueCursorPos_RamLocation	dw
TimerCountValCursorPos_RamLocation	dw

InputMode		db
SubMode			db
TimerNavRow					db
TimerOldVal					dw
SNESTimerSelect	db		;; CAN PROLLY MAKE THIS DB NOW
SNESTimerValue	db
SNESTimerCountValue	dw
SNESTimerValueBG2CopyPos		dw
TimerCountValBG2CopyPos			dw

;temporary
SPCRam00buffer	dw
SPCRam02buffer	dw
TempPtr			dw

;Display Underline
Underline_DestinationAddress		dw
Underline_DestinationTileCount		dw		;; doesnt have to be word

JoyWaitTimerCountValue	db
Joy1PressTemp			dw


; Temps
temp		dw
temp2		dw
temp3		dw

SPCCommandInitiated	db
SNESbPlaySong	db


bg2copy		dsw	$400

enumend		db

.ende

.enum $1d00 DESC EXPORT
	VBlankPtr	dw
	
.ende

; MODES
.enum $00 EXPORT
	TimerOptionsMode_Nav	db
    TimerOptionsMode_Row0	db
	TimerOptionsMode_Row1 	db
	TimerOptionsMode_Row2 	db
	
	PatternListMode_Nav		db
	PatternListMode_Edit	db
	
.ende
