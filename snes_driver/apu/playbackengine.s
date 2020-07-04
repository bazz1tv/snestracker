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

; SongSettings vals
mvol_val db
evol_val db
esa_val db
edl_val db

efb_val db
; keep these 8 together (iterated)
c0_val db
c1_val db
c2_val db
c3_val db
c4_val db
c5_val db
c6_val db
c7_val db
PUBLIC_END    dsb 0

activeInstrument  dsb 8

;CurTrackData
note_pitch           dw  ; workable copy of the actual pitch value to be stored to DSP
;note_idx_mod12     db  ; tracker's note % 12
note_idx            db  ; tracker's note index
note_octave         db
;EndCurTrackData

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
__ret:
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
  set1 flags.bNextRow ; set the flag indicating we will process the next row
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


/* 12-Tone Equal Temperament (12-TET) (ET) is the most popular method of
intervalizing an octave. The octave is split into twelve equally distant steps.
This system is a simple compromise between the ability to modulate keys vs. having perfect
tuning, though our ears have grown quite accustomed to the sound of these pitch
relations. An example of instruments using ET is the piano and guitar.

ET establishes a constant ratio between all 12 steps of octave. Because musical
notes are built by a ratio, the distance between each note expands as you move
upward and contracts as you move downward. This means the math used to make adjustments
upon any note cannot be linear (fixed offsets).

Every increasing octave has double the resolution of the last.
The distance between each octave doubles!  0-1--2---3----4-----5------6-------7
Be sure to understand that this distance growth is not only per octave, the growth
is happening per note!

ET table can be uncovered most simply by obtaining the twelth root of 2. 2^(1/12)

For an individual octave:
00 1.0
01 1.059463094359295
02 1.122462048309372
03 1.18920711500272 
04 1.259921049894872
05 1.334839854170033
06 1.414213562373093
07 1.498307076876679
08 1.587401051968196
09 1.681792830507425
10 1.781797436280674
11 1.887748625363382
12 2

I could have implemented 8.16 fixed point math (somehow) to be able to perform
these calculations, but instead I went for precalculated tables.
*/

finetuneEqualTemperamentLUT:
;.db $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01
;.db $02, $02, $02, $02, $03, $03, $03, $03, $03, $03, $03, $03
;.db $04, $04, $04, $04, $06, $06, $06, $06, $06, $07, $07, $07
;.db $08, $08, $09, $0a, $0a, $0b, $0b, $0c, $0d, $0d, $0e, $0f
;.db $10, $11, $12, $13, $14, $15, $17, $18, $19, $1b, $1d, $1e
;.db $20, $22, $24, $26, $28, $2b, $2d, $30, $33, $36, $39, $3c
;.db $40, $44, $48, $4c, $51, $55, $5b, $60, $66, $6c, $72, $79

.db $01, $01, $01, $01, $01, $01, $01, $01, $02, $02, $02, $02
;.db $02, $02, $02, $02, $03, $03, $03, $03, $03, $03, $03, $03
.db $04, $04, $04, $04, $06, $06, $06, $06, $06, $07, $07, $08
.db $04, $04, $04, $04, $06, $06, $06, $06, $06, $07, $07, $08
.db $08, $08, $09, $0a, $0a, $0b, $0b, $0c, $0d, $0d, $0e, $0f
.db $08, $08, $09, $0a, $0a, $0b, $0b, $0c, $0d, $0d, $0e, $0f
.db $08, $08, $09, $0a, $0a, $0b, $0b, $0c, $0d, $0d, $0e, $0f
.db $10, $11, $12, $13, $14, $15, $17, $18, $19, $1b, $1d, $1e

; The table below is used in conjunction with the above table
OctaveDivideLUT:
.db 4, 8, 4, 4, 2, 1, 1
; 2 4 8 16
; (i >> 2) * 1 => (i / 4) * 1 => i/4
; (i >> 3) * 4 => (i / 8) * 4 => i/2
; (i >> 2) * 4 => (i / 4) * 4 => i
; (i >> 2) * 8 => (i / 4) * 8 => 2i
; (i >> 1) * 8 => (i / 2) * 8 => 4i
; (i >> 0) * 8 => (  i  ) * 8 => 8i
; (i >> 0) * 16 =>(  i  ) * 16 => 16i

; IN: x = curtrack 
;     note
; CLOBBERS: A, Y
;           note
;           s.0
;           BC (final scaled pitch offset)
;           DE (instrument pointer)
DoFinetune:
  push x  ; curtrack
    ; Load instrument pointer in advance
    mov a, activeInstrument + X
    call !loadInstrPtr  ; then, [DE] -> Instrument
    mov a, note_pitch + 1 ; load P(HI)

    clr1 s.0      ; initially mark the polarity marker as positive
    ; Load up the FineTune Value
    mov y, #Instrument.finetune           ; Y: finetune idx
    mov a, [de] + y ; A: finetune value
  ; FINE TUNE 1st Level Scaling
    bpl @positive
@negative:  
    set1 s.0  ;mark this guy as negative
    ; make the value positive so we can treat it the same
    ; TODO handle 0x80 case
    eor a, #$FF
    inc a
@positive:
    push a
      mov x, note_idx
      mov a, !finetuneEqualTemperamentLUT + X
      mov y, a
      mov x, note_octave
      mov a, !OctaveDivideLUT + x
      mov x, a
    pop a
    push y      ; the multiplier from the ET lut
      push y
        mov y, #0
        div ya, x
        mov d, y  ; remainder. We can use this after later multiplication to make for up error
      pop y       ; multiplier from ET LUT
      mul ya
      mov b, y    ; scaled pitch offset (before error makeup)
      mov c, a    ; ""
    pop a         ; multiplier from ET lut
    mov y, #0
    mov x, #2     ; / 2
    div ya, x
    mov x, d      ; IF there was an original remainder from the 1st division above, add accum to bc
    bne @noRemainder
    clrc
    adc a, c
    mov c, a
    adc b, #0
