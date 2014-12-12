;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     This code was created as an example for Fancia of the zsnes boards.
;-     I meant to add music ... but never got around to it.
;-        ... sorry Fancia.
;-
;-     Anyway, if anyone uses this code, please share your creations with me
;-     as I am always curious :)
;------------------------------------------------------------------------

;=== Include MemoryMap, VectorTable, HeaderInfo ===
.INCLUDE "show.inc"

;=== Global Variables ===

;=== Include Library Routines & Macros ===
.INCLUDE "LoadGraphics.asm"
.INCLUDE "InitSNES.asm"
.INCLUDE "2input.asm"

;=== Define Macros ===

;initially there are no pictures - this is incremented as pictures are added
.DEFINE NUM_PICTS 0	

; IncludePicture FILE
.MACRO IncludePicture
	.REDEFINE NUM_PICTS NUM_PICTS+1

	.BANK NUM_PICTS SLOT 0
	.ORG 0
	.SECTION "PictureData\@"
	clr_\1:
		.incbin "Pictures\\\1.clr"	
	map_\1:
		.incbin "Pictures\\\1.map"	
	pic_\1:
		.incbin "Pictures\\\1.pic"	
	endpic_\1:
	.ENDS
.ENDM


; IncludePictInfo FILE
.MACRO IncludePictInfo
	.db :clr_\1 + $40			;picture bank number
	.db $03				;picture mode
	.dw clr_\1				;address offset - palette
	.dw map_\1				;address offset - map
	.dw endpic_\1 - pic_\1		;size of pic
	.dw pic_\1				;address offset - pic
.ENDM



;=== Define Variables ===
.ENUM $00
pics_remaining	db
picture_bank	dw	;high byte not used, but is overwritten
pict_info_ptr	dw
.ENDE

;==============================================================================
; main
;==============================================================================

.BANK 0 SLOT 0
.ORG HEADER_OFF
.SECTION "ShowCode" SEMIFREE

Main:
	InitializeSNES

	rep #$10		;A/mem = 8bit, X/Y=16bit
	sep #$20

	jsr SetupVideo	;setup the video mode, and settings
	jsr JoyInit		;start the joypad read routines, enable NMI

start_show:
	lda.w num_pictures
	sta pics_remaining
	ldx #picture_list
	stx pict_info_ptr

next_picture:
	jsr DisplayPicture	

	rep #$20		; A/mem = 16 bits

	;move x to point to the next 'info structure pointer'
	lda pict_info_ptr
	clc
	adc #$000A		
	sta pict_info_ptr

	;clear 'player 1 buttons pressed' status
	stz Joy1Press

	sep #$20		; A/mem = 8 bits

	;wait for button press	
no_button:
	lda	Joy1Press
	and	#$F0
	bne	button_pressed
	lda	Joy1Press+1
	beq	no_button
button_pressed:

	dec pics_remaining
	bne next_picture
	bra start_show 

;==========================================================================================

;DisplayPicture -
;    input:  pict_info_ptr = location of current picture info block 
;			(data should be in current bank)
;
;
DisplayPicture:
      lda #$8F		;Turn off screen (force blank)
      sta $2100		

	ldx pict_info_ptr
	lda $0000, x	;load data bank
	sta picture_bank

	lda $0001, x	;load video mode
	sta $2105		

	rep #$20		; A/mem = 16 bits

	lda $0002, x	;load picture palette
	tax
	lda picture_bank
	jsr DMAPalette

	ldx pict_info_ptr
	lda $0004, x	;load picture map
	ldx #$0800
	stx $2116
	tax
	ldy #$0800
	lda picture_bank
	jsr LoadVRAM

	ldx pict_info_ptr
	lda $0006, x	;load picture size
	tay

	lda $0008, x	;load picture data
	ldx #$1000
	stx $2116
	tax
	lda picture_bank
	jsr LoadVRAM

	sep #$20		; A/mem = 8 bits

      lda #$0F		;Turn on screen, full brightness
      sta $2100		

	rts

;==========================================================================================

VBlank:
	rep #$30		;A/Mem=16bits, X/Y=16bits
	phb
	pha
	phx
	phy
	phd

	sep #$20		; mem/A = 8 bit, X/Y = 16 bit

	lda #$00		;set DataBank = $00
	pha
	plb

	;update the joypad data
	JSR GetInput

	lda $4210		;clear NMI Flag
	rep #$30		;A/Mem=16bits, X/Y=16bits
	
	pld 
	ply 
	plx 
	pla 
	plb 
      rti

;============================================================================
; SetupVideo -- Set the video mode for the demo
;----------------------------------------------------------------------------
; In: None
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
SetupVideo:
	php

	rep #$10		;A/mem = 8bit, X/Y=16bit
	sep #$20
      
	lda #$03		;Set video mode 3, 8x8 tiles (256 color BG1, 16 color BG2)
      sta $2105         

	lda #$08		;Set BG1's Tile Map VRAM offset to $0800 (word address)
      sta $2107		;   and the Tile Map size to 32 tiles x 32 tiles

	lda #$21		;Set BG1's Character VRAM offset to $1000 (word address)
      sta $210B		;Set BG2's Character VRAM offset to $2000 (word address)

	lda #$01		;Turn on BG1
      sta $212C

	lda #$FF		;Scroll BG1 down 1 pixel
	sta $210E
	sta $210E

	plp
	rts

.ENDS

;==========================================================================================

;don't put these in a section ... the macro does that for us

	IncludePicture cutewendy
	IncludePicture cutewendy1
	IncludePicture cutewendy2
	IncludePicture cutewendy3
	IncludePicture cutewendy4

	IncludePicture fauxpas
	IncludePicture fauxpas1
	IncludePicture fauxpas2
	IncludePicture fauxpas3

	IncludePicture millie
	IncludePicture millie1
	IncludePicture millie2
	IncludePicture millie3
	IncludePicture millie4

	IncludePicture sinfest
	IncludePicture sinfest1
	IncludePicture sinfest2
	IncludePicture sinfest3
	IncludePicture sinfest4

;==========================================================================================

.BANK 0 SLOT 0
.ORG HEADER_OFF
.SECTION "SlideShowInfo" SEMIFREE

.db "Start of SlideShowInfo:"

num_pictures:
.db NUM_PICTS

picture_list:
	IncludePictInfo cutewendy
	IncludePictInfo cutewendy1
	IncludePictInfo cutewendy2
	IncludePictInfo cutewendy3
	IncludePictInfo cutewendy4
	IncludePictInfo fauxpas
	IncludePictInfo fauxpas1
	IncludePictInfo fauxpas2
	IncludePictInfo fauxpas3
	IncludePictInfo millie
	IncludePictInfo millie1
	IncludePictInfo millie2
	IncludePictInfo millie3
	IncludePictInfo millie4
	IncludePictInfo sinfest
	IncludePictInfo sinfest1
	IncludePictInfo sinfest2
	IncludePictInfo sinfest3
	IncludePictInfo sinfest4

.ENDS

;==========================================================================================


