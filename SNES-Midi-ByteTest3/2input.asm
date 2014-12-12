;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     If you use my code, please share your creations with me
;-     as I am always curious :)
;------------------------------------------------------------------------

;JoyPad variables = $20 to $27

.ENUM $20
Joy1 		DW		; Current button state of joypad1, bit0=0 if it is a valid joypad
Joy2		DW		;same thing for all pads...

Joy1Press	DW		; Holds joypad1 keys that are pressed and have been pressed since clearing this mem location
Joy2Press	DW		;same thing for all pads...

.ENDE


.DEFINE HVBJOY	$4212
.DEFINE JOY0	$4218
.DEFINE JOY1	$421A
.DEFINE JOY2	$421C
.DEFINE JOY3	$421E
.DEFINE JOYSER0	$4016
.DEFINE JOYSER1	$4017
.DEFINE WRIO	$4201

.BANK 0
.ORGA HEADER_OFF
.SECTION "JoyInit" SEMIFREE

JoyInit:
	php	

	rep	#$10	;8 bit mem/A, 16 bit X/Y
	sep	#$20

	lda #$C0	;have the automatic read of the SNES read the first pair of JoyPads
	sta WRIO

	ldx #$0000
	stx Joy1Press
	stx Joy2Press

	LDA #$81
	STA $4200   ;Enable JoyPad Read and NMI

	WAI		;Wait for NMI to fill the variables with real JoyPad data

;******* in ZSNES, the auto-read doesn't work unless NMI is enabled... is this correct?
;
;	lda $4212
;	lda $4212
;
;_W11:
;	lda $4212
;	and #$01
;	beq _W11		;Wait till automatic JoyPort read starts
;
;	jsr GetInput	;fill the variables with real Joypad data
;
;_W12:
;	lda $4212
;	and #$01
;	beq _W12		;Wait till automatic JoyPort read starts
;
;	jsr GetInput	;fill the variables with real Joypad data

	plp
	rts
.ENDS

.BANK 0
.ORG HEADER_OFF
.SECTION "JoyInput" SEMIFREE

GetInput:
	php
	sep 	#$20	;8 bit mem/A

	lda #$01
_W1:	bit $4212
	bne _W1		;Wait till automatic JoyPort read is complete
	

	rep	#$30		;16 bit mem/A, 16 bit X/Y

	; ********** Get Joypads 1,2

	lda JOY0		;get JoyPad1
	tax 			
	eor Joy1		;A = A xor JoyState = (changes in joy state)
	stx Joy1		;update JoyState
	ora Joy1Press	;A = (joy changes) or (buttons pressed)
	and Joy1		;A = ((joy changes) or (buttons pressed)) and (current joy state)  
	sta Joy1Press	;store A = (buttons pressed since last clearing reg) and (button is still down)

	lda JOY1		;get JoyPad2
	tax 			
	eor Joy2		;A = A xor JoyState = (changes in joy state)
	stx Joy2		;update JoyState
	ora Joy2Press	;A = (joy changes) or (buttons pressed)
	and Joy2		;A = ((joy changes) or (buttons pressed)) and (current joy state)  
	sta Joy2Press	;store A = (buttons pressed since last clearing reg) and (button is still down)

	; ********** Make sure Joypads 1,2 are valid

	sep #$30		;A/mem = 8bit, X/Y = 8bit
	lda JOYSER0
	eor #$01
	and #$01		; A = -bit0 of JoySer0
	ora Joy1		
	sta Joy1		; joy state = (joy state) or A.... so bit0 of Joy1State = 0 only if it is a valid joypad

	lda JOYSER1
	eor #$01
	and #$01		; A = -bit0 of JoySer1
	ora Joy2		
	sta Joy2		; joy state = (joy state) or A.... so bit0 of Joy1State = 0 only if it is a valid joypad

	; ********** Change all invalid joypads to have a state of no button presses

	rep #$30		;A/mem = 16bit, X/Y = 16bit

	ldx #$0001
	lda #$000F

	bit Joy1		; A = joy state, if any of the bottom 4 bits are on... either nothing is plugged
	beq _joy2		; into the joy port, or it is not a joypad
	stx Joy1		; if it is not a valid joypad put $0001 into the 2 joy state variables
	stz Joy1Press

_joy2:
	bit Joy2		; A = joy state, if any of the bottom 4 bits are on... either nothing is plugged
	beq _done		; into the joy port, or it is not a joypad
	stx Joy2		; if it is not a valid joypad put $0001 into the 2 joy state variables
	stz Joy2Press

_done:
	plp
	rts

.ENDS
