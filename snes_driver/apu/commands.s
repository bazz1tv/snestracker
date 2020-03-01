; vim: syntax=snes ft=spc700
.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/enums.i"

.BANK 0 SLOT SPC_CODE_SLOT
.org 0
.SECTION "Commands" SEMIFREE

PlaySong:
	set1 bFlags.bPlaySong
	ret
	
StopSong:
  clr1 bFlags.bPlaySong
	ret

FetchRamValue:   
  ; fetch RAM value
  mov a,spcport2     	; fetch lo byte if addr from io2
  mov temp,a     		; store as lo byte of word pointer
  mov a,spcport3     	; fetch high byte from io3
  mov temp+1,a     	; store as hi byte of word pointer
  mov x,#$00      	; init index to nada
  mov a,[temp+x]   	; fetch RAM value
  mov spcport2,a     	; pass back to snes via io3
	mov a, temp
	clrc
	adc a,#1
	bcs _adjustcarry
-	mov temp, a
	mov a,[temp+x]
	mov spcport3,a
	ret
_adjustcarry:
	inc temp+1
	bra -
	
; Addr in Port2.3
; Val in Port 0	
WriteRamByte:   
  ; fetch RAM value
  mov a,spcport2     	; fetch lo byte if addr from io2
  mov temp,a     		; store as lo byte of word pointer
  mov a,spcport3     	; fetch high byte from io3
  mov temp+1,a     	; store as hi byte of word pointer
	mov a,spcport0
  mov x,#$00      	; init index to nada
  mov [temp+x],a   ; fetch RAM value
	ret

.ends
