.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/regdefs.i"
.INCLUDE "apu/enums.i"

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "Commands" SEMIFREE

PlaySong:
	set1 flags.bPlaySong
	ret
	
StopSong:
  clr1 flags.bPlaySong
	ret

FetchRamValue:   
  ; fetch RAM value
  movw ya,spcport2  ; fetch ptr from io2/3
  movw temp,ya
  mov x,#1        	; init index to nada
- mov a,[temp+x]   	; fetch RAM value
  mov spcport2+x,a  ; pass back to snes via io2/3
	dec x
  bpl -
  ret

; Addr in Port2.3
; Val in Port 0	
WriteRamByte:   
  ; fetch RAM value
	mov a,spcport0
  mov x, #0
  mov [spcport2+x],a   ; fetch RAM value
	ret

.ends
