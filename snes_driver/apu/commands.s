.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/regdefs.i"
.INCLUDE "apu/enums.i"

.BANK 0 SLOT SPC_CODE_SLOT
.ORG 0
.SECTION "Commands" SEMIFREE

FetchRamValue:   
  ; fetch RAM value
  movw ya,spcport2  ; fetch ptr from io2/3
  movw temp,ya
  mov y,#1        	; init index to nada
- mov a,[temp]+Y   	; fetch RAM value
	mov x, a
  mov spcport2+Y,x  ; pass back to snes via io2/3
	dec y
  bpl -
  ret

; Addr in Port2.3
; Val in Port 0	
WriteRamByte:   
  ; fetch RAM value
	mov x, #0
  /*movw ya, spcport2
	movw temp, ya*/
	mov a,spcport0
  mov [spcport2+x], a   ; fetch RAM value
	ret

.ends
