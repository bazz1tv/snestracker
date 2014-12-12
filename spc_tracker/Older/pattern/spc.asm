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

dir			.equ	$5D

.org $200

Main:
	clrp	; 
	call Init
	
	; Setup Timer
	
	
derp:
	jmp derp
	
Init:
	mov $f2, #$7D		; Set echo delay to 0
	mov $f3, #0

    mov $F2, #$00       ; left volume = 127
    mov $F3, #50
    mov $F2, #$01       ; right volume = 127 
    mov $F3, #50

    mov $F2, #$10       ; left volume = 127
    mov $F3, #127
    mov $F2, #$11       ; right volume = 127 
    mov $F3, #127


    ; DIR
    mov $F2, #dir   ; starting address = $2000
    mov $F3, #$20

    

    mov $F2, #$6C   ; turn off mute
    mov $F3, #00100000b	; Turn off ECHO

	ret 


; ====== Write a byte to the S-SDSP =========
wdsp:
    mov $00F2,y
    mov $00F3,a
    ret

keyon:
	; Key ON
	mov $F2, #$4C
	mov $F3, a
	ret


.END
EOF
	