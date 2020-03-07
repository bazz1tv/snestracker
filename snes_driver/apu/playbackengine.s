; WARNING : Be careful about the DIV YA,X opcode. See FULLSNES.htm.
;	Ensure results will be < 0x200
.include "apu/memorymap.i"
.include "apu/regdefs.i"
.include "apu/enums.i"
.include "apu/playbackengine.i"
; Zero page variables!
.RAMSECTION "gp-dp0" BANK 0 SLOT SPC_DP0_SLOT
/* "Public" RAM made aware to PC tracker */
PUBLIC_START  dsb 0
ticks         db    ; 125 uS increments to timer 0
spd           db

instrtable_ptr dw

sequencer_ptr dw	; points to sequencer table
sequencer_i db ; index into the sequencer table

patterntable_ptr	dw ; pattern table turns pattern index into pattern address
PUBLIC_END    dsb 0

spddec db	; copy of spd to dec from

pattern_ptr dw	; ptr to the current pattern, maybe only temp ram needed)
patternlen_rows db ; 00 == 256

ptrack_ptrs dsw 8 ; pointers to each track data for a pattern
ptrack_ptr dw
;curtrack db ; plenty of wasted bits that could be used as bit flags here
.ENDS

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "Playback Engine Code" SEMIFREE

LoadSpdDec:
	mov a, spd
	mov spddec, a
	ret

DoRow:
	ret
DoTick:
	ret

; IN: (A) current track idx
; CLOBS: (X)
Loadptrack_ptr:
	asl a
	clrc
	adc a, #ptrack_ptrs
	mov x,a
	mov a,(X)+
	mov ptrack_ptr, a
	mov a,(X)+
	mov ptrack_ptr+1, a
	ret


.define rlecounter temp
; Expects <pattern_ptr> to be loaded and valid
; IN: NONE
; OUT: YA = address of next ptrack
; CLOBBERS: A,X,Y
;           ptrack_ptr
;           temp (aliased as rlecounter)
QuickReadPTrack:
  mov rlecounter, #0
	mov x, patternlen_rows
  dec x ; subtract 1 early so we can use BPL
	mov y, #0
  ;--- check if we are in an rlecount
@loop:
  mov a, rlecounter
  bne @norle
  dec rlecounter
  bne @done_read_row_noptr_update
@norle:
	mov a, [ptrack_ptr] + y
	; if negative, then we have compression on this row
	bpl @nocomp ; iow, all "normal" rows will be read
  inc y
@@test_cbnote:
  and a, #CBIT_NOTE
  beq @@test_cbinstr
  inc y
@@test_cbinstr:
  and a, #CBIT_INSTR
  beq @@test_cbvol
  inc y
@@test_cbvol:
  and a, #CBIT_VOL
  beq @@test_cbfx
  inc y
@@test_cbfx:
  and a, #CBIT_FX
  beq @@test_cbfxparam
  inc y
@@test_cbfxparam:
  and a, #CBIT_FXPARAM
  beq @@test_rle2
  inc y
@@test_rle2:
  and a, #CBIT_RLE | CBIT_RLE_ONLY1
  beq @@test_rle1
  dec x
  dec x
  bra @done_read_row
@@test_rle1:
  and a, #CBIT_RLE_ONLY1
  beq @@test_rle_g2
  dec x
  bra @done_read_row
@@test_rle_g2: ; RLE > 2
  and a, #CBIT_RLE
  beq +
  mov a, [ptrack_ptr] + y
  mov rlecounter, a
  inc y
+ bra @done_read_row
@nocomp:
	inc y
	; read note
	inc y
	; read inst
	inc y
	; read vol
	inc y
	; read fx
	inc y
	; read fxparam
	inc y
@done_read_row:
  ; to avoid Y from overflowing, rewrite the pointer. We could have
  ; increased performance by doing this conditionally when Y approaches
  ; overflow, but size constraint is important on this platform, so keep
  ; instruction size to a minimum
  mov a,y
  mov y, #0
  addw ya, ptrack_ptr
  movw ptrack_ptr, ya
@done_read_row_noptr_update:
  dec x
  bpl @loop
_ret:
  ; Once we are done it's time to copy the pointer over. That is done from
  ; outside
	ret
.undef rlecounter

; Expects <pattern_ptr> to be loaded and valid
; IN: NONE
; OUT: ptrack_ptrs (8 words)
;      patternlen_rows
; CLOBBERS: A,X,Y
;           ptrack_ptr
Load_ptrack_ptrs:
; Loadpatternlen_rows
	;mov curtrack, #0
	mov x, #ptrack_ptrs	; track idx MAYBE
	mov y, #0
	mov a, [pattern_ptr] + y
	mov patternlen_rows, a

	; the first track is at pattern_ptr + 1, skipping the row length byte
	mov a, #1
	addw ya, pattern_ptr
  movw ptrack_ptr, ya
@write_ptrack_ptr_loop:
  ; write the location of the next track pattern data (starts at track 0)
  mov (x)+, a
  mov a,y
  mov (x)+, a
  cmp x, #ptrack_ptrs + (8*2) ; all 8 words have been written for 8 tracks
	beq _ret
; could save 2 bytes of codespace here by putting the cmp at the bra
; below, this will cause the last track to be iterated unnecessarily, as
; the only price to pay.
  push x ; save the ptrack_ptrs indexed ptr (points to the current track + 1)
    call !QuickReadPTrack ; YA has curtrack + 1 ptrack ptr
	pop x ; curtrack +1
	bra @write_ptrack_ptr_loop

; Clobbers A,Y
; OUT: A = pattern number
GetPatternNumberFromsequencer_i:
	mov a, sequencer_i
	asl a
	mov y,a
	mov a, [sequencer_ptr] + y
	ret

; PREREQS: patterntable_ptr
; IN: A (pattern number)
; CLOBBERS Y
; OUT: pattern_ptr
; use the pattern number in A, to do a lookup from the patterntable_ptr
Load_pattern_ptr:
  asl a
  mov y, a
  mov a, [patterntable_ptr] + y
  mov pattern_ptr, a ; LO byte of pattern addr
  inc y
  mov a, [patterntable_ptr] + y
  mov pattern_ptr+1, a ; HI byte of pattern addr
  ret

; A precursor to LoadPattern (unused so far)
NextPattern:
	inc sequencer_i
	mov y, sequencer_i
	bpl LoadPattern	; a negative value (0x80-0xff) indicates end of sequence
	; in this case, why don't we for now, start from 0
	mov sequencer_i, #0

LoadPattern:
	call !LoadSpdDec

	; assume sequencer_i has been loaded from Tracker
	call !GetPatternNumberFromsequencer_i
  ; A = pattern number
  call !Load_pattern_ptr

  ; Now we read through the pattern data to load up the Track pattern
	; addresses
	call !Load_ptrack_ptrs

	; send a setpattern and setrow command to tracker
	mov a, sequencer_i
	mov reportTrackerArg, a
	mov reportTrackerCmd, #reportTrackerCmd_SetPattern
	mov reportTrackerArg, #0
	mov reportTrackerCmd, #reportTrackerCmd_SetRow

	ret

PlaySong:
	; Timer setting code
	; clear any running timers (or at least timer0)
	;
	; if more bit setting and clearing on other timers is needed, use a
	; RamBuffer that reflects the Control register to reduce code size
	clr1 control.ctrlbit_t0 ; stop Timer 0
	mov a, ticks
	mov t0div, a

	call !LoadPattern

	set1 control.ctrlbit_t0 ; start Timer 0
	set1 flags.bPlaySong
	ret

StopSong:
	clr1 flags.bPlaySong
	ret

.ends
