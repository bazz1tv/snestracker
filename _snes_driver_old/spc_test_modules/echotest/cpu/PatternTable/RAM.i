
.include "apu/enums.i"

.RAMSECTION "patterntable_vars" BANK 0 SLOT 1


	SNESFilterListLength			Db
	SNESFilterList_CurrentEntry 	db
	SNESFilterList					dsb MAX_PATTERNS
	PatternTableCol					db
	PatternTableRow					db

.ends