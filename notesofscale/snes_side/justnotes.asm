control 	.equ 	$f1
timer0		.equ	$fa
counter0	.equ	$fd
kon			.equ	$4c
dspaddr		.equ	$f2
dspval		.equ	$f3

v0pl		.equ	$02
v0ph		.equ	$03


.org $200


	
	
Main:
	clrp
    call Init
	
	; Setup Timer
	mov timer0,  #$00	; 32ms timing value, 00 for 256 iterations of 125 microseconds
	mov control, #$01	; this will destroy all other timers
infin:
	; timer is started
	
	clrc
	mov a, #0

wait_for_tick:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick
	
	; play C-4
	mov y,#v0pl
	mov a,#$00
	call wdsp
	inc y
	mov a,#$10
	call wdsp
	call v0keyon
	
	clrc
	mov a, #0
wait_for_tick2:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick2
	
	; play D-4
	mov y,#v0pl
	mov a,#$00
	call wdsp
	inc y
	mov a,#$12
	call wdsp
	call v0keyon
	
	clrc
	mov a, #0
wait_for_tick3:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick3
	
	; play E-4
	mov y,#v0pl
	mov a,#$00
	call wdsp
	inc y
	mov a,#$14
	call wdsp
	call v0keyon
	
	clrc
	mov a, #0
wait_for_tick4:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick4
		
	; play F-4
	mov y,#v0pl
	mov a,#$55
	call wdsp
	inc y
	mov a,#$15
	call wdsp
	call v0keyon
	
	clrc
	mov a, #0
wait_for_tick5:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick5
		
	; play G-4
	mov y,#v0pl
	mov a,#$00
	call wdsp
	inc y
	mov a,#$18
	call wdsp
	call v0keyon
	
	clrc
	mov a, #0
wait_for_tick6:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick6
		
	; play A-4
	mov y,#v0pl
	mov a,#$ab
	call wdsp
	inc y
	mov a,#$1a
	call wdsp
	call v0keyon
	
	clrc
	mov a, #0
wait_for_tick7:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick7
		
	; play B-4
	mov y,#v0pl
	mov a,#$00
	call wdsp
	inc y
	mov a,#$1e
	call wdsp
	call v0keyon
	
	clrc
	mov a, #0
wait_for_tick8:
	adc a,counter0	; read counter
	cmp a,#5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick8
		
	; play C-5
	mov y,#v0pl
	mov a,#$00
	call wdsp
	inc y
	mov a,#$20
	call wdsp
	call v0keyon
	
	

    jmp infin
    
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
    
    mov $F2, #$5D   ; starting address = $9000
    mov $F3, #$90
    
	; for piano
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
	
v0keyon:
	; Key ON
	mov $F2, #$4C
	mov $F3, #$01
	ret
    

.END
EOF