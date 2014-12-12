.ENUM $00 EXPORT
Joy1Raw     DW              ; Holder of RAW joypad data from register
Joy2Raw     DW

Joy1Press   DW              ; Contains only pressed buttons (not held down)
Joy2Press   DW

Joy1Held    DW              ; Contains buttons that are Held
Joy2Held    DW

Joy1HeldLong	DW
Joy1HeldLonger	DW

Joy1HeldCount	DB

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
TimerRegValCursorPos		dw
TimerCountValCursorPos		dw
TimerScreenInputMode		db
TimerNavRow					db
TimerOldVal					dw
TimerNum	dw
TimerRegVal	db
TimerCountVal	dw

;Display Underline
duaddr		dw
ducount		dw		;; doesnt have to be word

; Temps
temp		dw

enum1_end	db

.ENDE

.enum enum1_end export
bg2copy		dsw	$400
.ende