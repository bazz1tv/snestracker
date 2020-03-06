; We can make struct definitions that don't need to be defined in RAM,
; used purely by their definition as a way to informatically load offsets
; from RAM pointers

.struct PatternSequencer
num_entries   db ;length in bytes (same as number of pattern entries)
sequences     db
/* sequences is described to assembler as 1 byte in size but that is just to
	 make it addressable. In reality, it's the size specified by num_entries.
	 Each entry is just a byte specifying the pattern number */
.ends
