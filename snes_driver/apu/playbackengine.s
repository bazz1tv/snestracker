; WARNING : Be careful about the DIV YA,X opcode. See FULLSNES.htm.
;	Ensure results will be < 0x200
.include "apu/memorymap.i"
.include "apu/regdefs.i"
.include "apu/enums.i"
.include "apu/playbackengine.i"
; Zero page variables!
.RAMSECTION "gp-dp0" BANK 0 SLOT SPC_DP0_SLOT
spddec db	; copy of spd to dec from
inc_to_patlen db ; " patternlen_rows

pattern_ptr dw	; ptr to the current pattern, maybe only l ram needed)
patternlen_rows db ; 00 == 256

ptrack_ptrs dsw 8 ; pointers to each track data for a pattern
ptrack_ptr dw
rlecounters dsb 8

konbuf db
koffbuf db ; not sure if needed

; WARNING : keep the public stuff out of address 0000 and 0001 because
; that is the jump address stored from IPL ROM
;curtrack db ; plenty of wasted bits that could be used as bit flags here
/* "Public" RAM made aware to PC tracker */
PUBLIC_START  dsb 0
ticks         db    ; 125 uS increments to timer 0
spd           db
dspdir_i      db

instrtable_ptr dw

sequencer_ptr dw	; points to sequencer table
sequencer_i db ; index into the sequencer table

patterntable_ptr	dw ; pattern table turns pattern index into pattern address
extflags db
PUBLIC_END    dsb 0

.ENDS

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "Playback Engine Code" SEMIFREE

ReloadSpdDec:
	mov a, spd
	mov spddec, a
	ret

; IN: (X) current track idx
; CLOBS: (A)
Loadptrack_ptr:
	push x
		mov a, x  ; move track# into A as arg
		asl a
		clrc
		adc a, #ptrack_ptrs
		mov x,a
		mov a,(X)+
		mov ptrack_ptr, a
		mov a,(X)+
		mov ptrack_ptr+1, a
	pop x
	ret

.define rlecounter rlecounters
; Expects <pattern_ptr> to be loaded and valid
; IN: NONE
; OUT: YA = address of next ptrack
; CLOBBERS: A,X,Y
;           ptrack_ptr
;           l
;           rlecounter(s)
QuickReadPTrack:
  mov rlecounter, #0
	mov x, patternlen_rows
  ;--- check if we are in an rlecount
@loop:
  mov y, #0
  mov a, rlecounter
  beq @norle
  dec rlecounter
  bpl @done_read_row
@norle:
	mov a, [ptrack_ptr] + y
  mov l, a
	; if negative, then we have compression on this row
	bpl @nocomp ; iow, all "normal" rows will be read
  inc y

@@test_cbnote:
  bbc l.CBIT_NOTE, @@test_cbinstr
  inc y
@@test_cbinstr:
  bbc l.CBIT_INSTR, @@test_cbvol
  inc y
@@test_cbvol:
  bbc l.CBIT_VOL, @@test_cbfx
  inc y
@@test_cbfx:
  bbc l.CBIT_FX, @@test_cbfxparam
  inc y
@@test_cbfxparam:
  bbc l.CBIT_FXPARAM, @@test_rle
  inc y
@@test_rle:
  bbc l.CBIT_RLE_ONLY1, @@@test_rle_g2
  mov a, #1
  bbc l.CBIT_RLE, @@@store_rle
  inc a
  bra @@@store_rle
@@@test_rle_g2: ; RLE > 2
  bbc l.CBIT_RLE, @done_read_row
  mov a, [ptrack_ptr] + y
  inc y
@@@store_rle:
  mov rlecounter, a
  bra @done_read_row
@nocomp:
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
  dec x
  bne @loop
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
  mov inc_to_patlen, y
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
	mov y, sequencer_i
	mov a, [sequencer_ptr] + y
  bpl @not_eos
; restart sequence
  mov y, #0
  mov a, [sequencer_ptr] + y
  mov sequencer_i, y
@not_eos:
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
	bbs extflags.REPEATPATTERN, LoadPattern
	inc sequencer_i
LoadPattern:
	call !ReloadSpdDec

	; assume sequencer_i has been loaded from Tracker
	call !GetPatternNumberFromsequencer_i
  ; A = pattern number
  call !Load_pattern_ptr

  ; Now we read through the pattern data to load up the Track pattern
	; addresses
	call !Load_ptrack_ptrs

  ; Readahead-check when crossing between patterns
  mov x, #7
