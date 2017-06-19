control 	.equ 	$f1
timer0		.equ	$fa
counter0	.equ	$fd
kon			.equ	$4c
dspaddr		.equ	$f2
dspval		.equ	$f3


.org $200


	
	
Main:
	clrp
	;mov x, #$fF
	;mov sp, x
	; RAM location for swapping intervals
	
	;mov !$0001, #0
	mov !$4010,#$00
	mov !$4011,#$10
	mov !$4012,#$00
	mov !$4013,#$18
	mov !$4014,#$00
	
    call Init
	
	
	
	; Setup Timer
	; An example to wait 15ms.

	mov timer0,  #$00	; 32ms timing value, 00 for 256 iterations of 125 microseconds
	mov control, #$01	; this will destroy all other timers
infin:
	; timer is started
	clrc
	mov a, #0

wait_for_tick:
	adc a,counter0	; read counter
	cmp a,#32
	; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick
	
	; SWAP pitches
	mov a,$04
	beq playb
playc:
	dec $4
	
	mov a,#$00
    mov y,#$02       ; pitch (low byte) 
	call wdsp
	

	
	mov a,#$10
    inc y       ; pitch (high byte) 
	call wdsp
	
	bra keyon
playb:
	inc $4

    mov a,#$00
	mov y,#$02
	call wdsp;
	
	mov a,#$18
	inc y
	call wdsp;
	
	
keyon:
	; Key ON
	mov $F2, #$4C
	mov $F3, #$01
    bra infin
    
Init:
    mov $F2, #$00       ; left volume = 127
    mov $F3, #127
    mov $F2, #$01       ; right volume = 127 
    mov $F3, #127

    mov $F2, #$02       ; pitch (low byte) 
    mov $F3, #$00
    mov $F2, #$03       ; pitch (high byte) 
    mov $F3, #$10
    
    mov $F2, #$04       ; source number = 0 
    mov $F3, #$00
    
    mov $F2, #$5D   ; starting address = $3000
    mov $F3, #$90
    
    mov a, #$00
    mov !$9000, a
    mov a, #$10
    mov !$9001, a
    mov a, #$00 ;#$91
    mov !$9002, a
    mov a, #$10    ; #$16
    mov !$9003, a
    
    
    mov $F2, #$05 
    mov $F3, #$00 
    mov $F2, #$07 
    mov $F3, #$1F ;#127   ; set GAIN, direct mode (bit7=0), full volume
    
    mov $F2, #$0C 
    mov $F3, #127   ; L/R vol 100% both voices 
    mov $F2, #$1C
    mov $F3, #127
    
    mov $F2, #$6C   ; turn off mute
    mov $F3, #0
	
	ret 
    
    
; ====== Write a byte to the S-SDSP =========
wdsp:
    mov !$00F2,y
    mov !$00F3,a
    ret
    

.END
EOF