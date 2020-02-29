control 	.equ 	$f1
timer0		.equ	$fa
counter0	.equ	$fd
kon			.equ	$4c
dspaddr		.equ	$f2
dspval		.equ	$f3

v0pl		.equ	$02
v0ph		.equ	$03

v1pl		.equ	$12
v1ph		.equ	$13

pattern_toggle .equ	$04
numticks	.equ	$05
pattern_count	.equ	$02

.org $200


	
	
Main:
	clrp
    call Init
	
	mov $5,#5
	mov pattern_count, #0
	
	; Setup Timer
	mov timer0,  #$00	; 32ms timing value, 00 for 256 iterations of 125 microseconds
	mov control, #$01	; this will destroy all other timers
infin:
	; timer is started
	
	clrc
	mov a, #0

wait_for_tick:
	adc a,counter0	; read counter
	cmp a,$5
						; 1 Second = 32Ms MAX timer value * 32 = 1024 ms = 1.024 s
	bcc wait_for_tick
	
	; play C-4 Piano (sampled at C-4)
	mov y,#v0pl
	mov a,#$6b
	call wdsp
	inc y
	mov a,#$1a
	call wdsp
	mov a, #1b	; play both
	call keyon
	
	
; Pattern Block
	
	clrc
	mov a,#0
		
thatsallfolks:
	;jmp $ffc0
	;jmp thatsallfolks
	
    
Init:
	mov $f2, #$7D
	mov $f3, #0

    mov $F2, #$00       ; left volume = 127
    mov $F3, #127
    mov $F2, #$01       ; right volume = 127 
    mov $F3, #127
	
    mov $F2, #$10       ; left volume = 127
    mov $F3, #127
    mov $F2, #$11       ; right volume = 127 
    mov $F3, #127
    
    
    ; DIR
    mov $F2, #$5D   ; starting address = $9000
    mov $F3, #$90
    
	; DIR
	; for piano
    mov $F2, #$04       ; source number = 0 
    mov $F3, #$00
    mov a, #$00
    mov !$9000, a
    mov a, #$10
    mov !$9001, a
    mov a, #$00 ;#$91
    mov !$9002, a
    mov a, #$10    ; #$16
    mov !$9003, a
	
	; for bass
    mov $F2, #$14       ; source number = 1
    mov $F3, #$01
    mov a, #$00
    mov !$9004, a
    mov a, #$40
    mov !$9005, a
    mov a, #$00 ;#$91
    mov !$9006, a
    mov a, #$40    ; #$16
    mov !$9007, a
    
    ; Voice 0 Piano
    mov $F2, #$05 
    mov $F3, #$00 
    mov $F2, #$07 
    mov $F3, #$1F ;#127   ; set GAIN, direct mode (bit7=0), full volume
    ; Voice 1 Bass
    mov $F2, #$15 
    mov $F3, #$00
    mov $F2, #$17 
    mov $F3, #$1F ;#127   ; set GAIN, direct mode (bit7=0), full volume
	
    mov $F2, #$0C 
    mov $F3, #127   ; L/R vol 100% both voices 
    mov $F2, #$1C
    mov $F3, #127
    
    mov $F2, #$6C   ; turn off mute
    mov $F3, #00100000b
	
	ret 
    
    
; ====== Write a byte to the S-SDSP =========
wdsp:
    mov !$00F2,y
    mov !$00F3,a
    ret
	
keyon:
	; Key ON
	mov $F2, #$4C
	mov $F3, a
	ret
    

.END
EOF