- call !Loadptrack_ptr
  call !readahead ; will "plant" a future tick KOFF if note encountered
  dec x
  bpl -

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

  call !ReloadSpdDec
	call !LoadPattern

  ; although the pattern player will eventually update the tracker, this
  ; immediate update gives the impression of a snappy tracker response.
  ; send a setpattern and setrow command to tracker
  mov a, sequencer_i
  mov reportTrackerArg, a
  mov reportTrackerCmd, #reportTrackerCmd_SetPattern
  mov reportTrackerArg, #0
  mov reportTrackerCmd, #reportTrackerCmd_SetRow

	set1 control.ctrlbit_t0 ; start Timer 0
	set1 flags.bPlaySong
	ret

.macro lda
  mov a, \1
.endm

StopSong:
	clr1 flags.bPlaySong
__ret:
  ret

; assuming the tick setting for 120BPM, spd 6 is A7
; truthfully I think the KOFF wait time could be less than A7, but I am
; really not interested in using a second timer. What could be done is
; using one timer at a tick speed that is a multiple faster than the
; requested tick speed
.define post_koff_wait_time ($A7 * 1)
; I like the sound of 1 tick @ 120 BPM, spd 6. So figure out how much time
; that is and get that same amount of time across different BPM and SPD
; settings
calc_kofftick:
  ; divisor
  mov x, ticks
  ; divisor / 2 (for remainder rounding)
  mov a, x
  lsr a
  mov l, a ; l = divisor/2

  ; dividend
  mov y, #>post_koff_wait_time
  mov a, #<post_koff_wait_time
	div ya, x
	; answer is in a, but we might want to add a tick based on rounding the
	; remainder. do a rough check of remainder/2
	cmp y, l ; (remainder - (ticks / 2))
           ; (if remainder approx >= (ticks/2): inc
           ; (approx <= : no inc
	bcc @ret ; < : no inc
	inc a
@ret:
	ret

ContinueSong:
  ; We need to check the timer for a start of row. iow, first check that a
  ; timer tick has occurred (and how many). decrement that value from the
  ; spddec until hits zero (or negative if u want to check at top of
  ; loop); I like the negative one because it safeguards against potential
  ; overflows.
  mov a, spddec
  setc
  sbc a, t0out
  ; has even 1 tick passed?
  cmp a, spddec
  beq __ret

; 1 tick HAS passed
  mov spddec, a
  dec a
  bpl +  ; this essentially gives if (a <= 0) mark next row for processing
  set1 flags.bNextRow
  bra @onlyfx
+ ; at this point, a tick has been fired
  ; TODO: impl fx and stuff
  mov a, koffbuf
  beq @no_koff
  call !calc_kofftick
  cmp a, spddec; , kofftick
  bne @no_koff
@koff:
  mov dspaddr, #koff
  mov a, koffbuf
  mov dspdata, a
  mov koffbuf, #0
@no_koff:

@onlyfx:
  ; do FX processing here on every tick

  bbc flags.bNextRow, @ret
@nextrow:
  clr1 flags.bNextRow
  call !ReloadSpdDec
  ; If it's the start of a new row call the routine to read the
  ; next row man!
  mov konbuf, #0
  call !ReadPTracks
  mov a, konbuf
  beq @no_kon
  mov dspaddr, #koff
  mov dspdata, #0
  mov dspaddr, #kon
  mov dspdata, a
  ; TODO: KOFF
@no_kon:
@ret:
  ret

; IN: X=vnum (0-7)
; OUT: A=vbit (1<<[0-7])
; CLOBBERS: A
VoiceNumToVoiceBit:
  push x
    mov a, #0
    setc
@bitshift:
    rol a
    dec x
    bpl @bitshift
  pop x
  ret

; IN: <ptrack_ptr>,
;     Y = idx into ptrack_ptr,
;     X = curtrack
; OUT: updated Y index, Curtrack DSP PITCH written
; CLOBBERS: A
ReadNote:
  push y
    mov a, [ptrack_ptr] + y
    ;dec a ; account for 0 note being "taken"
    asl a
    mov y, a; Y: idx into notelut
    call !VoiceNumToVoiceBit
    or a, konbuf
    mov konbuf, a ; A has bit that this voice represents
    call !getDSPIDX
    clrc
    adc a, #plo
    push a
      mov dspaddr, a
      mov a, !noteLUT2 + y
      mov dspdata, a
    pop a
    inc a
    mov dspaddr, a
    mov a, !noteLUT2+1 + y
    mov dspdata, a
  pop y
  inc y
  ret

; IN: X: curtrack
; OUT: A = DSP base addr for this voice
getDSPIDX:
  mov a, x ; convert curtrack into DSP offset
asl_a_x4:
.rept 4
  asl a  ; curtrack x 0x10
.endr
  ret

; IN: <ptrack_ptr>,
;     Y = idx into ptrack_ptr,
;     X = curtrack
; OUT: updated Y index, Curtrack DSP written
; CLOBBERS: A, <de>
ReadInstr:
  push x
  push y
    mov a, [ptrack_ptr] + y ; A instrument number == SRCN??
    ;push a
    asl a
    mov y, a ; Y: idx into instrument table
    mov a, [instrtable_ptr] + y
    mov e, a
    inc y
    mov a, [instrtable_ptr] + y
    mov d, a ; DE -> instrument
    call !getDSPIDX 
    ; TODO: Implement PAN around here
    mov x,a ; NOTE X overwritten here with DSP addr (pushed tho)
    mov y, #0 ; reset index for indexing instrument
    mov dspaddr, x ; Vol_l
    mov a, [de] + y ; vol
    mov dspdata, a

    inc x
    mov dspaddr, x ; vol_r
    mov dspdata, a ; mimic vol to vol_r (pan not yet impl'd)

    inc x
    inc x
    inc x
    mov dspaddr, x ; srcn
    inc y ; pan
    inc y ; src
    mov a, [de] + y
    mov dspdata, a ; srcn

    inc x
    mov dspaddr, x ;adsr1
    inc y
    mov a, [de] + y ; adsr1
    mov dspdata, a
    
    inc x
    mov dspaddr, x ;adsr2
    inc y
    mov a, [de] + y ; adsr2
    mov dspdata, a
    ; write to DSP hi byte
  pop y
  inc y
  pop x
  ret

; IN: <ptrack_ptr>,
;     Y = idx into ptrack_ptr,
;     X = curtrack
; OUT: updated Y index, Curtrack DSP Vol re-written
; TODO: ticks vol FX
; CLOBBERS: A
ReadVol:
  push x
    call !getDSPIDX 
    mov x,a ; NOTE X overwritten here with DSP addr (pushed tho)
    mov a, [ptrack_ptr] + y ; A == vol byte
    inc y
    
    mov dspaddr, x ; Vol_l
    mov dspdata, a
    inc x
    mov dspaddr, x ; vol_r
    mov dspdata, a ; mimic vol to vol_r (pan not yet impl'd)
  pop x
  ret



ReadFx:
  ; TODO
  inc y
  ret
ReadFxParam:
  ; TODO
  inc y
  ret

/*ReadCommon:
  push x
  mov x,a
  mov a, [ptrack_ptr] + y
  pop y ; old x (curtrack) now in y
  push y
  pop y
  inc y
  jmp [!idea+X]
idea:
.dw ReadNote, ReadInstr, ReadVol, ReadFx, ReadFxParam
*/
_incyret1:
  inc y
  ret

; CLOBBERS: A, Y
readahead:
  mov a, inc_to_patlen
  inc a
  cbne patternlen_rows, @ret

  mov y, #0
; do a readahead to see if we need to mark KOFF
  mov a, rlecounters + x
  bne @@next_row_is_blank
  mov a, [ptrack_ptr] + y
  bpl @@comp_note ; no compression, go to same routine that has a note
@@comp:
  and a, #1<<CBIT_NOTE
  beq @@next_row_is_blank ; no note, so don't mark koff
@@comp_note:
  ; Time to mark for KOFF
  call !VoiceNumToVoiceBit
  or a, koffbuf
  mov koffbuf, a ; A has bit that this voice represents
@@next_row_is_blank:
@ret
  ret


; Expects <ptrack_ptrs> to be loaded and valid
; IN: inc_to_patlen: start row
;     patternlen_rows: the length of the pattern
; OUT: YA = address of next ptrack
; CLOBBERS: A,X,Y
;           ptrack_ptr
;           l
;           rlecounters
;           patternlen_rows
ReadPTracks:
  mov x, #7 ; put the curtrack into x
  ;--- check if we are in an rlecount
@next_track:
  mov y, #0
  call !Loadptrack_ptr
  mov a, rlecounters + x
  beq @no_active_rle
  dec rlecounters + x
  bpl @track_done
@no_active_rle:
  mov a, [ptrack_ptr] + y
  ; if negative, then we have compression on this row
  bmi @comp
  ; These are the rules that an uncompressed row would follow.
  mov a, #(1<<CBIT) | (1<<CBIT_NOTE) | (1<<CBIT_INSTR) | (1<<CBIT_VOL) | (1<<CBIT_FX) | (1<<CBIT_FXPARAM)
  dec y ; account for lack of compression byte
@comp:
  inc y
  mov l, a
; WARNING: Don't let any of these calls tamper with "l", otherwise
; rewrite all the code
@@test_cbnote:
  bbc l.CBIT_NOTE, @@test_cbinstr
  call !ReadNote ; TODO
@@test_cbinstr:
  bbc l.CBIT_INSTR, @@test_cbvol
  call !ReadInstr ; TODO
@@test_cbvol:
  bbc l.CBIT_VOL, @@test_cbfx
  call !ReadVol
@@test_cbfx:
  bbc l.CBIT_FX, @@test_cbfxparam
  call !ReadFx
@@test_cbfxparam:
  bbc l.CBIT_FXPARAM, @@test_rle
  call !ReadFxParam
@@test_rle:
  bbc l.CBIT_RLE_ONLY1, @@@test_rle_g2
  mov a, #1
  bbc l.CBIT_RLE, @@@store_rle
  inc a
  bra @@@store_rle
@@@test_rle_g2: ; RLE > 2
  bbc l.CBIT_RLE, @done_read_row
  mov a, [ptrack_ptr] + y
  inc y
@@@store_rle:
  mov rlecounters + x, a
@done_read_row:
  ; to avoid Y from overflowing, rewrite the pointer. We could have
  ; increased performance by doing this conditionally when Y approaches
  ; overflow, but size constraint is important on this platform, so keep
  ; instruction size to a minimum
  push x
    mov a, x
    asl a
    mov x, a
    mov a,y
    mov y, #0
    addw ya, ptrack_ptr
    mov ptrack_ptrs + x, a
    mov ptrack_ptrs + 1 + x, y
  pop x
@track_done:
  ; if this is the last row, do not do a readahead (it will be done when
  ; the next pattern is loaded)
  call !readahead
  dec x
  bpl @next_track
@row_done:
  mov a, inc_to_patlen
  inc a
  cbne patternlen_rows, @ret
  call !@report ; report the last row of this pattern
  jmp !NextPattern
@ret:
  mov inc_to_patlen, a
@report:
  dec a ; the tracker should highlight the row that has just been
  ; processed, rather than the next row yet to be processed
  mov reportTrackerArg, a
  mov reportTrackerCmd, #reportTrackerCmd_SetRow
  mov a, sequencer_i
  mov reportTrackerArg, a
  mov reportTrackerCmd, #reportTrackerCmd_SetPattern
  ret

; Credz to loveemu / Real
; I wanted to make my own Note table :'3
noteLUT:
.dw  $0000,
; c-2
.dw  $0040, $0044, $0048, $004c, $0051, $0055, $005b,
.dw  $0060, $0066, $006c, $0072, $0079,
; c-1
.dw  $0080, $0088, $0090, $0098,
.dw  $00a1, $00ab,
.dw  $00b5, $00c0, $00cb, $00d7, $00e4, $00f2, 

noteLUT2:
; c0
.dw  $0100, $010f, $011f, $0130,
.dw  $0143, $0156,
.dw  $016a, $0180, $0196, $01af, $01c8, $01e3, 
; c1
.dw  $0200, $021e, $023f, $0261,
.dw  $0285, $02ab,
.dw  $02d4, $02ff, $032d, $035d, $0390, $03c7, 
; c2
.dw  $0400, $043d, $047d, $04c2,
.dw  $050a, $0557,
.dw  $05a8, $05fe, $065a, $06ba, $0721, $078d, 
; c3
.dw  $0800, $087a, $08fb, $0984, 
.dw  $0a14, $0aae,
.dw  $0b50, $0bfd, $0cb3, $0d74, $0e41, $0f1a, 
; "C-4"
.dw  $1000, $10f4, $11f6, $1307,
.dw  $1429, $155c,
.dw  $16a1, $17f9, $1966, $1ae9, $1c82, $1e34, 
; "C-5"
.dw  $2000, $21e7, $23eb, $260e,
.dw  $2851, $2ab7,
.dw  $2d41, $2ff2, $32cc, $35d1, $3904, $3c68,
; C-6
.dw  $3fff

.ends
