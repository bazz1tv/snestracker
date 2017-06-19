.org $1000
Main:
    call Init
infin:
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
    mov $F3, #$30
    
    mov a, #$00
    mov !$3000, a
    mov a, #$02
    mov !$3001, a
    mov a, #$91
    mov !$3002, a
    mov a, #$02    ; #$16
    mov !$3003, a
    
    
    mov $F2, #$05 
    mov $F3, #$00 
    mov $F2, #$07 
    mov $F3, #$1f   ; set GAIN, direct mode (bit7=0), full volume
    
    mov $F2, #$0C 
    mov $F3, #127   ; L/R vol 100% both voices 
    mov $F2, #$1C
    mov $F3, #127
    
    mov $F2, #$6C   ; turn off mute
    mov $F3, #0
    
    mov $F2, #$4C
    mov $F3, #$01
    
loop:
    bra loop

.END
EOF