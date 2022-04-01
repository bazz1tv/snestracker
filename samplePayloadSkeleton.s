.SECTION "MusicData"

; FORMAT
; Start Address to upload to SPC RAM, DATA SIZE
; Data to upload
spx_binary:
	.dw 2, spc_end-spc_start		; start address to upload to in SPC
spc_start:
	.INCBIN "derp2.spc" skip $00102 read $e0
spc_end:
	.dw SPC_CODE_START, a2-a1
a1:
.INCBIN "derp2.spc" skip $100 + SPC_CODE_START read $3000
a2:

	.dw $0000, SPC_CODE_START	; 00 to finish transfer, then address to jump SPC700 too to begin code execution
.ENDS