@noRemainder

    ; YA = final scaled pitch offset
    mov y, note_pitch + 1
    mov a, note_pitch    ; YA = NoteLUT2 hardware base pitch value
    bbc s.0, @add_offset
@subtract_offset  
    SUBW ya, bc
    bra @offset_done
@add_offset  
    ADDW ya, bc
@offset_done
  ; ---- check the pitch isn't beyond allowable range 3FFF ----
    cmp y, #$40
    bmi @val_ok
    mov y, #$3f
    mov a, #$ff
@val_ok  
    ; It is time to update the pitch values
    mov note_pitch + 0, a  ; store pitch(LO)
    mov note_pitch + 1, y  ; store pitch(HI)
  pop x ; curtrack
  ret

; The octave that the NoteLUT is specified in
.define NoteLUTOctave 6
; IN: <ptrack_ptr>,
;     Y = idx into ptrack_ptr,
;     X = curtrack
; OUT: updated Y index, Curtrack DSP PITCH written
; CLOBBERS: A, de
ReadNote:
  push x
  push y
    ; Update the key-on buffer
    call !VoiceNumToVoiceBit
    or a, konbuf
    mov konbuf, a           ; A has bit that this voice represents
    ; -------------------------
    mov a, activeInstrument + x ; The only reason I read the instrument here is
    call !ReadInstr             ; to keep it updated with Tracker interface changes
    ; -------------------------
    mov a, [ptrack_ptr] + y ; the NOTE number
    mov note_idx, a
    mov x, #12              ; divide by octave
    mov y, #0
    div ya, x
                            ; A = octave. Y = note
    mov note_octave, a
;    mov note_idx_mod12, y         ; store the 0-11 note index for later. TODO- use a temp variable
    mov a, note_idx
    asl a
    mov y, a

    mov a, !noteLUT2 + y
    mov note_pitch, a             ; store plo into curtrack Note temporary variable
    mov a, !noteLUT2+1 + y  ; phi
    mov note_pitch + 1, a         ; store phi "
  pop y
  inc y
  pop x
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

; IN:
;     A = instrument number
;     X = curtrack
; OUT: de = pointer to instrument
; CLOBBERS: A, <de>
loadInstrPtr:
  push y
  asl a
  mov y, a  ; Y: idx into instrument table
  mov a, [instrtable_ptr] + y
  mov e, a
  inc y
  mov a, [instrtable_ptr] + y
  mov d, a ; DE -> instrument
  pop y
  ret

; IN: A = instrument number
;     X = curtrack
; OUT: Curtrack DSP written
; CLOBBERS: A, de
ReadInstr:
  push x
  push y
    mov activeInstrument + X, a ; record this track's active instrument number
    ; instrument number MUST BE in A now
    call !loadInstrPtr
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

    inc x           ; pitch (lo) idx
    inc x           ; p-hi
    inc x           ; srcn
    mov dspaddr, x ; srcn
    inc y ; finetune
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

  pop y
  pop x ; track idx
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

; IN: X: curtrack
;     [ptrack_ptr]
;     rlecounters
;     inc_to_patlen
;     patternlen_rows
; CLOBBERS: A, Y
readahead:
  mov a, inc_to_patlen
  inc a
  cbne patternlen_rows, +
  ret
+
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
  call !ReadNote
@@test_cbinstr:
  bbc l.CBIT_INSTR, @@test_cbvol
  mov a, [ptrack_ptr] + y     ; instrument number
  inc y
  call !ReadInstr
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
  push x ; save the curtrack
  ; to avoid Y from overflowing, rewrite the pointer. We could have
  ; increased performance by doing this conditionally when Y approaches
  ; overflow, but size constraint is important on this platform, so keep
  ; instruction size to a minimum
    mov a, x
    asl a
    mov x, a
    mov a,y
    mov y, #0
    addw ya, ptrack_ptr
    mov ptrack_ptrs + x, a
    mov ptrack_ptrs + 1 + x, y
  pop x
  
  ; Do processing here. Such as finetune or effects
  call !DoFinetune
  ; once processing is done. Load the note into DSP
  call !getDSPIDX
  clrc
  adc a, #plo
  mov dspaddr, a
  mov dspdata, note_pitch
  inc dspaddr
  mov dspdata, note_pitch + 1

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

; It turns out I produced this same exact table myself in my old code.
; check out 09f91126 pc-side/bak/C/notes_mapping
/*
noteLUT:
.dw  $0000,
; c-2
.dw  $0040, $0044, $0048, $004c, $0051, $0055, $005b,
.dw  $0060, $0066, $006c, $0072, $0079,
; c-1
.dw  $0080, $0088, $0090, $0098,
.dw  $00a1, $00ab,
.dw  $00b5, $00c0, $00cb, $00d7, $00e4, $00f2, 
*/
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
.dw  $05a8, $05fe, $0659, $06ba, $0721, $078d, 
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
.dw  $2d41, $2ff2, $32cc, $35d1, $3905, $3c68,

; C-6
;.dw  $3fff
.dw $4000, $43ce, $47d6,/* $4c1c,
.dw $50a3, $556e,
.dw $5a83, $5fe4, $6598, $6ba3, $7209, $78d1,*/

.ends
