.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/regdefs.i"
.INCLUDE "apu/enums.i"
.INCLUDE "apu/commands.i"
.INCLUDE "apu/playbackengine.i"

; Zero page variables!
.RAMSECTION "gp-dp0" BANK 0 SLOT SPC_DP0_SLOT
l               db
h               db

c				db
b				db

e               db
d               db

g				db
f				db

s				db
r				db

PrevCmd         db

flags           db
.ends

.equ hl l EXPORT
.equ bc c EXPORT
.equ de e EXPORT
.equ fg g EXPORT
.equ rs s EXPORT

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "SPCCODE" FORCE
	
MAIN:
	; Following only used to trigger an SPC dump by bsnes-plus
	;mov dspaddr, #kon
	;mov dspdata, #1

	CLRP ; set dp to 0
	;mov x, #$EF
	;mov sp, x
	mov dspaddr, #flg
	mov dspdata, #$e0 ; reset, no echo writes, mute amp
	mov a, #0
  mov control, a ; disable timers and ROM region
	mov dspaddr, #mvol_l
	mov dspdata, a
	mov dspaddr, #mvol_r
	mov dspdata, a
	mov dspaddr, #evol_l
	mov dspdata, a
	mov dspaddr, #evol_r
	mov dspdata, a

	call !koffAllNotes
	mov dspaddr, #pmon
	mov dspdata, a
	mov dspaddr, #non
	mov dspdata, a
	mov dspaddr, #eon
	mov dspdata, a

	/* When being run from the tracker, to avoid wait times, the RAM is
	 * pre-cleared to 0 by the spc template. But when we run this in the wild,
	 * if the echo buffer has values in it when echo is enabled by FLG, there
	 * will be noise. We can either delay 0.5s to ensure the echo buffer has
	 * cleared itself, or run a RAM clear routine that is quicker for lower
	 * EDL settings, but for max EDL, it will take just about as long.
	 * NOTE: This wait is different from the typical EDL wait.
	 * This is an additional wait for echo buffer to clear. */

	; IPL ROM cleared the zero page, so this bit will be clear on real run.
	bbs extflags.SKIP_ECHOBUF_CLEAR, @skip_echobuf_clear
	; clear echo buffer from (ESA * $100) ranging through (EDL * 0x800)( max $7800 bytes )
	mov a, #$0f ;max edl_val
	asl a
	asl a
	asl a	; * 8 (will represent $800 increments)
	mov x, a
	mov a, y ; #0
	mov y, #$88; min esa_val
	movw hl, ya
	mov y, a ; y = 0
-	mov [hl] + y, a
	dbnz y, -
	inc h
	dec x
	bne -
@skip_echobuf_clear:
; Enter MAIN LOOP
MainLoop:
	bbc flags.bPlaySong, Poll
  call !ContinueSong

Poll:
  mov a,spcport1  		; Fetch io1
  cmp a,PrevCmd     	; same as PrevCmd?
  beq MainLoop

	nop	; need to waste a cycle to ensure the port data has been loaded. or
	; load spc parameter port
  mov PrevCmd,spcport1     	; Store as PrevCmd
    
  cmp a, #CmdEnd
  bpl PollExit
  mov spcport1, a
  ; push the PollExit address to stack and do a table jump
  asl a ; word-sized index into jump table
  mov x, a
  mov a, #>PollExit
  push a
  mov a, #<PollExit
  push a
  jmp [!CmdJumpTable+X]

PollExit:
	mov a,PrevCmd		; pass back cmd to SNES to tell it we're done
	mov spcport1,a	; io1
  bra MainLoop   ; repoll snes

CmdJumpTable:
  .dw EmptyHandler, FetchRamValue, WriteRamByte, PlaySong, StopSong, SetPattern
  .dw PlayInstrument
EmptyHandler:
  ret

; ====== WRITE A BYTE TO THE S-SDSP =========
WDSP:
    MOV $F2,Y
    MOV $F3,A
    RET
; ====== WRITE A BYTE TO THE S-SDSP =========
WDSPX:
    MOV $F2,X
    MOV $F3,A
    RET

.ENDS
