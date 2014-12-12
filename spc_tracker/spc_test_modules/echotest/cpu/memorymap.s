.include "cpu/memorymap.i"

; I made this so I don't have to use .LOROM or .HIROM directives. I donno what they do, the
; manual only says that they ovverride ROMBANKMAP. I don't want that to be overridden, especially in a case where I am using LoHiROM together. So I decided to manually specify the vector (this is for LoRom/HIROM here)
.BANK 0
; === Cartridge Header - part 1 - =====================
.ORGA    $FFB0
.DB     "00"                        ; New Licensee Code
.DB     "SNES"                      ; ID
.ORGA    $FFC0
.DB     "SNES TRACKER :D      "     ; Title (can't be more than 21 bytes, and should probably be a full 21 bytes)
;       "123456789012345678901"
.ORGA    $FFD5
.DB     $21                         ; Memory Mode   ( $20 = Slow LoRom, $21 = Slow HiRom )

; === Cartridge Header - part 2 - =====================
.ORGA    $FFD6
.DB     $02                   ; Contents ( $00 = ROM only, $01 = ROM and RAM, $02 = ROM and Save RAM)
.DB     $06                   ; ROM Size   ( $08 = 2 Mbit, $09 = 4 Mbit, $0A = 8Mbit, $0B = 16Mbit ... etc )
.DB     $05                   ; SRAM Size ( $00 = 0 bits, $01 = 16 kbits, $02 = 32 kbits, $03 = 64 kbits )
.DB     $01                   ; Country ( $01 = USA )
.DB     $00                   ; Licensee Code
.DB     $01                   ; Version
.DW    $0000                  ; Checksum Complement  (not calculated ... but who cares?)
.DW    $0000                  ; Checksum

	;.SNESHEADER
  ;ID "DICK"                     ; 1-4 letter string, just leave it as "SNES"
  
  ;NAME "SNES TRACKER :D      "  ; Program Title - can't be over 21 bytes,
  ;    "123456789012345678901"  ; use spaces for unused bytes of the name.

  ;SLOWROM
  
  ;CARTRIDGETYPE $02             ; $00 = ROM only, see WLA documentation for others
  ;ROMSIZE $06                   ; $08 = 2 Mbits,  see WLA doc for more..
  ;SRAMSIZE $05                  ; 4Mbit SRAM         see WLA doc for more..
  ;COUNTRY $01                   ; $01 = U.S.  $00 = Japan, that's all I know
  ;LICENSEECODE $00              ; Just use $00
  ;VERSION $01                   ; $00 = 1.00, $01 = 1.01, etc.
;.ENDSNES


.ORGA $Ffe4
.SECTION "__Vectors" FORCE
	.dw EmptyHandler.w 	;COP EmptyHandler
	.dw EmptyHandler.w 	;  BRK EmptyHandler
	.dw EmptyHandler.w 	;  ABORT EmptyHandler
	.dw VBlank.w 			;  NMI VBlank
	.dw $0000			; unused
	.dw EmptyHandler.w 	;  IRQ EmptyHandler
;7FF0-7FF3 - unused
	.dw $0000,$0000 	; unused
; 7FF4
; Define Emulation Mode interrupt vector table
	.dw EmptyHandler.w 	;COP EmptyHandler
	.dw $0000 			;unused
	.dw EmptyHandler.w 	; ABORT EmptyHandler
	.dw EmptyHandler.w 	;NMI EmptyHandler
	.dw Start.w 		;RESET Start
	.dw EmptyHandler.w 	;IRQ/BRK EmptyHandler
;.ENDEMUVECTOR
.ENDS

.BANK 0					; Defines the ROM bank and the slot it is inserted in memory.
.orga $8000	; .ORG 0 is really $8000, because the slot starts at $8000
.SECTION "EmptyVectors" SEMIFREE

EmptyHandler:
        rti

.ENDS