
.include "apu/enums.i"

.DEFINE MAX_FILTERS 8

.RAMSECTION "patterntable_vars" BANK 0 SLOT 1


	SNESFilterListLength			Db
	SNESFilterList_CurrentEntry 	db
	SNESFilterList					dsb MAX_FILTERS
	PatternTableCol					db
	PatternTableRow					db

.ends