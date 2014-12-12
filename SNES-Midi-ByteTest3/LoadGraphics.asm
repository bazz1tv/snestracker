;------------------------------------------------------------------------
;-  Written by: Neviksti
;-     If you use my code, please share your creations with me
;-     as I am always curious :)
;-
;-  Note: some of these routines were originally from examples released
;-     by Grog. I don't remember which ones.
;------------------------------------------------------------------------


;============================================================================
; LoadPalette -- Macro that simplifies calling DMAPalette to load a palette
;----------------------------------------------------------------------------
; In: SRC_ADDR -- 24 bit address of source data
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
; Modifies: A, X
;----------------------------------------------------------------------------

;LoadPalette SRC_ADDRESS
;   requires:  mem/A = 8 bit, X/Y = 16 bit
.MACRO LoadPalette 
	lda #:\1
	ldx #\1
	jsr DMAPalette
.ENDM



;============================================================================
; LoadBlockToVRAM -- Macro that simplifies calling LoadVRAM to copy data to VRAM
;----------------------------------------------------------------------------
; In: SRC_ADDR -- 24 bit address of source data
;     DEST -- VRAM address to write to (WORD address!!)
;     SIZE -- number of BYTEs to copy
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
; Modifies: A, X, Y
;----------------------------------------------------------------------------

;LoadBlockToVRAM SRC_ADDRESS, DEST, SIZE
;   requires:  mem/A = 8 bit, X/Y = 16 bit
.MACRO LoadBlockToVRAM 
	ldx #\2		; DEST
	stx $2116
	lda #:\1		; SRCBANK	
	ldx #\1		; SRCOFFSET
	ldy #\3		; SIZE
	jsr LoadVRAM
.ENDM



.BANK 0 SLOT 0
.ORGA HEADER_OFF
.SECTION "LoadVramCode" SEMIFREE

;============================================================================
; LoadVRAM -- Load data into VRAM
;----------------------------------------------------------------------------
; In: A:X  -- points to the data
;     Y     -- Number of bytes to copy (0 to 65535)  (assumes 16-bit index)
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
; Modifies: none
;----------------------------------------------------------------------------
; Notes:  Assumes VRAM address has been previously set!!
;----------------------------------------------------------------------------
LoadVRAM:
	pha
	phx
	phy			
	phb
	php		;Preserve registers
		
	sep #$20	;Careful not to SEP $10, or it will erase upper half of Y!
			; X/Y = 16 bit

      stz $420B	;Clear the DMA control register
      stx $4302	;Store the data offset into DMA source offset
	sty $4305   ;Store the size of the data block
      sta $4304	;Store the data bank of the source data

	lda #$80
	sta $2115	;set VRAM transfer mode to word-access, increment by 1

      lda #$01	;Set the DMA mode (word, normal increment)
      sta $4300       
      lda #$18    ;Set the destination register (VRAM gate)
      sta $4301      
      lda #$01    ;Initiate the DMA transfer
      sta $420B

	plp		;Restore registers
	plb
	ply
	plx
	pla
	rts		;Return to caller
;============================================================================

.ENDS

.BANK 0 SLOT 0
.ORG HEADER_OFF
.SECTION "DMAPaletteCode" SEMIFREE

;============================================================================
; DMAPalette -- Load entire palette using DMA
;----------------------------------------------------------------------------
; In: A:X  -- points to the data
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
; Modifies: none
;----------------------------------------------------------------------------
DMAPalette:
	pha
	phx
	phb
	php		;Preserve registers

	sep #$20	;A/mem = 8bit

	stz $2121	;start at color 0 
	
      stz $420B	;Clear the DMA control register
      stx $4302	;Store the data offset into DMA source offset
	ldx #$0200
	stx $4305   ;Store the size of the data block
      sta $4304	;Store the data bank holding the tile data

      lda #$00	;Set the DMA mode (byte, normal increment)
      sta $4300       
      lda #$22    ;Set the destination register ( $2122: CG-RAM Write )
      sta $4301      
      lda #$01    ;Initiate the DMA transfer
      sta $420B

	plp		;Restore registers
	plb
	plx
	pla
	rts		;Return to caller
;============================================================================

.ENDS



