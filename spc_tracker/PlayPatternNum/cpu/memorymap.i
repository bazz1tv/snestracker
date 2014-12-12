; fill Flash ROM empty $FF
.EMPTYFILL $ff
;.DEFINE HIROMOFFSET $8000
.ROMBANKMAP
	BANKSTOTAL $2
	BANKSIZE $10000
	BANKS $2
.ENDRO
	


.MEMORYMAP        
	; WRAM MIRROR SLOTS For EASY DIRECT PAGING ALIGNMENT            
	SLOTSIZE $100
		SLOT 0 $0             
		SLOT 1 $100			
		SLOT 2 $200			
		SLOT 3 $300
		SLOT 4 $400			
		SLOT 5 $500			
		SLOT 6 $600
		SLOT 7 $700			
		SLOT 8 $800			
		SLOT 9 $900
		SLOT $A $a00
		SLOT $B $b00
		SLOT $C $c00
		SLOT $D $d00
		SLOT $E $e00
		SLOT $F $f00
	SLOTSIZE $200
		SLOT $10 $1000
	SLOTSIZE $100
		SLOT $11 $1100
	SLOTSIZE $200
		SLOT $12 $1200
	SLOTSIZE $100
		SLOT $13 $1300
	SLOTSIZE $200
		SLOT $14 $1400
	SLOTSIZE $100		; 
		SLOT $15 $1500
	SLOTSIZE $100
		SLOT $16 $1600	
		SLOT $17 $1700	
		SLOT $18 $1800	
		SLOT $19 $1900	
		SLOT $1a $1a00	
		SLOT $1b $1b00	
		SLOT $1c $1c00	
	SLOTSIZE $100		; one last $200 section for the WRAM mirror
		SLOT $1d $1d00
	SLOTSIZE $100
		SLOT $1e $1e00
		;SLOT $1f $1f00	; Leave for STACK

	; DONE WRAM MIRROR SLOTS
		

	
	SLOTSIZE $e000
		SLOT $1f $2000	; bank 7E RAM 2000-ffff
	SLOTSIZE $10000
		SLOT $20 $0000	; bank 7f RAM 0000-ffff
	
	SLOTSIZE $10000
		SLOT $21 $0000
		
	DEFAULTSLOT $21		; Default Slot LoROM
.ENDME
	        
	
.SNESHEADER
  ID "SNES"                     ; 1-4 letter string, just leave it as "SNES"
  
  NAME "SNES Tile Demo       "  ; Program Title - can't be over 21 bytes,
  ;    "123456789012345678901"  ; use spaces for unused bytes of the name.

  SLOWROM
  HIROM

  CARTRIDGETYPE $00             ; $00 = ROM only, see WLA documentation for others
  ROMSIZE $08                   ; $08 = 2 Mbits,  see WLA doc for more..
  SRAMSIZE $00                  ; No SRAM         see WLA doc for more..
  COUNTRY $01                   ; $01 = U.S.  $00 = Japan, that's all I know
  LICENSEECODE $00              ; Just use $00
  VERSION $00                   ; $00 = 1.00, $01 = 1.01, etc.
.ENDSNES

.SNESNATIVEVECTOR               ; Define Native Mode interrupt vector table
  COP EmptyHandler
  BRK EmptyHandler
  ABORT EmptyHandler
  NMI VBlank
  IRQ virq
.ENDNATIVEVECTOR

.SNESEMUVECTOR                  ; Define Emulation Mode interrupt vector table
  COP EmptyHandler
  ABORT EmptyHandler
  NMI EmptyHandler
  RESET Start
  IRQBRK EmptyHandler
.ENDEMUVECTOR


  


