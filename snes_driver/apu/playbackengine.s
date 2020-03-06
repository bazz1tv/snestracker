; Zero page variables!
.RAMSECTION "gp-dp0" BANK 0 SLOT SPC_DP0_SLOT
sequencer_ptr dw	; points to sequencer table
sequencer_i db ; index into the sequencer table

patterntable_ptr	; pattern table turns pattern index into pattern address
pattern_ptr dw	; ptr to the current pattern, maybe only temp ram needed)
pattern_i db ; index into the pattern table (could just use temp RAM tho)
patternlen_rows db ; 00 == 256
.ENDS
