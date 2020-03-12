;============================================================================
; Includes
;============================================================================

;== Include MemoryMap, Vector Table, and HeaderInfo ==
.INCLUDE "cpu/memorymap.i"

;== Include SNES Initialization routines ==
.include "cpu/initsnes.i"
.include "cpu/loadgraphics.i"
.include "cpu/enums.i"

;============================================================================
; Main Code
;============================================================================

.BANK 0 SLOT 0
.ORG 0
.SECTION "MainCode"

Start:
	InitSNES    ; Clear registers, etc.

	LoadPalette pal, 0, 4
	; Load Tile data to VRAM
	LoadBlockToVRAM tiles, $0000, tiles_end-tiles	; 2 tiles, 2bpp, = 32 bytes
	LoadBlockToVRAM map, $1000, map_end-map

	; Setup Video modes and other stuff, then turn on the screen
	jsr SetupVideo

	ldx.w #spx_binary
	lda #:spx_binary

	stx spx_binary_loc
	sta spx_binary_loc+2
	jsr LoadMusic
	jsr SPCPlaySong
	;lda #$33
	;ldx #$1000
	;jsr SPCWriteRamByte
DERP:
	bra DERP
	;jmp TimerOptions

	.equ brightness_value $0a	; address
	.equ loop_value $8000

	stz $2100
	ldy #loop_value
	ldx.w #showoff_logo
	stx $10

	;FadeIn 3
	lda #$01		; add value
	sta $01
	lda #$0f		; value to compare for
	sta $02

	ldx #4			; loop for $04ffff
loop_back:
	dey
	bne loop_back
	dey
	dex
	cpx #0
	bne loop_back

	; restore initial loop values for loopback
	ldy #loop_value
	ldx #1

	; update brightness settings
	; $01,$02 control whether the value is added ($01 = 1, $02 = $0f) or subtracted ($01 = $FF, $02 = 0)
	lda brightness_value
	clc
	adc $01
	sta brightness_value
	sta $2100
	cmp $02
	beq indirect_jump
	bra loop_back

indirect_jump:
	jmp ($10)	; jump to address at $10

showoff_logo:
	;Wait 4
	ldy #$8fff	; loop delay = #$04ffff
	ldx #8
	; we really just wait for awhile that's all
wait_loop:
	dey
	bne wait_loop
	dey
	dex
	cpx #0
	bne wait_loop
; done waiting

	;FadeOut 3
	ldx.w #enter_tracker	; our destination after fadeout
	stx $10
	lda #$ff
	sta $01
	lda #$00
	sta $02
	ldy #$ffff
	ldx #1
	bra loop_back

enter_tracker:
	lda #$80
	sta $2100
	; Blank screen
	; clear memory we used
	ldx #$1000
	stx $2116
	lda #$80
	sta $2115
	ldy #0
	ldx #$400
-	sty $2118
	dex
	bne -

	ldx #$0000
	stx $2116
	lda #$80
	sta $2115
	ldy #0
	ldx #$1080/2
-	sty $2118
	dex
	bne -

	; Here we will check for SNESTracker First time Run?? By checking SRAM at Bank 70
	;lda $700000 ; = First run?
	;bne NotFirstTime
-
	bra -

	; We are HERE
	; VRAM got cleared, it's time to get our Main Menu loaded up

;============================================================================
; SetupVideo -- Sets up the video mode and tile-related registers
;----------------------------------------------------------------------------
; In: None
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
SetupVideo:
	lda #$00
	sta $2105           ; Set Video mode 0, 8x8 tiles, 4 color BG1/BG2/BG3/BG4

	lda #4<<2          ; Set BG1's Tile Map offset to $1000 (Word address)
	sta $2107           ; And the Tile Map size to 32x32

	stz $210B           ; Set BG1's Character VRAM offset to $0000 (word address)

	lda #$01            ; Enable BG1
	sta $212C

	lda #$FF
	sta $210E
	stz $210E

	rts
;============================================================================
; Call with Address stored in $00
LoadMusic:
.index 16
	; loop until spc is ready
scr_checkready:
	lda #$AA
	cmp $2140
	bne scr_checkready
	lda #$BB
	cmp $2141
	bne scr_checkready

	ldy #0
scr_firstblock:
	; load spx
	lda #$01
	sta $2141

	lda [spx_binary_loc], y
	iny
	sta $2142
	lda [spx_binary_loc], y
	iny
	sta $2143

	lda #$CC
	sta $2140

scr_check2:
	cmp $2140
	; check for $CC
	bne scr_check2

	; size bytes -> X
	rep #$20
	lda [spx_binary_loc], y
	tax
	sep #$20
	iny
	iny
	lda #$FF
; byte to transfer
REDO:
scr_data_loop:
; switch counter to hi-byte
	xba
; load a byte
	lda [spx_binary_loc], y
	iny
	sta $2141
	xba			; counter to lobyte
	ina
	sta $2140

	;check port0
scr_check4:
	cmp $2140
	bne scr_check4

	dex
	bne scr_data_loop

	pha
	rep #$20
	lda [spx_binary_loc], y
	beq scr_terminate
	iny
	iny
	sep #$20
	sta $2142
	xba
	sta $2143

	; X has starting address
	lda #1	; non-zero
	sta $2141
	pla
	ina
scr_nz1:
	ina
	beq scr_nz1

	sta $2140
scr_check5:
	cmp $2140
	bne scr_check5

	rep #$20
	lda [spx_binary_loc], y
	tax
	sep #$20
	iny
	iny
	lda #$FF ; reset counter too
	bra REDO

scr_terminate:
	sep #$20
	sta $2141
	rep #$20
	iny
	iny
	lda [spx_binary_loc], y
	sep #$20
	sta $2142
	xba
	sta $2143
	pla
	ina
scr_nz2:
	ina
	beq scr_nz2

	sta $2140
scr_check6:
	cmp $2140
	bne scr_check6

	; PROTOCOL COMPLETE.

	RTS

.ENDS

.include "apu/memorymap_defs.i"
.SECTION "MusicData"

;.incdir "apu"
spx_binary:

	.dw 2, spc_end-spc_start		; start address to upload to in SPC
spc_start:
	;.INCBIN "spc.bin"
	.INCBIN "derp2.spc" skip $00102 read $e0
spc_end:

	.dw SPC_CODE_START, a2-a1
a1:
.INCBIN "derp2.spc" skip $100 + SPC_CODE_START read $3000
a2:

	.dw $0000, SPC_CODE_START	; 00 to finish transfer, then address to jump SPC700 too to begin code execution
.ENDS


;============================================================================
; Character Data
;============================================================================
.BANK 1 SLOT 0
.ORG 0
.SECTION "CharacterData3"
.incdir "gfx"
map:
	.incbin "mainlogo/mainlogo.map"
map_end:
tiles:
	.incbin "mainlogo/mainlogo.pic"
tiles_end:
pal:
	.incbin "mainlogo/mainlogo.clr"

.ENDS

