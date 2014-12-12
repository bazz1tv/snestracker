

;=== Include MemoryMap, VectorTable, HeaderInfo ===
.INCLUDE "header.i"
.include "loadgraphics.i"
.include "initsnes.i"
.include "strings.i"
.include "fadein.i"
.include "enums.i"
.include "TimerOptionsScreen.i"
;=== Global Variables ===


;==============================================================================
; main
;==============================================================================

.BANK 0 SLOT 0
.ORG 0
.SECTION "TimerOptionsCode"

TimerOptions:	

	rep #$10
	sep #$20
	
	jsr UploadSelectionRect	; tile #1
	jsr UploadUnderline		; Tile #2
	;jsr BG2Placement
	jsr PlaceSelectionRect
	
	
	; do some color addition
	lda #%00000010
	sta $2130
	lda #%00100100
	sta $2131
	
	lda #%00000010
	sta $212D
	
	
	
	
	; Load Colors for Palette #17,18
	lda #17
	sta $2121
	lda #%00000000
	sta $2122
	lda #%00000011
	sta $2122
	lda #%11100011
	sta $2122
	lda #%00011100
	sta $2122
	lda #$ff
	sta $2122
	lda #%011111111
	sta $2122
	
	
	jsr QuickSetup	; set VRAM, the video mode, background and character pointers, 
	
	
	; INIT
	stz TimerNavRow
	stz TimerScreenInputMode
	rep #$20
	lda #$0106
	sta TimerCursorPos
	lda #$0202
	sta TimerRegValCursorPos
	lda #$0302
	sta TimerCountValCursorPos
	sep #$20
	
	php
	rep #$30
	lda TimerCursorPos
	sta duaddr
	ldy #5
	sty ducount
	jsr DisplayUnderline
	
	;pld
	plp
					
	rep #$30
	sep #$20			
	
	lda #$81
	sta $4200
	
	fadein 1, $8000
	

forever1:
	wai	;wait for next frame
	;do whatever you feel like here	

	;React to Buttons
	jsr TimerOptionsScreenJoy
	
	
	
	
+	jsr DisplayStrings
	
	jsr ClearSelectionRect
	jsr PlaceSelectionRect
	
	jsr ClearUnderlineLocations
	php
	rep #$30
	lda duaddr
	ldy ducount
	jsr DisplayUnderline
	plp
	; Display our Underlines
	
	
	
	jmp forever1

; you can optimize this later
; make it a jsr routine instead of a macro, save space
.macro SetCursorPosB ARGS Pos
	php
	sep #$20
	and #0
	xba
	lda Pos+1	; Y coord
	rep #$20
	asl
	asl
	asl
	asl
	asl
	sta temp
	lda #0
	sep #$20
	lda.b Pos	;X coord
	rep #$20
	clc
	adc temp
	sta.w Cursor
	
	plp
.endm

DisplayStrings:
	php
	rep #$10
	sep #$20
	
	
	
	; not faulty
	
	SetCursorPosB TimerCursorPos
	PrintString "Timer: 1 2 3"
	
	
	ldy #TimerRegVal.w
	SetCursorPosB TimerRegValCursorPos
	PrintString "Timer Val: $%x"

	ldy #TimerCountVal.w
	SetCursorPosB TimerCountValCursorPos
	PrintString "Count Val: $%y"
	
	plp
	rts
	
String1: 
	.dw TimerCursorPos
	.db "Timer: 1 2 3",0
String2: 
	.dw TimerRegValCursorPos
	.db "Timer Val: $  ", 0
String3:
	 .dw TimerCountValCursorPos
	.db "Count Val: $    ", 0
	


	

	
UploadSelectionRect:
	php 
	rep #$10
	sep #$20
	lda #$80
	sta $2115
	ldx #$4010
	stx $2116
	rep #$20
	ldx #0
-	lda SelectionRect.w,X
	sta $2118
	inx
	inx
	cpx #$20
	bne -
	plp
	rts
	
UploadUnderline:
	php
	rep #$10
	sep #$20
	lda #$80
	sta $2115
	ldx #$4020
	stx $2116
	rep #$20
	ldx #0
-	lda Underline.w,X
	sta $2118
	inx
	inx
	cpx #$20
	bne -
	plp
	rts
	
ClearSelectionRect:
	php
	rep #$30
	lda TimerLUT.w
	asl
	clc
	adc #$40
	tax
	stz bg2copy,X
	.rept 5
		inx
		inx
		stz bg2copy,X
	.endr
-	plp
	rts
	 
PlaceSelectionRect:
	php
	sep #$20
	lda TimerScreenInputMode
	cmp #MODE_ROW0
	bne -
	rep #$30
	lda TimerNum
	asl
	tax
	lda TimerLUT.w,X
	asl
	clc
	adc #$20*2
	tax
	sep #$20
	lda #$01
	sta bg2copy,X
	lda #%00000100
	sta bg2copy+1,X
	plp
	rts
	
TimerLUT:
	.dw 13, 15, 17
	
; Selection Rectangle
SelectionRect:
.db $ff,00,$ff,00
.db $ff,00,$ff,00
.db $ff,00,$ff,00
.db $ff,00,$ff,00
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00
.db $00,00,00,00

Underline:
.db 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,$FF,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00

BG2Placement:
	sep #$20
	ldx #$c00
	stx $2116
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000100
	sta $2119
	
	lda #$01
	sta $2118
	lda #%00000000
	sta $2119
	rts

.ENDS

