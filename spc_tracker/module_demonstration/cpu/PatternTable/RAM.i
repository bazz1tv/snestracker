
.include "apu/enums.i"

.RAMSECTION "patterntable_vars" BANK 0 SLOT 1


	SNESPatternListLength			Db
	SNESPatternList_CurrentEntry 	db
	SNESPatternList					dsb MAX_PATTERNS
	PatternTableCol					db
	PatternTableRow					db

.ends