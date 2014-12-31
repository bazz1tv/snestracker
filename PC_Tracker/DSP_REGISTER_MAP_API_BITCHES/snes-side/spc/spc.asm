.org $2000
Main:
    call Init
infin:
    bra infin
    
Init:
    
    
    mov $F2, #$5D   ; starting address = $3000
    mov $F3, #$30
    
    mov a, #$00
    mov !$3000, a
    mov a, #$02
    mov !$3001, a
    mov a, #$91
    mov !$3002, a
    mov a, #$16
    mov !$3003, a

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
    
    
    mov $F2, #$05 
    mov $F3, #$00 
    mov $F2, #$07 
    mov $F3, #$1f   ; set GAIN, direct mode (bit7=0), full volume
    


    # Voice 1

    mov $F2, #$10       ; left volume = 127
    mov $F3, #1
    mov $F2, #$11       ; right volume = 127 
    mov $F3, #1

    mov $F2, #$12       ; pitch (low byte) 
    mov $F3, #$00 
    mov $F2, #$13       ; pitch (high byte) 
    mov $F3, #$10
    
    mov $F2, #$14       ; source number = 0 
    mov $F3, #$00
    
    
    mov $F2, #$15 
    mov $F3, #$00 
    mov $F2, #$17 
    mov $F3, #$1f   ; set GAIN, direct mode (bit7=0), full volume
    
    mov $F2, #$0C 
    mov $F3, #127   ; L/R vol 100% both voices 
    mov $F2, #$1C
    mov $F3, #127
    
    
    

    mov $F2, #$6C   ; turn off mute
    mov $F3, #0

    ;
    mov $FA, #$20
    mov $f1, #1

    mov a, #0
    mov !$800, a

    mov $F2, #$4C
    mov $f3, #0
    mov $F3, #$01

loop:    
    #mov a, $fd
    beq loop

    ; if a > 0
    clrc
    adc a, !$800
    mov !$800, a

    cmp a, #$40
    bne loop

    mov a, #0
    mov !$800, a
    

    #mov $F2, #$4C
    #mov $f3, #0
    #mov $F3, #$02
    

    bra loop

.END
EOF