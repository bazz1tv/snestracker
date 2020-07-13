.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/regdefs.i"
.INCLUDE "apu/enums.i"
.include "apu/playbackengine.i"

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "Commands" SEMIFREE

PrepHardware:
  ; Song Settings
  ; load fir filter coefficients
  mov y, #8
  mov a, #c7
- mov x, c0_val - 1 + y
  mov dspaddr, a
  mov dspdata, x
  setc
  sbc a, #$10
  dbnz y, -

  mov dspaddr, #dir
  mov dspdata, dspdir_i

  mov dspaddr, #esa
  mov dspdata, esa_val
  MOV dspaddr, #efb
  MOV dspdata, efb_val

  MOV dspaddr, #edl
  mov y, dspdata ; old EDL
  MOV dspdata, edl_val ; update edl value

  ; wouldn't hurt to wait an extra 16ms
  inc y

  ; We should wait EDL * 16ms here to avoid sound glitch from activating EDL
  clr1 control.ctrlbit_t0
  mov t0div, #128
  set1 control.ctrlbit_t0
- mov a, t0out
  beq -
  dbnz y, -

  mov dspaddr, #mvol_l
  mov dspdata, mvol_val
  mov dspaddr, #mvol_r
  mov dspdata, mvol_val

; it is now safe to set echo volume
  mov dspaddr, #evol_l
  mov dspdata, evol_val
  mov dspaddr, #evol_r
  mov dspdata, evol_val

  ; Timer setting code
  ; clear any running timers (or at least timer0)
  ;
  ; if more bit setting and clearing on other timers is needed, use a
  ; RamBuffer that reflects the Control register to reduce code size
  clr1 control.ctrlbit_t0 ; stop Timer 0
  mov t0div, ticks

  set1 control.ctrlbit_t0 ; start Timer 0

  MOV dspaddr, #flg   ; TURN OFF MUTE, enable echo
  MOV dspdata, #%00000000
  ret

PlaySong:
  call !PrepHardware
PlaySongFromSetPatternCmd:
  call !LoadPattern

  ; although the pattern player will eventually update the tracker, this
  ; immediate update gives the impression of a snappy tracker response.
  ; send a setpattern and setrow command to tracker
  ; mov a, sequencer_i
  ; mov reportTrackerArg, a
  ; mov reportTrackerCmd, #reportTrackerCmd_SetPattern
  ; mov a, #0
  ; mov reportTrackerArg, a
  ; mov reportTrackerCmd, #reportTrackerCmd_SetRow

  set1 flags.bPlaySong
  ret

.macro lda
  mov a, \1
.endm

StopSong:
  clr1 flags.bPlaySong
; decays playing sounds (otherwise they could last infinitely depending on ADSR)
; Clobbers: [dspaddr]
koffAllNotes:
  mov dspaddr, #koff
  mov dspdata, #$FF
__ret:
  ret

; KOF time is ~8ms, which is $40 ticks of main timers.
.define post_koff_wait_time ($40 * 1)
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
  bne @notzero
  inc a    ; necessary pad since lowest spd value "processed" is 1
@notzero:
  ; answer is in a, but we might want to add a tick based on rounding the
  ; remainder. do a rough check of remainder/2
  cmp y, l ; (remainder - (ticks / 2))
           ; (if remainder approx >= (ticks/2): inc
           ; (approx <= : no inc
  bcc @ret ; < : no inc
  inc a
@ret:
  ret

; 1 byte argument in port 2 - Pattern# to play
SetPattern:
  call !StopSong
  mov sequencer_i, spcport2
  jmp !PlaySongFromSetPatternCmd

; IN: spcport0: instr #
;     spcport2: note index (see enum Note in CPP code)
;     spcport3: track# (0-7)
;CLOBBERS:  a,x,y, rs, ptrack_ptr
PlayInstrument:
  mov konbuf, #0
  call !PrepHardware
  ; Hack this so it uses pre-existing functionality from the pattern reader
  mov x, spcport3 ; track# (curtrack)
  mov a, spcport0 ; instr#
  mov activeInstrument + x, a
  mov a, spcport2 ; note
  call !ReadNote
  call !TrackRowPostProcess
  jmp !HackishPlayInstrumentFunctionality

; clobbers: Y A X
FetchRamValue:   
  ; fetch RAM value
  movw ya,spcport2  ; fetch ptr from io2/3
  movw hl,ya
  mov y,#1        	; init index to nada
- mov a,[hl]+Y   	; fetch RAM value
	mov x, a
  mov spcport2+Y,x  ; pass back to snes via io2/3
	dec y
  bpl -
  ret

; Addr in Port2.3
; Val in Port 0
; Clobbers: A X
WriteRamByte:   
  ; fetch RAM value
	mov x, #0
	mov a,spcport0
  mov [spcport2+x], a   ; fetch RAM value
	ret

.ends
