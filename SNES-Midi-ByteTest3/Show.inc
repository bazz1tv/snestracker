;---------------------------------------------------------------------------------
; SNES ROM bank map
;---------------------------------------------------------------------------------
; Written by: Neviksti
;  This is the standard include file I use for programming the main CPU processor
;  with WLA.  If anyone uses my code, please share your creations with me
;  as I am always curious :)
;---------------------------------------------------------------------------------

; == LoRom ========================= 
;
;.MEMORYMAP              ; Tell WLA that the SNES has ROM at locations ;$8000-$FFFF in every bank
;SLOTSIZE $8000          ; and that this area is $8000 bytes in size.
;DEFAULTSLOT 0           ; There is only a single slot in SNES, other consoles
;SLOT 0 $8000            ;       may have more slots per bank.
;.ENDME
;
;.ROMBANKSIZE $8000      ; Every ROM bank is 32 KBytes in size
;.ROMBANKS 8             ; 2Mbits -- Tells WLA that you want to use 8 ROM banks.
;.DEFINE HEADER_OFF $0000
; ===========================

; == HiRom ========================= 

.MEMORYMAP             ; Tell WLA that the SNES has ROM at locations 0000-$FFFF in every bank
SLOTSIZE $10000        ; and that this area is $10000 bytes in size.
DEFAULTSLOT 0          ; There is only a single slot in SNES, other consoles
SLOT 0 $0000           ;	may have more slots per bank.
.ENDME

.ROMBANKSIZE $10000    ; Every ROM bank is 64 KBytes in size, also necessary.
.ROMBANKS 32           ; 16Mbits -- Tells WLA that you want to use 32 ROM banks.
.DEFINE HEADER_OFF $8000
; ===========================

;-------------------------------------------------------------------------------

.BANK 0 SLOT 0	; The SLOT 0 may be ommited, as SLOT 0 is the DEFAULTSLOT
; === Cartridge Header - part 1 - =====================
.ORG    $7FB0 + HEADER_OFF
.DB     "00"                        ; New Licensee Code
.DB     "SNES"                      ; ID
.ORG    $7FC0 + HEADER_OFF
.DB     "Fancia's - Slide Show"     ; Title (can't be more than 21 bytes, and should probably be a full 21 bytes)
;       "123456789012345678901"
.ORG    $7FD5 + HEADER_OFF
.DB     $21                         ; Memory Mode   ( $20 = Slow LoRom, $21 = Slow HiRom )


.BANK 0 SLOT 0
; === Cartridge Header - part 2 - =====================
.ORG    $7FD6 + HEADER_OFF
.DB     $00                   ; Contents ( $00 = ROM only, $01 = ROM and RAM, $02 = ROM and Save RAM)
.DB     $0B                   ; ROM Size   ( $08 = 2 Mbit, $09 = 4 Mbit, $0A = 8Mbit, $0B = 16Mbit ... etc )
.DB     $00                   ; SRAM Size ( $00 = 0 bits, $01 = 16 kbits, $02 = 32 kbits, $03 = 64 kbits )
.DB     $01                   ; Country ( $01 = USA )
.DB     $00                   ; Licensee Code
.DB     $00                   ; Version
.DW    $0000                  ; Checksum Complement  (not calculated ... but who cares?)
.DW    $0000                  ; Checksum


.BANK 0 SLOT 0
; === Interrupt Vector Table ====================
.ORG    $7FE4 + HEADER_OFF   ; === Native Mode ===
.DW     EmptyHandler          ; COP
.DW     EmptyHandler          ; BRK
.DW     EmptyHandler          ; ABORT
.DW     VBlank                ; NMI
.DW     $0000                 ; (Unused)
.DW     EmptyHandler          ; IRQ

.ORG    $7FF4 + HEADER_OFF   ; === Emulation Mode ===
.DW     EmptyHandler          ; COP
.DW     $0000                 ; (Unused)
.DW     EmptyHandler          ; ABORT
.DW     EmptyHandler          ; NMI
.DW     Main                  ; RESET
.DW     EmptyHandler          ; IRQ/BRK

; ============================================


.BANK 0 SLOT 0
.org HEADER_OFF
.SECTION "EmptyVectors" SEMIFREE

EmptyHandler:
	rti

EmptyVBlank:
	rep #30
	pha
	php

	sep #$20
	lda $4210		;clear NMI Flag

	plp
	pla
	rti

.ENDS

.EMPTYFILL $FF


; --- Reminder to self: WLA doesn't seem to handle the .BASE directive 
;       correctly yet.  I need to check into this

;.BASE $40		;SLOW HiRom
;.BASE $80		;FAST LoRom
;.BASE $C0		;FAST HiRom

