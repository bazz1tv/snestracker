;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     There are many times that one just needs to write a quick
;-     program to test this or that.  This is the perfect skeleton
;-     code for that.
;- 
;-     For an example, this code prints "Hello world!!" on the screen,
;-     shows some print functions, and tests the emulator's use of the
;-     decimal flag.
;-
;-     If you use my code, please share your creations with me
;-     as I am always curious :)
;------------------------------------------------------------------------

;=== Include MemoryMap, VectorTable, HeaderInfo ===
.INCLUDE "plotter.inc"

;=== Include Library Routines & Macros ===
.INCLUDE "InitSNES.asm"
.INCLUDE "LoadGraphics.asm"
.INCLUDE "2Input.asm"
.INCLUDE "Strings.asm"
.INCLUDE "QuickSetup.asm"

;=== Global Variables ===

.ENUM $80	
	xpos		dw
	ypos		dw
	x_tile		dw
	x_remainder dw
	y_tile 		dw
	y_remainder dw
	temp 		dw
	old_vram_pos dw
	vram_plot_position dw
	x_pixel		dw
	x_tilesize	dw
	y_tilesize	dw
.ENDE

;==============================================================================
; main
;==============================================================================

.BANK 0 SLOT 0
.ORG 0
.SECTION "MainCode"

Main:
	InitializeSNES	

	rep #$10
	sep #$20
	
	jsr init_global_variables
	jsr bg1_plotter_init
	
	jsr QuickSetup	; set VRAM, the video mode, background and character pointers, 
				; and turn on the screen
	rep #$30
				
	
	sep #$20			
	
	; stored sequential tile map.
	; every 8x8 segment uses a different tile value into VRAM
	

	jsr JoyInit		;setup joypads and enable NMI

	;PrintString "\n Hello World!!"
	;PrintString "\n\n        "

forever:
	wai	;wait for next frame

	;do whatever you feel like here	

	;See what buttons were pressed
_up:
	lda	Joy1+1
	and	#$08
	beq	_down

	;UP WAS PRESSED
	lda ypos
	cmp #0
	beq _left
	dec ypos
	bra	_left

_down:
	lda	Joy1+1
	and	#$04
	beq	_left

	; DOWN WAS PRESSED
	lda ypos
	cmp #223
	beq _left
	inc ypos
	bra	_left

_left:
	lda	Joy1+1
	and	#$02
	beq	_right

	;LEFT PRESSED
	lda xpos
	cmp #0
	beq _done
	dec xpos
	bra	_done

_right:
	lda	Joy1+1
	and	#$01
	beq	_done

	; RIGHT PRESSED
	lda xpos
	cmp #255
	beq _done
	inc xpos
	bra	_done
	
_done:
; Done WITH JOY INPUT
	jsr conversions	; Do conversions from Pixel to VRAM Coordinates

	;let's print the current frame number
	SetCursorPos 1, 2
	ldy #xpos
	PrintString "Pixel: (%d, "
	
	;SetCursorPos 0, 2
	ldy #ypos
	PrintString "%d)     "
	
	
	;tax
	; ok now we have the VRAM address @ X
	;sep #$20
	;lda #$00
	;sta $2115
	;stx $2116
	;stx old_vram_pos
	;sep #$30
	rep #$10
	sep #$20
	
	SetCursorPos 4,2
	ldy #x_tile
	PrintString "Tile: (%d "
	ldy #x_remainder
	PrintString "r%d, "
	
	;SetCursorPos 5,2
	ldy #y_tile
	PrintString "%d "
	
	;SetCursorPos 6,2
	
	
	;SetCursorPos 7,2
	ldy #y_remainder
	PrintString "r%d)    "
	
	SetCursorPos 9,2
	ldy #old_vram_pos+1
	PrintString "VRAM: $%x"
	ldy #old_vram_pos
	PrintString "%x"

	;rep #$20
	jmp forever
	
