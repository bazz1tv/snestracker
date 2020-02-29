.INCLUDE "apu/memorymap.i"	
.INCLUDE "apu/enums.i"


.BANK 0 SLOT 2
.ORGA $200
.SECTION "Commands" SEMIFREE

; Used by SelectTimer function
InitTimer:
	; SETUP TIMER
	
	; to get CountAddr
	mov a, SPCTimerSelect
	clrc
	adc a,#COUNTERBASE
	mov SPCCounterAddr,a
	
	
	call !UpdateTimerReg
	
	mov a, SPCTimerSelect
	
	cmp a, #2
	bne +
	inc a
	
+	inc a
	MOV CONTROL, a	; 
	ret

SelectTimer:
	call !WriteRamByte
	call !InitTimer
	ret
	
WriteTimerValue:
	call !WriteRamByte
	call !UpdateTimerReg
	ret
	
UpdateTimerReg:
	MOV a, SPCTimerSelect
	clrc
	adc a, #$fa
	mov SPCTimerAddr, a
	mov x,a
	MOV A,SPCTimerValue
	MOV (X),A
	ret

PlaySong:
	
	
	mov SPCbPlaySong, #1
	ret
	
StopSong:
	mov SPCbPlaySong, #0
	ret

FetchRamValue:   
    ; fetch RAM value
    mov a,spcport2     	; fetch lo byte if addr from io2
    mov SPCtemp,a     		; store as lo byte of word pointer
    mov a,spcport3     	; fetch high byte from io3
    mov SPCtemp+1,a     	; store as hi byte of word pointer
    mov x,#$00      	; init index to nada
    mov a,[SPCtemp+x]   	; fetch RAM value
    mov spcport2,a     	; pass back to snes via io3
	mov a, SPCtemp
	clrc
	adc a,#1
	bcs _adjustcarry
-	mov SPCtemp, a
	mov a,[SPCtemp+x]
	mov spcport3,a
	ret
_adjustcarry:
	inc SPCtemp+1
	bra -
	
	
	
	
	
; Addr in Port2.3
; Val in Port 0	
WriteRamByte:   
    ; fetch RAM value
    mov a,spcport2     	; fetch lo byte if addr from io2
    mov SPCtemp,a     		; store as lo byte of word pointer
    mov a,spcport3     	; fetch high byte from io3
    mov SPCtemp+1,a     	; store as hi byte of word pointer
	mov a,spcport0
    mov x,#$00      	; init index to nada
    mov [SPCtemp+x],a   ; fetch RAM value
	ret


	;.export derp1,derp2

.ends