; Conversion Routine
;--------------------
; Converts	
conversions:
	php
	rep #$30
	phx
	pha
	; Time to process the conversion
	; X / 8
	rep #$10
	sep #$20
	
	ldx xpos
	stx $4204	; Dividend low byte
	;stz $4205	; Dividend high
	lda x_tilesize
	sta $4206	; Divisor
	nop			; Wait 16 Cycles
	nop			; Nop is 2 Cycles
	nop			; Or do more useful operations here 
	nop			; To not waste time
	nop
	nop
	nop
	nop
	ldx $4214	; Result (WORD)
	stx x_tile
	ldx $4216	; Remainder (WORD)
	stx x_remainder
	
	; Y / 8
	;sep #$20	; Repeat for Y
	ldx ypos
	stx $4204
	;stz $4205
	lda y_tilesize
	sta $4206
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ldx $4214
	stx y_tile
	ldx $4216
	stx y_remainder
	
	; Based on X/Y Values.. We will change the dot in CHR data 
	; from $1000 as the base
	
	
	;Conversion Routine: Tile Coords -> VRAM ADDRESS
	; $1000 + { [ (32*y_tile) + (x_tile) ] *32/2 } + [ y_remainder*(2/2) ]
	; Explained in README.TXT
	rep #$20
	and #0		; Clear Accumulator
	
	; 32*ytile = horizontal dimension * ytile
	; 32/2 = one tile size in words. in this case, one 4bpp tile is 16 words
	; y_remainder*(2/2) 2 bytes a line, 1 word a line
	
	; This code will change if it gets altered to deal with setting different tile colors.
	; and different bpp
	
	; [ y_tile * 32
	lda y_tile
	asl
	asl
	asl
	asl
	asl
	;-------
	; + (xtile) ]
	clc
	adc x_tile
	;------
	;*32/2
	asl
	asl
	asl
	asl
	;-----
	; $1000 + 
	clc 
	adc #$1000
	;----
	; + y_remainder
	clc
	adc y_remainder
	; Store result  to variable
	sta vram_plot_position
	
	; Now Get Pixel position in the actual byte
	; This code Shifts Right for X remainder amount of times
	; To correctly set the right pixel
	sep #$20
	lda #$80
	ldx x_remainder
	beq +
-	lsr
	dex
	cpx #0
	bne -
+
	sta x_pixel		; The address itself contains the Y pixel information
					; So we only need an x_pixel variable that we write
					; to the Address with
	rep #$30
	pla
	plx
	plp
	rts
;--------------------\
;bg1_plotter init
; Initializes the BG1 SC Map
; to Use seperate individual tiles for each and every segment
; of the tilemap
; Done in incremental fashion
; So tile 0 uses CHR 0, 1 uses CHR 1, etc.
; So actually, the map tile reflects the CHR tile.
bg1_plotter_init:
	; Setup BG1 MAP
	php
	rep #$30
	pha
	phx
	sep #$20
.accu 8
.index 16
	lda #$80
	sta $2115
	ldx #$800
	stx $2116
	rep #$30
	and.w #0
-	sta $2118;
	ina
	cmp #$400
	bne -
	; Should start Jumping to this code :: More standardized
	; Less code buildup (redudant code buildup)
	rep #$30
	plx
	pla
	plp
	rts
;-----------------------

	
	; No Regard for bank register or D reg.
	; or programming bank register
code_exit:
	ply
	plx
	pla
	plp
	rts
	

;------------------------	
; init global variables
;----------------------
init_global_variables:
	php
	rep #$20

	stz xpos		
	stz ypos		
	stz x_tile		
	stz x_remainder 
	stz y_tile 		
	stz y_remainder 
	stz temp 		
	stz old_vram_pos
	stz vram_plot_position
	stz x_pixel
			
	stz x_tilesize	
	stz y_tilesize	
	
	sep #$20
	lda #8
	sta x_tilesize
	sta y_tilesize
	
	plp
	rts
;-------------------------------	
.ENDS

